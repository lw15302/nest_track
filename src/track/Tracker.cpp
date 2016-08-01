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
  initRectArray(rectX);
  initRectArray(rectY);
  initRectArray(rectW);
  initRectArray(rectH);
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
    case W:
      return w;
    case H:
      return h;
    default:
      break;
  }
}

/**
 * Add the current bounding box value to the array of box values. These area
 * used to calculate the average value over the previous 5 frames
 * @param box - current bounding box from VideoPlayer object
 */
void Tracker::averageTrackerProperties(cv::Rect box)
{
  if(samplePosition == SAMPLE) samplePosition = 0;

  rectX[samplePosition] = box.x;
  rectY[samplePosition] = box.y;
  rectW[samplePosition] = box.width;
  rectH[samplePosition] = box.height;

  setAvg(X, rectX);
  setAvg(Y, rectY);
  setAvg(W, rectW);
  setAvg(H, rectH);

  samplePosition++;
}


/**
 * Private functions
 */

/**
 * Initialises property array values to 0.
 * @param rectProp - array with property values of interst
 */
void Tracker::initRectArray(int rectProp[])
{
  for(int i = 0; i < SAMPLE; i++) {
    rectProp = 0;
  }
}


/**
 * Calculates the average property value over the past 5 frames and assigns it
 * to the appropriate field
 * @param p        - the property of interest
 * @param rectProp - the property array of interst
 */
void Tracker::setAvg(Property p, int rectProp[])
{
  int value = 0;
  for(int i = 0; i < SAMPLE; i++) {
    value += rectProp[i];
  }
  value = value/SAMPLE;

  switch(p) {
    case X:
      x = value;
      break;
    case Y:
      y = value;
      break;
    case W:
      w = value;
      break;
    case H:
      h = value;
      break;
    default:
      break;
  }
}
