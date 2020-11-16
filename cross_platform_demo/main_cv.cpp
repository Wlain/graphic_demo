//
// Created by william on 2020/11/15.
//
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] const char * argv[]) {
    //read image
    cv::Mat img = cv::imread("/Users/william/git/demo/graphic_demo/cross_platform_demo/resources/tianlei.jpg", cv::IMREAD_COLOR);//载入
    cv::imshow("opencv window", img);
    cv::waitKey(0);//等待
    cv::destroyAllWindows();
    return 0;
}
