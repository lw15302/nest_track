#include <VideoPlayer.hpp>
/**
 * Public functions
 */

 /**
  * Create VideoCapture object, check that it is valid and initialise the
  * stream
  */
int VideoPlayer::run(void)
{
  cv::VideoCapture capture(0);

  if( !capture.isOpened() ) {
    std::cout << "Cannot open video stream" << std::endl;
    return -1;
  }

  cv::namedWindow("Tracker");

  openStream(capture);
}


/**
 * Protected functions
 */

/**
 * Uses the OpenCV VideoCapture object to create open the video stream and
 * display it on the screen
 * @param VideoCapture object initialised in run()
 */
void VideoPlayer::openStream(cv::VideoCapture capture)
{
  while(1) {
    cv::Mat frame;
    bool read = capture.read(frame);

    if(!read) {
      std::cout << "Cannot read frame" << std::endl;
      break;
    }

    cv::imshow("Tracker", frame);

    if(cv::waitKey(30) == 27) {
      std::cout << "Exiting program" << std::endl;
      break;
    }
  }
}
