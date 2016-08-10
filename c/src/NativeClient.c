#include "../include/NativeClient.h"

JNIEXPORT jboolean JNICALL Java_connect_NativeClient_connect(JNIEnv *env, jobject obj)
{
  int sockfd, portno = 51717, n;
  char serverIp[] = "127.0.0.1";
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[256];
  int data;

  setbuf(stdout, NULL);
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

  if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
    error( "ERROR connecting" );
  }

  if(connectionCheck(sockfd)) {
    printf("\nConnection established");
    return TRUE;
  }
  else {
    printf("\nConnection failed");
    close( sockfd );
    return FALSE;
  }
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


void error(char *msg) {
  perror(msg);
  exit(0);
}
