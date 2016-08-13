#include "../include/NativeClient.h"

JNIEXPORT jboolean JNICALL Java_connect_NativeClient_connect(JNIEnv *env, jobject obj)
{
  int sockfd = setUpConnection();
  if(sockfd = -1) return FALSE;

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


JNIEXPORT jboolean JNICALL Java_connect_NativeClient_track(JNIEnv *env, jobject obj)
{
  int sockfd = setUpConnection();
  if(sockfd = -1) return FALSE;
  int data;

  sendData(sockfd, INITIATE_TRACKING);
  printf("\nSent request to start tracking: %d", INITIATE_TRACKING);
  data = getData( sockfd );
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


JNIEXPORT jboolean JNICALL Java_connect_NativeClient_stopTrack(JNIEnv *env, jobject obj)
{
  int sockfd = setUpConnection();
  int data;

  sendData(sockfd, STOP_TRACKING);
  printf("\nSent request to stop tracking: %d", STOP_TRACKING );
  data = getData(sockfd);
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


int setUpConnection(){
  int sockfd, portno = 51717, n;
  char serverIp[] = "127.0.0.1";
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[256];
  int data;

  setbuf(stdout, NULL);
  if( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) {
    fprintf(stderr, "ERROR opening socket\n");
    return -1;
  }
  if( ( server = gethostbyname( serverIp ) ) == NULL ) {
    fprintf(stderr, "ERROR no such host\n");
    return -1;
  }

  bzero( (char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;

  bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
    fprintf(stderr, "ERROR connecting" );
    return -1;
  }
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


int getData( int sockfd )
{
  char buffer[32];
  int n;

  if ( (n = read(sockfd,buffer,31) ) < 0 )
       error(  "ERROR reading from socket" );
  buffer[n] = '\0';
  return atoi( buffer );
}


Bool connectionCheck(int sockfd)
{
  sendData(sockfd, CONNECTION_REQUEST);
  printf("\nSent connection request to server: %d", CONNECTION_REQUEST);
  int data = getData(sockfd);
  printf("\nReply from server: %d", data);
  if(data == CONNECTION_APPROVED) return TRUE;
  else return FALSE;
}


// void error(char *msg) {
//   perror(msg);
//   exit(0);
// }
