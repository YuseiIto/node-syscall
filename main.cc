#include <napi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <poll.h>

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

Napi::Number node_close(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    long fd= info[0].As<Napi::Number>().Uint32Value();
    return  Napi::Number::New(env,close(fd));

}

Napi::Object node_stat(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string path = info[0].As<Napi::String>().Utf8Value();

    struct stat* buf;
    stat(path.c_str(),buf);
    
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("st_dev",Napi::Number::New(env,buf->st_dev));
    obj.Set("st_ino",Napi::Number::New(env,buf->st_ino));
    obj.Set("st_mode",Napi::Number::New(env,buf->st_mode));
    obj.Set("st_nlink",Napi::Number::New(env,buf->st_nlink));
    obj.Set("st_uid",Napi::Number::New(env,buf->st_uid));
    obj.Set("st_gid",Napi::Number::New(env,buf->st_gid));
    obj.Set("st_rdev",Napi::Number::New(env,buf->st_rdev));
    obj.Set("st_size",Napi::Number::New(env,buf->st_size));
    obj.Set("st_blksize",Napi::Number::New(env,buf->st_blksize));
    obj.Set("st_blocks",Napi::Number::New(env,buf->st_blocks));
    obj.Set("st_atime",Napi::Number::New(env,buf->st_atime));
    obj.Set("st_mtime",Napi::Number::New(env,buf->st_mtime));
    obj.Set("st_ctime",Napi::Number::New(env,buf->st_ctime));

    return  obj;
}


Napi::Object node_lstat(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string path = info[0].As<Napi::String>().Utf8Value();

    struct stat* buf;
    lstat(path.c_str(),buf);
    
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("st_dev",Napi::Number::New(env,buf->st_dev));
    obj.Set("st_ino",Napi::Number::New(env,buf->st_ino));
    obj.Set("st_mode",Napi::Number::New(env,buf->st_mode));
    obj.Set("st_nlink",Napi::Number::New(env,buf->st_nlink));
    obj.Set("st_uid",Napi::Number::New(env,buf->st_uid));
    obj.Set("st_gid",Napi::Number::New(env,buf->st_gid));
    obj.Set("st_rdev",Napi::Number::New(env,buf->st_rdev));
    obj.Set("st_size",Napi::Number::New(env,buf->st_size));
    obj.Set("st_blksize",Napi::Number::New(env,buf->st_blksize));
    obj.Set("st_blocks",Napi::Number::New(env,buf->st_blocks));
    obj.Set("st_atime",Napi::Number::New(env,buf->st_atime));
    obj.Set("st_mtime",Napi::Number::New(env,buf->st_mtime));
    obj.Set("st_ctime",Napi::Number::New(env,buf->st_ctime));

    return  obj;
}

Napi::Object node_fstat(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    long fd = info[0].As<Napi::Number>().Uint32Value();

    struct stat* buf;
    fstat(fd,buf);
    
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("st_dev",Napi::Number::New(env,buf->st_dev));
    obj.Set("st_ino",Napi::Number::New(env,buf->st_ino));
    obj.Set("st_mode",Napi::Number::New(env,buf->st_mode));
    obj.Set("st_nlink",Napi::Number::New(env,buf->st_nlink));
    obj.Set("st_uid",Napi::Number::New(env,buf->st_uid));
    obj.Set("st_gid",Napi::Number::New(env,buf->st_gid));
    obj.Set("st_rdev",Napi::Number::New(env,buf->st_rdev));
    obj.Set("st_size",Napi::Number::New(env,buf->st_size));
    obj.Set("st_blksize",Napi::Number::New(env,buf->st_blksize));
    obj.Set("st_blocks",Napi::Number::New(env,buf->st_blocks));
    obj.Set("st_atime",Napi::Number::New(env,buf->st_atime));
    obj.Set("st_mtime",Napi::Number::New(env,buf->st_mtime));
    obj.Set("st_ctime",Napi::Number::New(env,buf->st_ctime));

    return  obj;
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

Napi::Number node_lseek(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    long fields=info[0].As<Napi::Number>().Uint32Value();
    long offset=info[1].As<Napi::Number>().Uint32Value();
    long whence=info[2].As<Napi::Number>().Uint32Value();
    return  Napi::Number::New(env,lseek(fields,offset,whence));
}

Napi::Number node_brk(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    void* end_data_segment=(void *)info[0].As<Napi::Number>().Uint32Value();
    
    return  Napi::Number::New(env,brk(end_data_segment));
}

void node_exit(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int exit_code=info[0].As<Napi::Number>().Uint32Value();
    
    exit(exit_code); 
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
    exports.Set(Napi::String::New(env, "close"), 
          Napi::Function::New(env, node_close));
    exports.Set(Napi::String::New(env, "stat"), 
          Napi::Function::New(env, node_stat));
    exports.Set(Napi::String::New(env, "lstat"), 
          Napi::Function::New(env, node_lstat));
    exports.Set(Napi::String::New(env, "fstat"), 
          Napi::Function::New(env, node_fstat));
    exports.Set(Napi::String::New(env, "poll"), 
          Napi::Function::New(env, node_poll));
    exports.Set(Napi::String::New(env, "lseek"), 
          Napi::Function::New(env, node_lseek));
    exports.Set(Napi::String::New(env, "brk"), 
          Napi::Function::New(env, node_brk));
    exports.Set(Napi::String::New(env, "exit"), 
          Napi::Function::New(env, node_exit));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace demo
