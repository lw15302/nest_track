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
  if(!capture.open("cut.mp4")) {
      throw "Cannot open video stream";
  }
  cv::namedWindow("Tracker");
}


/**
 * Private functions
 */

/**
 * Creates a comparisonFrame subtractor and calls the main video capturing function
 */
void VideoPlayer::openStream()
{
  captureStream();
}


/**
 * Contains the main loop that captures and displays the frames from the
 * camera. It also calls a number of functions associated with image processing
 * and object detection
 */
void VideoPlayer::captureStream() {
  while(1) {
    differenceFrame = cv::Mat();
    if(!capture.read(frame)) break;
    if(!capture.read(comparisonFrame)) return;
    comparisonFrame = tracker.transform(comparisonFrame);
    cv::imshow("Original", frame);
    frame = tracker.transform(frame);
    cv::imshow("Transformed", frame);

    // removeBackground();
    cv::absdiff(frame, comparisonFrame, differenceFrame);

    differenceFrame = tracker.boundingBox(differenceFrame);
    cv::imshow("Tracker", differenceFrame);

    if(checkExit()) break;
  }
  frame.release();
  differenceFrame.release();
  comparisonFrame.release();
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
