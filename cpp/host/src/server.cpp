#include "../include/server.hpp"

int main(int argc, char *argv[]) {
  run();
  return 0;
}


void sendData( int sockfd, int data ) {
  int n;

  char buffer[32];
  sprintf( buffer, "%d\n", data );

  if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
    error( "ERROR writing to socket" );

  buffer[n] = '\0';
}


int getData( int sockfd ) {
  char buffer[32];
  int n;

  if ( (n = read(sockfd,buffer,31) ) < 0 )
    error( "ERROR reading from socket" );

  buffer[n] = '\0';
  return atoi( buffer );
}


void  run()
{
  int sockfd, newsockfd, portno = 51717, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  int data;

  printf( "using port #%d\n", portno );

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) error( "ERROR opening socket" );

  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons( portno );

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error( "ERROR on binding" );

  listen(sockfd,5);
  clilen = sizeof(cli_addr);

  while ( 1 ) {
    printf( "waiting for new client...\n" );

    if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr,
    (socklen_t*) &clilen) ) < 0 )
    error( "ERROR on accept" );

    printf( "Opened new communication with client\n" );

    data = getData( newsockfd );
    printf( "Received - %d\n", data );
    processSignal(data, newsockfd);
  }
  close( newsockfd );
}


void processSignal(int data, int sockfd)
{
  switch(data) {
    case CONNECTION_REQUEST:
      sendData(sockfd, CONNECTION_APPROVED);
      printf("Repling - %d\n", CONNECTION_APPROVED);
      break;
    case INITIATE_TRACKING:
      sendData(sockfd, TRACKING_INITIATED);
      initiateTracking();
    default:
      sendData(sockfd, CONNECTION_DENIED);
      printf("Repling - %d\n", CONNECTION_DENIED);
      break;
  }
}


void initiateTracking()
{

}


void error( std::string msg ) {
  std::cout << msg << std::endl;
  exit(1);
}
