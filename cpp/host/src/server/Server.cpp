#include "../../include/server/Server.hpp"

void Server::run()
{
  int sockfd, newsockfd, portno = 51717, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  int data;
  isRunning = false;
  player = NULL;
  int yes = 1;
  // t = std::thread();
  // t.detach();

  std::cout << "using port #" << portno << std::endl;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) error( "ERROR opening socket" );

  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons( portno );

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    error("setsockopt");
    exit(1);
}

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    error( "ERROR on binding" );

  }

  listen(sockfd,5);
  clilen = sizeof(cli_addr);

  while ( 1 ) {
    std::cout << "\n-------------------------------------" << std::endl;
    std::cout << "waiting for new client" << std::endl;

    if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr,
    (socklen_t*) &clilen) ) < 0 ){
      error( "ERROR on accept" );
    }

    std::cout << "opened new communication with client" << std::endl;

    data = getData( newsockfd );
    std::cout << "Received: " << data << std::endl;
    processSignal(data, newsockfd);
  }
  close( newsockfd );
}

void Server::sendData( int sockfd, int data )
{
  int n;

  char buffer[32];
  sprintf( buffer, "%d\n", data );

  if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 ) {
    error( "ERROR writing to socket" );
  }

  buffer[n] = '\0';
}


void Server::sendTrackingData(int sockfd)
{
  int n;
  // int* buffer;

  // dataToBuffer(buffer);
  int* data = convertRawData(rawData);

  // printf("data in senddata: ");
  // for(int i = 0; i < DATA_SIZE; i++) {
  //   if(i %20 == 0) {
  //     printf("\n");
  //   }
  //   printf("%d  ", rawData[i]);
  // }
  // printf("\n");

  if ( (n = write(sockfd,data,(DATA_SIZE - 1)) ) < 0 ) {
    error(  "ERROR reading from socket" );
  }
  // buffer[n] = '\0';
}


int Server::getData( int sockfd ) {
  char buffer[32];
  int n;

  if ( (n = read(sockfd,buffer,31) ) < 0 ) {
    error( "ERROR reading from socket" );
  }

  buffer[n] = '\0';
  return atoi( buffer );
}



void Server::processSignal(int data, int sockfd)
{
  switch(data) {
    case CONNECTION_REQUEST:
      reply(sockfd, CONNECTION_APPROVED);
      break;
    case INITIATE_TRACKING:
      reply(sockfd, TRACKING_INITIATED);
      tracking(START);
      break;
    case STOP_TRACKING:
      reply(sockfd, STOP_TRACKING);
      tracking(STOP);
      break;
    case GET_TRACKING_DATA:
      if(isRunning) {
        rawData = player->getTrackingData();
        // std::cout << "data: ";
        // for(int i = 0; i < DATA_SIZE; i++) {
        //   std::cout << rawData[i] << " ";
        //   if(i % 10 == 0) {
        //     std::cout << "\n";
        //   }
        // }
        // std::cout << "\n";
        sendTrackingData(sockfd);
        resetRawData();
      }
      else {
        reply(sockfd, DENIED);
      }
      break;
    default:
      reply(sockfd, DENIED);
      break;
  }
}


void Server::reply(int sockfd, int reply)
{
  std::cout << "Replying: " << reply << std::endl;
  sendData(sockfd, reply);
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
