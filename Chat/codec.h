#ifndef CHAT_CODEC_H_
#define CHAT_CODEC_H_

#include"../TcpConnection.h"
#include"../Buffer.h"
#include"../base/Logging.h"

class LengthHeaderCodec
{
public:
    typedef std::function<void(const TcpConnectionPtr&,
                               const std::string&)> StringMessageCallback;
    LengthHeaderCodec(const StringMessageCallback& cb)
    : cb_(cb)
    { }
    ~LengthHeaderCodec()=default;
    void onMessage(const TcpConnectionPtr& conn,Buffer* buf);
    void send(const TcpConnectionPtr& conn,const std::string& mess);
private:
    StringMessageCallback cb_;
    const static size_t kHeaderLen=sizeof(uint32_t);


};


void LengthHeaderCodec::onMessage(const TcpConnectionPtr& conn,Buffer* buf)
{
    while (buf->readableBytes()>=kHeaderLen)
    {
        const void* data=buf->peek();
        int32_t be32=*static_cast<const int32_t*>(data);
        const int32_t len=sockets::networkToHost32(be32);
        if(len<0||len>65536)
        {
            Log<<"Invalid length ";
            conn->shutdown();
            break;
        }
        else if (buf->readableBytes()>=len+kHeaderLen)
        {
            buf->retrieve(kHeaderLen);
            std::string message=buf->retrieveAsString(len);
            cb_(conn,message);
        }
        else
        {
            break;
        }
        
        
    }
    
}


// it only can be called in Tcpconnection's loop thread
void LengthHeaderCodec::send(const TcpConnectionPtr& conn,const std::string& mess)
{
    Buffer buf;
    buf.append(mess.data(),mess.size());
    uint32_t len=static_cast<uint32_t>(mess.size());
    uint32_t be32=sockets::hostToNetwork32(len);
    buf.prepend(&be32,sizeof be32);
    conn->send(&buf);
}








#endif