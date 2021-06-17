#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void hough(Mat&, Mat&, int);

int main(){
    int threshold = 0;
    String imgName = "../images/highway.jpg";
    Mat src = imread(imgName, IMREAD_ANYCOLOR);

    cout << "#######################" << endl;
    cout << "### HOUGH STRAIGHTS ###" << endl;
    cout << "#######################" << endl;
    cout << endl << "Insert threshold: ";
    cin >> threshold;

    Mat dst;
    hough(src, dst, threshold);

    imshow("Hough straights", dst);
    waitKey(0);

return 0;
}

void hough(Mat &src, Mat &dst, int th){
    Mat gray, smoothed, edgeCanny;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, smoothed, Size(3,3), 0, 0);
    Canny(smoothed, edgeCanny, 90, 160, 3);

    int dist = hypot(src.rows, src.cols);
    Mat votes = Mat::zeros(dist*2, 180, CV_8U);

    double rho, theta;
    for(int i=0; i<edgeCanny.rows; i++){
        for(int j=0; j<edgeCanny.cols; j++){
            if(edgeCanny.at<uchar>(i,j) == 255){
                for(theta=0; theta<180; theta++){
                    rho = j * cos((theta-90) * (CV_PI/180)) + i * sin((theta-90) * (CV_PI/180));
                    votes.at<uchar>(rho, theta)++;
                }
            }
        }
    }

    dst = src.clone();
    for(int r=0; r<votes.rows; r++){
        for(int t=0; t<votes.cols; t++){
            if(votes.at<uchar>(r,t) >= th){
                theta = (t-90) * (CV_PI/180);
                int x = r * cos(theta);
                int y = r * sin(theta);
                double sin_t = sin(theta);
                double cos_t = cos(theta);
                Point pt1(cvRound(x + dist * (-sin_t)), cvRound(y + dist * (cos_t)));
                Point pt2(cvRound(x - dist * (-sin_t)), cvRound(y - dist * (cos_t)));
                line(dst, pt1, pt2, Scalar(0,0,255), 2, 0);
            }
        }
    }
}