#include <Tracking.hpp>
#include <opencv2/opencv.hpp>

class Tracker
{
  public:
    cv::Mat transform(cv::Mat frame);
    cv::Mat boundingBox(cv::Mat frame);

    Tracker();
    int getX();

  private:
    int threshold;
    int threshold_max;
    int cSize;
    int trackX, trackY, trackRad;
    int lastX;

    Tracking tracking;
    cv::RNG rng;
    cv::Mat frame;

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > contours_poly;
    std::vector< cv::Point2f > centre;
    std::vector< float > radius;

    void getContours();
    void getAverageTrackerProperties(int index);
    void getBoundingShapes(int index);
    void drawOnFrame(int index);
    void getLargestContour(int* index);
};
