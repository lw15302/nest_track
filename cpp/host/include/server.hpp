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
#include <iostream>

typedef enum signal {
  DENIED = -1,
  CONNECTION_REQUEST = 991645,
  CONNECTION_APPROVED = 1058395,
  INITIATE_TRACKING = 7405245,
  TRACKING_INITIATED = 8135084
} Signal;

void run();
void sendData( int sockfd, int x );
void error( std::string msg );
void processSignal(int data, int newsockfd);
int getData( int sockfd );
void initiateTracking();
