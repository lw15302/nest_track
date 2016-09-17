#include <Tracking.hpp>

/**
 * Public functions
 */

/**
 * Tracking class constructor
 */
Tracking::Tracking()
{
  x = y = rad = 0;
}


/**
 * Returns the Tracking field of interest
 * @param  p - enumerated type specifying the field of interest
 * @return   - value of the field of interst
 */
int Tracking::get(Property p)
{
  switch(p) {
    case X:
      return x;
    case Y:
      return y;
    case RADIUS:
      return rad;
    default:
      break;
  }
}

/**
 * Add the current bounding box value to the array of box values. These area
 * used to calculate the average value over the previous 5 frames
 * @param box - current bounding box from VideoPlayer object
 */
void Tracking::setTrackingProperties(cv::Point2f centre, float radius)
{
  x = centre.x;
  y = centre.y;
  rad = radius;
}

void Tracking::setRegion(Region region)
{
  this->region = region;
}


Region Tracking::getRegion()
{
  return this->region;
}
