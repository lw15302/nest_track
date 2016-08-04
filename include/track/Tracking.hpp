#include <opencv2/opencv.hpp>

#define SAMPLE 5
typedef enum Property {X, Y, RADIUS} Property;

class Tracking
{
  public:
    Tracking();

    int get(Property p);
    void set(int val, Property p);
    void averageTrackingProperties(cv::Point2f centre, float radius);

  private:
    int x, y, rad;
    int arrayX[SAMPLE], arrayY[SAMPLE], arrayRad[SAMPLE];
    int samplePosition;

    void setAvg(Property p, int prop[]);
    void initArray(int prop[]);
};
