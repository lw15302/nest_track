#include <opencv2/opencv.hpp>
#include <Tracker.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>
#include <chrono> // do I need this
#include <ctime>

#define DATA_SIZE 1000
static std::mutex mtx;

class VideoPlayer
{
  public:
    VideoPlayer();
    ~VideoPlayer();
    void run(void);
    int* getTrackingData();
    void setTrackStatus(bool status);


  private:
    int* dataSet;
    int* dataOut;
    int lastX;
    int dataIndex;
    Tracker tracker;

    cv::Mat comparisonFrame;
    cv::Mat differenceFrame;
    cv::Mat frame;
    cv::VideoCapture capture;

    // std::string originalWindow;
    // std::string trackerWindow;

    std::clock_t start, current;
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
