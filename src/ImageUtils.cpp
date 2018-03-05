#include <iostream>
#include <sstream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ImageUtils.h"
#include "controllers/Scene3DRenderer.h"


using namespace cv;
using namespace nl_uu_science_gmt;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;


/**
 *	Averages a video into a png
 */
void ImageUtils::averageVideo(const std::string folder, const std::string file) {
	string filename(folder);
	filename.append("/").append(file).append(".avi");
	VideoCapture video(filename);
	if (!video.isOpened()) {
		string error = "Error when reading ";
		error.append(filename);
		throw error;
	}

	Mat frame;
	Mat avgFrame;

	string outFile(folder);
	outFile.append("/").append(file).append(".png");

	int nFrames = 0;
	for (;;) {
		video >> frame;

		
		if (frame.empty()) {
			break;
		} else {
			nFrames++;
			if (avgFrame.empty()) {
				avgFrame = Mat::zeros(frame.size(), CV_32FC3);
			}
			//avgFrame += frame;
			cv::accumulate(frame, avgFrame);
		}
	}

	if (nFrames > 0) {
		avgFrame = avgFrame / nFrames;
	}

	avgFrame.convertTo(avgFrame, CV_8U);	
	imwrite(outFile, avgFrame);
}

void ImageUtils::showAllVideos(std::string folder = "/data/", std::string file = "video.avi") {
	VideoCapture video[4];
	for (int i = 0; i < 4; i++) {
		namedWindow("Video" + to_string(i + 1), 1);
		string filepath = folder + "cam" + to_string(i + 1) + "/" + file;
		video[i] = VideoCapture(filepath);
		if (!video[i].isOpened()) {
			string error = "Error when reading ";
			error.append(filepath);
			throw error;
		}
	}

	//m_video.set(CV_CAP_PROP_POS_FRAMES, frame_number);

	int framenr = 0;
	for (;;) {
		framenr++;
		for (int i = 0; i < 4; i++) {
			Mat frame;
			video[i] >> frame;
			string videoWindow = "Video" + to_string(i + 1);
			imshow(videoWindow, frame);
		}

		char key = waitKey(100);
		if (key == 'c' || key == 'C')
			std::cout << framenr << endl;
	}
}

void ImageUtils::doKMeans(Scene3DRenderer scene3d, vector<Point2f>& points, vector<int>& labels, Mat& centers, int frame) {
	scene3d.setCurrentFrame(721);
	scene3d.processFrame();
	scene3d.getReconstructor().update();
	vector<Reconstructor::Voxel*> voxels = scene3d.getReconstructor().getVisibleVoxels();
	for (int i = 0; i < voxels.size(); i++) {
		float x = voxels.at(i)->x;
		float y = voxels.at(i)->y;
		points.push_back(Point2f(x, y)); // Ignore height
	}
	kmeans(points, 4, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0), 3, KMEANS_PP_CENTERS, centers);

	// cout << "CENTERS: " << centers << endl; // [604.23083, 721.55988; 698.37085, -552.45032; 1977.1892, -793.18921; 1980.0151, 650.80756]
	// cout << "LABELS: " << labels << endl; //  [2;2;2;2;2;2;2;2;2;2;2;2;1;2;1;1;... 
}

void ImageUtils::createColorModel(Scene3DRenderer scene3d, vector<Point2f>& points, vector<int>& labels, Mat& centers, int frame) {
	ImageUtils::doKMeans(scene3d, points, labels, centers, frame);

	Mat image(scene3d.getCameras()[0]->getFrame().rows, scene3d.getCameras()[0]->getFrame().cols, CV_8UC3);

	FileStorage fs;
	fs.open("data/cam1/config.xml", FileStorage::READ);
	Mat cameraMat, distMat, rvecs, tvecs;
	if (fs.isOpened())
	{
		fs["CameraMatrix"] >> cameraMat;
		fs["DistortionCoeffs"] >> distMat;
		fs["RotationValues"] >> rvecs;
		fs["TranslationValue"] >> tvecs;
	}
	else {
		cout << "Error";
	}

	vector<Point2f> imagePoints;
	projectPoints(scene3d.getReconstructor().getVisibleVoxels(), rvecs, tvecs, cameraMat, distMat, imagePoints);

	for (int i = 0; i < imagePoints.size(); i++) {
		if (labels[i] == 0) {
			cout << imagePoints[i];
			image.at<Vec3b>(imagePoints[i]) = Vec3b(0, 0, 255);
		}
	}
	namedWindow("Test", 1);
	imshow("Test", image); 
}