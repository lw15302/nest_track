#include <opencv2/opencv.hpp>
#include <Tracker.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>
#include <chrono>

static std::mutex mtx;

class VideoPlayer
{
  public:
    VideoPlayer();
    ~VideoPlayer();
    void run(void);
    void setTrackStatus(bool status);
    void terminate();
    void exit();
    void joinThread();

  private:
    cv::Mat comparisonFrame;
    cv::Mat differenceFrame;
    cv::Mat frame;
    cv::VideoCapture capture;

    std::string originalWindow;
    std::string trackerWindow;

    std::thread track_t;
    std::atomic<bool> track;
    std::thread::id tId;
    Tracker tracker;

    void initVC();
    void openStream();
};
