#ifndef _LINK_H_
#define _LINK_H_

#include <napi.h>


namespace node_syscall{

 Napi::Number node_link(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    const char* oldpath = info[0].As<Napi::String>().Utf8Value().c_str();
    const char* newpath = info[1].As<Napi::String>().Utf8Value().c_str();

    return Napi::Number::New(env,link(oldpath,newpath));
}


Napi::Number node_symlink(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    const char* oldpath = info[0].As<Napi::String>().Utf8Value().c_str();
    const char* newpath = info[1].As<Napi::String>().Utf8Value().c_str();

    return Napi::Number::New(env,symlink(oldpath,newpath));
}

Napi::Value node_readlink(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    const char* path = info[0].As<Napi::String>().Utf8Value().c_str();
    size_t bufsiz=info[1].As<Napi::Number>().Uint32Value();

    char* buf=(char*)malloc(bufsiz);

    ssize_t res=readlink(path,buf,bufsiz);
    if (res==-1) return Napi::Number::New(env,errno);

    return Napi::Buffer<char>::Copy(env,buf,res);
}


Napi::Number node_unlink(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    const char* pathname = info[0].As<Napi::String>().Utf8Value().c_str();    
    return Napi::Number::New(env,unlink(pathname));
}


void InitializeLinkSyscalls(Napi::Env* env,Napi::Object* exports){
    exports->Set(Napi::String::New(*env, "link"), Napi::Function::New(*env, node_link));
    exports->Set(Napi::String::New(*env, "symlink"), Napi::Function::New(*env, node_symlink));
    exports->Set(Napi::String::New(*env, "readlink"),  Napi::Function::New(*env, node_readlink));
    exports->Set(Napi::String::New(*env, "unlink"),  Napi::Function::New(*env, node_unlink));
 }
 
}

#endif
