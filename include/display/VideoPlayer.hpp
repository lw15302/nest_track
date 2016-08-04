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
    cv::Ptr<cv::BackgroundSubtractor> pMOG2;
    cv::Mat background;
    cv::Mat frame;
    cv::Mat frame1;
    cv::VideoCapture capture;

    Tracker tracker;

    void initBackground();
    void openStream();
    void captureStream();
    bool checkExit();
};
