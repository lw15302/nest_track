#include <iostream>
#include <opencv2/opencv.hpp>

class VideoPlayer
{
  public:
    int run(void);
  protected:
    void openStream(cv::VideoCapture capture);
};
