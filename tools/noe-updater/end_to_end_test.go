package main

import (
 "archive/zip"
 "bytes"
 "crypto/ed25519"
 "crypto/rand"
 "encoding/base64"
 "encoding/json"
 "os"
 "os/exec"
 "path/filepath"
 "strings"
 "testing"
)

// This test crosses the real signer CLI and updater, not just a hand-built fixture.
// No external service, production signing key, network access or editor install is used.
func TestOfflineSignerToUpdaterEndToEnd(t *testing.T) {
 if testing.Short(){t.Skip("invokes offline Go compiler")}
 if _, err := exec.LookPath("go"); err != nil {t.Skip("Go compiler unavailable")}
 root := t.TempDir()
 oldFile := filepath.Join(root,"engine.bin")
 if err:=os.WriteFile(oldFile, []byte("old engine"), 0600);err!=nil{t.Fatal(err)}
 newFile := []byte("new engine bytes for signed update")
 sourceDir:=t.TempDir()
 if err:=os.WriteFile(filepath.Join(sourceDir,"engine.bin"),newFile,0600);err!=nil{t.Fatal(err)}
 pub,private,err:=ed25519.GenerateKey(rand.Reader);if err!=nil{t.Fatal(err)}
 keyFile:=filepath.Join(t.TempDir(),"offline.key")
 if err:=os.WriteFile(keyFile,[]byte(base64.StdEncoding.EncodeToString(private)),0600);err!=nil{t.Fatal(err)}
 m:=Manifest{Version:"0.1.1",BaseVersion:"0.1.0",Notes:"Update test with a verified patch",
  Changes:[]Change{{Path:"engine.bin",Op:"replace",Before:digest([]byte("old engine")),After:digest(newFile)}}}
 manifestPath:=filepath.Join(t.TempDir(),"manifest.json")
 raw,err:=json.Marshal(m);if err!=nil{t.Fatal(err)}
 if err:=os.WriteFile(manifestPath,raw,0600);err!=nil{t.Fatal(err)}
 pkg:=filepath.Join(t.TempDir(),"NoeEngine-0.1.1.noepatch")
 cmd:=exec.Command("go","run",".","-manifest",manifestPath,"-files",sourceDir,"-key",keyFile,"-out",pkg)
 cmd.Dir=filepath.Join("..","noe-patch-sign")
 cmd.Env=append(os.Environ(),"GOTOOLCHAIN=local","GOPROXY=off")
 if output,err:=cmd.CombinedOutput();err!=nil{t.Fatalf("signer failed: %v: %s",err,output)}
 receipt:=filepath.Join(t.TempDir(),"last_update.json")
 if err:=apply(root,pkg,receipt,pub,"0.1.1");err!=nil{t.Fatalf("signed installer rejected a valid release: %v",err)}
 final,err:=os.ReadFile(oldFile);if err!=nil||!bytes.Equal(final,newFile){t.Fatal("installed file did not match signed bytes")}
 notice,err:=os.ReadFile(receipt);if err!=nil{t.Fatal(err)}
 if !bytes.Contains(notice,[]byte("\\"status\\":\\"installed\\"")){t.Fatal("missing successful update receipt")}

 // Incorrect expected release number cannot be used to install.
 staleRoot:=t.TempDir()
 if err:=os.WriteFile(filepath.Join(staleRoot,"engine.bin"),[]byte("old engine"),0600);err!=nil{t.Fatal(err)}
 if err:=apply(staleRoot,pkg,"",pub,"0.1.2");err==nil{t.Fatal("accepted incorrect release number")}
 // Wrong pinned public key is rejected.
 wrongPub,_,_:=ed25519.GenerateKey(rand.Reader)
 if err:=apply(staleRoot,pkg,"",wrongPub,"0.1.1");err==nil{t.Fatal("accepted untrusted signing key")}
}

// A valid manifest signature must NOT authorize a modified file entry.
func TestSignedArchiveContentTamperingFails(t *testing.T) {
 validPayload:=[]byte("release bytes")
 change:=Change{Path:"editor.bin",Op:"add",After:digest(validPayload)}
 pkg,pub:=fixture(t,change,validPayload)
 z,err:=zip.OpenReader(pkg);if err!=nil{t.Fatal(err)}
 defer z.Close()
 forged:=filepath.Join(t.TempDir(),"forged.noepatch")
 out,err:=os.Create(forged);if err!=nil{t.Fatal(err)}
 writer:=zip.NewWriter(out)
 for _,entry:=range z.File {
  stream,err:=entry.Open();if err!=nil{t.Fatal(err)}
  content:=new(bytes.Buffer);_,err=content.ReadFrom(stream);stream.Close();if err!=nil{t.Fatal(err)}
  w,err:=writer.Create(entry.Name);if err!=nil{t.Fatal(err)}
  data:=content.Bytes()
  if strings.HasPrefix(entry.Name,"files/"){data=[]byte("forged release bytes")}
  if _,err=w.Write(data);err!=nil{t.Fatal(err)}
 }
 if err=writer.Close();err!=nil{t.Fatal(err)}
 if err=out.Close();err!=nil{t.Fatal(err)}
 if err=apply(t.TempDir(),forged,"",pub);err==nil{t.Fatal("accepted archive with modified payload")}
}
