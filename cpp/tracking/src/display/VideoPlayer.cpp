#include "../../include/display/VideoPlayer.hpp"
/**
 * Public functions
 */

 /**
 * VideoPlayer class constructor. Initialises the thresholds used in
 * transform() and a random number generator used for bounding box highlight
 * colours
 */
VideoPlayer::VideoPlayer () : track(true)
{
  tracker = Tracker();
  std::cout << "Inside VideoPlayer Constructor" << std::endl;
  // originalWindow = "Original";
  // trackerWindow = "Tracker";
  // std::lock_guard<std::mutex> lock(mtx);
  dataSet = (int*)calloc(DATA_SIZE, sizeof(int*));
  dataOut = (int*)malloc(DATA_SIZE * sizeof(int*));
  lastX = 0;
  dataIndex = 0;
  track_t = std::thread(&VideoPlayer::run, this);

  // std::lock_guard<std::mutex> unlock(mtx);
}


VideoPlayer::~VideoPlayer()
{
  std::cout << "Inside VideoPlayer Destructor" << std::endl;
  terminate();
  std::cout << "destructor flag 1" << std::endl;
  exit();
  std::cout << "destructor flag 2" << std::endl;
  joinThread();
  std::cout << "destructor flag 3" << std::endl;
  free(dataSet);
  std::cout << "destructor flag 4" << std::endl;
  free(dataOut);
  std::cout << "destructor flag 5" << std::endl;
}


void VideoPlayer::joinThread()
{
  if(track_t.joinable()) {
    std::cout << "Trying to join thread" << std::endl;
    track_t.join();
    std::cout << "joining thread" << std::endl;
    return;
  }
}

/**
 * Calls the initialisation functions
 */
void VideoPlayer::run(void)
{
  std::cout << "inside run" << std::endl;
  initVC();
  openStream();
  // if(track) {
  //   delete this;
  // }
  return;
}


/**
 * Initialises the OpenCV VideoCapture object. Throws exception if there is
 * a problem
 */
void VideoPlayer::initVC()
{
  // mtx.lock();
  if(!capture.open("cut.mp4")) {
      throw "Cannot open video stream";
  }
  std::cout << "attempting namedWindow()1" << std::endl;
  // cv::namedWindow(originalWindow, cv::WINDOW_NORMAL);
  std::cout << "attempting namedWindow()2" << std::endl;
  // cv::namedWindow(trackerWindow, cv::WINDOW_NORMAL);
  std::cout << "start tracking" << std::endl;

}


/**
 * Private functions
 */


/**
 * Contains the main loop that captures and displays the frames from the
 * camera. It also calls a number of functions associated with image processing
 * and object detection
 */
void VideoPlayer::openStream() {
  int n = 0;
  start = std::clock();
  while(track) {
    differenceFrame = cv::Mat();
    if(!capture.read(frame)) break;
    // cv::imshow(originalWindow, frame);
    frame = tracker.transform(frame);
    if(!capture.read(comparisonFrame)) break;
    comparisonFrame = tracker.transform(comparisonFrame);
    cv::absdiff(frame, comparisonFrame, differenceFrame);


    differenceFrame = tracker.boundingBox(differenceFrame); //Rename function as it no longer creates a boundingBox
    setTrackingData();

    // cv::imshow(trackerWindow, differenceFrame);
    cv::waitKey(30);
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::cout <<"Exiting tracking" << std::endl;

  // return;
}


void VideoPlayer::exit()
{
  // cv::destroyWindow(originalWindow);
  // cv::destroyWindow(trackerWindow);
  // frame.release();
  // differenceFrame.release();
  // comparisonFrame.release();

  capture.release();
}


void VideoPlayer::terminate()
{
  track = false;
  // std::lock_guard<std::mutex> lock(mtx);
}


void VideoPlayer::setTrackStatus(bool status)
{
  track = status;
}


void VideoPlayer::setTrackingData()
{
  int x = tracker.getX();
  std::cout << "x position: " << x << std::endl;
  int difference = abs(x - lastX);
  std::cout << "difference: " << difference << std::endl;
  float currentTime;

  if(difference >= 3) {
    current = std::clock();
    double elapsed = (1000.0)*(current - start) / (double)CLOCKS_PER_SEC;
    if(x < lastX) {
      elapsed *= -1;
    }
    dataSet[dataIndex] = elapsed;
    dataIndex++;
    std::cout << "time: " << elapsed << std::endl;
  }
  // printf("data in set: ");
  // for(int i = 0; i < DATA_SIZE; i++) {
  //   if(i % 20 == 0) {
  //     printf("\n");
  //   }
  //   std::cout << dataSet[i] << " ";
  // }
  // printf("\n");
  lastX = x;
}


void VideoPlayer::resetData()
{
  int i;
  for(i = 0; i < DATA_SIZE; i++) {
    dataSet[i] = 0;
  }
  dataIndex = 0;
}



int* VideoPlayer::getTrackingData()
{
  memcpy(dataOut, dataSet, sizeof(dataSet));
  int i;
  std::cout << "data in VideoPlayer: ";
  // for(i = 0; i < DATA_SIZE; i++) {
  //   std::cout << dataOut[i] << " ";
  //   if(i % 20 == 0) {
  //     printf("\n");
  //   }
  // }
  // printf("\n");

  resetData();
  return dataOut;
}
