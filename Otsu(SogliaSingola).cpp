#include <iostream>
#include <opencv2/opencv.hpp>

#define H_SIZE 256

using namespace std;
using namespace cv;

void otsu(Mat&, Mat&);

int main(){
    String imgName = "../images/lena.jpg";
    Mat src = imread(imgName, IMREAD_GRAYSCALE);

    if(src.empty()){
        cout <<"Wrong image path selected! Try again..." << endl;
        return -1;
    }

    Mat dst;
    otsu(src, dst);

    imshow("Original", src);
    imshow("Otsu", dst);
    waitKey(0);
return 0;
}

void otsu(Mat &src, Mat &dst){
    Mat smoothed;
    GaussianBlur(src, smoothed, Size(3,3), 0, 0);

    double histogram[H_SIZE] = {0}, normHist[H_SIZE] = {0};
    double MN = smoothed.rows*smoothed.cols;
    double globAVG = 0;

    for(int i=0; i<smoothed.rows; i++){
        for(int j=0; j<smoothed.cols; j++){
            int val = smoothed.at<uchar>(i,j);
            histogram[val]++;
        }
    }

    for(int k=0; k<H_SIZE; k++){
        normHist[k] = histogram[k]/MN;
        globAVG += k * normHist[k];
    }

    double P1k, p1k, P2k, p2k, m1k, m2k;
    double varInterclasse = 0.0, varMax = 0.0;
    int bestTh = 0;

    for(int i=0; i<H_SIZE; i++){
        P1k += normHist[i];
        p1k += i * normHist[i];

        P2k = 1 - P1k;
        p2k = globAVG - p1k;

        m1k = p1k / P1k;
        m2k = p2k / P2k;

        varInterclasse = P1k * P2k * (m1k - m2k) * (m1k - m2k);
        if(varInterclasse > varMax){
            varMax = varInterclasse;
            bestTh = i;
        }
    } 

    cout << "La soglia ottimale e': " << bestTh << endl;
    cout << "La massima varianza interclasse e': " << varMax << endl;

    dst = Mat::zeros(smoothed.rows, smoothed.cols, CV_8UC1);
    for(int i=0; i<smoothed.rows; i++){
        for(int j=0; j<smoothed.cols; j++){
            if(smoothed.at<uchar>(i,j) > bestTh){
                dst.at<uchar>(i,j) = 255;
            }
        }
    }
}