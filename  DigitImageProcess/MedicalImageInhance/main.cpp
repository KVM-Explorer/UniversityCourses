#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv4/opencv2/imgproc.hpp>

void GammaTransform(cv::Mat &image,cv::Mat &dist,double gamma);
using namespace std;
int main() {

    auto image = cv::imread("image01.png");
    cv::Mat hist,gray;
    // 转为灰度图
    cv::cvtColor(image,gray,cv::COLOR_BGR2GRAY);
    cv::imshow("Origin",image);

//    // 直方图均衡化
//    cv::equalizeHist(gray,hist);
//    cv::imshow("Hist",hist);
//
//    // 中值滤波
//    cv::Mat media;
//    cv::medianBlur(hist,media,3);
//    cv::imshow("media Blur",media);
//    cv::waitKey(0);

    // Gamma 变换
//    cv::Mat gamma_image;
//    GammaTransform(image,gamma_image,0.5);
//    cv::imshow("Gamma",gamma_image);
//
//    // 中值滤波
//    cv::Mat media;
//    cv::medianBlur(gamma_image,media,3);
//    cv::imshow("media Blur",media);

    // Canny 检测
    cv::Mat canny_image;
    cv::Canny(image,canny_image,100,150);
//    cv::imshow("canny",canny_image);


//    cv::Mat result;
//    cv::add(image,canny_image,result);
//    cv::imshow("result",result);
    // 边缘检测

    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;

    cv::findContours(canny_image,contours,hierarchy,cv::RETR_LIST,cv::CHAIN_APPROX_NONE);
    cv::Mat contour_image = cv::Mat::zeros(image.rows,image.cols,CV_8UC3);
    cv::drawContours(contour_image,contours,-1,cv::Scalar(255,255,255));
    cv::imshow("countours",contour_image);

    // 图像叠加
    cv::Mat sum;
    cv::addWeighted(image,0.8,contour_image,0.2,0,sum);
    cv::imshow("Sum",sum);

    cv::Mat gamma_image;
    GammaTransform(sum,gamma_image,0.6);
    cv::imshow("Gamma",gamma_image);

    // 中值滤波
    cv::Mat media;
    cv::medianBlur(gamma_image,media,3);
    cv::imshow("media Blur",media);



    cv::waitKey(0);

    return 0;
}

void GammaTransform(cv::Mat &image,cv::Mat &dist,double gamma)
{
    cv::Mat image_gamma;
    image.convertTo(image_gamma,CV_64F,1.0/255.0);

    cv::pow(image_gamma,gamma,dist);

    dist.convertTo(dist,CV_8U,255,0);
}
