#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void harris(Mat&, Mat&, int, float, int);

int main(){
	String imgName = "../images/harris.jpg";
	Mat src = imread(imgName, IMREAD_GRAYSCALE);
	
	if(src.empty()){
		cout << "Wrong image selected!" << endl;
		return -1;
	}
	
	int kSize = 0, th = 0;
    float k = 0;
	cout << "Insert value for harris algorithm: "<< endl;
	cout << "Insert filter size: ";
	cin >> kSize;
	cout << "Insert scalar: ";
	cin >> k;
	cout << "Insert threshold: ";
	cin >> th;
	
	Mat dst;
	harris(src, dst, kSize, k, th);
	
	imshow("Original", src);
	imshow("Harris", dst);
	waitKey(0);
	
return 0;
}


void harris(Mat &src, Mat &dst, int kSize, float k, int th){
	Mat dx, dy;
	Sobel(src, dx, CV_32F, 1, 0, kSize);
	Sobel(src, dy, CV_32F, 0, 1, kSize);
	
	Mat dx2, dy2, dxy;
	pow(dx, 2, dx2);
	pow(dy, 2, dy2);
	multiply(dx, dy, dxy);
	
	Mat dx2g, dy2g, dxyg;
	GaussianBlur(dx2, dx2g, Size(7,7), 2.0, 0.0);
	GaussianBlur(dy2, dy2g, Size(7,7), 0.0, 2.0);
	GaussianBlur(dxy, dxyg, Size(7,7), 2.0, 2.0);
	
	Mat R, detA, detB, mTrace;
	multiply(dx2g, dy2g, detA);
	multiply(dxyg, dxyg, detB);
	pow((dx2g + dy2g), 2, mTrace);
	R = ((detA - detB) - k * mTrace);
	
	normalize(R, R, 0, 255, NORM_MINMAX, CV_32FC1);
	
	Mat gray = src.clone();
	cvtColor(gray, dst, COLOR_GRAY2BGR);

	for(int i=0; i<src.rows; i++){
		for(int j=0; j<src.cols; j++){
			if((int)R.at<float>(i,j) > th){
				circle(dst, Point(j,i), 5, Scalar(0, 0, 255), 1.5, 8, 0);
			}
		}
	}
}