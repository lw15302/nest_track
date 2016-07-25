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


VideoPlayer::VideoPlayer()
{
  threshold = 100;
  threshold_max = 255;

}



/**
 * Private functions
 */

/**
 * Uses the OpenCV VideoCapture object to create open the video stream and
 * display it on the screen
 * @params - VideoCapture object initialised in run()
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

    cv::Mat transformedFrame = transform(frame);
    cv::Mat boundingBoxFrame = boundingBox(transformedFrame);

    cv::imshow("Tracker", boundingBoxFrame);

    if(cv::waitKey(30) == 27) {
      std::cout << "Exiting program" << std::endl;
      break;
    }
  }
}

/**
 * Converts frame to grayscale, applies blur and binary threshold
 * @params - frame to convert
 * @return - converted frame
 */
cv::Mat VideoPlayer::transform(cv::Mat frame)
{
  cv::Mat transformed;
  cv::cvtColor(frame, transformed, CV_BGR2GRAY);
  cv::blur(transformed, transformed, cv::Size(3, 3));
  cv::threshold(transformed, transformed, threshold, threshold_max, cv::THRESH_BINARY);
  return transformed;
}


cv::Mat VideoPlayer::boundingBox(cv::Mat frame)
{
  cv::Mat withRect;
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours(frame, contours, hierarchy, CV_RETR_TREE,
    CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

  int cSize = contours.size();

  std::vector<std::vector<cv::Point> > contours_poly(cSize);
  std::vector<cv::Rect> boundRect(cSize);
  std::vector<cv::Point2f> centre(cSize);
  std::vector<float> radius(cSize);

  for(int i = 0; i < cSize; i++) {
    cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
    boundRect[i] = boundingRect(cv::Mat(contours_poly[i]));
    cv::minEnclosingCircle((cv::Mat)contours_poly[i], centre[i], radius[i]);
  }

  withRect = cv::Mat::zeros(frame.size(), CV_8UC3);

  for(int i = 0; i < cSize; i++) {
    cv::Scalar colour = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
    cv::drawContours(withRect, contours_poly, i, colour, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
    cv::rectangle(withRect, boundRect[i].tl(), boundRect[i].br(), colour, 2, 8, 0);
    cv::circle(withRect, centre[i], (int)radius[i], colour, 2, 8, 0);
  }

  return withRect;
}
