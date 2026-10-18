// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
// This standalone updater must be preinstalled alongside a trusted NoeEngine build.
// It never executes scripts from a patch and only modifies files listed in an
// Ed25519-signed manifest. It runs as the existing user, never as root.
package main

import (
 "archive/zip"
 "crypto/ed25519"
 "crypto/sha256"
 "encoding/base64"
 "encoding/hex"
 "encoding/json"
 "errors"
 "flag"
 "fmt"
 "io"
 "os"
 "os/exec"
 "path/filepath"
 "strings"
 "syscall"
 "time"
)
type Change struct { Path string `json:"path"`; Op string `json:"op"`; Before string `json:"before"`; After string `json:"after"` }
type Manifest struct { Version string `json:"version"`; BaseVersion string `json:"base_version"`; Notes string `json:"notes"`; Changes []Change `json:"changes"` }
const maxSize = 100 << 20
func digest(b []byte) string { h:=sha256.Sum256(b);return hex.EncodeToString(h[:]) }
func safePath(name string) bool {
 if name==""||len(name)>512||strings.HasPrefix(name,"/")||strings.Contains(name,"\\")||strings.Contains(name,"\x00")||filepath.Clean(name)!=name{return false}
 for _,s:=range strings.Split(name,"/"){if s==""||s=="."||s==".."||strings.HasPrefix(s,"."){return false}}
 return true
}
func safeTarget(root, rel string)(string,error){
 if !safePath(rel){return "",errors.New("unsafe path")}
 current:=root
 for _,seg:=range strings.Split(rel,"/"){
  current=filepath.Join(current,seg)
  info,err:=os.Lstat(current)
  if err!=nil&&!os.IsNotExist(err){return "",err}
  if err==nil&&info.Mode()&os.ModeSymlink!=0{return "",errors.New("symlink path")}
 }
 return current,nil
}
func fileHash(path string)(string,error){b,e:=os.ReadFile(path);if e!=nil{return "",e};return digest(b),nil}
func verifyZip(path string,pub ed25519.PublicKey)(Manifest,map[string][]byte,error){
 var m Manifest
 z,err:=zip.OpenReader(path);if err!=nil{return m,nil,err};defer z.Close()
 var raw,sig []byte; files:=map[string][]byte{};size:=uint64(0)
 if len(z.File)>1024{return m,nil,errors.New("too many zip entries")}
 for _,entry:=range z.File {
  if entry.Mode()&os.ModeSymlink!=0||entry.FileInfo().IsDir()||entry.UncompressedSize64>maxSize{return m,nil,errors.New("unsupported zip entry")}
  if entry.Name!="manifest.json"&&entry.Name!="manifest.sig"&&!(strings.HasPrefix(entry.Name,"files/")&&safePath(strings.TrimPrefix(entry.Name,"files/"))){return m,nil,errors.New("unsafe zip entry")}
  if _,ok:=files[entry.Name];ok{return m,nil,errors.New("duplicate entry")}
  size+=entry.UncompressedSize64;if size>maxSize{return m,nil,errors.New("oversize archive")}
  r,e:=entry.Open();if e!=nil{return m,nil,e};b,e:=io.ReadAll(io.LimitReader(r,maxSize+1));r.Close();if e!=nil||len(b)>maxSize{return m,nil,errors.New("zip read limit")}
  files[entry.Name]=b
  if entry.Name=="manifest.json"{raw=b};if entry.Name=="manifest.sig"{sig=b}
 }
 if len(raw)==0||len(sig)!=ed25519.SignatureSize||!ed25519.Verify(pub,raw,sig){return m,nil,errors.New("bad manifest signature")}
 if e:=json.Unmarshal(raw,&m);e!=nil{return m,nil,e}
 if len(m.Changes)==0||len(m.Changes)>1000||m.Version==""||len(m.Version)>64||len(m.Notes)>10000{return m,nil,errors.New("invalid manifest")}
 seen:=map[string]bool{};expected:=map[string]bool{"manifest.json":true,"manifest.sig":true}
 for _,c:=range m.Changes{
  if !safePath(c.Path)||seen[c.Path]||!(c.Op=="replace"||c.Op=="add"||c.Op=="delete"){return m,nil,errors.New("invalid operation")}
  seen[c.Path]=true
  if c.Op!="add"&&len(c.Before)!=64{return m,nil,errors.New("missing base checksum")}
  if c.Op!="delete"{
   if len(c.After)!=64||digest(files["files/"+c.Path])!=c.After{return m,nil,errors.New("file checksum mismatch")}
   expected["files/"+c.Path]=true
  }
 }
 if len(expected)!=len(files){return m,nil,errors.New("unexpected patch files")}
 return m,files,nil
}
func apply(root,pkg,receipt string,pub ed25519.PublicKey)error{
 if len(pub)!=ed25519.PublicKeySize{return errors.New("missing trusted public key")}
 root,err:=filepath.Abs(root);if err!=nil{return err}
 if info,e:=os.Lstat(root);e!=nil||!info.IsDir()||info.Mode()&os.ModeSymlink!=0{return errors.New("install root not a directory")}
 m,files,err:=verifyZip(pkg,pub);if err!=nil{return err}
 for _,c:=range m.Changes {
  target,e:=safeTarget(root,c.Path);if e!=nil{return e}
  _,e=os.Lstat(target)
  if c.Op=="add" {if !os.IsNotExist(e){return fmt.Errorf("file exists: %s",c.Path)};continue}
  if e!=nil{return e}
  sum,e:=fileHash(target);if e!=nil||sum!=c.Before{return fmt.Errorf("base mismatch: %s",c.Path)}
 }
 backup,err:=os.MkdirTemp(root,".noe-rollback-");if err!=nil{return err}
 applied:=[]Change{}
 rollback:=func(){for i:=len(applied)-1;i>=0;i--{
  c:=applied[i];target,_:=safeTarget(root,c.Path);os.Remove(target)
  if c.Op!="add"{old:=filepath.Join(backup,filepath.FromSlash(c.Path));os.MkdirAll(filepath.Dir(target),0755);os.Rename(old,target)}
 }}
 for _,c:=range m.Changes {
  target,e:=safeTarget(root,c.Path);if e!=nil{rollback();return e}
  if c.Op!="add" {
   old:=filepath.Join(backup,filepath.FromSlash(c.Path))
   if e=os.MkdirAll(filepath.Dir(old),0700);e!=nil{rollback();return e}
   if e=os.Rename(target,old);e!=nil{rollback();return e}
  }
  applied=append(applied,c)
  if c.Op!="delete" {
   if e=os.MkdirAll(filepath.Dir(target),0755);e!=nil{rollback();return e}
   if e=os.WriteFile(target,files["files/"+c.Path],0755);e!=nil{rollback();return e}
   if got,e:=fileHash(target);e!=nil||got!=c.After{rollback();return errors.New("post-write checksum mismatch")}
  }
 }
 if receipt!="" {
  payload,_:=json.Marshal(map[string]string{"version":m.Version,"notes":m.Notes,"status":"installed"})
  if err=os.MkdirAll(filepath.Dir(receipt),0700);err!=nil{rollback();return err}
  if err=os.WriteFile(receipt,payload,0600);err!=nil{rollback();return err}
 }
 os.RemoveAll(backup)
 return nil
}
func main(){
 root:=flag.String("root","","writable installation directory")
 pkg:=flag.String("package","","signed .noepatch bundle")
 pub:=flag.String("public-key","","base64 trusted Ed25519 public key")
 receipt:=flag.String("receipt","","post-install notice path")
 restart:=flag.String("restart","","installed NoeEngine binary")
 wait:=flag.Int("wait-pid",0,"wait for editor to close")
 flag.Parse()
 key,err:=base64.StdEncoding.DecodeString(*pub);if err!=nil||len(key)!=ed25519.PublicKeySize{fmt.Fprintln(os.Stderr,"invalid public key");os.Exit(2)}
 if *root==""||*pkg==""{fmt.Fprintln(os.Stderr,"missing root or package");os.Exit(2)}
 if *wait>0{
  stopped:=false
  for i:=0;i<300;i++{
   p,e:=os.FindProcess(*wait);if e!=nil{stopped=true;break}
   if p.Signal(syscall.Signal(0))!=nil{stopped=true;break}
   time.Sleep(200*time.Millisecond)
  }
  if !stopped{fmt.Fprintln(os.Stderr,"editor did not exit; refusing to patch running process");os.Exit(1)}
 }
 if err:=apply(*root,*pkg,*receipt,ed25519.PublicKey(key));err!=nil{fmt.Fprintln(os.Stderr,"update failed:",err);os.Exit(1)}
 if *restart!=""{
  if !strings.HasPrefix(*restart,filepath.Clean(*root)+string(os.PathSeparator)){fmt.Fprintln(os.Stderr,"unsafe restart executable");os.Exit(2)}
  cmd:=exec.Command(*restart);cmd.Stdout=os.Stdout;cmd.Stderr=os.Stderr
  if err:=cmd.Start();err!=nil{fmt.Fprintln(os.Stderr,"restart failed:",err);os.Exit(1)}
 }
}
