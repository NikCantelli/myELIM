#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void canny(Mat&, Mat&, int);
void nonMaximaSuppression(Mat&, Mat&, Mat&);
void isteresiThresholding(Mat&, Mat&, int, int);

int main(){
	String imgName = "../images/blox.jpg";
	Mat src = imread(imgName, IMREAD_GRAYSCALE);
	if(src.empty()){
		cout << "Wrong file selected!" << endl;
		return -1;
	}
	
	int kSize;
	cout << "Inserisci la dimensione del filtro: ";
	cin >> kSize;
	
	Mat dst;
	canny(src, dst, kSize);
	
	imshow("Original", src);
	imshow("Canny", dst);
	waitKey(0);
	
return 0;
}

void canny(Mat &src, Mat &dst, int k){
	int thL = 0, thH = 0;
	Mat smoothed;
	GaussianBlur(src, smoothed, Size(5,5), 0, 0);
	
	Mat sobX, sobY;
	Sobel(src, sobX, CV_32F, 1, 0, k);
	Sobel(src, sobY, CV_32F, 0, 1, k);
	
	Mat sobX2, sobY2;
	pow(sobX, 2, sobX2);
	pow(sobY, 2, sobY2);
	
	Mat magn;
	sqrt((sobX2 + sobY2), magn);
	normalize(magn, magn, 0, 255, NORM_MINMAX, CV_8U);
	
	Mat orient;
	phase(sobX, sobY, orient, true);
	
	Mat nms = Mat::zeros(magn.rows, magn.cols, CV_8U);
	nonMaximaSuppression(magn, orient, nms);

	imshow("NSM", nms);
	waitKey(0);
	
	cout << "Inserire la soglia minima e quella massima: " << endl;
    cout << "Soglia minima >> ";
    cin >> thL;
    cout << "Soglia massima >> ";
    cin >> thH;
	
	Mat out = Mat::zeros(nms.rows, nms.cols, CV_8U);
	isteresiThresholding(nms, out, thL, thH);
	
	dst = out;
}

void nonMaximaSuppression(Mat &magnitudo, Mat &orientazione, Mat &dest){
	for(int i=1; i<magnitudo.rows; i++){
		for(int j=1; j<magnitudo.cols; j++){
			float angolo = orientazione.at<float>(i,j);
			angolo = angolo > 180 ? angolo - 360 : angolo;
			if( ((angolo > -22.5) && (angolo <= 22.5)) || ((angolo > 157.5) && (angolo <= -157.5)) ){
				if(magnitudo.at<uchar>(i,j) > magnitudo.at<uchar>(i,j-1) && magnitudo.at<uchar>(i,j) > magnitudo.at<uchar>(i,j+1))
					dest.at<uchar>(i,j) = magnitudo.at<uchar>(i,j);
			}
			else if( ((angolo > -67.5) && (angolo <= -22.5)) || ((angolo > 112.5) && (angolo <= 157.5)) ){
				if(magnitudo.at<uchar>(i,j) > magnitudo.at<uchar>(i-1,j-1) && magnitudo.at<uchar>(i,j) > magnitudo.at<uchar>(i+1,j+1))
					dest.at<uchar>(i,j) = magnitudo.at<uchar>(i,j);
			}
			else if( ((angolo > -112.5) && (angolo <= -67.5)) || ((angolo > 67.5) && (angolo <= 112.5)) ){
				if(magnitudo.at<uchar>(i,j) > magnitudo.at<uchar>(i-1,j) && magnitudo.at<uchar>(i,j) > magnitudo.at<uchar>(i+1,j))
					dest.at<uchar>(i,j) = magnitudo.at<uchar>(i,j);
			}
			else if( ((angolo > -157.5) && (angolo <= -112.5)) || ((angolo > 22.5) && (angolo <= 67.5)) ){
				if(magnitudo.at<uchar>(i,j) > magnitudo.at<uchar>(i+1,j-1) && magnitudo.at<uchar>(i,j) > magnitudo.at<uchar>(i-1,j+1))
					dest.at<uchar>(i,j) = magnitudo.at<uchar>(i,j);			
			}
		}
	}
}

void isteresiThresholding(Mat &src, Mat &dst, int thL, int thH){
	for(int i=1; i<src.rows; i++){
		for(int j=1; j<src.cols; j++){
			if(src.at<uchar>(i,j) > thH)
				dst.at<uchar>(i,j) = 255;
			else if(src.at<uchar>(i,j) < thL)
				dst.at<uchar>(i,j) = 0;
			else if(src.at<uchar>(i,j) >= thL && src.at<uchar>(i,j) <= thH){
				for(int x=-1; x<=1; x++)
					for(int y=-1; y<=1; y++)
						if(src.at<uchar>(i+x,j+y) > thH)
							dst.at<uchar>(i+x, j+y) = 255;
			}
		}
	}
}