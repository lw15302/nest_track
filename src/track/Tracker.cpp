#include <Tracker.hpp>

/**
 * Public functions
 */

/**
 * Tracker class constructor
 */
Tracker::Tracker()
{
  samplePosition = 0;
  initArray(arrayX);
  initArray(arrayY);
  initArray(arrayRad);
}


/**
 * Returns the Tracker field of interest
 * @param  p - enumerated type specifying the field of interest
 * @return   - value of the field of interst
 */
int Tracker::get(Property p)
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
void Tracker::averageTrackerProperties(cv::Point2f centre, float radius)
{
  if(samplePosition == SAMPLE) samplePosition = 0;
  std::cout << "centre: " << centre << std::endl;
  std::cout << "centreX: " << centre.x << std::endl;
  std::cout << "centreY: " << centre.y << std::endl;
  arrayX[samplePosition] = centre.x;
  arrayY[samplePosition] = centre.y;
  arrayRad[samplePosition] = radius;

  setAvg(X, arrayX);
  setAvg(Y, arrayY);
  setAvg(RADIUS, arrayRad);

  std::cout << "X: " << x << std::endl;
  std::cout << "Y: " << y << std::endl;

  samplePosition++;
}


/**
 * Private functions
 */

/**
 * Initialises property array values to 0.
 * @param rectProp - array with property values of interst
 */
void Tracker::initArray(int prop[])
{
  for(int i = 0; i < SAMPLE; i++) {
    prop[i] = 0;
    std::cout << "value in initArray: " << prop[i] << std::endl;
  }
}


/**
 * Calculates the average property value over the past 5 frames and assigns it
 * to the appropriate field
 * @param p        - the property of interest
 * @param rectProp - the property array of interst
 */
void Tracker::setAvg(Property p, int prop[])
{
  int value = 0;
  for(int i = 0; i < SAMPLE; i++) {
    value += prop[i];
    // std::cout << "value in loop: " << value << std::endl;
  }
  value = value/SAMPLE;
  std::cout << "value in setAvg: " << value << std::endl;

  switch(p) {
    case X:
      x = value;
      break;
    case Y:
      y = value;
      break;
    case RADIUS:
      rad = value;
      break;
    default:
      break;
  }
}
