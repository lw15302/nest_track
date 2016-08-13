#include "../include/Server.hpp"

int main(int argc, char *argv[]) {
  Server s = Server();
  s.run();
  return 0;
}


void Server::sendData( int sockfd, int data ) {
  int n;

  char buffer[32];
  sprintf( buffer, "%d\n", data );

  if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
    error( "ERROR writing to socket" );

  buffer[n] = '\0';
}


int Server::getData( int sockfd ) {
  char buffer[32];
  int n;

  if ( (n = read(sockfd,buffer,31) ) < 0 )
    error( "ERROR reading from socket" );

  buffer[n] = '\0';
  return atoi( buffer );
}


void Server::run()
{
  int sockfd, newsockfd, portno = 51717, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  int data;

  std::cout << "using port #" << portno << std::endl;

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
    std::cout << "waiting for new client" << std::endl;

    if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr,
    (socklen_t*) &clilen) ) < 0 )
    error( "ERROR on accept" );
    std::cout << "\n-------------------------------------" << std::endl;
    std::cout << "opened new communication with client" << std::endl;

    data = getData( newsockfd );
    std::cout << "Received: " << data << std::endl;
    processSignal(data, newsockfd);
  }
  close( newsockfd );
}


void Server::processSignal(int data, int sockfd)
{
  switch(data) {
    case CONNECTION_REQUEST:
      std::cout << "Replying: " << CONNECTION_APPROVED << std::endl;
      sendData(sockfd, CONNECTION_APPROVED);
      break;
    case INITIATE_TRACKING:
      std::cout << "Replying: " << TRACKING_INITIATED << std::endl;
      sendData(sockfd, TRACKING_INITIATED);
      tracking(START);
      break;
    case STOP_TRACKING:
      std::cout << "Replying: " << STOPPING_TRACKING << std::endl;
      sendData(sockfd, STOPPING_TRACKING);
      tracking(STOP);
      break;
    default:
      std::cout << "Replying: " << DENIED << std::endl;
      sendData(sockfd, DENIED);
      break;
  }
}


void Server::tracking(Command c)
{
  switch(c) {
    case START:
      player = VideoPlayer();
      player.setTrackStatus(true);
      track();
      player.setTrackStatus(false);
      // t = std::thread(&Server::track, this);
      // t.detach();
      break;
    case STOP:
      player.setTrackStatus(false);
      break;
    default:
      break;
  }
}


void Server::track()
{
  player.run();
  return;
}


void Server::error( std::string msg ) {
  std::cout << msg << std::endl;
  exit(1);
}
