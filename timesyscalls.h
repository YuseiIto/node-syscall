#ifndef _TIMESYSCALLS_H_
#define _TIMESYSCALLS_H_

#include <napi.h>


namespace node_syscall{

Napi::Object timevalToTimeObj(struct timeval* timeval,Napi::Env* env){
    Napi::Object res = Napi::Object::New(*env);
    res.Set("tv_sec",Napi::Number::New(*env,timeval->tv_sec));
    res.Set("tv_usec",Napi::Number::New(*env,timeval->tv_usec));

    return res;
}

Napi::Object timezoneToTimezoneObj(struct timezone *tz,Napi::Env* env){
    Napi::Object tz_obj = Napi::Object::New(*env);
    tz_obj.Set("tz_minuteswest",Napi::Number::New(*env,tz->tz_minuteswest));
    tz_obj.Set("tz_dsttime",Napi::Number::New(*env,tz->tz_dsttime));   
    return tz_obj;
}

Napi::Value node_gettimeofday(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    struct timeval tv;
    struct timezone tz;

    int res=gettimeofday(&tv,&tz);

    if (res!=0) return Napi::Number::New(env,res);

    Napi::Object ret= Napi::Object::New(env);
    ret.Set("tv",timevalToTimeObj(&tv,&env));
    ret.Set("tz",timezoneToTimezoneObj(&tz,&env));

    return ret;
}

Napi::Number node_time(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    return Napi::Number::New(env,time(nullptr));
}

Napi::Number node_utime(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    const char* filename = info[0].As<Napi::String>().Utf8Value().c_str();

    Napi::Object arg=info[1].As<Napi::Object>();
    
    struct utimbuf  ut;
    ut.actime=arg.Get("actime").As<Napi::Number>().Uint32Value();
    ut.modtime=arg.Get("modtime").As<Napi::Number>().Uint32Value();

    return Napi::Number::New(env,utime(filename,&ut));
}

void InitializeTimesSyscalls(Napi::Env* env,Napi::Object* exports){
    exports->Set(Napi::String::New(*env, "gettimeofday"), Napi::Function::New(*env, node_gettimeofday));
    exports->Set(Napi::String::New(*env, "time"), Napi::Function::New(*env, node_time));
    exports->Set(Napi::String::New(*env, "utime"), Napi::Function::New(*env, node_utime));
}
 
}

#endif
