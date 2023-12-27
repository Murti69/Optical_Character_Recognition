#pragma once

#ifndef IMGPROCESSING_H
#define IMGPROCESSING_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include<iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
using namespace std;
using namespace cv;

class ImageProcessing {
private:
    Mat inputImage, ResizeImage, PreprocessedImg, WarpImg;
public:
    ImageProcessing(); // Default constructor
    ImageProcessing(const string& filename); // parameterized constructor
    void resizeImage(Mat inputImage);//to resize the image
    void Preprocessing(Mat Img);
    vector<Point> GetContours(Mat Img);
    void drawPoints(vector<Point> points, Scalar color);
    vector<Point> ReOrder(vector<Point> points);
    void Warping(Mat Img, vector<Point> points, float w,float h);
    Mat GetInputImg();
    Mat GetResizeImg();
    Mat GetPreprocessedImg();
    Mat GetWarpImg();
};

#endif // IMGPROCESSING_H
