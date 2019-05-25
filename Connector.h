#ifndef CONNECTOR_H_
#define CONNECTOR_H_

#include"base/noncopyable.h"
#include"net/InetAddress.h"
#include"TcpConnection.h"
class Channel;
class EventLoop;

class Connector : noncopyable
{
 public:
  typedef std::function<void (int sockfd)> NewConnectionCallback;

  Connector(EventLoop* loop, const InetAddress& serverAddr);
  ~Connector();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { connectioncallback_ = cb; }

  void start(); 
  void restart();  
  void stop();  
  const InetAddress& serverAddress() const { return serverAddr_; }

 private:
  enum States { kDisconnected, kConnecting, kConnected };
  
  void setState(States s) { state_ = s; }
  void startInLoop();
  void stopInLoop();
  void connect();
  void connecting(int sockfd);
  void handleWrite();
  void handleError();
  void retry(int sockfd);
  void resetChannelInLoop();
  int resetChannel();

  EventLoop* loop_;
  InetAddress serverAddr_;
  bool connect_; 
  States state_;  
  std::unique_ptr<Channel> channel_;
  NewConnectionCallback connectioncallback_;
  int retryDelayMs_;
};

#endif