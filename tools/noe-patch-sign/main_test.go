package main

import (
 "crypto/ed25519"
 "crypto/rand"
 "encoding/base64"
 "encoding/json"
 "os"
 "path/filepath"
 "testing"
)
func TestSignerRefusesInvalidMetadataAndRemovesPartialPatches(t *testing.T){
 _,priv,err:=ed25519.GenerateKey(rand.Reader);if err!=nil{t.Fatal(err)}
 d:=t.TempDir()
 key:=filepath.Join(d,"sign.key")
 if err=os.WriteFile(key,[]byte(base64.StdEncoding.EncodeToString(priv)),0600);err!=nil{t.Fatal(err)}
 source:=filepath.Join(d,"payload")
 if err=os.Mkdir(source,0700);err!=nil{t.Fatal(err)}
 if err=os.WriteFile(filepath.Join(source,"game.bin"),[]byte("new"),0600);err!=nil{t.Fatal(err)}
 manifest:=filepath.Join(d,"manifest.json")
 output:=filepath.Join(d,"release.noepatch")
 m:=Manifest{Version:"1.0.1",BaseVersion:"1.0.0",Notes:"Test signed update",
  Changes:[]Change{{Path:"game.bin",Op:"replace",Before:sum([]byte("old")),
   After:sum([]byte("not what the signer will read"))}}}
 raw,err:=json.Marshal(m);if err!=nil{t.Fatal(err)}
 if err=os.WriteFile(manifest,raw,0600);err!=nil{t.Fatal(err)}
 if err=run(manifest,source,key,output);err==nil{t.Fatal("accepted incorrect payload digest")}
 if _,err=os.Stat(output);!os.IsNotExist(err){t.Fatal("signer left a partially written, publishable-looking package")}
 m.Version="bad-release"
 m.Changes[0].After=sum([]byte("new"))
 raw,err=json.Marshal(m);if err!=nil{t.Fatal(err)}
 if err=os.WriteFile(manifest,raw,0600);err!=nil{t.Fatal(err)}
 if err=run(manifest,source,key,output);err==nil{t.Fatal("accepted invalid version")}
}
func TestReleaseValidationRejectsPathAndChecksumAmbiguity(t *testing.T){
 if !releaseVersion.MatchString("1.2.3-beta.1"){t.Fatal("valid prerelease rejected")}
 for _,value:=range []string{"1.2","1.2.3/evil","1.2.3\nextra"}{
  if releaseVersion.MatchString(value){t.Fatalf("accepted invalid version %q",value)}
 }
 for _,bad:=range []string{"../secrets","/tmp/root","foo//bar",".git/config","file\\escape"} {
  if safe(bad){t.Fatalf("accepted invalid path %q",bad)}
 }
 if hexDigest.MatchString("abcdef") {t.Fatal("accepted invalid digest")}
}
