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
    cv::Mat comparisonFrame;
    cv::Mat differenceFrame;
    cv::Mat frame;

    cv::VideoCapture capture;

    Tracker tracker;

    void openStream();
    void captureStream();
    bool checkExit();
};
