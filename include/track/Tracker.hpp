#include <opencv2/opencv.hpp>

#define SAMPLE 20
typedef enum Property {X, Y, RADIUS} Property;

class Tracker
{

  public:
    Tracker();

    int get(Property p);
    void set(int val, Property p);
    void averageTrackerProperties(cv::Point2f centre, float radius);

  private:
    int x, y, rad;
    int arrayX[SAMPLE], arrayY[SAMPLE], arrayRad[SAMPLE];
    int samplePosition;

    void setAvg(Property p, int prop[]);
    void initArray(int prop[]);
};
