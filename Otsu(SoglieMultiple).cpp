#include <opencv2/opencv.hpp>
#include <iostream>

#define H_SIZE 256

using namespace std;
using namespace cv;

void otsu2(Mat&, Mat&);

int main(){
    String imgName = "../images/lena.jpg";
    Mat src = imread(imgName, IMREAD_GRAYSCALE);

    if(src.empty()){
        cout << "Wrong image PATH selected!\nPlease, try again..." << endl;
        return -1;
    }

    Mat dst;
    otsu2(src, dst);

    imshow("Original", src);
    imshow("Otsu 2", dst);
    waitKey(0);

return 0;
}

void otsu2(Mat &src, Mat&dst){
    Mat smoothed;
    GaussianBlur(src, smoothed, Size(3,3), 0, 0);

    double histogram[H_SIZE] = {0.0}, normHistogram[H_SIZE] = {0.0};
    double MN = smoothed.rows*smoothed.cols;
    int globAVG = 0;

    for(int i=0; i<smoothed.rows; i++){
        for(int j=0; j<smoothed.cols; j++){
            int val = smoothed.at<uchar>(i,j);
            histogram[val]++;
        }
    }

    for(int i=0; i<H_SIZE; i++){
        normHistogram[i] = histogram[i]/MN;
        globAVG += i * normHistogram[i];
    }

    double P1k = 0.0, p1k = 0.0, m1k = 0.0;
    double maxVar = 0.0;
    int bestTh1 = 0, bestTh2 = 0;

    for(int i=0; i<H_SIZE; i++){
        P1k += normHistogram[i];
        p1k += i * normHistogram[i];
        m1k = p1k / P1k;

        double P2k = 0.0, p2k = 0.0, m2k = 0.0;
        for(int j=i+1; j<H_SIZE; j++){
            P2k += normHistogram[j];
            p2k += j * normHistogram[j];
            m2k = p2k / P2k;

            double P3k = 1 - P2k - P1k;
            double p3k = globAVG - p2k - p1k;
            double m3k = p3k / P3k;

            double varInterclasse = P1k * pow((m1k - globAVG), 2) + P2k * pow((m2k - globAVG), 2) + P3k * pow((m3k - globAVG), 2);
            if(varInterclasse > maxVar){
                maxVar = varInterclasse;
                bestTh1 = i;
                bestTh2 = j;
            }
        }
    }

    cout << "Soglia ottimale MAX = " << bestTh2 << endl;
    cout << "Soglia ottimale MIN = " << bestTh1 << endl;

    dst = Mat::zeros(smoothed.rows, smoothed.cols, CV_8UC1);
    for(int i=0; i<smoothed.rows; i++){
        for(int j=0; j<smoothed.cols; j++){
            int val = smoothed.at<uchar>(i,j);
            if(val > bestTh2){
                dst.at<uchar>(i,j) = 255;
            } else if(val < bestTh1){
                dst.at<uchar>(i,j) = 127;
            }
        }
    }
}