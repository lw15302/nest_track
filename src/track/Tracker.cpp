#include <Tracker.hpp>

Tracker::Tracker()
{
  samplePosition = 0;
  initRectArray(rectX);
  initRectArray(rectY);
  initRectArray(rectW);
  initRectArray(rectH);
}

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


void Tracker::set(int val, Property p)
{
  switch(p) {
    case X:
      x = val;
      break;
    case Y:
      y = val;
      break;
    default:
      break;
  }
}


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


void Tracker::initRectArray(int rectProp[])
{
  for(int i = 0; i < SAMPLE; i++) {
    rectProp = 0;
  }
}


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
