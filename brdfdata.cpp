//============================================================================
// Name        : brdfdata.cpp
// Author      : Christian Thurow
// Description : contains functions for the CBRDFdata class
//============================================================================

#include "stdafx.h"
#include "brdfdata.h"
#include "highgui.h"
#include <iostream>
#include <fstream>
#include "glut.h"
#include "levmar\levmar.h"

bool CBRDFdata::LoadImages()
{
	m_images.clear();
	for(int i=0; i<m_numImages; i++)
	{
		string path = "img\\";
		char num[4];
		itoa(i+1, num, 10);
		string extension = ".png";
		
		path += num;
		path += extension;

		IplImage* newImg = cvLoadImage(path.c_str(), CV_LOAD_IMAGE_COLOR);
		if(newImg == NULL)
			return false;

		if(m_width <= -1 || m_height <= -1)
		{
			m_width = newImg->width;
			m_height = newImg->height;
		}

		m_images.push_back(newImg);
	}

	return true;
}

//puts out all input images onto the screen
void CBRDFdata::PrintImages()
{
	int i=0;
	for(vector<IplImage*>::iterator it = m_images.begin(); it != m_images.end(); it++, i++)
	{
		string name = "image: ";
		char num[4];
		itoa(i+1, num, 10);
		name += num;
		cv::namedWindow(name.c_str(), CV_WINDOW_AUTOSIZE);
		cv::imshow(name.c_str(), (*it));
    }
}

//faulty
void CBRDFdata::NormaliseImages()
{
	for(vector<IplImage*>::iterator it = m_images.begin(); it != m_images.end(); it++)
	{
		//find darkest pixel in channel
		int min_r = 0, min_g = 0, min_b = 0;
		
		for(int x = 0; x < (*it)->width; x++)
			for(int y = 0; y < (*it)->height; y++)
			{				
				CvScalar pix = cvGet2D((*it), y, x);
				if(pix.val[0] < min_r)
					min_r = pix.val[0];
				if(pix.val[1] < min_g)
					min_g = pix.val[1];
				if(pix.val[2] < min_b)
					min_b = pix.val[2];
			}


		//subtract darkest pixel from all pixels in image
		CvScalar dark;		
		dark.val[0] = min_r;
		dark.val[1] = min_g;
		dark.val[2] = min_b;

		IplImage* darkImage;
		CvSize size;
		size.height = (*it)->height;
		size.width = (*it)->width;
		darkImage = cvCreateImage(size, (*it)->depth, 3);
		cvSet(darkImage, dark);

		cvSub((*it), darkImage, (*it));
    }
}

void CBRDFdata::PrintNormalisedImages()
{
	int i=0;
	for(vector<IplImage*>::iterator it = m_images.begin(); it != m_images.end(); it++, i++)
	{
		string name = "normed image: ";
		char num[4];
		itoa(i+1, num, 10);
		name += num;
		cv::namedWindow(name.c_str(), CV_WINDOW_AUTOSIZE);
		cv::imshow(name.c_str(), (*it));
    }
}

bool CBRDFdata::LoadDarkImage()
{
	string path = "img\\";
	string name = "dark";
	string extension = ".png";
		
	path += name;
	path += extension;

	m_dark = cvLoadImage(path.c_str(), CV_LOAD_IMAGE_COLOR);
	if(m_dark == NULL)
		return false;

	return true;
}

void CBRDFdata::SubtractAmbientLight()
{
	if(m_dark == NULL)
		return;
	
	for(vector<IplImage*>::iterator it = m_images.begin(); it != m_images.end(); it++)
	{
		cvSub((*it), m_dark, (*it));
	}
}

void CBRDFdata::LoadCameraParameters(std::string filename)
{
	std::vector<char> buffer;
	ReadInFile(filename, &buffer);

	int i=0;
	for(std::vector<char>::iterator it = buffer.begin(); it != buffer.end() && (*it != NULL); ++it)
	{
		char curr = *it;
		if(curr == '<')
		{
			it++;
			string currentParameter = "";
			for(; it != buffer.end() && (*it) != '>'; ++it) //read in parameter name
			{
				currentParameter += (*it);
			}
			string currentValue = "";
			if((*it) == '>') //parameter detected
				it++;

			//read in value
			for(; it != buffer.end() && (*it) != '<'; ++it)
			{
				currentValue += (*it);
			}
			if((*it) == '<') //end of value reached
				it++;

			WriteValue(currentParameter, currentValue);

			//go to end of current line
			for(; it != buffer.end() && (*it) != '>'; ++it) //read in parameter name
			{}
			if((*it) == '>') //detected end of line
				it++;
		}
	}
}

void CBRDFdata::WriteValue(std::string parameter, std::string value)
{
	double val = atof(value.c_str());

	if(parameter == "cx")
		cvSetReal1D(m_principal_point, 0, val);

	if(parameter == "cy")
		cvSetReal1D(m_principal_point, 1, val);

	if(parameter == "f")
		m_focalLength = val;

	if(parameter == "sx")
		m_pixelSizeX = val;

	if(parameter == "nx")
		cvSetReal1D(m_n, 0, val);

	if(parameter == "ny")
		cvSetReal1D(m_n, 1, val);

	if(parameter == "nz")
		cvSetReal1D(m_n, 2, val);

	if(parameter == "ox")
		cvSetReal1D(m_o, 0, val);

	if(parameter == "oy")
		cvSetReal1D(m_o, 1, val);

	if(parameter == "oz")
		cvSetReal1D(m_o, 2, val);

	if(parameter == "ax")
		cvSetReal1D(m_a, 0, val);

	if(parameter == "ay")
		cvSetReal1D(m_a, 1, val);

	if(parameter == "az")
		cvSetReal1D(m_a, 2, val);

	if(parameter == "px")
		cvSetReal1D(m_p, 0, val);

	if(parameter == "py")
		cvSetReal1D(m_p, 1, val);

	if(parameter == "pz")
		cvSetReal1D(m_p, 2, val);

}

//returns 0 if file could not be read, 1 otherwise
bool CBRDFdata::ReadInFile(std::string filename, std::vector<char>* buffer)
{
	buffer->clear();

	std::ifstream file;
	file.open(filename, std::ios::in | ios::binary);

	if(!file.fail())
	{ 
		while(!file.eof())
		{
			char currChar;
			file.get(currChar);
			buffer->push_back(currChar);
		}
		file.close();
    } 
	else
       return false;

	return true;
}

//returns 0 if file could not be read, 1 otherwise
bool CBRDFdata::ReadInFileAsLines(std::string filename, std::vector<char*>* buffer)
{
	buffer->clear();

	std::ifstream file;
	file.open(filename, std::ios::in | ios::binary);

	if(!file.fail())
	{ 
		while(!file.eof())
		{
			char* currLine = new char[1000];
			file.getline(currLine, 1000);
			buffer->push_back(currLine);
		}
		file.close();
    } 
	else
       return false;

	return true;
}

void CBRDFdata::ScaleMesh()
{
	double maxX = 0.0;
	double maxY = 0.0;
	double maxZ = 0.0;

	double minX = 0.0;
	double minY = 0.0;
	double minZ = 0.0;

	//get max values
	for(int i=0; i<m_faces->m_numFaces; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(m_faces[i].m_point[j].m_x > maxX)
				maxX = m_faces[i].m_point[j].m_x;
			if(m_faces[i].m_point[j].m_y > maxY)
				maxY = m_faces[i].m_point[j].m_y;
			if(m_faces[i].m_point[j].m_z > maxZ)
				maxZ = m_faces[i].m_point[j].m_z;

			if(m_faces[i].m_point[j].m_x < minX)
				minX = m_faces[i].m_point[j].m_x;
			if(m_faces[i].m_point[j].m_y < minY)
				minY = m_faces[i].m_point[j].m_y;
			if(m_faces[i].m_point[j].m_z < minZ)
				minZ = m_faces[i].m_point[j].m_z;
		}
	}

	double diffX = maxX - minX;
	double diffY = maxY - minY;
	double diffZ = maxZ - minZ;

	//scale
	double scaleFactor = 10.0;
	for(int i=0; i<m_faces->m_numFaces; i++)
	{
		for(int j=0; j<3; j++)
		{
			m_faces[i].m_point[j].m_x /= diffX;
			m_faces[i].m_point[j].m_y /= diffY;
			m_faces[i].m_point[j].m_z /= diffZ;

			m_faces[i].m_point[j].m_x *= scaleFactor;
			m_faces[i].m_point[j].m_y *= scaleFactor;
			m_faces[i].m_point[j].m_z *= scaleFactor;
		}
	}

}

void CBRDFdata::LoadModel(std::string filename)
{
	//load 3d mesh from file
	//calc surface normals of object, if not already done..
	//give every surface a number, if they don't already have one(needed for later mapping with pixels) -> number of index
	//choose appropriate data structure, remember: we need to be able to store the BRDF infos as well for each surface/triangle

	vector<char*>* linesInFile = new vector<char*>;
	ReadInFileAsLines(filename, linesInFile);

	int numVertices = GetNumVertices(linesInFile);

	m_vertices = new vertex[numVertices];
	
	WriteVertices(linesInFile);

	int num_faces = GetNumFaces(linesInFile);

	m_faces = new triangle[num_faces];
	m_faces[0].m_numFaces = num_faces;

	WriteFaces(linesInFile);

	CalcFaceNormals();

	ScaleMesh();
}

void CBRDFdata::CalcFaceNormals()
{
	bool fail = false;

	for(int i=0; i<m_faces[0].m_numFaces; i++)
	{
		m_faces[i].m_numFaces = m_faces[0].m_numFaces;
		vertex P = m_faces[i].m_point[0];
		vertex Q = m_faces[i].m_point[1];
		vertex R = m_faces[i].m_point[2];

		double a1 = Q.m_x - P.m_x;
		double a2 = Q.m_y - P.m_y;
		double a3 = Q.m_z - P.m_z;

		double b1 = R.m_x - P.m_x;
		double b2 = R.m_y - P.m_y;
		double b3 = R.m_z - P.m_z;

		//Kreuzprodukt
		double c1 = a2*b3 - a3*b2;
		double c2 = a3*b1 - a1*b3;
		double c3 = a1*b2 - a2*b1;

		double norm = sqrt(c1*c1 + c2*c2 + c3*c3);

		if(norm == 0.0) fail = true;

		m_faces[i].m_normal.m_x = c1/norm;
		m_faces[i].m_normal.m_y = c2/norm;
		m_faces[i].m_normal.m_z = c3/norm;
	}

	if(fail)
		cout << "Error in: CalcFaceNormals()" << endl;
}

void CBRDFdata::WriteFaces(vector<char*>* linesInFile)
{
	
	std::vector<char*>::iterator it;
	for(it = linesInFile->begin(); it != linesInFile->end() && (*it != NULL); it++)
	{
		if((*it)[0] == 'f')
			break;
	}

	int i=0;

	while((*it)[0] == 'f')
	{
		int startpos = 2;
		int pos = 2;
		int coordcount = 0;
		bool wait_for_next = false;

		char* currLine = (*it);
		char* currNumber[3];
		
		//search for spaces
		for (; ; pos++)
		{
			if (currLine[pos] != ' ' && currLine[pos] != '/' && currLine[pos] != '\0') continue;

			if (currLine[pos] == ' ')
			{
				wait_for_next = false;
				startpos = pos + 1;
				continue;
			}

			if (currLine[pos] == '\0') break;
			if (wait_for_next) continue;

			currNumber[coordcount] = new char[100];
			strncpy(currNumber[coordcount], currLine+startpos, pos-startpos);
			currNumber[coordcount][pos-startpos] = '\0';
			coordcount++;

			startpos = pos + 1;

			if (currLine[pos] == '/') wait_for_next = true;
		}

		m_faces[i].m_point[0] = m_vertices[atoi(currNumber[0])-1];
		m_faces[i].m_point[1] = m_vertices[atoi(currNumber[1])-1];
		m_faces[i].m_point[2] = m_vertices[atoi(currNumber[2])-1];

		it++;
		i++;
	}	
}

int CBRDFdata::GetNumFaces(vector<char*>* linesInFile)
{
	int numFaces = 0;

	//go to first occurance of f
	std::vector<char*>::iterator it;
	for(it = linesInFile->begin(); it != linesInFile->end() && (*it != NULL); it++)
	{
		if((*it)[0] == 'f')
			break;
	}
	std::string currLine = (*it);
	for(; it != linesInFile->end() && (*it != NULL); it++)
	{
		if((*it)[0] != 'f')
		{
			break;
		}
	}
	currLine = (*it);
	if((*it)[0] == '#')
	{
		std::string numVertices = "";
		for(int i=0; i<strlen(*it); i++)
		{
			if((*it)[i] > 47 && (*it)[i] < 58)
				numVertices += (*it)[i];
		}
		numFaces = atoi(numVertices.c_str());
	}

	return numFaces;
}

void CBRDFdata::WriteVertices(vector<char*>* linesInFile)
{
	std::vector<char*>::iterator it;
	for(it = linesInFile->begin(); it != linesInFile->end() && (*it != NULL); it++)
	{
		if((*it)[0] == 'v')
			break;
	}

	int i=0;

	while((*it)[0] == 'v')
	{
		int startpos = 2;
		int pos = 2;
		int coordcount = 0;

		char* currLine = (*it);
		char* currNumber[3];
		
		//search for spaces
		for (; ; pos++)
		{
			if (currLine[pos] != ' ' && currLine[pos] != '\0') continue;

			currNumber[coordcount] = new char[100];
			strncpy(currNumber[coordcount], currLine+startpos, pos-startpos);
			currNumber[coordcount][pos-startpos] = '\0';
			coordcount++;

			startpos = pos + 1;

			if (currLine[pos] == '\0') break;
		}

		m_vertices[i].m_x = atof(currNumber[0]);
		m_vertices[i].m_y = atof(currNumber[1]);
		m_vertices[i].m_z = atof(currNumber[2]);

		it++;
		i++;
	}
}

int CBRDFdata::GetNumVertices(vector<char*>* linesInFile)
{
	int numVerts = 0;
	//go to first occurance of v 
	std::vector<char*>::iterator it;
	for(it = linesInFile->begin(); it != linesInFile->end() && (*it != NULL); it++)
	{
		if((*it)[0] == 'v')
		{
			break;
		}
	}
	std::string currLine = (*it);
	for(; it != linesInFile->end() && (*it != NULL); it++)
	{
		if((*it)[0] != 'v')
		{
			break;
		}
	}
	currLine = (*it);
	if((*it)[0] == '#')
	{
		std::string numVertices = "";
		for(int i=0; i<strlen(*it); i++)
		{
			if((*it)[i] > 47 && (*it)[i] < 58)
				numVertices += (*it)[i];
		}
		numVerts = atoi(numVertices.c_str());
	}
	

	return numVerts;
}

void CBRDFdata::SaveValuesToSurface(int currentSurface, CvMat* brdf, int colorChannel) //BGR
{
	//saves the brdf information stored in brdf to the corresponding surface on the model
	//attention: mind the color channel.. so we will need that information 3x at the surface..
	//brdf matrix contains parameters: kd, ks and n - in that order

	m_faces[currentSurface].brdf[colorChannel].kd = cvGetReal1D(brdf, 0);
	m_faces[currentSurface].brdf[colorChannel].ks = cvGetReal1D(brdf, 1);
	m_faces[currentSurface].brdf[colorChannel].n = cvGetReal1D(brdf, 2);
}

CvMat* CBRDFdata::GetCameraOrigin()
{
	return m_p;
}

CvMat* CBRDFdata::GetA()
{
	return m_a;
}

CvMat* CBRDFdata::GetO()
{
	return m_o;
}

CvMat* CBRDFdata::GetN()
{
	return m_n;
}

double CBRDFdata::GetCX()
{
	return cvGetReal1D(m_principal_point, 0);
}

double CBRDFdata::GetCY()
{
	return cvGetReal1D(m_principal_point, 1);
}

CvMat* CBRDFdata::CalcPixel2SurfaceMapping()
{
	CvMat* map = cvCreateMat(m_height, m_width, CV_32S);
	cvSet(map, cvScalar(0.0));

	//create map: pixel of image to surface on model(triangle?!) -> 0 means pixel not on model; val_x >0 means belongs to surface x
	//return matrix has size of input image and contains zeros for all pixels that don't contain the object of interest, otherwise the number to
	//the surface that pixel points
	//this map should be the same for all images, so we only have to calc it ones

	for(int i=0; i<m_faces->m_numFaces; i++)
	{
		GLdouble winX = 0.0;
		GLdouble winY = 0.0;
		GLdouble winZ = 0.0;

		GLdouble objectX = 0.0;
		GLdouble objectY = 0.0;
		GLdouble objectZ = 0.0;
		
		for (int j = 0; j < 3; j++) //get center of current triangle
		{
			objectX += m_faces[i].m_point[j].m_x;
			objectY += m_faces[i].m_point[j].m_y;
			objectZ += m_faces[i].m_point[j].m_z;
		}
		
		objectX /= 3.0; objectY /= 3.0; objectZ /= 3.0;

		GLdouble model_view[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, model_view);

		GLdouble projection[16];
		glGetDoublev(GL_PROJECTION_MATRIX, projection);

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		int errorValue = gluProject(objectX, objectY, objectZ, model_view, projection, viewport, &winX, &winY, &winZ);

		if(winY >=0 && winX >=0)
			cvSetReal2D(map, winY, winX, i);
	}

	return map;
}

void CBRDFdata::InitLEDs()
{
	//we will need the absolute positions of the leds here hard coded!
	//we measured:
	//coordinate origin: x,y = 0; z = 11,5cm
	//led 1,2,3,4: radius from coordinate origin: 30,5cm, height of the leds from zero: 36,5cm
	//led 5,6,7,8: --------------------------------||---, height ------------------||-- 26,0cm
	//led 9,10,11,12: -----------------------------||---, height ------------------||-- 15,0cm
	//led 13,14,15,16: ----------------------------||---, height ------------------||--  4,5cm

	m_led = new vertex[m_numImages];

	for (int i = 0; i < m_numImages; i++)
	{		
		//y coordinate
		switch (i / 4)
		{
			case 0: 
				m_led[i].m_y = 365.0 - 115.0;
				break;
			case 1: 
				m_led[i].m_y = 260.0 - 115.0;
				break;
			case 2: 
				m_led[i].m_y = 150.0 - 115.0;
				break;
			default: 
				m_led[i].m_y = 45.0 - 115.0;
				break;
		}
		
		//x+z coordinates
		switch (i % 4)
		{
		case 0:
			m_led[i].m_x = 305.0 * sin(6.0/33.0*CV_PI*0.5);
			m_led[i].m_z = 305.0 * cos(6.0/33.0*CV_PI*0.5);
			break;
		case 1:
			m_led[i].m_x = 305.0 * sin(13.0/33.0*CV_PI*0.5);
			m_led[i].m_z = 305.0 * cos(13.0/33.0*CV_PI*0.5);
			break;
		case 2:
			m_led[i].m_x = 305.0 * sin(20.0/33.0*CV_PI*0.5);
			m_led[i].m_z = 305.0 * cos(20.0/33.0*CV_PI*0.5);
			break;
		default:
			m_led[i].m_x = 305.0 * sin(27.0/33.0*CV_PI*0.5);
			m_led[i].m_z = 305.0 * cos(27.0/33.0*CV_PI*0.5);
			break;
		}
	}

	//for (int i = 0; i < m_numImages; i++)
	//{
	//	cout << i << ": x:" << m_led[i].m_x << " y:" << m_led[i].m_y << " z:" << m_led[i].m_z << endl;
	//}
}

CvMat* CBRDFdata::GetCosRV(int currentSurface)
{
	CvMat* theta = cvCreateMat(1, m_numImages, CV_32F);
	//matrix theta contains angle for led1 in position 0 and angle for led16 in position 15(m_numImages-1)

	//TODO: check if the below code works!
	for (int i = 0; i < m_numImages; i++)
	{
		triangle surface = m_faces[currentSurface];
		
		//center of triangle
		double x = 0;
		double y = 0;
		double z = 0;
		
		for(int j = 0; j < 3; j++)
		{
			x += surface.m_point[j].m_x;
			y += surface.m_point[j].m_y;
			z += surface.m_point[j].m_z;
		}
		
		x /= 3.0; y /= 3.0; z /= 3.0;

		//observer vector
		double Vx = cvGetReal1D(m_p, 0) - x;
		double Vy = cvGetReal1D(m_p, 1) - y;
		double Vz = cvGetReal1D(m_p, 2) - z;

		double length = sqrt(Vx*Vx + Vy*Vy + Vz*Vz);

		Vx /= length;
		Vy /= length;
		Vz /= length;

		//-1 * light vector, this orientation is needed for the formula below
		double Lx = x - m_led[i].m_x;
		double Ly = x - m_led[i].m_y;
		double Lz = x - m_led[i].m_z;
			
		length = sqrt(Lx*Lx + Ly*Ly + Lz*Lz);
			
		Lx /= length;
		Ly /= length;
		Lz /= length;
		
		//need reflexion vector, see http://wiki.delphigl.com/index.php/Reflexion
		//P = (N*L) * N
		//R = -2 * (N*L) * N + L = -2 * P + L

		double scale_factor = surface.m_normal.m_x * Lx + surface.m_normal.m_y * Ly + surface.m_normal.m_z * Lz;
		double Px = scale_factor * surface.m_normal.m_x;
		double Py = scale_factor * surface.m_normal.m_y;
		double Pz = scale_factor * surface.m_normal.m_z;

		double Rx = Lx - 2*Px;
		double Ry = Ly - 2*Py;
		double Rz = Lz - 2*Pz;

		//reflexion vector * observer vector = theta
		//it actually return cosTheta! but thats ok, we only need cosTheta!
		double angle = Rx * Vx + Ry * Vy + Rz * Vz;
		//double actualAngle = acos(angle)*180.0/CV_PI;
		cvSetReal1D(theta, i, angle);
	}

	return theta;
}

CvMat* CBRDFdata::GetCosLN(int currentSurface)
{
	CvMat* phi = cvCreateMat(1, m_numImages, CV_32F);
	//matrix phi contains angle for led1 in position 0 and angle for led16 in position 15(m_numImages-1)

	//TODO: check if the below code works!
	for (int i = 0; i < m_numImages; i++)
	{
		triangle surface = m_faces[currentSurface];
		
		//center of triangle
		double x = 0;
		double y = 0;
		double z = 0;
		
		for(int j = 0; j < 3; j++)
		{
			x += surface.m_point[j].m_x;
			y += surface.m_point[j].m_y;
			z += surface.m_point[j].m_z;
		}
		
		x /= 3.0; y /= 3.0; z /= 3.0;

		//light vector
		double Lx = m_led[i].m_x - x;
		double Ly = m_led[i].m_y - y;
		double Lz = m_led[i].m_z - z;
			
		double length = sqrt(Lx*Lx + Ly*Ly + Lz*Lz);
			
		Lx /= length;
		Ly /= length;
		Lz /= length;
			
		//light vector * normal vector = phi
		//it actually return cosPhi! but thats ok, we only need cosPhi!
		double angle = Lx * surface.m_normal.m_x + Ly * surface.m_normal.m_y + Lz * surface.m_normal.m_z;
		//double actualAngle = acos(angle)*180.0/CV_PI;
		cvSetReal1D(phi, i, angle);
	}

	return phi;
}

//calcs the product of surface normal and H
CvMat* CBRDFdata::GetCosNH(int currentSurface)
{
	CvMat* theta_dash = cvCreateMat(1, m_numImages, CV_32F);
	//matrix theta_dash contains angle for led1 in position 0 and angle for led16 in position 15(m_numImages-1)

	for (int i = 0; i < m_numImages; i++)
	{
		triangle surface = m_faces[currentSurface];
		
		//center of triangle
		double x = 0;
		double y = 0;
		double z = 0;
		
		for(int j = 0; j < 3; j++)
		{
			x += surface.m_point[j].m_x;
			y += surface.m_point[j].m_y;
			z += surface.m_point[j].m_z;
		}
		
		x /= 3.0; y /= 3.0; z /= 3.0;

		//|light vector + observer vector| = half vector
		double Hx = m_led[i].m_x - 2*x + cvGetReal1D(m_p, 0);
		double Hy = m_led[i].m_y - 2*y + cvGetReal1D(m_p, 1);
		double Hz = m_led[i].m_z - 2*z + cvGetReal1D(m_p, 2);
			
		double length = sqrt(Hx*Hx + Hy*Hy + Hz*Hz);
			
		Hx /= length;
		Hy /= length;
		Hz /= length;
			
		//half vector * normal vector = theta
		//it actually return cosTheta'! but thats ok, we only need cosTheta'!
		double angle = Hx * surface.m_normal.m_x + Hy * surface.m_normal.m_y + Hz * surface.m_normal.m_z;
		//double actualAngle = acos(angle)*180.0/CV_PI;
		cvSetReal1D(theta_dash, i, angle);
	}

	return theta_dash;
}

CvMat* CBRDFdata::GetIntensities(int x, int y, int colorChannel) //BGR
{
	CvMat* I = cvCreateMat(1, m_numImages, CV_32F);
	//matrix I contains image Intensities for iamge1 position 0 and intensity for image16 in position 15(m_numImages-1)
	//this function just gets the values of one color channel!
	int num = 0;
	for(vector<IplImage*>::iterator it = m_images.begin(); it != m_images.end(); it++)
	{
		CvScalar i = cvGet2D((*it), (*it)->height-1-y, x); //because ogl screen y-coordinate is inverted!
		double currIntensity = i.val[colorChannel]/255.0;
		cvSetReal1D(I, num++, currIntensity);
	}
	return I;
}

struct extraData
{
	double* angles;
	int modelInfo;
};

/* model to be fitted to measurements */
void BRDFFunc(double *p, double *x, int m, int n, void *data)
{
	register int i;

	extraData* incommingData = (extraData*)data;
	double* angles = incommingData->angles;
	int model = incommingData->modelInfo;

	for(int i=0; i<n; i++)
	{
		double currCosPhi = angles[i];
		if(model == 0) //PHONG!
		{
			double currCosTheta = angles[i+n*2];
			x[i] = p[0]*currCosPhi + ((p[2] + 2.0)/2.0*CV_PI)*p[1]*(pow(currCosTheta,p[2]));
		}
		else if(model == 1) //BLINN-PHONG!
		{		
			double currCosThetaDash = angles[i+n];
			x[i] = p[0]*currCosPhi + p[1]*(pow(currCosThetaDash,p[2]));
		}
	}	
}

CvMat* CBRDFdata::SolveEquation(CvMat* phi, CvMat* thetaDash, CvMat* theta, CvMat* I)
{
	CvMat* brdf = cvCreateMat(1, 3, CV_32F);
	//solve equation I = kd*cos(phi) + ks*cos^n(phi) with 16 sets of values
	//returns the resulting parameters kd, ks and n, in that order in brdf
	//phi: contains 16 values
	//I:   contains 16 values

	double p[3] = {0.5, 1.0, 1.0};
	//double p[3] = {1.1, 0.65, 0.2};
	double* x = new double[m_numImages];
	for(int i=0; i<m_numImages; i++)
	{
		x[i] = cvGetReal1D(I, i);
	}

	extraData* data = new extraData();

	data->angles = new double[phi->cols + thetaDash->cols + theta->cols];
	int j = 0;
	for(; j<m_numImages; j++)
	{
		data->angles[j] = cvGetReal1D(phi, j);
	}
	
	for(int a=0; j<m_numImages*2; j++, a++)
	{
		data->angles[j] = cvGetReal1D(thetaDash, a);
	}

	for(int a=0; j<m_numImages*3; j++, a++)
	{
		data->angles[j] = cvGetReal1D(theta, a);
	}
	
	data->modelInfo = m_model;

	int m = 3; //parameters
	int n = m_numImages; //measurements
	int itmax = 2000;
	double opts[LM_OPTS_SZ];
	double info[LM_INFO_SZ];
	/* optimization control parameters; passing to levmar NULL instead of opts reverts to defaults */
	opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
	opts[4]=LM_DIFF_DELTA; // relevant only if the finite difference Jacobian version is used

	int error = dlevmar_dif(BRDFFunc, p, x, m, n, itmax, opts, info, NULL, NULL, data);
	if(error == -1)
		cout << "Error in SolveEquation(..)" << endl;

//#ifdef _DEBUG
//
//	cout << "Levenberg-Marquardt returned in " << info[5] << "iter, reason " << info[6] << ", sumsq " << info[1] << "[" << info[0] << "g]" << endl;
//	cout << "Best fit parameters: "<< p[0] << ", " <<  p[1] << ", "  << p[2] << ", " << endl;
//
//#endif

	cvSetReal1D(brdf, 0, p[0]);
	cvSetReal1D(brdf, 1, p[1]);
	cvSetReal1D(brdf, 2, p[2]);

	delete[] data->angles;
	delete data;

	return brdf;
}

void CBRDFdata::CalcBRDFEquation(CvMat* pixelMap)
{
	int shizzle = 0;
	double min_kd = -100.0;
	double max_kd = 1000.0;
	double avg_kd = 0.0;
	double min_ks = -100.0;
	double max_ks = 1000.0;
	double avg_ks = 0.0;
	double min_n = -100.0;
	double max_n = 1000.0;
	double avg_n = 0.0;
	unsigned int count_kd = 0;
	unsigned int count_ks = 0;
	unsigned int count_n = 0;

	//for each pixel do:
	for(int x=0; x < m_width; x++)
		for(int y=0; y < m_height; y++)
		{
			int currentSurface = cvGetReal2D(pixelMap, y, x);

			shizzle++;

			if(currentSurface > 0) //pixel corresponds to a surface on the model
			{				
				CvMat* phi = GetCosLN(currentSurface);
				CvMat* thetaDash = GetCosNH(currentSurface);
				CvMat* theta = GetCosRV(currentSurface);

				for(int colorChannel=0; colorChannel<3; colorChannel++) //do the calculation once for each color-channel
				{
					//build vector I
					CvMat* I = GetIntensities(x, y, colorChannel); //BGR
				
					//solve equation with 16 sets of values
					CvMat* brdf = SolveEquation(phi, thetaDash, theta, I);
				
					//when complete write values to surface in model
					SaveValuesToSurface(currentSurface, brdf, colorChannel); //BGR

					if(cvGetReal1D(brdf, 0) > 0 && cvGetReal1D(brdf, 0) < 1000)
					{
						avg_kd += cvGetReal1D(brdf, 0);
						count_kd++;
					}
					
					if(cvGetReal1D(brdf, 1) > 0 && cvGetReal1D(brdf, 1) < 1000)
					{
						avg_ks += cvGetReal1D(brdf, 1);
						count_ks++;
					}
				
					if(cvGetReal1D(brdf, 2) > 0 && cvGetReal1D(brdf, 2) < 1000)
					{
						avg_n += cvGetReal1D(brdf, 2);
						count_n++;
					}
					
				}
			}

			//progress display
			double percent = (double)shizzle*100.0 / (double)(m_width*m_height);
			if ((int)shizzle % 100 == 0)
				cout << (int)percent << "% done\r";
		}

	cout << "100% done\n";

	//output statistics about brdf values:

	cout << /*"kd_min: " << min_kd << ", kd_max: " << max_kd << */", kd_avg: " << avg_kd/count_kd << endl;
	cout << /*"ks_min: " << min_ks << ", ks_max: " << max_ks << */", ks_avg: " << avg_ks/count_ks << endl;
	cout << /*"n_min: " << min_n << ", n_max: " << max_n  << */", n_avg: " << avg_n/count_n << endl;
}
