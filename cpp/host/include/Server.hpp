#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "../../tracking/include/display/VideoPlayer.hpp"
#include <string>
#include <memory>
#include <iostream>

typedef enum command {
  START = 1,
  STOP = 0
} Command;

typedef enum signal {
  DENIED = -1,
  CONNECTION_REQUEST = 991645,
  CONNECTION_APPROVED = 1058395,
  INITIATE_TRACKING = 7405245,
  TRACKING_INITIATED = 8135084,
  STOP_TRACKING = 6372947,
  STOPPING_TRACKING = 335920,
  GET_TRACKING_DATA = 4093754
} Signal;

template<typename T, typename... Args>
  std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class Server
{
  public:
    void run();

  private:
    std::unique_ptr<VideoPlayer> player;
    bool isRunning;

    void sendData( int sockfd, int x );
    void error( std::string msg );
    void processSignal(int data, int newsockfd);
    int getData( int sockfd );
    void tracking(Command c);
    void track();
    void reply(int sockfd, int reply);
};
