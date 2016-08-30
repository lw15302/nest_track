#include <Tracker.hpp>

Tracker::Tracker()
{
  tracking = Tracking();
  threshold = 75;
  threshold_max = 255;
  lastY = 0;
  pMOG2 = cv::createBackgroundSubtractorMOG2();
}


/**
 * Converts frame to grayscale, applies blur and binary threshold
 * @params - frame to convert
 * @return - converted frame
 */
cv::Mat Tracker::transform(cv::Mat frame)
{
  cv::Mat outFrame = cv::Mat();
  cv::cvtColor(frame, outFrame, CV_BGR2GRAY);
  cv::threshold(frame, outFrame, threshold, threshold_max, cv::THRESH_BINARY);
  cv::erode(frame, outFrame, cv::Mat());
  cv::dilate(frame, outFrame, cv::Mat());
  cv::blur(frame, outFrame, cv::Size(3, 3));
  // cv::Canny(frame, outFrame, 100, 200);
  return frame;
}


cv::Mat Tracker::backgroundSubtraction(cv::Mat frame)
{
  pMOG2->apply(frame, fgMaskMOG2, 0.1);
  return fgMaskMOG2;
}



/**
 * Draws a bounding box around the object with the largest contour
 */
cv::Mat Tracker::highlight(cv::Mat frame)
{
  boundFrame = frame;
  int largestIdx = 0;

  getContours();
  if( !contours.empty() ) {
    getLargestContour(&largestIdx);
    getBoundingShapes(largestIdx);
    getAverageTrackerProperties(largestIdx);
    drawOnFrame(largestIdx);
  }
  return boundFrame;
}


/**
 * Finds all of the contours in the current frame and adds them to a vector.
 * Finds the centre and radius of the object, for use in drawing a circle
 */
void Tracker::getContours()
{
  cv::findContours(boundFrame, contours, hierarchy, CV_RETR_LIST,
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
  cv::cvtColor(boundFrame, boundFrame, CV_GRAY2BGR);
  cv::drawContours(boundFrame, contours_poly, index, colour, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
  // std::cout <<"centre: " << circCent << std::endl;
  // std::cout <<"radius: " << trackRad << std::endl;
  cv::circle(boundFrame, circCent, trackRad, colour, 2, 8, 0);
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
  std::cout<<"contourArea: " << area << std::endl;
}


void Tracker::setFrameProp(double frameHeight)
{
  this->frameHeight = frameHeight;
  thirdHeight = (frameHeight / 3);
  twoThirdHeight = (frameHeight / 3) * 2;
  std::cout <<"frame height: " << frameHeight << " third: " << thirdHeight << " twothird: " << twoThirdHeight << std::endl;
}


int Tracker::findActivity()
{
  Direction travelling = travelDirection();

  checkClear();

  std::cout << "ypos: " << trackY << std::endl;
  if(travelling == IN) {
    std::cout << "findActivity flag1" << std::endl;
    if(trackY >= twoThirdHeight) {
      std::cout << "findActivity flag2" << std::endl;
      if(tracking.getRegion() == REGION1) {
        std::cout << "findActivity flag3" << std::endl;
        tracking.setRegion(NONE);
        lastY = trackY;
        return 1;
      }
      lastY = trackY;
    }
    else if(trackY <= thirdHeight) {
      std::cout << "findActivity flag4" << std::endl;
      std::cout << "Region1 1 set, ypos: " << trackY << std::endl;
      tracking.setRegion(REGION1);
      std::cout << "findActivity flag5" << std::endl;
      lastY = trackY;
      return -1;
    }
    lastY = trackY;
  }
  else if(travelling == OUT) {
    std::cout << "findActivity flag6" << std::endl;
    if(trackY <= thirdHeight) {
      std::cout << "findActivity flag7" << std::endl;
      if(tracking.getRegion() == REGION2) {
        std::cout << "findActivity flag8" << std::endl;
        tracking.setRegion(NONE);
        lastY = trackY;
        return 2;
      }
      lastY = trackY;
    }
    else if(trackY >= twoThirdHeight) {
      std::cout << "findActivity flag9" << std::endl;
      tracking.setRegion(REGION2);
      lastY = trackY;
      return -1;
    }
    lastY = trackY;
  }
  else {
    lastY = trackY;
    return -1;
  }
}

Direction Tracker::travelDirection()
{
  int difference = trackY - lastY;

  if(difference < 0) {
    return OUT;
  }
  else if(difference > 0) {
    return IN;
  }
  else return STATIONARY;
}


void Tracker::checkClear()
{
  int absoluteDifference = abs(trackY - lastY);
  std::cout << "absolute difference: " << absoluteDifference << std::endl;
  if(absoluteDifference > 200) {
    tracking.setRegion(NONE);
  }
}
