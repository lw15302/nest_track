#include <iostream>
#include <Tracker.hpp>
#include <opencv2/opencv.hpp>



class VideoPlayer
{
  public:
    VideoPlayer();
    void run(void);
    void initVC();

  private:
    int threshold;
    int threshold_max;
    int trackX, trackY, trackW, trackH;
    int cSize;

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > contours_poly;
    std::vector<cv::Point2f> centre;
    std::vector<float> radius;

    cv::Rect boundRect;
    cv::Ptr<cv::BackgroundSubtractor> pMOG2;
    cv::Mat fgMaskMOG2;
    cv::RNG rng;
    cv::Mat background;
    cv::Mat frame;
    cv::VideoCapture capture;

    Tracker tracking;

    void openStream();
    void captureBackground();
    void captureStream();
    void transform();
    void boundingBox();
    void getContours();
    void getAverageTrackerProperties();
    void getBoundingShapes(int index);
    void drawOnFrame(int index);
    void getLargestContour(int* index);
    bool checkExit();
};
