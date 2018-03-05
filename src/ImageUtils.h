#ifndef BACKGROUND_UTILS_
#define BACKGROUND_UTILS_

#include <iostream>
#include <sstream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "controllers/Scene3DRenderer.h"

using namespace cv;
using namespace std;
using namespace nl_uu_science_gmt;

class ImageUtils {
public: 
	static void averageVideo(const std::string folder, const std::string file);
	static void showAllVideos(std::string folder, std::string file);
	static void doKMeans(Scene3DRenderer scene3d, vector<Point2f>& points, vector<int>& labels, Mat& centers, int frame);
	static void createColorModel(Scene3DRenderer scene3d, vector<Point2f>& points, vector<int>& labels, Mat& centers, int frame);
};

#endif /* BACKGROUND_UTILS_ */