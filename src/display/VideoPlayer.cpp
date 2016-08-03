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
   tracking = Tracker();
   threshold = 100;
   threshold_max = 255;
   rng(12345);
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
void VideoPlayer::initVC() {
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
  pMOG2 = cv::createBackgroundSubtractorMOG2();
  captureStream();
}


/**
 * Contains the main loop that captures and displays the frames from the
 * camera. It also calls a number of functions associated with image processing
 * and object detection
 */
void VideoPlayer::captureStream() {
  while(1) {
    if(!capture.read(frame)) break;

    transform();
    pMOG2->apply(frame, frame);

    boundingBox();
    cv::imshow("Tracker", frame);
    if(checkExit()) break;
  }
}


/**
 * Converts frame to grayscale, applies blur and binary threshold
 * @params - frame to convert
 * @return - converted frame
 */
void VideoPlayer::transform()
{
  cv::cvtColor(frame, frame, CV_BGR2GRAY);
  cv::threshold(frame, frame, threshold, threshold_max, cv::THRESH_BINARY);
  cv::blur(frame, frame, cv::Size(3, 3));
}


/**
 * Draws a bounding box around the object with the largest contour
 */
void VideoPlayer::boundingBox()
{
  int largestIdx = 0;

  getContours();
  getLargestContour(&largestIdx);
  getBoundingShapes(largestIdx);
  getAverageTrackerProperties(largestIdx);
  drawOnFrame(largestIdx);
}


/**
 * Finds all of the contours in the current frame and adds them to a vector.
 * Finds the centre and radius of the object, for use in drawing a circle
 */
void VideoPlayer::getContours()
{
  cv::findContours(frame, contours, hierarchy, CV_RETR_LIST,
    CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    cSize = contours.size();

    contours_poly = std::vector<std::vector<cv::Point> >(cSize);
    centre = std::vector<cv::Point2f>(cSize);
    radius = std::vector<float>(cSize);
}


/**
 * Sets the tracking properties based on an average of the past 5 frames. These
 * values are used later to draw a bounding rectangle
 */
void VideoPlayer::getAverageTrackerProperties(int index)
{
  std::cout << "centre in getAverageTrackerProperties: " << centre[index] << std::endl;
  tracking.averageTrackerProperties(centre[index], radius[index]);

  trackX = tracking.get(X);
  trackY = tracking.get(Y);
  trackRad = tracking.get(RADIUS);
  // std::cout<< "x:"<<trackX<<" y:"<<trackY<<" w:"<<trackW<<" h:"<<trackH<<std::endl;
}


/**
 * Finds the bounding box around the largest contour. Also finds the minimum
 * enclosing circle around the largest contour
 * @param index - the index of the largest contour in the contours vector
 */
void VideoPlayer::getBoundingShapes(int index)
{
  cv::approxPolyDP(cv::Mat(contours[index]), contours_poly[index], 5, true);
  // boundRect = boundingRect(cv::Mat(contours_poly[index]));
  cv::minEnclosingCircle((cv::Mat)contours_poly[index], centre[index], radius[index]);
}


/**
 * Draws the contours and the bounding shapes on the screen
 * @param index - the index of the largest contour in the contours vector
 */
void VideoPlayer::drawOnFrame(int index)
{
  cv::Point2f circCent(trackX, trackY);
  cv::Scalar colour = cv::Scalar(rng.uniform(0, 255),rng.uniform(0, 255), rng.uniform(0, 255));
  cv::drawContours(frame, contours_poly, index, colour, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
  // cv::rectangle(frame, cv::Point(trackX, trackY), cv::Point(trackX + trackW, trackY + trackY), colour, 2, 8, 0);
  std::cout << "flag" << std::endl;
  std::cout <<"centre: " << circCent << std::endl;
  std::cout <<"radius: " << trackRad << std::endl;
  cv::circle(frame, circCent, trackRad, colour, 2, 8, 0);
}

/**
 * Finds the largest contour in the array of contours
 * @param index - the index of the largest contour in the contours vector
 */
void VideoPlayer::getLargestContour(int* index)
{
  double area = 0;
  double tempArea  = 0;

  for(int i = 0; i < cSize; i++) {
    tempArea = cv::contourArea(contours[i], false);
    if(tempArea > area) *index = i;
    area = tempArea;
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
