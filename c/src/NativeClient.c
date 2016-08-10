#include "../include/NativeClient.h"

JNIEXPORT void JNICALL Java_connect_NativeClient_connect(JNIEnv *env, jobject obj)
{
  int sockfd, portno = 51717, n;
  char serverIp[] = "127.0.0.1";
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[256];
  int data;

  setbuf(stdout, NULL);
  printf("Inside client.c\n");
  if( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) {
    error("ERROR opening socket\n");
  }
  if( ( server = gethostbyname( serverIp ) ) == NULL ) {
    error("ERROR no such host\n");
  }
  bzero( (char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);
  printf("flag1\n");
  if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
    error( "ERROR connecting" );
  }
  printf("flag2\n");

  if(connectionCheck(sockfd)) maintainConnection();
  else close( sockfd );

  close( sockfd );
}


JNIEXPORT void JNICALL Java_connect_NativeClient_disconnect(JNIEnv *env, jobject obj)
{

}


void sendData( int sockfd, int x ) {
  int n;

  char buffer[32];
  sprintf( buffer, "%d\n", x );
  if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
      error( "ERROR writing to socket" );
  buffer[n] = '\0';
}


int getData( int sockfd ) {
  char buffer[32];
  int n;

  if ( (n = read(sockfd,buffer,31) ) < 0 )
       error(  "ERROR reading from socket" );
  buffer[n] = '\0';
  return atoi( buffer );
}


Bool connectionCheck(int sockfd)
{

}


void error(char *msg) {
  perror(msg);
  exit(0);
}
