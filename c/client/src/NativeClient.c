#include "../include/NativeClient.h"

JNIEXPORT jboolean JNICALL Java_connect_NativeClient_connect(JNIEnv *env, jobject obj, jobjectArray ip)
{
  int sockfd = setUpConnection(ip, env);
  if(sockfd == -1) return FALSE;

  if(connectionCheck(sockfd)) {
    printf("\nConnection established");
    close(sockfd);
    return TRUE;
  }
  else {
    printf("\nConnection failed");
    close( sockfd );
    return FALSE;
  }
}


JNIEXPORT jboolean JNICALL Java_connect_NativeClient_track(JNIEnv *env, jobject obj, jobjectArray ip)
{
  int sockfd = setUpConnection(ip, env);
  if(sockfd == -1) return FALSE;
  int data;

  sendData(sockfd, INITIATE_TRACKING);
  printf("\nSent request to start tracking: %d", INITIATE_TRACKING);
  data = getReply( sockfd );
  printf("\nReceived reply: %d", data);

  if(data = TRACKING_INITIATED) {
    printf("\nTracking initiated");
    close(sockfd);
    return TRUE;
  }
  else {
    printf("\nFailed to initiate tracking");
    close(sockfd);
    return FALSE;
  }
}


JNIEXPORT jboolean JNICALL Java_connect_NativeClient_stopTrack(JNIEnv *env, jobject obj, jobjectArray ip)
{
  int sockfd = setUpConnection(ip, env);
  if(sockfd == -1) return FALSE;
  int data;

  sendData(sockfd, STOP_TRACKING);
  printf("\nSent request to stop tracking: %d", STOP_TRACKING );
  data = getReply(sockfd);
  printf("\nReceived reply: %d", data);

  if(data = STOPPING_TRACKING) {
    printf("\nTracking stopped");
    close(sockfd);
    return TRUE;
  }
  else {
    printf("\nFailed to stop tracking");
    close(sockfd);
    return FALSE;
  }
}

JNIEXPORT jintArray JNICALL Java_connect_NativeClient_getReply(JNIEnv *env, jobject obj, jobjectArray ip)
{
  int sockfd = setUpConnection(ip, env);
  if(sockfd == -1) return (jintArray)-1;
  int* data = (int*)malloc(DATA_SIZE * sizeof(int*));

  sendData(sockfd, GET_TRACKING_DATA);
  printf("\nSent request to get data: %d", GET_TRACKING_DATA);
  sleep(500);
  data = getData(sockfd);

  jintArray dataPacket = convertDataPacket(data, env);
  free(data);
  return dataPacket;
}


int setUpConnection(jobjectArray ip, JNIEnv* env)
{
  int sockfd, portno = 51717, n;
  char* serverIp = (char *)malloc(20 * sizeof(char));
  getIp(serverIp, ip, env);
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[256];
  int data;

  setbuf(stdout, NULL);
  if( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) {
    fprintf(stderr, "ERROR opening socket\n");
    free(serverIp);
    return -1;
  }

  if( ( server = gethostbyname( serverIp ) ) == NULL ) {
    fprintf(stderr, "ERROR no such host\n");
    free(serverIp);
    return -1;
  }

  bzero( (char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;

  bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
    fprintf(stderr, "ERROR connecting" );
    free(serverIp);
    return -1;
  }
  free(serverIp);
  return sockfd;
}


void sendData( int sockfd, int x )
{
  int n;

  char buffer[32];
  sprintf( buffer, "%d\n", x );
  if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
      error( "ERROR writing to socket" );
  buffer[n] = '\0';
}


int getReply( int sockfd )
{
  char buffer[32];
  int n;

  if ( (n = read(sockfd,buffer,31) ) < 0 )
       error(  "ERROR reading from socket" );
  buffer[n] = '\0';
  return atoi( buffer );
}


int* getData(int sockfd)
{
  char* buffer[DATA_SIZE];
  int* dataSet = (int*)malloc(DATA_SIZE * sizeof(int*));
  int n;

  if ( (n = read(sockfd,buffer,(DATA_SIZE - 1)) ) < 0 )
  error(  "ERROR reading from socket" );
  buffer[n] = '\0';

  convertBuffer(buffer, dataSet);
}


void convertBuffer(char* buffer[DATA_SIZE], int* dataSet)
{
  int i;
  for(i = 0; i < DATA_SIZE; i++) {
    dataSet[i] = atoi(buffer[i]);
  }
}


Bool connectionCheck(int sockfd)
{
  sendData(sockfd, CONNECTION_REQUEST);
  printf("\nSent connection request to server: %d", CONNECTION_REQUEST);
  int data = getReply(sockfd);
  printf("\nReply from server: %d", data);
  if(data == CONNECTION_APPROVED) return TRUE;
  else return FALSE;
}


void getIp(char* serverIp, jobjectArray ip, JNIEnv* env) {
  char* x1;
  char* x2;
  char* x3;
  char* x4;


  x1 = getIpIndex(0, ip, env);
  x2 = getIpIndex(1, ip, env);
  x3 = getIpIndex(2, ip, env);
  x4 = getIpIndex(3, ip, env);

  sprintf(serverIp, "%s.%s.%s.%s", x1, x2, x3, x4);
  printf("\nSending request to: %s", serverIp);
}


jintArray convertDataPacket(int* originalData, JNIEnv* env)
{
  const jint length = (*env)->GetArrayLength(env, (jarray)originalData);
  jintArray convertedData = (*env)->NewIntArray(env, length);
  (*env)->SetIntArrayRegion(env, convertedData, 0, length, originalData);
  return convertedData;
}


char* getIpIndex(int index, jobjectArray ip, JNIEnv* env)
{
  jstring string = (*env)->GetObjectArrayElement(env, ip, index);
  char* raw = (char *)(*env)->GetStringUTFChars(env, string, 0);
  return raw;
}
