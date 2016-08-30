#include <opencv2/opencv.hpp>
#include <Tracker.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>
#include <cmath>
#include <chrono>
#include <time.h>

#define DATA_SIZE 1000
static std::mutex mtx;

class VideoPlayer
{
  public:
    VideoPlayer();
    ~VideoPlayer();

    std::array<int, DATA_SIZE> getTrackingData();
    void setTrackStatus(bool status);
    void run(void);

  private:
    Tracker tracker;
    int dataIndex;
    int lastY;
    bool accessingData;

    cv::Mat frame;
    cv::Mat comparisonFrame;
    cv::Mat differenceFrame;
    cv::VideoCapture capture;

    std::array<int, DATA_SIZE> dataSet;
    std::time_t start, current;
    std::atomic<bool> track;
    std::thread track_t;
    std::thread::id tId;

    void initVC();
    void openStream();
    void terminate();
    void exit();
    void joinThread();
    void setTrackingData();
    void resetData();
};
