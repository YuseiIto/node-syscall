#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <napi.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace node_syscall{
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

    socklen_t addrlen =info[2].As<Napi::Number>().Uint32Value();

    for (uint32_t i=0;i<addrlen;i++){
        addr.sa_data[i]=addr_obj.Get("sa_data").As<Napi::Object>().Get(i).As<Napi::Number>().Uint32Value();
    }
    addr.sa_family=addr_obj.Get("sa_family").As<Napi::Number>().Uint32Value();

    return  Napi::Number::New(env,bind(sock,&addr,addrlen));
}

Napi::Number node_listen(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    int socketfd=info[0].As<Napi::Number>().Uint32Value();
    int backlog=info[1].As<Napi::Number>().Uint32Value();

    return Napi::Number::New(env,listen(socketfd,backlog));
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


void InitializeSocketSyscalls(Napi::Env* env,Napi::Object* exports){
    exports->Set(Napi::String::New(*env, "socket"), Napi::Function::New(*env, node_socket));
    exports->Set(Napi::String::New(*env, "accept"), Napi::Function::New(*env, node_accept));
    exports->Set(Napi::String::New(*env, "bind"), Napi::Function::New(*env, node_bind));
    exports->Set(Napi::String::New(*env, "listen"),  Napi::Function::New(*env, node_listen));
    exports->Set(Napi::String::New(*env, "connect"), Napi::Function::New(*env, node_connect));


    // Protocol family
    exports->Set(Napi::String::New(*env, "AF_UNIX"), Napi::Number::New(*env, AF_UNIX));
    exports->Set(Napi::String::New(*env, "AF_LOCAL"), Napi::Number::New(*env, AF_LOCAL));
    exports->Set(Napi::String::New(*env, "AF_INET"), Napi::Number::New(*env, AF_INET));
    exports->Set(Napi::String::New(*env, "AF_AX25"), Napi::Number::New(*env, AF_AX25));
    exports->Set(Napi::String::New(*env, "AF_IPX"), Napi::Number::New(*env, AF_IPX));
    exports->Set(Napi::String::New(*env, "AF_APPLETALK"), Napi::Number::New(*env, AF_APPLETALK));
    exports->Set(Napi::String::New(*env, "AF_X25"), Napi::Number::New(*env, AF_X25));
    exports->Set(Napi::String::New(*env, "AF_INET6"), Napi::Number::New(*env, AF_INET6));
    exports->Set(Napi::String::New(*env, "AF_DECnet"), Napi::Number::New(*env, AF_DECnet));
    exports->Set(Napi::String::New(*env, "AF_KEY"), Napi::Number::New(*env, AF_KEY));
    exports->Set(Napi::String::New(*env, "AF_NETLINK"), Napi::Number::New(*env, AF_NETLINK));
    exports->Set(Napi::String::New(*env, "AF_PACKET"), Napi::Number::New(*env, AF_PACKET));
    exports->Set(Napi::String::New(*env, "AF_RDS"), Napi::Number::New(*env, AF_RDS));
    exports->Set(Napi::String::New(*env, "AF_PPPOX"), Napi::Number::New(*env, AF_PPPOX));
    exports->Set(Napi::String::New(*env, "AF_LLC"), Napi::Number::New(*env, AF_LLC));
    exports->Set(Napi::String::New(*env, "AF_IB"), Napi::Number::New(*env, AF_IB));
    exports->Set(Napi::String::New(*env, "AF_MPLS"), Napi::Number::New(*env, AF_MPLS));
    exports->Set(Napi::String::New(*env, "AF_CAN"), Napi::Number::New(*env, AF_CAN));
    exports->Set(Napi::String::New(*env, "AF_TIPC"), Napi::Number::New(*env, AF_TIPC));
    exports->Set(Napi::String::New(*env, "AF_BLUETOOTH"), Napi::Number::New(*env, AF_BLUETOOTH));
    exports->Set(Napi::String::New(*env, "AF_ALG"), Napi::Number::New(*env, AF_ALG));
    exports->Set(Napi::String::New(*env, "AF_VSOCK"), Napi::Number::New(*env, AF_VSOCK));
    exports->Set(Napi::String::New(*env, "AF_KCM"), Napi::Number::New(*env, AF_KCM));
    exports->Set(Napi::String::New(*env, "AF_XDP"), Napi::Number::New(*env, AF_XDP));


    // Semantics
    exports->Set(Napi::String::New(*env, "SOCK_STREAM"), Napi::Number::New(*env, SOCK_STREAM));
    exports->Set(Napi::String::New(*env, "SOCK_DGRAM"), Napi::Number::New(*env, SOCK_DGRAM));
    exports->Set(Napi::String::New(*env, "SOCK_SEQPACKET"), Napi::Number::New(*env, SOCK_SEQPACKET));
    exports->Set(Napi::String::New(*env, "SOCK_RAW"), Napi::Number::New(*env, SOCK_RAW));
    exports->Set(Napi::String::New(*env, "SOCK_RDM"), Napi::Number::New(*env, SOCK_RDM));


    // Semantics behavior
    exports->Set(Napi::String::New(*env, "SOCK_NONBLOCK"), Napi::Number::New(*env, SOCK_NONBLOCK));
    exports->Set(Napi::String::New(*env, "SOCK_CLOEXDEC"), Napi::Number::New(*env, SOCK_CLOEXEC));


}
 
}

#endif
