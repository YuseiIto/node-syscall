#include <napi.h>
#include <sys/types.h>
#include <unistd.h>

namespace node_syscall {

Napi::Number node_getpid(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    pid_t pid=getpid();
    return  Napi::Number::New(env, pid);
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "getpid"), 
                Napi::Function::New(env, node_getpid));

    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace demo
