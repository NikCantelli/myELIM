#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void houghCircle(Mat&, Mat&, int);

int main(){
    String imgName = "../images/coins.jpg";
    Mat src = imread(imgName, IMREAD_ANYCOLOR);

    int threshold;
    cout << "#####################" << endl;
    cout << "### HARRIS CIRCLE ###" << endl;
    cout << "#####################" << endl;
    cout << "\nInsert threshold: ";
    cin >> threshold;

    Mat dst;
    houghCircle(src, dst, threshold);

    imshow("Hough Circles", dst);
    waitKey(0);

return 0;
}

void houghCircle(Mat &src, Mat &dst, int th){
    Mat gray, smoothed, edgeCanny;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, smoothed, Size(3,3), 0, 0);
    Canny(smoothed, edgeCanny, 90, 160, 3);

    int rMin = 80;
    int rMax = 100;
    int sizes [] = {edgeCanny.rows, edgeCanny.cols, rMax-rMin+1};
    Mat H = Mat(3, sizes, CV_8U, Scalar(0));

    int x, y, theta;
    for(int i=0; i<edgeCanny.rows; i++){
        for(int j=0; j<edgeCanny.cols; j++){
            if(edgeCanny.at<uchar>(i,j) == 255){
               for(int r=rMin; r<=rMax; r++){
                   for(theta=0; theta<360; theta++){
                       y = j - r * cos((theta * (CV_PI/180)));
                       x = i - r * sin((theta * (CV_PI/180)));
                       if((x > 0 && x < edgeCanny.rows) && (y > 0 && y < edgeCanny.cols)){
                           H.at<uchar>(x,y,r-rMin)++;
                       }
                   }
               }
            }
        }
    }

    dst = src.clone();

    for(int r=rMin; r<=rMax; r++){
        for(int i=0; i<edgeCanny.rows; i++){
            for(int j=0; j<edgeCanny.cols; j++){
                if(H.at<uchar>(i,j,r-rMin) >= th){
                    circle(dst, Point(j,i), 3, Scalar(0), 2, 8, 0);
                    circle(dst, Point(j,i), r, Scalar(255, 0, 0), 2, 8, 0);
                }
            }
        }
    }

}