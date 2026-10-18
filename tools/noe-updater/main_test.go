package main
import (
 "archive/zip"
 "crypto/ed25519"
 "crypto/rand"
 "encoding/json"
 "os"
 "path/filepath"
 "testing"
)
func fixture(t *testing.T,change Change,payload []byte)(string,ed25519.PublicKey){
 t.Helper()
 pub,priv,_:=ed25519.GenerateKey(rand.Reader)
 manifest:=Manifest{Version:"1.0.1",BaseVersion:"1.0.0",Notes:"Bugfix and restart improvements",Changes:[]Change{change}}
 raw,_:=json.Marshal(manifest)
 path:=filepath.Join(t.TempDir(),"release.noepatch")
 file,_:=os.Create(path);z:=zip.NewWriter(file)
 entry,_:=z.Create("manifest.json");entry.Write(raw)
 entry,_=z.Create("manifest.sig");entry.Write(ed25519.Sign(priv,raw))
 if change.Op!="delete"{entry,_=z.Create("files/"+change.Path);entry.Write(payload)}
 z.Close();file.Close()
 return path,pub
}
func TestValidSignedPatchAndReceipt(t *testing.T){
 root:=t.TempDir();os.WriteFile(filepath.Join(root,"file.txt"),[]byte("old"),0600)
 change:=Change{Path:"file.txt",Op:"replace",Before:digest([]byte("old")),After:digest([]byte("new"))}
 pkg,pub:=fixture(t,change,[]byte("new"));receipt:=filepath.Join(t.TempDir(),"last_update.json")
 if err:=apply(root,pkg,receipt,pub);err!=nil{t.Fatal(err)}
 data,_:=os.ReadFile(filepath.Join(root,"file.txt"));if string(data)!="new"{t.Fatal("wrong content")}
 if _,err:=os.Stat(receipt);err!=nil{t.Fatal(err)}
}
func TestLocalChangeIsNotOverwritten(t *testing.T){
 root:=t.TempDir();os.WriteFile(filepath.Join(root,"file.txt"),[]byte("edited"),0600)
 change:=Change{Path:"file.txt",Op:"replace",Before:digest([]byte("old")),After:digest([]byte("new"))}
 pkg,pub:=fixture(t,change,[]byte("new"))
 if err:=apply(root,pkg,"",pub);err==nil{t.Fatal("expected conflict")}
 data,_:=os.ReadFile(filepath.Join(root,"file.txt"));if string(data)!="edited"{t.Fatal("local edits lost")}
}
func TestUntrustedSignatureAndPath(t *testing.T){
 for _,name:=range []string{"../escape","/etc/passwd","a/../b",".hidden","folder\\x"}{
  if safePath(name){t.Fatalf("accepted unsafe %q",name)}
 }
 root:=t.TempDir();change:=Change{Path:"safe.txt",Op:"add",After:digest([]byte("x"))}
 pkg,_:=fixture(t,change,[]byte("x"));different,_,_:=ed25519.GenerateKey(rand.Reader)
 if apply(root,pkg,"",different)==nil{t.Fatal("accepted wrong signing key")}
}
func TestSymlinkEscapesRejected(t *testing.T){
 root:=t.TempDir();outside:=t.TempDir();os.Symlink(outside,filepath.Join(root,"redirect"))
 change:=Change{Path:"redirect/file.txt",Op:"add",After:digest([]byte("hello"))}
 pkg,pub:=fixture(t,change,[]byte("hello"))
 if apply(root,pkg,"",pub)==nil{t.Fatal("symlink bypass")}
}

func TestUpdatePreservesExecutableModeAndSafeDefaultForNewFiles(t *testing.T){
 root:=t.TempDir()
 binary:=filepath.Join(root,"noeengine")
 if err:=os.WriteFile(binary,[]byte("old binary"),0750);err!=nil{t.Fatal(err)}
 if err:=os.Chmod(binary,0750);err!=nil{t.Fatal(err)}
 change:=Change{Path:"noeengine",Op:"replace",Before:digest([]byte("old binary")),After:digest([]byte("new binary"))}
 pkg,pub:=fixture(t,change,[]byte("new binary"))
 if err:=apply(root,pkg,"",pub);err!=nil{t.Fatal(err)}
 info,err:=os.Stat(binary);if err!=nil{t.Fatal(err)}
 if info.Mode().Perm()!=0750{t.Fatalf("lost installed executable permissions: %v",info.Mode().Perm())}
 added:=Change{Path:"notes.txt",Op:"add",After:digest([]byte("hello"))}
 pkg2,pub2:=fixture(t,added,[]byte("hello"))
 if err:=apply(root,pkg2,"",pub2);err!=nil{t.Fatal(err)}
 info,err=os.Stat(filepath.Join(root,"notes.txt"));if err!=nil{t.Fatal(err)}
 if info.Mode().Perm()&0111!=0{t.Fatalf("untrusted new file was executable: %v",info.Mode().Perm())}
}
