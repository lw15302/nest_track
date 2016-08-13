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
  cv::namedWindow("Tracker", CV_WINDOW_AUTOSIZE);
  cv::namedWindow("Original", CV_WINDOW_AUTOSIZE);
  std::cout << "start tracking" << std::endl;
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
  std::cout << "track outside loop: " << track << std::endl;
  while(track) {
    std::cout << "tracking" << std::endl;
    differenceFrame = cv::Mat();
    std::cout << "tracking" << std::endl;

    if(!capture.read(frame)) break;
    std::cout << "tracking1" << std::endl;
    cv::imshow("Original", frame);
    std::cout << "tracking2" << std::endl;
    frame = tracker.transform(frame);
    std::cout << "tracking3" << std::endl;
    if(!capture.read(comparisonFrame)) break;
    std::cout << "tracking4" << std::endl;
    comparisonFrame = tracker.transform(comparisonFrame);
    std::cout << "tracking5" << std::endl;
    cv::absdiff(frame, comparisonFrame, differenceFrame);
    std::cout << "tracking6" << std::endl;


    differenceFrame = tracker.boundingBox(differenceFrame); //Rename function as it no longer creates a boundingBox
    std::cout << "tracking7: " << track << std::endl;


    cv::imshow("Tracker", differenceFrame);
    std::cout << "tracking8" << std::endl;
    cv::waitKey(30);
  }
  std::cout <<"Exiting loop" << std::endl;
  exit();
}


void VideoPlayer::exit()
{
  cv::destroyAllWindows();
  frame.release();
  differenceFrame.release();
  comparisonFrame.release();
  capture.release();
}


void VideoPlayer::setTrackStatus(bool status)
{
  track = status;
}
