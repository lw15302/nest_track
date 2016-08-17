#include "../include/NativeClient.h"

/**
 * Sends a signal to the server to check if there is a suitable connection.
 * If a server at the specified address is detected, then the server sends
 * back an approved request. This approval is then passed back to the Java
 * application
 * @param  env - a pointer to the JNI interface
 * @param  obj - a reference to the Java object
 * @param  ip  - the IP address of the server to connect to
 * @return     - connection success
 */
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


/**
 * Sends a request to start the OpenCV tracking application. The server sends
 * back a reply giving the initiation success
 * @param  env - a pointer to the JNI interface
 * @param  obj - a reference to the Java object
 * @param  ip  - the IP address of the server to connect to
 * @return     - tracking success
 */
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


/**
 * Sends a request to the server to stop OpenCV the tracking application. The
 * server sends back a reply giving the success
 * @param  env - a pointer to the JNI interface
 * @param  obj - a reference to the Java object
 * @param  ip  - the IP address of the server to connect to
 * @return     - stop tracking success
 */
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


/**
 * Sends a request for data and returns an array containing the
 * reply
 * @param  env - a pointer to the JNI interface
 * @param  obj - a reference to the Java object
 * @param  ip  - the IP address of the server to connect to
 * @return     - data returned from the server
 */
JNIEXPORT jintArray JNICALL Java_connect_NativeClient_getData(JNIEnv *env, jobject obj, jobjectArray ip)
{
  int sockfd = setUpConnection(ip, env);
  if(sockfd == -1) return (jintArray)-1;
  int* data = (int*)malloc(DATA_SIZE * sizeof(int*));

  sendData(sockfd, GET_TRACKING_DATA);
  printf("\nSent request to get data: %d", GET_TRACKING_DATA);
  sleep(100);
  data = getData(sockfd);

  jintArray dataPacket = convertDataPacket(data, env);
  free(data);
  return dataPacket;
}


/**
 * [setUpConnection description]
 * @param  ip  - the IP address of the server to connect to
 * @param  env - a pointer to the JNI interface
 * @return     - returns the id of the socket. -1 returned on failure
 */
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


/**
 * Sends signal to the server via the specified socket
 * @param sockfd - id of the socket
 * @param x      - signal to send to the server
 */
void sendData( int sockfd, int x )
{
  int n;

  char buffer[32];
  sprintf( buffer, "%d\n", x );
  if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
      error( "ERROR writing to socket" );
  buffer[n] = '\0';
}


/**
 * Receives a reply from the server on the specified socket
 * @param  sockfd - id of the socket
 * @return        - reply signal
 */
int getReply( int sockfd )
{
  char buffer[32];
  int n;

  if ( (n = read(sockfd,buffer,31) ) < 0 )
       error(  "ERROR reading from socket" );
  buffer[n] = '\0';
  return atoi( buffer );
}


/**
 * Gets the data from the server on the specified socket following a data
 * request
 * @param  sockfd - id of the socket
 * @return        - array containing data
 */
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


/**
 * Formats the data set into a buffer ready to be sent back to the application
 * @param buffer  - array of integers in string form to be sent back to the application
 * @param dataSet - data set from the server
 */
void convertBuffer(char* buffer[DATA_SIZE], int* dataSet)
{
  int i;
  for(i = 0; i < DATA_SIZE; i++) {
    dataSet[i] = atoi(buffer[i]);
  }
}


/**
 * Checks if there is a valid connection to the specified address
 * @param  sockfd - id of the socket
 * @return        - connection success
 */
Bool connectionCheck(int sockfd)
{
  sendData(sockfd, CONNECTION_REQUEST);
  printf("\nSent connection request to server: %d", CONNECTION_REQUEST);
  int data = getReply(sockfd);
  printf("\nReply from server: %d", data);
  if(data == CONNECTION_APPROVED) return TRUE;
  else return FALSE;
}


/**
 * Converts the IP address sent by the application into a C readable format
 * @param serverIp - the C readable character array of the server
 * @param ip       - the IP address from the application
 * @param  env     - a pointer to the JNI interface
 */
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


/**
 * Converts the data from the server into an appropriate format to send back
 * to the application
 * @param  originalData - data from the server
 * @param  env          - a pointer to the JNI interface
 * @return              - data in jintArray format
 */
jintArray convertDataPacket(int* originalData, JNIEnv* env)
{
  const jint length = (*env)->GetArrayLength(env, (jarray)originalData);
  jintArray convertedData = (*env)->NewIntArray(env, length);
  (*env)->SetIntArrayRegion(env, convertedData, 0, length, originalData);
  return convertedData;
}


/**
 * Gets the value at the specified index of the IP address array
 * @param  index - index at which the value of interest is stored
 * @param  ip    - the full IP address
 * @param  env   - a pointer to the JNI interface
 * @return       - the segment of the address of interest as a character array
 */
char* getIpIndex(int index, jobjectArray ip, JNIEnv* env)
{
  jstring string = (*env)->GetObjectArrayElement(env, ip, index);
  char* raw = (char *)(*env)->GetStringUTFChars(env, string, 0);
  return raw;
}
