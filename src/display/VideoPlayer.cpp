#include <VideoPlayer.hpp>
/**
 * Public functions
 */

 /**
 * VideoPlayer class constructor. Initialises the thresholds used in
 * transform() and a random number generator used for bounding box highlight
 * colours
 */
 VideoPlayer::VideoPlayer()
 {
   tracker = Tracker();
 }


/**
 * Calls the initialisation functions
 */
void VideoPlayer::run(void)
{
  initVC();
  openStream();
}


/**
 * Initialises the OpenCV VideoCapture object. Throws exception if there is
 * a problem
 */
void VideoPlayer::initVC()
{
  if(!capture.open(0)) {
      throw "Cannot open video stream";
  }
  cv::namedWindow("Tracker");
}


/**
 * Private functions
 */

/**
 * Creates a background subtractor and calls the main video capturing function
 */
void VideoPlayer::openStream()
{
  initBackground();
  captureStream();
}


void VideoPlayer::initBackground()
{
  // pMOG2 = cv::createBackgroundSubtractorMOG2();
  if(!capture.read(background)) return;
  cv::cvtColor(background, background, CV_BGR2GRAY);
  cv::threshold(background, background, 100, 255, cv::THRESH_BINARY);
  // cv::erode(background, background, cv::Mat());
}


/**
 * Contains the main loop that captures and displays the frames from the
 * camera. It also calls a number of functions associated with image processing
 * and object detection
 */
void VideoPlayer::captureStream() {
  while(1) {
    frame = cv::Mat();
    if(!capture.read(frame1)) break;

    frame1 = tracker.transform(frame1);
    cv::subtract(frame1, background, frame);
    // pMOG2->apply(frame, frame);
    cv::imshow("frame before contours", frame);
    frame = tracker.boundingBox(frame);
    // cv::imshow("frame1", frame1);
    cv::imshow("Tracker", frame);
    // cv::imshow("back", background);
    if(checkExit()) break;
  }
}


bool VideoPlayer::checkExit()
{
  if(cv::waitKey(30) == 27) {
    std::cout << "Exiting program" << std::endl;
    capture.release();
    return true;
  }
  return false;
}
