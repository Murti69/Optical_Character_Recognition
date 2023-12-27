#include "imgProc.h"
#include<vector>
using namespace std;
ImageProcessing::ImageProcessing() {}

ImageProcessing::ImageProcessing(const string& filename) {
    inputImage = imread(filename);
}

void ImageProcessing::resizeImage(Mat inputImage) {
    resize(inputImage, ResizeImage, Size(), 0.5, 0.5);
}
void ImageProcessing::Preprocessing(Mat Img) {
    cvtColor(Img, PreprocessedImg, COLOR_BGR2GRAY);
    GaussianBlur(PreprocessedImg, PreprocessedImg, Size(3, 3), 3, 0);
    Canny(PreprocessedImg, PreprocessedImg, 25, 75);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(PreprocessedImg, PreprocessedImg, kernel);
}
vector<Point> ImageProcessing::GetContours(Mat Img) 
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    int MaxArea=0;

    findContours(Img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //drawContours(Img,contours,-1,Scalar(255,0,255),2);
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    vector<Point> biggest;
    for (int i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]);
        cout << area << endl;

        string objectType;
        if (area > 1000)
        {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            if (area > MaxArea && conPoly[i].size()==4)
            {
                drawContours(inputImage, conPoly, i, Scalar(255, 0, 255), 2);
                biggest = { conPoly[i][0],conPoly[i][1], conPoly[i][2], conPoly[i][3] };
                MaxArea = area;
            }
           // drawContours(inputImage, conPoly, i, Scalar(255, 0, 255), 2);
            //rectangle(Img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);

        }
    }
    return biggest;
    //It will return initial points.
    //we need to reorder these initial points before drawing contour
}
void ImageProcessing::drawPoints(vector<Point> points, Scalar color)
{
    for (int i = 0; i < points.size(); i++)
    {
        circle(inputImage, points[i], 10, color, FILLED);
        putText(inputImage, to_string(i), points[i], FONT_HERSHEY_PLAIN, 3, color, 3);
    }
}
vector<Point> ImageProcessing::ReOrder(vector<Point> points)
{
    vector<Point> newPoints;
    vector<int> SumPoints, SubPoints;
    for (int i = 0; i < 4; i++)
    {
        SumPoints.push_back(points[i].x + points[i].y);
        SubPoints.push_back(points[i].x - points[i].y);
    }
    newPoints.push_back(points[min_element(SumPoints.begin(), SumPoints.end()) - SumPoints.begin()]);//0
    newPoints.push_back(points[max_element(SubPoints.begin(), SubPoints.end()) - SubPoints.begin()]);//1
    newPoints.push_back(points[min_element(SubPoints.begin(), SubPoints.end()) - SubPoints.begin()]);//2
    newPoints.push_back(points[max_element(SumPoints.begin(), SumPoints.end()) - SumPoints.begin()]);//3

    return newPoints;
}

void ImageProcessing::Warping(Mat Img, vector<Point> points, float w, float h)
{
    Point2f src[4] = { points[0],points[1],points[2],points[3] };
    Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };
    Mat Matrix = getPerspectiveTransform(src, dst);
    warpPerspective(Img, WarpImg, Matrix, Point(w, h));
}

Mat ImageProcessing::GetInputImg()
{

    return inputImage;
}

Mat ImageProcessing::GetResizeImg()
{
    return ResizeImage;
}

Mat ImageProcessing::GetPreprocessedImg()
{
    return PreprocessedImg;
}

Mat ImageProcessing::GetWarpImg()
{
    return WarpImg;
}



