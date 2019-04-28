//============================================================================
// Name        : main.cpp
// Author      : Christian Thurow
// Description : BRDF Calculation for Rapid Prototyping Project at TU-Berlin
//============================================================================

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "cv.h"
#include "highgui.h"
#include "brdfdata.h"
#include <gl\GL.h> //do we really need that or just glut?
#include "glut.h"
#include "glutcallbacks.h"

void Render(CBRDFdata* data, int argc, char** argv);

using namespace std;

CBRDFdata m_brdf;
int m_width = 800;
int m_height = 600;

// main function
int main(int argc, char** argv)
{	
	//data structure for brdf
	m_brdf = CBRDFdata();

	cout << "loading data.." << endl;
	//read in 3d model + calc surface normals
	m_brdf.LoadModel("img\\timber.obj");
	m_brdf.m_model = 1; //0: Phong, 1: Blinn-Phong

	//read in 16 images
	//assuming images are named 1.jpg to 16.jpg
	m_brdf.LoadImages();
	m_brdf.LoadDarkImage();
	m_brdf.SubtractAmbientLight(); //not really important, but correct

	//optional output
	//m_brdf.PrintNormalisedImages();
	
	//read in geometry and camera infos
	m_brdf.LoadCameraParameters("img\\timber.cal");

	//initialise led positions
	m_brdf.InitLEDs();
	//render model
	cout << "begin rendering" << endl;
	Render(&m_brdf, argc, argv);

	//cvWaitKey(0);

    return 0;
}

void Render(CBRDFdata* data, int argc, char** argv)
{
	//render model in opengl
	//let user interactively choose whether to show BRDF-correct lighting
	// ->on key press, activate a shader that takes into account the brdf values of each surface

	//show ground plane with origin
	//show model, in correct relation to origin
	//show camera position

	//let user rotate and translate around

    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(1000, 100);
	glutInitWindowSize(m_width, m_height);
	glutInit(&argc, argv);
    glutCreateWindow("Scanned Mesh");

    // register GLUT callbacks (see glutcallbacks.c)
    RegisterCallbacks();
    // initialize OpenGL (see glutcallbacks.c)
    Init();
    // start GLUT event loop
    glutMainLoop();
}
