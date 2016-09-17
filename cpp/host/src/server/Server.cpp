#include "../../include/server/Server.hpp"

void Server::run()
{
  int socketfd, newsocketfd, port = 51717, clilen;
  char buffer[256];
  struct sockaddr_in server_address, client_address;
  int n;
  int data;
  isRunning = false;
  player = NULL;
  int yes = 1;

  std::cout << "using port #" << port << std::endl;

  socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if (socketfd < 0) error( "ERROR opening socket" );

  bzero((char *) &server_address, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons( port );

  if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    error("setsockopt");
    exit(1);
}

  if (bind(socketfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
    error( "ERROR on binding" );

  }

  listen(socketfd,5);
  clilen = sizeof(client_address);

  while ( 1 ) {
    std::cout << "\n-------------------------------------" << std::endl;
    std::cout << "waiting for new client" << std::endl;

    if ( ( newsocketfd = accept( socketfd, (struct sockaddr *) &client_address,
    (socklen_t*) &clilen) ) < 0 ){
      error( "ERROR on accept" );
    }

    std::cout << "opened new communication with client" << std::endl;

    data = getData( newsocketfd );
    std::cout << "Received: " << data << std::endl;
    processSignal(data, newsocketfd);
  }
  close( newsocketfd );
}

void Server::sendData( int socketfd, int data )
{
  int n;

  char buffer[32];
  sprintf( buffer, "%d\n", data );

  if ( (n = write( socketfd, buffer, strlen(buffer) ) ) < 0 ) {
    error( "ERROR writing to socket" );
  }

  buffer[n] = '\0';
}


void Server::sendTrackingData(int socketfd)
{
  int n;
  int* data = convertRawData(rawData);
  if ( (n = write(socketfd,data,(DATA_SIZE - 1)) ) < 0 ) {
    error(  "ERROR reading from socket" );
  }
}


int Server::getData( int socketfd ) {
  char buffer[32];
  int n;

  if ( (n = read(socketfd,buffer,31) ) < 0 ) {
    error( "ERROR reading from socket" );
  }

  buffer[n] = '\0';
  return atoi( buffer );
}



void Server::processSignal(int data, int socketfd)
{
  switch(data) {
    case CONNECTION_REQUEST:
      reply(socketfd, CONNECTION_APPROVED);
      break;
    case INITIATE_TRACKING:
      reply(socketfd, TRACKING_INITIATED);
      tracking(START);
      break;
    case STOP_TRACKING:
      reply(socketfd, STOP_TRACKING);
      tracking(STOP);
      break;
    case GET_TRACKING_DATA:
      if(isRunning) {
        rawData = player->getTrackingData();
        sendTrackingData(socketfd);
        resetRawData();
      }
      else {
        reply(socketfd, DENIED);
      }
      break;
    default:
      reply(socketfd, DENIED);
      break;
  }
}


void Server::reply(int socketfd, int reply)
{
  std::cout << "Replying: " << reply << std::endl;
  sendData(socketfd, reply);
}


void Server::tracking(Command c)
{
  switch(c) {
    case START:
      if(!isRunning) {
        std::cout << "inside start" << std::endl;
        player = std::make_shared<VideoPlayer>();
        isRunning = true;
        player->setTrackStatus(isRunning);
      }
      break;
    case STOP:
      if(isRunning) {
        isRunning = false;
        player->setTrackStatus(isRunning);
        player.reset();
      }
      break;
    default:
      break;
  }
}


void Server::dataToBuffer(char* buffer[DATA_SIZE])
{
  int i;
  for(i = 0; rawData[i] != 0; i++) {
    sprintf(buffer[i], "%d", rawData[i]);
  }
}


void Server::resetRawData()
{
  int i;
  for(i = 0; i < DATA_SIZE; i++) {
    rawData[i] = 0;
  }
}


void Server::error( std::string msg ) {
  std::cout << msg << std::endl;
  exit(1);
}


int* Server::convertRawData(std::array<int,DATA_SIZE> rawData)
{
  int* data = (int*)calloc(DATA_SIZE, sizeof(int*));
  for(int i = 0; i < DATA_SIZE; i++) {
    data[i] = rawData[i];
  }
  return data;
}
