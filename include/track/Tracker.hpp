#include <opencv2/opencv.hpp>

enum Coordinate {X, Y}

class Tracker
{
  private:
    int x;
    int y;

    int get(Coordinate c);
    void set(int val, Coordinate c);
}
