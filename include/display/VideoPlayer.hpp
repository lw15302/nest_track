#include <iostream>
#include <Tracker.hpp>
#include <opencv2/opencv.hpp>

class VideoPlayer
{
  public:
    VideoPlayer();
    int run(void);
    void initVC();

  private:
    int threshold;
    int threshold_max;
    cv::Ptr<cv::BackgroundSubtractor> pMOG2;
    cv::Mat fgMaskMOG2;
    cv::RNG rng;
    cv::Mat background;
    cv::Mat frame;
    cv::VideoCapture capture;


    void openStream();
    void captureBackground();
    void captureStream();
    void transform();
    void boundingBox();
};