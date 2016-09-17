#include "../include/NativeClient.h"

/**
 * Sends a signal to the serv to check if there is a suitable connection.
 * If a serv at the specified address is detected, then the serv sends
 * back an approved request. This approval is then passed back to the Java
 * application
 * @param  env - a pointer to the JNI interface
 * @param  obj - a reference to the Java object
 * @param  ip  - the IP address of the serv to connect to
 * @return     - connection success
 */
JNIEXPORT jboolean JNICALL Java_connect_NativeClient_connect(JNIEnv *env, jobject obj, jobjectArray ip)
{
  int socketfd = setUpConnection(ip, env);
  if(socketfd == -1) return FALSE;

  if(connectionCheck(socketfd)) {
    printf("\nConnection established");
    close(socketfd);
    return TRUE;
  }
  else {
    printf("\nConnection failed");
    close( socketfd );
    return FALSE;
  }
}


/**
 * Sends a request to start the OpenCV tracking application. The serv sends
 * back a reply giving the initiation success
 * @param  env - a pointer to the JNI interface
 * @param  obj - a reference to the Java object
 * @param  ip  - the IP address of the serv to connect to
 * @return     - tracking success
 */
JNIEXPORT jboolean JNICALL Java_connect_NativeClient_track(JNIEnv *env, jobject obj, jobjectArray ip)
{
  int socketfd = setUpConnection(ip, env);
  if(socketfd == -1) return FALSE;
  int data;

  sendData(socketfd, INITIATE_TRACKING);
  printf("\nSent request to start tracking: %d", INITIATE_TRACKING);
  data = getReply( socketfd );
  printf("\nReceived reply: %d", data);

  if(data == TRACKING_INITIATED) {
    printf("\nTracking initiated");
    close(socketfd);
    return TRUE;
  }
  else {
    printf("\nFailed to initiate tracking");
    close(socketfd);
    return FALSE;
  }
}


/**
 * Sends a request to the serv to stop OpenCV the tracking application. The
 * serv sends back a reply giving the success
 * @param  env - a pointer to the JNI interface
 * @param  obj - a reference to the Java object
 * @param  ip  - the IP address of the serv to connect to
 * @return     - stop tracking success
 */
JNIEXPORT jboolean JNICALL Java_connect_NativeClient_stopTrack(JNIEnv *env, jobject obj, jobjectArray ip)
{
  int socketfd = setUpConnection(ip, env);
  if(socketfd == -1) return FALSE;
  int data;

  sendData(socketfd, STOP_TRACKING);
  printf("\nSent request to stop tracking: %d", STOP_TRACKING );
  data = getReply(socketfd);
  printf("\nReceived reply: %d", data);

  if(data == STOPPING_TRACKING) {
    printf("\nTracking stopped");
    close(socketfd);
    return TRUE;
  }
  else {
    printf("\nFailed to stop tracking");
    close(socketfd);
    return FALSE;
  }
}


/**
 * Sends a request for data and returns an array containing the
 * reply
 * @param  env - a pointer to the JNI interface
 * @param  obj - a reference to the Java object
 * @param  ip  - the IP address of the serv to connect to
 * @return     - data returned from the serv
 */
JNIEXPORT jintArray JNICALL Java_connect_NativeClient_getData(JNIEnv *env, jobject obj, jobjectArray ip)
{
  int socketfd = setUpConnection(ip, env);
  if(socketfd == -1) return (jintArray)-1;
  int* data = (int*)calloc(DATA_SIZE, sizeof(int*));

  sendData(socketfd, GET_TRACKING_DATA);
  printf("\nSent request to get data: %d", GET_TRACKING_DATA);
  sleep(1);
  data = getData(socketfd);
  jintArray dataPacket = convertDataPacket(data, env);
  free(data);

  return dataPacket;
}


/**
 * Connects to the serv at the specified address
 * @param  ip  - the IP address of the serv to connect to
 * @param  env - a pointer to the JNI interface
 * @return     - returns the id of the socket. -1 returned on failure
 */
int setUpConnection(jobjectArray ip, JNIEnv* env)
{
  int socketfd, port = 51717;
  char* servIp = (char *)malloc(20 * sizeof(char));
  getIp(servIp, ip, env);
  struct sockaddr_in serv_addr;
  struct hostent *serv;

  setbuf(stdout, NULL);
  if( ( socketfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) {
    fprintf(stderr, "ERROR opening socket\n");
    free(servIp);
    return -1;
  }

  if( ( serv = gethostbyname( servIp ) ) == NULL ) {
    fprintf(stderr, "ERROR no such host\n");
    free(servIp);
    return -1;
  }

  bzero( (char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;

  bcopy( (char *)serv->h_addr, (char *)&serv_addr.sin_addr.s_addr, serv->h_length);
  serv_addr.sin_port = htons(port);

  if ( connect(socketfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
    fprintf(stderr, "ERROR connecting" );
    free(servIp);
    return -1;
  }
  free(servIp);
  return socketfd;
}


/**
 * Sends signal to the serv via the specified socket
 * @param socketfd - id of the socket
 * @param x      - signal to send to the serv
 */
void sendData( int socketfd, int x )
{
  int n;

  char buffer[32];
  sprintf( buffer, "%d\n", x );
  if ( (n = write( socketfd, buffer, strlen(buffer) ) ) < 0 )
      error( "ERROR writing to socket" );
  buffer[n] = '\0';
}


/**
 * Receives a reply from the serv on the specified socket
 * @param  socketfd - id of the socket
 * @return        - reply signal
 */
int getReply( int socketfd )
{
  char buffer[32];
  int n;

  if ( (n = read(socketfd,buffer,31) ) < 0 )
       error(  "Cannot read from socket" );
  buffer[n] = '\0';
  return atoi( buffer );
}


/**
 * Gets the data from the serv on the specified socket following a data
 * request
 * @param  socketfd - id of the socket
 * @return        - array containing data
 */
int* getData(int socketfd)
{
  int* dataSet = (int*)calloc(DATA_SIZE, sizeof(int*));
  int n, i;

  if ( (n = read(socketfd,dataSet,(DATA_SIZE - 1)) ) < 0 )
  error(  "Cannot read from socket" );

  return dataSet;
}


/**
 * Checks if there is a valid connection to the specified address
 * @param  socketfd - id of the socket
 * @return        - connection success
 */
Bool connectionCheck(int socketfd)
{
  sendData(socketfd, CONNECTION_REQUEST);
  printf("\nSent connection request to serv: %d", CONNECTION_REQUEST);
  int data = getReply(socketfd);

  printf("\nReply from serv: %d", data);
  if(data == CONNECTION_APPROVED) return TRUE;
  else return FALSE;
}


/**
 * Converts the IP address sent by the application into a C readable format
 * @param servIp - the C readable character array of the serv
 * @param ip       - the IP address from the application
 * @param  env     - a pointer to the JNI interface
 */
void getIp(char* servIp, jobjectArray ip, JNIEnv* env) {
  char* x1;
  char* x2;
  char* x3;
  char* x4;

  x1 = getIpIndex(0, ip, env);
  x2 = getIpIndex(1, ip, env);
  x3 = getIpIndex(2, ip, env);
  x4 = getIpIndex(3, ip, env);

  sprintf(servIp, "%s.%s.%s.%s", x1, x2, x3, x4);
  printf("\nSending request to: %s", servIp);
}


/**
 * Converts the data from the serv into an appropriate format to send back
 * to the application
 * @param  originalData - data from the serv
 * @param  env          - a pointer to the JNI interface
 * @return              - data in jintArray format
 */
jintArray convertDataPacket(int* originalData, JNIEnv* env)
{
  jintArray convertedData = (*env)->NewIntArray(env, DATA_SIZE);
  (*env)->SetIntArrayRegion(env, convertedData, 0, DATA_SIZE, (jint*)originalData);

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
