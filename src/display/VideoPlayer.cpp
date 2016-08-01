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
  initVC();
  openStream();
}


void VideoPlayer::initVC() {
  if(!capture.open(0)) {
      throw "Cannot open video stream";
  }

  cv::namedWindow("Tracker");
}


VideoPlayer::VideoPlayer()
{
  tracking = Tracker();
  threshold = 100;
  threshold_max = 255;
  rng(12345);
}



/**
 * Private functions
 */


void VideoPlayer::openStream()
{
  pMOG2 = cv::createBackgroundSubtractorMOG2();
  if(!capture.read(background)) {
    throw "Unable to read frame for background";
  }
  captureStream();
}


void VideoPlayer::captureBackground() {
}



void VideoPlayer::captureStream() {
  while(1) {
    cv::Mat colourFrame;

    bool read = capture.read(frame);

    if(!read) {
      std::cout << "Cannot read frame" << std::endl;
      break;
    }
    cv::cvtColor(frame, frame, CV_BGR2GRAY);
    transform();
    pMOG2->apply(frame, fgMaskMOG2);

    boundingBox();

    cv::imshow("Tracker", fgMaskMOG2);

    if(cv::waitKey(30) == 27) {
      std::cout << "Exiting program" << std::endl;
      capture.release();
      break;
    }
  }
}


/**
 * Converts frame to grayscale, applies blur and binary threshold
 * @params - frame to convert
 * @return - converted frame
 */
void VideoPlayer::transform()
{
  // cv::cvtColor(frame, transformed, CV_BGR2GRAY);
  cv::threshold(frame, frame, threshold, threshold_max, cv::THRESH_BINARY);
  cv::blur(frame, frame, cv::Size(3, 3));
}


void VideoPlayer::boundingBox()
{
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours(frame, contours, hierarchy, CV_RETR_TREE,
    CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

  int cSize = contours.size();

  std::vector<std::vector<cv::Point> > contours_poly(cSize);
  cv::Rect boundRect;
  std::vector<cv::Point2f> centre(cSize);
  std::vector<float> radius(cSize);

  double area = 0;
  double tempArea  = 0;
  int largestIdx = 0;


  for(int i = 0; i < cSize; i++) {
    tempArea = cv::contourArea(contours[i], false);
    if(tempArea > area) largestIdx = i;
    area = tempArea;
  }

  cv::approxPolyDP(cv::Mat(contours[largestIdx]), contours_poly[largestIdx], 5, true);
  boundRect = boundingRect(cv::Mat(contours_poly[largestIdx]));
  cv::minEnclosingCircle((cv::Mat)contours_poly[largestIdx], centre[largestIdx], radius[largestIdx]);

  tracking.averageTrackerProperties(boundRect);
  trackX = tracking.get(X);
  trackY = tracking.get(Y);
  trackW = tracking.get(W);
  trackH = tracking.get(H);

  std::cout<< "x:"<<trackX<<" y:"<<trackY<<" w:"<<trackW<<" h:"<<trackH<<std::endl;

  cv::Scalar colour = cv::Scalar(0,255, 0);
    // rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
  cv::drawContours(frame, contours_poly, largestIdx, colour, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
  cv::rectangle(frame, cv::Point(trackX, trackY), cv::Point(trackX + trackW, trackY + trackY), colour, 2, 8, 0);
  cv::circle(frame, centre[largestIdx], (int)radius[largestIdx], colour, 2, 8, 0);
}
