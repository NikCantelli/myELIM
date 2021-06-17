#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void myKmeans (Mat&, Mat&, int);

int main (){
    String imgPath = "../images/noi.jpg";
    Mat input = imread (imgPath);
	
	if (input.empty()){
		cout << "Can't find image " << imgPath << endl;
		return -1;
	}

	int nClusters = 0;
    cout << "#########################" << endl;
    cout << "### K-MEANS ALGORITHM ###" << endl;
    cout << "#########################" << endl;
    cout << "\nHow many clusters do you want?" << endl;
    cout << "Insert number here >> ";
    cin >> nClusters;

	Mat dst;
	myKmeans (input, dst, nClusters);

	//show result
	imshow ("Input image", input);
	imshow ("K-Means", dst);
	waitKey(0);

return 0;
}


void myKmeans (Mat& src, Mat& dst, int nClusters){
	//Initialize dst
	dst = src.clone();
	//Use a vector of scalar for centers and a vector of points for clusters
	vector<Scalar> centers;
	vector<vector<Point>> clusters;
	for (int k = 0; k < nClusters; k ++){
		Point center;
		center.x = rand()%src.cols;
		center.y = rand()%src.rows;
		Scalar scalar(src.at<Vec3b>(center)[0], src.at<Vec3b>(center)[1], src.at<Vec3b>(center)[2]);
		centers.push_back (scalar);
		vector<Point> t;
		clusters.push_back(t);
	}

	//Now assign pixels to clusters and recalculate centers using mean until difference is not < than 0.1
	double oldCenterSum = 0;
	double difference = INFINITY;
	while (difference > 0.1){	
		//Clear clusters
		for (int k = 0; k < nClusters; k ++){
			clusters[k].clear();
		}
		//Update pixels in clusters
		for (int x = 0; x < src.cols; x ++){
			for (int y = 0; y < src.rows; y ++){
				//Calculate distance from each center of clusters
				double minDistance = INFINITY;
				int clusterIndex = 0;
				Point pixel(x, y);
				for (int k = 0; k < nClusters; k ++){
					Scalar center = centers[k];
					double distance = pow (center[0] - src.at<Vec3b>(pixel)[0], 2) +
					pow (center[1] - src.at<Vec3b>(pixel)[1], 2) +
					pow (center[2] - src.at<Vec3b>(pixel)[2], 2);
					//If distance is < than minDistance
					if (distance < minDistance){
						//Save pixel into cluster of index k
						minDistance = distance;
						clusterIndex = k;
					}
				}
				clusters[clusterIndex].push_back(pixel);
			}
		}
		double newCenterSum = 0;
		//Calculate mean of each cluster
		for (int k = 0; k < nClusters; k ++){
			Scalar center = centers[k];
			vector<Point> clusterPoints = clusters[k];
			//Save center values of blue, green and red
			double blue = center[0];
			double green = center[1];
			double red = center[2];
			//For each point in k cluster
			for (int j = 0; j < clusterPoints.size(); j ++){
				Point pixel = clusterPoints[j]; 	//Extract point
				blue += src.at<Vec3b>(pixel)[0];	//Add blue value of point
				green += src.at<Vec3b>(pixel)[1];	//Add green value of point
				red += src.at<Vec3b>(pixel)[2];	//Add red value of point
			}
			blue /= clusterPoints.size() + 1;		//Calculate mean of blue + center
			green /= clusterPoints.size() + 1;		//Calculate mean of green + center
			red /= clusterPoints.size() + 1;		//Calculate mean of red + center
			Scalar newCenter (blue, green, red);		//Create new center
			//Calculate distance of new center with old center and sum it to newCenterSum
			newCenterSum += pow (blue - center[0], 2) + pow (green - center[1], 2) + pow (red - center[2], 2);
			//Update new centers
			centers[k] = newCenter;
			 
		}
		newCenterSum /= nClusters;
		difference = abs(oldCenterSum - newCenterSum);
		cout << "Difference " << difference << endl;
		oldCenterSum = newCenterSum;
	}
	//For each cluster
	for (int k = 0; k < nClusters; k ++){
		vector<Point> clusterPoints = clusters[k];
		Scalar center = centers[k];
		//For each point of cluster
		for (int j = 0; j < clusterPoints.size(); j ++){
			//Assign center value to other pixels of cluster
			Point pixel = clusterPoints[j];
			dst.at<Vec3b>(pixel)[0] = center[0];
			dst.at<Vec3b>(pixel)[1] = center[1];
			dst.at<Vec3b>(pixel)[2] = center[2];
		}
	}
}
