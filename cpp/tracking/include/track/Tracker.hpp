#include <Tracking.hpp>
#include <opencv2/opencv.hpp>

typedef enum direction {
  STATIONARY,
  IN,
  OUT
} Direction;

class Tracker
{
  public:
    cv::Mat transform(cv::Mat frame);
    cv::Mat highlight(cv::Mat frame);
    cv::Mat backgroundSubtraction(cv::Mat frame);

    void setFrameProp(double frameHeight);
    int findActivity();


    Tracker();

  private:
    int threshold;
    int threshold_max;
    int cSize;
    int trackX, trackY, trackRad;
    int lastY;

    double frameHeight, thirdHeight, twoThirdHeight;

    Tracking tracking;
    cv::RNG rng;
    cv::Mat boundFrame;
    cv::Mat fgMaskMOG2;
    cv::Ptr<cv::BackgroundSubtractor> pMOG2;

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > contours_poly;
    std::vector< cv::Point2f > centre;
    std::vector< float > radius;

    void getContours();
    void getTrackerProperties(int index);
    void getBoundingShapes(int index);
    void drawOnFrame(int index);
    void getLargestContour(int* index);
    Direction travelDirection();
    void checkClear();
};
