#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "connect_NativeClient.h"

typedef enum signal {
  CONNECTION_DENIED = -1,
  CONNECTION_REQUEST = 991645,
  CONNECTION_APPROVED = 1058395
  INITIATE_TRACKING = 7405245;
  TRACKING_INITIATED = 8135084;
} Signal;

typedef enum bool {
  FALSE,
  TRUE
} Bool;

void error(char *msg);
void sendData( int sockfd, int x );
int getData( int sockfd );
Bool connectionCheck(int sockfd);
void maintainConnection();
