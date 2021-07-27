#include <napi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>

namespace node_syscall {

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

    struct stat* buf;
    stat(path,buf);

    return  statToStatObj(buf,&env);
}


Napi::Object node_lstat(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    const char* path = info[0].As<Napi::String>().Utf8Value().c_str();

    struct stat* buf;
    lstat(path,buf);
    return  statToStatObj(buf,&env);
}

Napi::Object node_fstat(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    long fd = info[0].As<Napi::Number>().Uint32Value();

    struct stat* buf;
    fstat(fd,buf);
    
    return  statToStatObj(buf,&env);
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

Napi::Number node_socket(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int domain=info[0].As<Napi::Number>().Uint32Value();
    int type=info[1].As<Napi::Number>().Uint32Value();
    int protocol=info[2].As<Napi::Number>().Uint32Value();

    return  Napi::Number::New(env,socket(domain,type,protocol));
}

Napi::Object node_accept(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int socketfd=info[0].As<Napi::Number>().Uint32Value();
    struct sockaddr addr;
    socklen_t addlen;

    accept(socketfd,&addr,&addlen);
    
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("sa_family",Napi::Number::New(env,addr.sa_family));
    obj.Set("sa_data",Napi::Buffer<char>::Copy(env,addr.sa_data,14));

    return  obj;
}

Napi::Number node_bind(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int sock=info[0].As<Napi::Number>().Uint32Value();

    Napi::Object addr_obj = info[1].As<Napi::Object>();
    struct sockaddr addr;

    for (int i=0;i<14;i++){
        addr.sa_data[i]=addr_obj.Get("sa_data").As<Napi::Object>().Get(i).As<Napi::Number>().Uint32Value();
    }
    
    addr.sa_family=addr_obj.Get("sa_family").As<Napi::Number>().Uint32Value();

    socklen_t addrlen =info[2].As<Napi::Number>().Uint32Value();

    return  Napi::Number::New(env,bind(sock,&addr,addrlen));
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

Napi::Number node_connect(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int fd=info[0].As<Napi::Number>().Uint32Value();
    
    Napi::Object addr_obj = info[1].As<Napi::Object>();
    struct sockaddr addr;
    for (int i=0;i<14;i++){
        addr.sa_data[i]=addr_obj.Get("sa_data").As<Napi::Object>().Get(i).As<Napi::Number>().Uint32Value();
    }

    socklen_t addrlen=info[2].As<Napi::Number>().Uint32Value();
    
    return  Napi::Number::New(env,connect(fd,&addr,addrlen));
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

Napi::Number node_fcntl(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    int fd=info[0].As<Napi::Number>().Uint32Value();
    int cmd=info[1].As<Napi::Number>().Uint32Value();

    return  Napi::Number::New(env,fcntl(fd,cmd));
}

Napi::Number node_fork(const Napi::CallbackInfo& info){

    Napi::Env env = info.Env();
    return  Napi::Number::New(env,fork());
}

Napi::Object timevalToTimeObj(struct timeval* timeval,Napi::Env* env){
    Napi::Object res = Napi::Object::New(*env);
    res.Set("tv_sec",Napi::Number::New(*env,timeval->tv_sec));
    res.Set("tv_usec",Napi::Number::New(*env,timeval->tv_usec));

    return res;
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


Napi::Number node_kill(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    int pid=info[0].As<Napi::Number>().Uint32Value();
    int sig=info[1].As<Napi::Number>().Uint32Value();

    return Napi::Number::New(env,kill(pid,sig));
}

Napi::Number node_listen(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    int socketfd=info[0].As<Napi::Number>().Uint32Value();
    int backlog=info[1].As<Napi::Number>().Uint32Value();

    return Napi::Number::New(env,listen(socketfd,backlog));
}

Napi::Number node_mkdir(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    const char* pathname=info[0].As<Napi::String>().Utf8Value().c_str();
    mode_t mode=info[1].As<Napi::Number>().Uint32Value();

    return Napi::Number::New(env,mkdir(pathname,mode));
}

Napi::Number node_link(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    const char* oldpath = info[0].As<Napi::String>().Utf8Value().c_str();
    const char* newpath = info[1].As<Napi::String>().Utf8Value().c_str();

    return Napi::Number::New(env,link(oldpath,newpath));
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

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "getpid"), 
                Napi::Function::New(env, node_getpid));
    exports.Set(Napi::String::New(env, "getppid"), 
                Napi::Function::New(env, node_getppid));
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
    exports.Set(Napi::String::New(env, "socket"), 
          Napi::Function::New(env, node_socket));
    exports.Set(Napi::String::New(env, "accept"), 
          Napi::Function::New(env, node_accept));
    exports.Set(Napi::String::New(env, "bind"), 
          Napi::Function::New(env, node_bind));
    exports.Set(Napi::String::New(env, "chdir"), 
          Napi::Function::New(env, node_chdir));
    exports.Set(Napi::String::New(env, "fchdir"), 
          Napi::Function::New(env, node_fchdir));
    exports.Set(Napi::String::New(env, "chmod"), 
          Napi::Function::New(env, node_chmod));
    exports.Set(Napi::String::New(env, "fchmod"), 
          Napi::Function::New(env, node_fchmod));
    exports.Set(Napi::String::New(env, "chown"), 
          Napi::Function::New(env, node_chown));
    exports.Set(Napi::String::New(env, "fchown"), 
          Napi::Function::New(env, node_fchown));
    exports.Set(Napi::String::New(env, "connect"), 
          Napi::Function::New(env, node_connect));
    exports.Set(Napi::String::New(env, "dup"), 
          Napi::Function::New(env, node_dup));
    exports.Set(Napi::String::New(env, "dup2"), 
          Napi::Function::New(env, node_dup2));
    exports.Set(Napi::String::New(env, "fork"), 
          Napi::Function::New(env, node_fork));
    exports.Set(Napi::String::New(env, "getrusage"), 
          Napi::Function::New(env, node_getrusage));
    exports.Set(Napi::String::New(env, "gettimeofday"), 
          Napi::Function::New(env, node_gettimeofday));
    exports.Set(Napi::String::New(env, "kill"), 
          Napi::Function::New(env, node_kill));
    exports.Set(Napi::String::New(env, "link"), 
          Napi::Function::New(env, node_link));
    exports.Set(Napi::String::New(env, "listen"), 
          Napi::Function::New(env, node_listen));
    exports.Set(Napi::String::New(env, "mkdir"), 
          Napi::Function::New(env, node_mkdir));
    exports.Set(Napi::String::New(env, "pipe"), 
          Napi::Function::New(env, node_pipe));
    
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace demo
