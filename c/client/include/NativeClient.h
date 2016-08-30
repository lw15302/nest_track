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

#define DATA_SIZE 1000

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

typedef enum bool {
  FALSE,
  TRUE
} Bool;


jintArray convertDataPacket(int* originalData, JNIEnv* env);
Bool connectionCheck(int sockfd);
int getReply( int sockfd );
int setUpConnection(jobjectArray ip, JNIEnv * env);
int* getData(int sockfd);
char* getIpIndex(int index, jobjectArray ip, JNIEnv* env);
void maintainConnection();
void getIp(char *serverIp, jobjectArray ip, JNIEnv* env);
void sendData( int sockfd, int x );
void convertBuffer(char* buffer[DATA_SIZE], int* dataSet);
void allocateBuffer(char* buffer[DATA_SIZE]);
void freeBuffer(char* buffer[DATA_SIZE]);
