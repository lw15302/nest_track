#include <iostream>
#include <Tracker.hpp>
#include <opencv2/opencv.hpp>

class VideoPlayer
{
  public:
    VideoPlayer();
    void run(void);
    void initVC();
    void setTrackStatus(bool status);

  private:
    cv::Mat comparisonFrame;
    cv::Mat differenceFrame;
    cv::Mat frame;

    cv::VideoCapture capture;
    bool track;
    Tracker tracker;

    void openStream();
    void captureStream();
    void exit();
};
