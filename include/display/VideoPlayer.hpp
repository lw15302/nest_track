#include <iostream>
#include <opencv2/opencv.hpp>

class VideoPlayer
{
  public:
    VideoPlayer();
    int run(void);

  private:
    int threshold;
    int threshold_max;
    cv::RNG rng;

    void openStream(cv::VideoCapture capture);
    cv::Mat transform(cv::Mat frame);
    cv::Mat boundingBox(cv::Mat frame);
};
