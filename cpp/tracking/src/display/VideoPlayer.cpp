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
  dataSet = {0};
  lastY = 0;
  dataIndex = 0;
  accessingData = false;

  track_t = std::thread(&VideoPlayer::run, this);
}


VideoPlayer::~VideoPlayer()
{
  terminate();
  exit();
  joinThread();
}


void VideoPlayer::joinThread()
{
  if(track_t.joinable()) {
    track_t.join();
    return;
  }
}

/**
 * Calls the initialisation functions
 */
void VideoPlayer::run(void)
{
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
  if(!capture.open("presentation_cut.mp4")) {
      throw "Cannot open video stream";
  }
  tracker.setFrameProp(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
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
  start = std::time(0);
  while(track) {
    while(accessingData) {}

    differenceFrame = cv::Mat();

    if(!capture.read(frame)) {
      track = false;
      break;
    }

    frame = tracker.transform(frame);

    if(!capture.read(comparisonFrame)) {
      track = false;
      break;
    }
    comparisonFrame = tracker.transform(comparisonFrame);
    cv::absdiff(frame, comparisonFrame, differenceFrame);
    differenceFrame = tracker.transform(differenceFrame);

    differenceFrame = tracker.backgroundSubtraction(differenceFrame);

    differenceFrame = tracker.highlight(differenceFrame);
    setTrackingData();

    std::this_thread::sleep_for(std::chrono::milliseconds(35));
  }
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout <<"Exiting tracking" << std::endl;
}


void VideoPlayer::exit()
{  capture.release( );
}


void VideoPlayer::terminate()
{
  track = false;
}


void VideoPlayer::setTrackStatus(bool status)
{
  track = status;
}


void VideoPlayer::setTrackingData()
{
  accessingData = true;
  int result = tracker.findActivity();

  if(result == 1) {
    float timeElapsed = std::abs(1000.0 * std::difftime(std::time(0), start));

    dataSet[dataIndex] = timeElapsed;
    dataIndex++;
  }
  else if(result == 2) {
    float timeElapsed = std::abs((1000.0) * std::difftime(std::time(0), start));
    timeElapsed *= -1;
    dataSet[dataIndex] = timeElapsed;
    dataIndex++;
  }
  accessingData = false;
}


void VideoPlayer::resetData()
{
  int i;
  for(i = 0; i < DATA_SIZE; i++) {
    dataSet[i] = 0;
  }
  dataIndex = 0;
}



std::array<int, DATA_SIZE> VideoPlayer::getTrackingData()
{
  std::array<int, DATA_SIZE> dataOut;
  accessingData = true;
  mtx.lock();
  if(!track) {
    for(int i = 0; i < 5; i++) {
      dataOut[i] = -1;
    }
    accessingData = false;
    mtx.unlock();
    return dataOut;
  }
  dataOut = dataSet;

  resetData();
  mtx.unlock();
  accessingData = false;

  std::cout << "Replying with data" << std::endl;
  return dataOut;
}
