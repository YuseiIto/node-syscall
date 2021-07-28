#include <napi.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <utime.h>
#include <sys/wait.h>

#include "link.h"
#include "socket.h"
#include "process.h"
#include "files.h"
#include "stat.h"
#include "timesyscalls.h"

namespace node_syscall {

Napi::Number node_brk(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    uint32_t* end_data_segment=(uint32_t *)info[0].As<Napi::Number>().Uint32Value();
    
    return  Napi::Number::New(env,brk(end_data_segment));
}


Napi::Number node_fcntl(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int fd=info[0].As<Napi::Number>().Uint32Value();
    int cmd=info[1].As<Napi::Number>().Uint32Value();

    return  Napi::Number::New(env,fcntl(fd,cmd));
}

Napi::Value node_getrusage(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int who=info[0].As<Napi::Number>().Uint32Value();

    struct rusage rusage;
    int res = getrusage(who,&rusage );

    if (res!=0) return Napi::Number::New(env,res);

    Napi::Object rusage_obj = Napi::Object::New(env);

    rusage_obj.Set("ru_utime",timevalToTimeObj(&rusage.ru_utime,&env));
    rusage_obj.Set("ru_stime",timevalToTimeObj(&rusage.ru_stime,&env));
    rusage_obj.Set("ru_maxrss",Napi::Number::New(env,rusage.ru_maxrss));
    rusage_obj.Set("ru_ixrss",Napi::Number::New(env,rusage.ru_ixrss));
    rusage_obj.Set("ru_idrss",Napi::Number::New(env,rusage.ru_idrss));
    rusage_obj.Set("ru_isrss",Napi::Number::New(env,rusage.ru_isrss));
    rusage_obj.Set("ru_minflt",Napi::Number::New(env,rusage.ru_minflt));
    rusage_obj.Set("ru_majflt",Napi::Number::New(env,rusage.ru_majflt));
    rusage_obj.Set("ru_nswap",Napi::Number::New(env,rusage.ru_nswap));
    rusage_obj.Set("ru_inblock",Napi::Number::New(env,rusage.ru_inblock));
    rusage_obj.Set("ru_oublock",Napi::Number::New(env,rusage.ru_oublock));
    rusage_obj.Set("ru_msgsnd",Napi::Number::New(env,rusage.ru_msgsnd));
    rusage_obj.Set("ru_msgrcv",Napi::Number::New(env,rusage.ru_msgrcv));
    rusage_obj.Set("ru_nsignals",Napi::Number::New(env,rusage.ru_nsignals));
    rusage_obj.Set("ru_nvcsw",Napi::Number::New(env,rusage.ru_nvcsw));
    rusage_obj.Set("ru_nivcsw",Napi::Number::New(env,rusage.ru_nivcsw));

    return  rusage_obj;
}





Napi::Value node_pipe(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    int filedes[2];
    int res=pipe(filedes);

    if (res!=0) return Napi::Number::New(env,res);

    Napi::Array obj =Napi::Array::New(env);
    obj.Set(Napi::Number::New(env,0),Napi::Number::New(env,filedes[0]));
    obj.Set(Napi::Number::New(env,1),Napi::Number::New(env,filedes[1]));

    return obj;
}

Napi::Object node_wait(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    int status;
    pid_t pid = wait(&status);

    Napi::Object ret= Napi::Object::New(env);
    ret.Set("status",Napi::Number::New(env,status));
    ret.Set("pid",Napi::Number::New(env,pid));

    return ret;
}

Napi::Number node_errono(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    return Napi::Number::New(env,errno);
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{

    InitializeLinkSyscalls(&env,&exports); // symlinks
    InitializeFilesSyscalls(&env,&exports); // File operation
    InitializeStatSyscalls(&env,&exports);// stat
    InitializeTimesSyscalls(&env,&exports); // Time
    exports.Set(Napi::String::New(env, "brk"), Napi::Function::New(env, node_brk)); // Memory operation
    InitializeProcessSyscalls(&env,&exports); // Process
    InitializeSocketSyscalls(&env,&exports); // Socket
    // Pipe
    exports.Set(Napi::String::New(env, "pipe"), Napi::Function::New(env, node_pipe));
    exports.Set(Napi::String::New(env, "getrusage"), Napi::Function::New(env, node_getrusage));
    exports.Set(Napi::String::New(env, "errno"),  Napi::Function::New(env, node_errono));

    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace demo
