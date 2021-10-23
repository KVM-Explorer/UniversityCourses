#include <iostream>
#include <opencv2/opencv.hpp>

struct Mapping
{
    int color;
    int num;
}origin_table[256],target_table[256];

int mapping_table[256];

void BuildColorMappingTable(cv::Mat origin,cv::Mat target);
cv::Mat TransformColor(cv::Mat &origin,int mapping[],int n);
void NormalizeImage(cv::Mat &origin,cv::Mat &target);
bool CompareColor(Mapping a,Mapping b)
{
    return a.num<b.num;
}
int main() {
    // 读取图片
    auto target = cv::imread("target.png");
    auto origin = cv::imread("origin.png");
    cv::imshow("origin",origin);
    cv::imshow("target",target);

    // 转换为HSV
    cv::Mat hsv_target,hsv_origin;
    cv::cvtColor(target,hsv_target,cv::COLOR_BGR2HSV);
    cv::cvtColor(origin,hsv_origin,cv::COLOR_BGR2HSV);


    // 提取Hue通道
    cv::Mat origin_channels[3],target_channels[3];
    cv::split(hsv_origin,origin_channels);
    cv::split(hsv_target,target_channels);


     //建立色彩映像表
//    BuildColorMappingTable(origin_channels[0], target_channels[0]);


    // 变换色彩
//    TransformColor(origin_channels[0],mapping_table,256);
    NormalizeImage(origin_channels[0],target_channels[0]);

    cv::Mat result;
    cv::merge(origin_channels,3,result);

    cv::cvtColor(result,result,cv::COLOR_HSV2BGR);
    cv::split(result,origin_channels);
    cv::swap(origin_channels[1],origin_channels[2]);
    cv::merge(origin_channels,3,result);
    cv::imshow("Result",result);
    cv::imshow("origin",origin);
    cv::waitKey(0);

}

void NormalizeImage(cv::Mat &origin,cv::Mat &target)
{
    // 均衡化
    cv::Mat hist_origin,hist_target;
    cv::equalizeHist(origin,hist_origin);
    cv::equalizeHist(target,hist_target);

    //计算均衡化的直方图
    int channels = 0;
    int dims =1;
    int hist_size = 256;
    float range[] = {0,255};
    const float *hist_range[] = {range};
    cv::Mat result_origin,result_target;
    cv::calcHist(&hist_origin,1,&channels,cv::Mat(),result_origin,dims,&hist_size,hist_range);
    cv::calcHist(&hist_target,1,&channels,cv::Mat(),result_target,dims,&hist_size,hist_range);

    // 计算图像的累计直方图
    float sum_origin[256]={0};
    float sum_target[256]={0};
    for(int i=0;i<256;i++)
    {
        if(i==0)
        {
            sum_origin[i] = result_origin.at<float>(i);
            sum_target[i] = result_target.at<float>(i);
        }
        else
        {
            sum_origin[i] = sum_origin[i-1] + result_origin.at<float>(i);
            sum_target[i] = sum_target[i-1] + result_target.at<float>(i);
        }
    }

    /// 规定化
    // 1, 累计概率差值
    float diff[256][256];
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; ++j) {
            diff[i][j] = fabs(sum_origin[i]-sum_target[j]);
        }
    }
    // 2.构建灰度映射
    cv::Mat lut(1,256,CV_8U);
    for (int i = 0; i < 256; ++i) {
        float min = diff[i][0];
        int index = 0;
        for(int j=0;j<256;j++)
        {
            if(min>diff[i][j])
            {
                min = diff[i][j];
                index = j;
            }
        }
        lut.at<uchar>(i) = static_cast<uchar>(index);
    }
    cv::LUT(origin,lut,origin);

}

void BuildColorMappingTable(cv::Mat origin,cv::Mat target)
{
    // 提取色彩
    for(int i=0;i<256;i++)
    {
        origin_table[i].color = i,origin_table[i].num = 0;
        target_table[i].color = i,target_table[i].num = 0;
    }


    for(int j=0;j<origin.cols;j++)
        for(int i=0;i<origin.rows;i++)
        {
            int color = origin.at<uchar>(j,i);
            origin_table[color].num++;
        }
    for(int j=0;j<target.cols;j++)
        for(int i=0;i<target.rows;i++)
        {
            int color = target.at<uchar>(j,i);
            target_table[color].num++;
        }
    std::sort(origin_table,origin_table+255, CompareColor);
    std::sort(target_table,target_table+255, CompareColor);


    // 建立映射
    for(int i=0;i<256;i++)
    {
        mapping_table[origin_table[i].color] =origin_table[i].color;
    }



}

cv::Mat TransformColor(cv::Mat &origin,int mapping[],int n)
{
    for(int j=0;j<origin.cols;j++)
        for(int i=0;i<origin.rows;i++)
        {
            int color = origin.at<uchar>(j,i);
            origin.at<uchar>(j,i) = mapping[color];
        }
}
