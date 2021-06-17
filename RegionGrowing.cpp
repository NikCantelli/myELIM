#include <iostream>
#include <opencv2/opencv.hpp>
#include <stack>

#define NEIGHBORHOOD_SIZE 8

using namespace std;
using namespace cv;


const double min_region = 0.01; 
const uchar max_region = 100; 

const Point Shift2D[NEIGHBORHOOD_SIZE]{
    Point(-1, -1),  //in alto a sx
    Point(-1, 0),   //in alto al centro
    Point(-1, 1),   //in altro a dx
    Point(0, -1),   //centro sx
    Point(0, 1),    //centro dx
    Point(1, -1),   //in basso a sx
    Point(1, 0),    //in basso al centro
    Point(1, 1)     //in basso a dx
};

void myRegionGrowing(Mat&, Mat&, int);
void grow(Mat&, Mat&, Mat&, Point, int);

int main(){
    String imgName = "../images/noi.jpg";
    Mat original = imread(imgName, IMREAD_ANYCOLOR);

    if(original.empty()){
        cout << "Wrong image selected!"<< endl;
        cout << "Can't find: " << imgName << endl;
        cout << "Try again..." << endl;
        exit(-1);
    }
    int threshold = 0;
    cout << "################################" << endl;
    cout << "### REGION GRWOING ALGORITHM ###" << endl;
    cout << "################################" << endl;
    cout << "\nInsert THRESHOLD >> ";
    cin >> threshold;

    Mat dest;
    myRegionGrowing(original, dest, threshold);

    imshow("Original", original);
    imshow("RegionGrowing", dest);
    waitKey(0);
    return 0;
}

void myRegionGrowing(Mat &src, Mat &dest, int th){
    if(src.rows > 500 && src.cols > 500){
        resize(src, src, Size(0,0), 0.5, 0.5);
    }
    int min_region_area = (int)(min_region * src.rows * src.cols);
    Mat region = Mat::zeros(src.rows, src.cols, CV_8U);
    dest = Mat::zeros(src.rows, src.cols, CV_8U);
    uchar padding = 1;
    for(int i=0; i<src.cols; i++){
        for(int j=0; j<src.rows; j++){
            if(dest.at<uchar>(Point(i,j)) == 0){
                grow(src, dest, region, Point(i,j), th);
                int region_area_size = (int)sum(region).val[0]; 
                if(region_area_size > min_region_area){
                    dest = dest + region * padding;
                    imshow("Mask", region*255);
                    waitKey(0);
                    padding++;
                    if (padding > max_region)
						exit(-1);
                } else {
                    dest = dest + region * 255;
                }
                region = region - region;
            }
        }
    }
}

void grow(Mat &src, Mat &dest, Mat &region, Point seed, int th){
    stack<Point> stack;
    stack.push(seed);
    while(!stack.empty()){
        Point center = stack.top(); 
        stack.pop(); 
        for(int i=0; i<NEIGHBORHOOD_SIZE; i++){
            Point puntoStimato = center + Shift2D[i];
            if(puntoStimato.x < 0 || puntoStimato.x > src.cols-1 || puntoStimato.y < 0 || puntoStimato.y > src.rows-1){
                continue;
            } else {
                int delta = int ( pow(src.at<Vec3b>(center)[0] - src.at<Vec3b>(puntoStimato)[0], 2) +
                                  pow(src.at<Vec3b>(center)[1] - src.at<Vec3b>(puntoStimato)[1], 2) +
                                  pow(src.at<Vec3b>(center)[2] - src.at<Vec3b>(puntoStimato)[2], 2));
                if( dest.at<uchar>(puntoStimato) == 0 && region.at<uchar>(puntoStimato) == 0 && delta < th){ 
                    region.at<uchar>(puntoStimato) = 1; 
                    stack.push(puntoStimato);
                }
            }
        }
    }
}