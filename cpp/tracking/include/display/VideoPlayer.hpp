#include <opencv2/opencv.hpp>
#include <Tracker.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>
#include <cmath>
#include <chrono> // do I need this
#include <time.h>

#define DATA_SIZE 1000
static std::mutex mtx;

class VideoPlayer
{
  public:
    VideoPlayer();
    ~VideoPlayer();
    void run(void);
    std::array<int, DATA_SIZE> getTrackingData();
    void setTrackStatus(bool status);


  private:
    std::array<int, DATA_SIZE> dataSet;
    int lastY;
    int dataIndex;
    bool accessingData;
    Tracker tracker;


    cv::Mat comparisonFrame;
    cv::Mat differenceFrame;
    cv::Mat frame;
    cv::VideoCapture capture;

    // std::string originalWindow;
    std::string trackerWindow;

    std::time_t start, current;
    std::thread track_t;
    std::atomic<bool> track;
    std::thread::id tId;

    void initVC();
    void openStream();
    void terminate();
    void exit();
    void joinThread();
    void setTrackingData();
    void resetData();
};
