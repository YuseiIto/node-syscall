#ifndef _FILES_H_
#define _FILES_H_

#include <napi.h>


namespace node_syscall{

Napi::Value node_read(const Napi::CallbackInfo& info)
{
    // read(fd,count) - > size

    Napi::Env env = info.Env();
    long fd= info[0].As<Napi::Number>().Uint32Value();
    long count= info[1].As<Napi::Number>().Uint32Value();

    uint8_t * buf=(uint8_t*) malloc(count);
    size_t len=read(fd,buf,count);


    return  Napi::Buffer<uint8_t>::Copy(env,buf,len);

}

Napi::Number node_write(const Napi::CallbackInfo& info)
{
    // open(path,mode) -> fd
    Napi::Env env = info.Env();
    long fd= info[0].As<Napi::Number>().Uint32Value();
    uint8_t* buf= (uint8_t *) (info[1].As<Napi::ArrayBuffer>().Data());
    long len = info[0].As<Napi::Number>().Uint32Value();

    return  Napi::Number::New(env,write(fd,buf,len));

}

Napi::Number node_open(const Napi::CallbackInfo& info)
{
    // open(path,mode) -> fd
    Napi::Env env = info.Env();
    std::string pathname_str= info[0].As<Napi::String>().Utf8Value();
    const char* pathname=pathname_str.c_str();
    mode_t mode = info[1].As<Napi::Number>().Uint32Value();
    return  Napi::Number::New(env,open(pathname,mode));

}

Napi::Number node_close(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    long fd= info[0].As<Napi::Number>().Uint32Value();
    return  Napi::Number::New(env,close(fd));

}

Napi::Number node_lseek(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    long fields=info[0].As<Napi::Number>().Uint32Value();
    long offset=info[1].As<Napi::Number>().Uint32Value();
    long whence=info[2].As<Napi::Number>().Uint32Value();
    return  Napi::Number::New(env,lseek(fields,offset,whence));
}

Napi::Number node_chdir(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    const char* path=info[0].As<Napi::String>().Utf8Value().c_str();
    return  Napi::Number::New(env,chdir(path));
}

Napi::Number node_fchdir(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int fd=info[0].As<Napi::Number>().Uint32Value();
    return  Napi::Number::New(env,fchdir(fd));
}


Napi::Number node_mkdir(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    const char* pathname=info[0].As<Napi::String>().Utf8Value().c_str();
    mode_t mode=info[1].As<Napi::Number>().Uint32Value();

    return Napi::Number::New(env,mkdir(pathname,mode));
}


Napi::Number node_rename(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    const char* oldpath = info[0].As<Napi::String>().Utf8Value().c_str();
    const char* newpath = info[1].As<Napi::String>().Utf8Value().c_str();

    return Napi::Number::New(env,rename(oldpath,newpath));
}

Napi::Number node_rmdir(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    const char* pathname = info[0].As<Napi::String>().Utf8Value().c_str();

    return Napi::Number::New(env,rmdir(pathname));
}


Napi::Number node_chmod(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    const char* path=info[0].As<Napi::String>().Utf8Value().c_str();
    int mode=info[1].As<Napi::Number>().Uint32Value();
    return  Napi::Number::New(env,chmod(path,mode));
}

Napi::Number node_fchmod(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int fd=info[0].As<Napi::Number>().Uint32Value();
    int mode=info[1].As<Napi::Number>().Uint32Value();
    
    return  Napi::Number::New(env,fchmod(fd,mode));
}

Napi::Number node_chown(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    const char* path=info[0].As<Napi::String>().Utf8Value().c_str();
    int owner=info[1].As<Napi::Number>().Uint32Value();
    int group=info[2].As<Napi::Number>().Uint32Value();

    return  Napi::Number::New(env,chown(path,owner,group));
}

Napi::Number node_fchown(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int fd=info[0].As<Napi::Number>().Uint32Value();
    int owner=info[1].As<Napi::Number>().Uint32Value();
    int group=info[2].As<Napi::Number>().Uint32Value();
    
    return  Napi::Number::New(env,fchown(fd,owner,group));
}

Napi::Number node_dup(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int oldfd=info[0].As<Napi::Number>().Uint32Value();
    
    return  Napi::Number::New(env,dup(oldfd));
}

Napi::Number node_dup2(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int oldfd=info[0].As<Napi::Number>().Uint32Value();
    int newfd=info[1].As<Napi::Number>().Uint32Value();

    return  Napi::Number::New(env,dup2(oldfd,newfd));
}

Napi::Number node_poll(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();

    Napi::Array node_fds = info[0].As<Napi::Array>();
    long nfds=info[1].As<Napi::Number>().Uint32Value();
    long timeout=info[2].As<Napi::Number>().Uint32Value();
    
    pollfd* fds=(pollfd*) malloc(sizeof(pollfd)*nfds);

    for (long i =0; i<nfds;i++){
        Napi::Object e = node_fds.Get(i).As<Napi::Object>();
        pollfd pfd;
        pfd.fd=e.Get("fd").As<Napi::Number>().Uint32Value();
        pfd.fd=e.Get("events").As<Napi::Number>().Uint32Value();
        pfd.fd=e.Get("revents").As<Napi::Number>().Uint32Value();
        fds[i]=pfd;
    }
    return  Napi::Number::New(env,poll(fds,nfds,timeout));
}

void InitializeFilesSyscalls(Napi::Env* env,Napi::Object* exports){
    exports->Set(Napi::String::New(*env, "read"), Napi::Function::New(*env, node_read));
    exports->Set(Napi::String::New(*env, "write"), Napi::Function::New(*env, node_write));
    exports->Set(Napi::String::New(*env, "open"), Napi::Function::New(*env, node_open));
    exports->Set(Napi::String::New(*env, "close"), Napi::Function::New(*env, node_close));
    exports->Set(Napi::String::New(*env, "lseek"), Napi::Function::New(*env, node_lseek));
    exports->Set(Napi::String::New(*env, "chdir"), Napi::Function::New(*env, node_chdir));
    exports->Set(Napi::String::New(*env, "fchdir"), Napi::Function::New(*env, node_fchdir));
    exports->Set(Napi::String::New(*env, "mkdir"), Napi::Function::New(*env, node_mkdir));
    exports->Set(Napi::String::New(*env, "rename"), Napi::Function::New(*env, node_rename));
    exports->Set(Napi::String::New(*env, "rmdir"), Napi::Function::New(*env, node_rmdir));
    exports->Set(Napi::String::New(*env, "chmod"), Napi::Function::New(*env, node_chmod));
    exports->Set(Napi::String::New(*env, "fchmod"), Napi::Function::New(*env, node_fchmod));
    exports->Set(Napi::String::New(*env, "chown"), Napi::Function::New(*env, node_chown));
    exports->Set(Napi::String::New(*env, "fchown"), Napi::Function::New(*env, node_fchown));
    exports->Set(Napi::String::New(*env, "dup"), Napi::Function::New(*env, node_dup));
    exports->Set(Napi::String::New(*env, "dup2"), Napi::Function::New(*env, node_dup2));
    exports->Set(Napi::String::New(*env, "poll"), Napi::Function::New(*env, node_poll));
 }
 
}

#endif
