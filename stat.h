#ifndef _STAT_H_
#define _STAT_H_

#include <napi.h>


namespace node_syscall{


Napi::Object statToStatObj(struct stat* stat,Napi::Env *env){
    
    Napi::Object obj = Napi::Object::New(*env);
    obj.Set("st_dev",Napi::Number::New(*env,stat->st_dev));
    obj.Set("st_ino",Napi::Number::New(*env,stat->st_ino));
    obj.Set("st_mode",Napi::Number::New(*env,stat->st_mode));
    obj.Set("st_nlink",Napi::Number::New(*env,stat->st_nlink));
    obj.Set("st_uid",Napi::Number::New(*env,stat->st_uid));
    obj.Set("st_gid",Napi::Number::New(*env,stat->st_gid));
    obj.Set("st_rdev",Napi::Number::New(*env,stat->st_rdev));
    obj.Set("st_size",Napi::Number::New(*env,stat->st_size));
    obj.Set("st_blksize",Napi::Number::New(*env,stat->st_blksize));
    obj.Set("st_blocks",Napi::Number::New(*env,stat->st_blocks));
    obj.Set("st_atime",Napi::Number::New(*env,stat->st_atime));
    obj.Set("st_mtime",Napi::Number::New(*env,stat->st_mtime));
    obj.Set("st_ctime",Napi::Number::New(*env,stat->st_ctime));

    return obj;
}

Napi::Object node_stat(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    const char* path = info[0].As<Napi::String>().Utf8Value().c_str();
    struct stat buf;
    stat(path,&buf);
    return  statToStatObj(&buf,&env);
}


Napi::Object node_lstat(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    const char* path = info[0].As<Napi::String>().Utf8Value().c_str();

    struct stat buf;
    lstat(path,&buf);
    return  statToStatObj(&buf,&env);
}

Napi::Object node_fstat(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    long fd = info[0].As<Napi::Number>().Uint32Value();

    struct stat buf;
    fstat(fd,&buf);
    
    return  statToStatObj(&buf,&env);
}

void InitializeStatSyscalls(Napi::Env* env,Napi::Object* exports){
    exports->Set(Napi::String::New(*env, "stat"), Napi::Function::New(*env, node_stat));
    exports->Set(Napi::String::New(*env, "lstat"), Napi::Function::New(*env, node_lstat));
    exports->Set(Napi::String::New(*env, "fstat"),  Napi::Function::New(*env, node_fstat));
}
 
}

#endif
