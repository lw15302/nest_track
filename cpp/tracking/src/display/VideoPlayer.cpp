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
  track_t = std::thread(&VideoPlayer::run, this);
  dataSet = (int*)calloc(DATA_SIZE, sizeof(int*));
  lastX = 0;
  dataIndex = 0;
  // std::lock_guard<std::mutex> unlock(mtx);
}


VideoPlayer::~VideoPlayer()
{
  std::cout << "Inside VideoPlayer Destructor" << std::endl;
  terminate();
  exit();
  joinThread();
  free(dataSet);
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
  start = std::clock_t();
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

  return;
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
  int difference = abs(x - lastX);
  double currentTime = 0;
  if(difference > 20) {
    currentTime = (std::clock_t() - start) / (double)CLOCKS_PER_SEC;
    if(x < lastX) {
      currentTime *= -1;
    }
    dataSet[dataIndex] = currentTime;
    dataIndex++;
  }
  lastX = x;
}


void VideoPlayer::resetData()
{
  int i;
  for(i = 0; i < DATA_SIZE; i++) {
    dataSet[i] = 0;
  }
}



int* VideoPlayer::getTrackingData()
{
  int* dataOut = dataSet;
  resetData();
  return dataOut;
}
