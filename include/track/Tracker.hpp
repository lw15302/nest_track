#include <opencv2/opencv.hpp>

#define SAMPLE 5
typedef enum Property {X, Y, W, H} Property;

class Tracker
{

  public:
    Tracker();

    int get(Property p);
    void set(int val, Property p);
    void averageTrackerProperties(cv::Rect box);

  private:
    int x, y, w, h;
    int rectX[SAMPLE], rectY[SAMPLE], rectW[SAMPLE], rectH[SAMPLE];
    int samplePosition;

    void setAvg(Property p, int rectProp[]);
    void initRectArray(int rectProp[]);
};
