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

class ImageUtils {
public: 
	static void averageVideo(const std::string folder, const std::string file);
	static void showAllVideos(std::string folder, std::string file);
	static void doKMeans(nl_uu_science_gmt::Scene3DRenderer scene3d, Mat& labels, Mat& centers);
};

#endif /* BACKGROUND_UTILS_ */