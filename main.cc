#include <napi.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

namespace node_syscall {

Napi::Number node_getpid(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    pid_t pid=getpid();
    return  Napi::Number::New(env, pid);
}

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


Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "getpid"), 
                Napi::Function::New(env, node_getpid));
    exports.Set(Napi::String::New(env, "read"), 
                Napi::Function::New(env, node_read));
    exports.Set(Napi::String::New(env, "write"), 
                Napi::Function::New(env, node_write));
    exports.Set(Napi::String::New(env, "open"), 
                Napi::Function::New(env, node_open));

    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace demo
