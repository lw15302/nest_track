#include <opencv2/opencv.hpp>

// #define SAMPLE 10
typedef enum Property {
  X,
  Y,
  RADIUS
} Property;

typedef enum region {
  NONE,
  REGION1,
  REGION2
} Region;

class Tracking
{
  public:
    Tracking();

    void setRegion(Region region);
    Region getRegion();

    int get(Property p);
    void set(int val, Property p);
    void setTrackingProperties(cv::Point2f centre, float radius);

  private:
    int x, y, rad;
    // int arrayX[SAMPLE], arrayY[SAMPLE], arrayRad[SAMPLE];
    int samplePosition;
    Region region;

    void setAvg(Property p, int prop[]);
    void initArray(int prop[]);
};
