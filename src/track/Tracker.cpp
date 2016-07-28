#include <Tracker.hpp>

int Tracker::get(Coordinate c) {
  switch(c) {
    case X:
      return x;
    case Y:
      return y;
    default:
      break;
  }
}


void Tracker::set(int val, Coordinate c) {
  switch(c) {
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
