#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

typedef enum signal {
  CONNECTION_DENIED = -1,
  CONNECTION_REQUEST = 991645,
  CONNECTION_APPROVED = 1058395
} Signal;

void run();
void sendData( int sockfd, int x );
void error( char *msg );
void processSignal(int data, int newsockfd);
int getData( int sockfd );
