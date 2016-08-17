#include <Tracker.hpp>

Tracker::Tracker()
{
  tracking = Tracking();
  threshold = 75;
  threshold_max = 255;
  rng(12345);
}


/**
 * Converts frame to grayscale, applies blur and binary threshold
 * @params - frame to convert
 * @return - converted frame
 */
cv::Mat Tracker::transform(cv::Mat frame)
{
  cv::cvtColor(frame, frame, CV_BGR2GRAY);
  cv::threshold(frame, frame, threshold, threshold_max, cv::THRESH_BINARY);
  cv::erode(frame, frame, cv::Mat());
  cv::dilate(frame, frame, cv::Mat());
  cv::blur(frame, frame, cv::Size(3, 3));
  cv::Canny(frame, frame, 100, 200);
  return frame;
}


/**
 * Draws a bounding box around the object with the largest contour
 */
cv::Mat Tracker::boundingBox(cv::Mat frame)
{
  this->frame = frame;
  int largestIdx = 0;

  getContours();
  if( !contours.empty() ) {
    getLargestContour(&largestIdx);
    getBoundingShapes(largestIdx);
    getAverageTrackerProperties(largestIdx);
    drawOnFrame(largestIdx);
  }
  return this->frame;
}


/**
 * Finds all of the contours in the current frame and adds them to a vector.
 * Finds the centre and radius of the object, for use in drawing a circle
 */
void Tracker::getContours()
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
void Tracker::getAverageTrackerProperties(int index)
{
  // std::cout << "centre in getAverageTrackerProperties: " << centre[index] << std::endl;
  tracking.averageTrackingProperties(centre[index], radius[index]);
  lastX = trackX;
  trackX = tracking.get(X);
  trackY = tracking.get(Y);
  trackRad = tracking.get(RADIUS);
  // if(trackX > lastX) std::cout << "target moving right" << std::endl;
  // else if(trackX < lastX) std::cout << "target moving left" << std::endl;
  // else std::cout << "target is stationary" <<  std::endl;

  // std::cout<< "x:"<<trackX<<" y:"<<trackY<<" w:"<<trackW<<" h:"<<trackH<<std::endl;
}


/**
 * Finds the bounding box around the largest contour. Also finds the minimum
 * enclosing circle around the largest contour
 * @param index - the index of the largest contour in the contours vector
 */
void Tracker::getBoundingShapes(int index)
{
  cv::approxPolyDP(cv::Mat(contours[index]), contours_poly[index], 5, true);
  // boundRect = boundingRect(cv::Mat(contours_poly[index]));
  cv::minEnclosingCircle((cv::Mat)contours_poly[index], centre[index], radius[index]);
}


/**
 * Draws the contours and the bounding shapes on the screen
 * @param index - the index of the largest contour in the contours vector
 */
void Tracker::drawOnFrame(int index)
{
  cv::Point2f circCent(trackX, trackY);
  cv::Scalar colour = cv::Scalar(0, 255, 0);
  cv::cvtColor(frame, frame, CV_GRAY2BGR);
  cv::drawContours(frame, contours_poly, index, colour, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
  // std::cout <<"centre: " << circCent << std::endl;
  // std::cout <<"radius: " << trackRad << std::endl;
  cv::circle(frame, circCent, trackRad, colour, 2, 8, 0);
}

/**
 * Finds the largest contour in the array of contours
 * @param index - the index of the largest contour in the contours vector
 */
void Tracker::getLargestContour(int* index)
{
  double area = 0;
  double tempArea  = 0;

  for(int i = 0; i < cSize; i++) {
    tempArea = cv::contourArea(contours[i], false);
    if(tempArea > area) *index = i;
    area = tempArea;
  }
  // std::cout<<"contourArea: " << area << std::endl;
}


int Tracker::getX()
{
  return trackX;
}
