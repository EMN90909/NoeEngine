package main

// Build offline: go run ./tools/noe-patch-sign -manifest changes.json -files patch-files -key signing-key -out release.noepatch
// A signing key must NEVER be uploaded to Supabase, GitHub, or a browser.
import (
 "archive/zip"
 "crypto/ed25519"
 "crypto/rand"
 "crypto/sha256"
 "encoding/base64"
 "encoding/hex"
 "encoding/json"
 "flag"
 "fmt"
 "os"
 "path/filepath"
 "regexp"
 "strings"
)
type Change struct {Path string `json:"path"`; Op string `json:"op"`; Before string `json:"before"`; After string `json:"after"`}
type Manifest struct {Version string `json:"version"`; BaseVersion string `json:"base_version"`; Notes string `json:"notes"`; Changes []Change `json:"changes"`}
var releaseVersion=regexp.MustCompile(`^[0-9]+\\.[0-9]+\\.[0-9]+(?:-[a-z0-9.]+)?package main

// Build offline: go run ./tools/noe-patch-sign -manifest changes.json -files patch-files -key signing-key -out release.noepatch
// A signing key must NEVER be uploaded to Supabase, GitHub, or a browser.
import (
 "archive/zip"
 "crypto/ed25519"
 "crypto/rand"
 "crypto/sha256"
 "encoding/base64"
 "encoding/hex"
 "encoding/json"
 "flag"
 "fmt"
 "os"
 "path/filepath"
 "regexp"
 "strings"
)
type Change struct {Path string `json:"path"`; Op string `json:"op"`; Before string `json:"before"`; After string `json:"after"`}
type Manifest struct {Version string `json:"version"`; BaseVersion string `json:"base_version"`; Notes string `json:"notes"`; Changes []Change `json:"changes"`}
)
var hexDigest=regexp.MustCompile(`^[a-f0-9]{64}package main

// Build offline: go run ./tools/noe-patch-sign -manifest changes.json -files patch-files -key signing-key -out release.noepatch
// A signing key must NEVER be uploaded to Supabase, GitHub, or a browser.
import (
 "archive/zip"
 "crypto/ed25519"
 "crypto/rand"
 "crypto/sha256"
 "encoding/base64"
 "encoding/hex"
 "encoding/json"
 "flag"
 "fmt"
 "os"
 "path/filepath"
 "regexp"
 "strings"
)
type Change struct {Path string `json:"path"`; Op string `json:"op"`; Before string `json:"before"`; After string `json:"after"`}
type Manifest struct {Version string `json:"version"`; BaseVersion string `json:"base_version"`; Notes string `json:"notes"`; Changes []Change `json:"changes"`}
)
func sum(x []byte) string {d:=sha256.Sum256(x);return hex.EncodeToString(d[:])}
func safe(p string) bool {if p==""||len(p)>512||strings.HasPrefix(p,"/")||strings.Contains(p,"\\")||filepath.Clean(p)!=p{return false};for _,v:=range strings.Split(p,"/"){if v=="."||v==".."||strings.HasPrefix(v,"."){return false}};return true}
func run(manifest,files,key,out string)error{
 raw,err:=os.ReadFile(manifest);if err!=nil{return err}
 var m Manifest;if err=json.Unmarshal(raw,&m);err!=nil{return err}
 if len(m.Changes)==0||len(m.Changes)>1000||len(m.Version)>64||len(m.Notes)>10000||
    !releaseVersion.MatchString(m.Version)||!releaseVersion.MatchString(m.BaseVersion){return fmt.Errorf("invalid manifest")}
 keyBytes,err:=os.ReadFile(key);if err!=nil{return err}
 secret,err:=base64.StdEncoding.DecodeString(strings.TrimSpace(string(keyBytes)));if err!=nil||len(secret)!=ed25519.PrivateKeySize{return fmt.Errorf("invalid private key")}
 file,err:=os.OpenFile(out,os.O_WRONLY|os.O_CREATE|os.O_EXCL,0600);if err!=nil{return err}
 defer file.Close();zipper:=zip.NewWriter(file)
 add:=func(name string,data []byte)error{w,err:=zipper.Create(name);if err!=nil{return err};_,err=w.Write(data);return err}
 if err=add("manifest.json",raw);err!=nil{return err}
 if err=add("manifest.sig",ed25519.Sign(ed25519.PrivateKey(secret),raw));err!=nil{return err}
 seen:=map[string]bool{};var total int64
 for _,c:=range m.Changes{
  if !safe(c.Path)||seen[c.Path]||(c.Op!="add"&&c.Op!="replace"&&c.Op!="delete"){return fmt.Errorf("invalid change: %s",c.Path)}
  seen[c.Path]=true
  if c.Op=="add"&&c.Before!=""{return fmt.Errorf("add must not have base checksum")}
  if c.Op!="add"&&!hexDigest.MatchString(c.Before){return fmt.Errorf("invalid base checksum: %s",c.Path)}
  if c.Op=="delete"{if c.After!=""{return fmt.Errorf("delete must not specify target content")};continue}
  if !hexDigest.MatchString(c.After){return fmt.Errorf("invalid target checksum: %s",c.Path)}
  payload,err:=os.ReadFile(filepath.Join(files,filepath.FromSlash(c.Path)));if err!=nil{return err}
  total+=int64(len(payload));if total>100<<20{return fmt.Errorf("bundle exceeds 100 MiB")}
  if sum(payload)!=c.After{return fmt.Errorf("target hash mismatch: %s",c.Path)}
  if err=add("files/"+c.Path,payload);err!=nil{return err}
 }
 return zipper.Close()
}
func main(){
 keygen:=flag.String("keygen","","write new private key to path, print public key")
 manifest:=flag.String("manifest","","JSON manifest")
 files:=flag.String("files","","directory of changed files")
 key:=flag.String("key","","base64 Ed25519 private key file, mode 0600")
 out:=flag.String("out","","output .noepatch")
 flag.Parse()
 if *keygen!=""{
  pub,priv,err:=ed25519.GenerateKey(rand.Reader);if err!=nil{panic(err)}
  file,openErr:=os.OpenFile(*keygen,os.O_WRONLY|os.O_CREATE|os.O_EXCL,0600);if openErr!=nil{panic(openErr)}
  if _,err=file.WriteString(base64.StdEncoding.EncodeToString(priv));err!=nil{file.Close();panic(err)}
  if err=file.Close();err!=nil{panic(err)}
  fmt.Println("ED25519 PUBLIC KEY:",base64.StdEncoding.EncodeToString(pub));return
 }
 if *manifest==""||*key==""||*out==""||*files==""{fmt.Fprintln(os.Stderr,"usage: -manifest -files -key -out");os.Exit(2)}
 if err:=run(*manifest,*files,*key,*out);err!=nil{fmt.Fprintln(os.Stderr,err);os.Exit(1)}
}
