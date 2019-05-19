#ifndef NET_SOCKET_H_
#define NET_SOCKET_H_


#include"../base/noncopyable.h"
#include<endian.h>
#include<stdint.h>
#include<sys/socket.h>
#include<errno.h>
#include<arpa/inet.h>

class InetAddress;

class Socket:noncopyable
{
public:
    explicit Socket(int sockfd)
    :sockfd_(sockfd)
    { }
    ~Socket();

    int fd() const
    {
        return sockfd_;
    }
    bool getTcpInfo();

    void bindAddress(const InetAddress &localaddr);

    int accept(InetAddress* peeraddr);

    void shutdownWrite();

    void setTcpNodelay(bool on);

    void serReuseAddr(bool one);

    void setKeepAlive(bool on);
private:
    const int sockfd_;

};


namespace sockets
{
    
    inline uint64_t hostToNetwork64(uint64_t host64)
    {
        return htobe64(host64);
    }
    inline uint32_t hostToNetwork64(uint32_t host32)
    {
        return htobe32(host32);
    }
    inline uint16_t hostToNetwork16(uint16_t host16)
    {
        return htobe16(host16);
    }




    inline uint64_t networkToHost64(uint64_t net64t)
    {
        return be64toh(net64t);
    }

    inline uint32_t networkToHost64(uint32_t net32t)
    {
        return be32toh(net32t);
    }
    inline uint16_t networkToHost16(uint16_t net16t)
    {
        return be16toh(net16t);
    }

    const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);

    struct sockaddr* sockaddr_cast(struct sockaddr_in* addr);

    int createNonblocking(sa_family_t familt);

    void Bind(int sockfd,const struct sockaddr* addr);

    void Listen(int sockfd);

    int Accept(int sockfd,struct sockaddr* addr);

    int Connet(int sockfd,const struct sockaddr* addr);

    void socket_close(int sockfd);

    void shutdownWrite(int sockfd);

    struct sockaddr_in* getLocalAddr(int sockfd);

    struct sockaddr_in* getPeerAddr(int sockfd);
} // namespace sockets



#endif