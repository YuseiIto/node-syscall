#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <napi.h>


namespace node_syscall{

    Napi::Number node_fork(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    return  Napi::Number::New(env,fork());
}


void node_exit(const Napi::CallbackInfo& info){
    int exit_code=info[0].As<Napi::Number>().Uint32Value();
    exit(exit_code); 
}

Napi::Number node_kill(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    int pid=info[0].As<Napi::Number>().Uint32Value();
    int sig=info[1].As<Napi::Number>().Uint32Value();

    return Napi::Number::New(env,kill(pid,sig));
}

Napi::Number node_setsid(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    return Napi::Number::New(env,setsid());
}


Napi::Number node_getpid(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    pid_t pid=getpid();
    return  Napi::Number::New(env, pid);
}

Napi::Number node_getppid(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    pid_t pid=getppid();
    return  Napi::Number::New(env, pid);
}


void InitializeProcessSyscalls(Napi::Env* env,Napi::Object* exports){
    exports->Set(Napi::String::New(*env, "fork"), Napi::Function::New(*env, node_fork));
    exports->Set(Napi::String::New(*env, "kill"), Napi::Function::New(*env, node_kill));
    exports->Set(Napi::String::New(*env, "exit"), Napi::Function::New(*env, node_exit));
    exports->Set(Napi::String::New(*env, "setsid"), Napi::Function::New(*env, node_setsid));
    exports->Set(Napi::String::New(*env, "getpid"), Napi::Function::New(*env, node_getpid));
    exports->Set(Napi::String::New(*env, "getppid"), Napi::Function::New(*env, node_getppid));
 }
 
}

#endif
