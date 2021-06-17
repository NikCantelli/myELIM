#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void houghCircles(Mat&, Mat&, int);

int main(){
    String imgName = "../images/coins.jpg";
    Mat src = imread(imgName, IMREAD_ANYCOLOR);

    int threshold;
    cout << "#####################" << endl;
    cout << "### HOUGH CIRCLE ###" << endl;
    cout << "#####################" << endl;
    cout << "\nInsert parameters: " << endl;
    cout << "THRESHOLD >> ";
    cin >> threshold;

    Mat dst;
    houghCircles(src, dst, threshold);

    imshow("Hough Circles", dst);
    waitKey(0);

return 0;
}

void houghCircles(Mat &src, Mat &dst, int th){
    Mat gray, smoothed, edge;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, smoothed, Size(7,7), 0, 0);
    Canny(smoothed, edge, 90, 170, 3);

    int rMax = 0, rMin = 0;
    cout << "R-MIN >> ";
    cin >> rMin;
    cout << "R-MAX >> ";
    cin >> rMax;

    int sizes[] = {src.rows, src.cols, rMax-rMin+1};
    Mat H = Mat(3, sizes, CV_8U, Scalar(0));

    int x, y, theta;
    for(int i=0; i<edge.rows; i++){
        for(int j=0;j<edge.cols; j++){
            if(edge.at<uchar>(i,j) == 255){
                for(int r=rMin; r<=rMax; r++){
                    for(theta=0; theta<360; theta++){
                        y = j - r * cos(theta * (CV_PI/180));
                        x = i - r * sin(theta * (CV_PI/180));
                        if((x > 0 && x < edge.rows) && (y > 0 && y < edge.cols)){
                            H.at<uchar>(x,y,r-rMin)++;
                        }
                    }
                }
            }
        }
    }

    dst = src.clone();

    for(int r=rMin; r<=rMax; r++){
        for(int i=0; i<edge.rows; i++){
            for(int j=0; j<edge.cols; j++){
                if(H.at<uchar>(i,j,r-rMin) >= th){
                    circle(dst, Point(j,i), 3, Scalar(255, 0, 0), 2, 8, 0);
                    circle(dst, Point(j,i), r, Scalar(0, 0, 255), 2, 8, 0);
                }
            }
        }
    }
}