/*
 * Glut.h
 *
 *  Created on: Nov 15, 2013
 *      Author: coert
 */

#ifndef GLUT_H_
#define GLUT_H_

#include <opencv2/core/core.hpp>

#ifdef _WIN32
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#include <GL/glu.h>
#endif

// i am not sure about the compatibility with this...
#define MOUSE_WHEEL_UP   3
#define MOUSE_WHEEL_DOWN 4

using namespace cv;
using namespace std;

namespace nl_uu_science_gmt
{

class Scene3DRenderer;

class Glut
{
	Scene3DRenderer &m_scene3d;

	// Assignment 3
	std::vector<cv::Scalar> baseColorModels;

	static Glut* m_Glut;

	static void drawGrdGrid();
	static void drawCamCoord();
	static void drawVolume();
	static void drawArcball();
	static void drawVoxels(vector<int>& correspondingLabels, vector<int>& labels);
	static void drawWCoord();
	static void drawInfo();
	void drawTrail();

	// Assignment 3
	void createBaseColorModels();
	void trackPeople(vector<int>& correspondingLabels, vector<int>& labels);
	void calculateCorrespondingLabels(vector<int>& correspondingLabels, vector<int>& labels);

	static inline void perspectiveGL(
			GLdouble, GLdouble, GLdouble, GLdouble);

#ifdef _WIN32
	static void SetupPixelFormat(HDC hDC);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

public:
	Glut(
			Scene3DRenderer &);
	virtual ~Glut();

#ifdef __linux__
	void initializeLinux(
			const char*, int, char**);
	static void mouse(
			int, int, int, int);
#endif
#ifdef _WIN32
	int initializeWindows(const char*);
	void mainLoopWindows();
#endif

	static void keyboard(
			unsigned char, int, int);
	static void motion(
			int, int);
	static void reshape(
			int, int);
	static void reset();
	static void idle();
	static void display(vector<int>& correspondingLabels, vector<int>& labels);
	static void update(
			int);
	static void quit();

	Scene3DRenderer& getScene3d() const
	{
		return m_scene3d;
	}
};

} /* namespace nl_uu_science_gmt */

#endif /* GLUT_H_ */
