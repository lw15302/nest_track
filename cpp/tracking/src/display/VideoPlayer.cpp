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
  trackerWindow = "Tracker";
  // std::lock_guard<std::mutex> lock(mtx);
  dataSet = {0};
  lastY = 0;
  dataIndex = 0;
  accessingData = false;

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
  if(!capture.open("presentation_cut.mp4")) {
      throw "Cannot open video stream";
  }
  std::cout << "attempting namedWindow()1" << std::endl;
  // cv::namedWindow(originalWindow, cv::WINDOW_NORMAL);
  std::cout << "attempting namedWindow()2" << std::endl;
  cv::namedWindow(trackerWindow, cv::WINDOW_NORMAL);
  std::cout << "start tracking" << std::endl;
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
    printf("\n n: %d\n", n++);

    while(accessingData) {}


    // std::this_thread::sleep_for(std::chrono::milliseconds(100));



    differenceFrame = cv::Mat();
    if(!capture.read(frame)) {
      track = false;
      break;
    }
    // cv::imshow(originalWindow, frame);
    frame = tracker.transform(frame);
    if(!capture.read(comparisonFrame)) {
      track = false;
      break;
    }
    comparisonFrame = tracker.transform(comparisonFrame);
    cv::absdiff(frame, comparisonFrame, differenceFrame);
    differenceFrame = tracker.transform(differenceFrame);
    imshow("diff", differenceFrame);
    differenceFrame = tracker.backgroundSubtraction(differenceFrame);

    differenceFrame = tracker.highlight(differenceFrame);

    setTrackingData();

    cv::imshow(trackerWindow, differenceFrame);
    cv::waitKey(30);
  }
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout <<"Exiting tracking" << std::endl;

  // return;
}


void VideoPlayer::exit()
{
  // cv::destroyWindow(originalWindow);
  cv::destroyWindow(trackerWindow);
  // frame.release();
  // differenceFrame.release();
  // comparisonFrame.release();

  capture.release( );
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
  // int y = tracker.getY();
  // std::cout << "x position: " << x << std::endl;
  // int direction = travelDirection(y);
  // if(direction == STATIONARY) return;
  accessingData = true;
  int result = tracker.findActivity();

  if(result == 1) {
    // double processingTime = std::difftime(std::time(0), processStart);
    float timeElapsed = std::abs(1000.0 * std::difftime(std::time(0), start));

    std::cout << "Ant entered nest at: " << timeElapsed << std::endl;
    dataSet[dataIndex] = timeElapsed;
    std::cout << dataSet[dataIndex] << " " << std::endl;
    dataIndex++;
  }
  else if(result == 2) {
    // double processingTime = std::difftime(std::time(0), processStart);
    float timeElapsed = std::abs((1000.0) * std::difftime(std::time(0), start));
    std::cout << "Ant left nest at: " << timeElapsed << std::endl;
    timeElapsed *= -1;
    dataSet[dataIndex] = timeElapsed;
    std::cout << dataSet[dataIndex] << " " << std::endl;
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


  for(int i = 0; i < DATA_SIZE; i++) {
    if(i % 20 == 0) printf("\n");
    std::cout << dataOut[i] << " ";
  }
  printf("\n");


  resetData();
  mtx.unlock();
  accessingData = false;

  std::cout << "Replying with data" << std::endl;
  return dataOut;
}
