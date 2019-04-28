//============================================================================
// Name        : brdfdata.h
// Author      : Christian Thurow
// Description : defines the BRDF data structure
//============================================================================

#include "cv.h"

struct vertex
{
	double m_x;
	double m_y;
	double m_z;
};

struct brdfSurface
{
	double kd;
	double ks;
	double n;
};

struct triangle
{
	vertex m_point[3];
	vertex m_normal;
	int m_numFaces;
	brdfSurface brdf[3]; //BGR
};

class CBRDFdata
{
private:

	int m_numImages;
	vector<IplImage*> m_images;
	int m_height; //resolution of the input images
	int m_width; //resolution of the input images
	IplImage* m_dark;
	CvMat* m_principal_point;
	double m_focalLength;
	double m_pixelSizeX;
	CvMat* m_n;
	CvMat* m_o;
	CvMat* m_a;
	CvMat* m_p;
	CvMat* m_ledPositions;
	vertex* m_vertices;
	vertex* m_led;

public:
	int m_model; //0: Phong, 1: Blinn-Phong
	triangle* m_faces;

	CBRDFdata()
	{
		m_height = -1;
		m_width = -1;
		m_numImages = 16;
		m_images.clear();
		m_principal_point = cvCreateMat(1, 2, CV_32F);
		m_focalLength = 0.0;
		m_pixelSizeX = 0.0;
		m_n = cvCreateMat(1, 3, CV_32F); //betrag von n = 1!
		m_o = cvCreateMat(1, 3, CV_32F); //betrag von o = 1!
		m_a = cvCreateMat(1, 3, CV_32F); //betrag von a = 1!
		m_p = cvCreateMat(1, 3, CV_32F);
		//o steht senkrecht auf n
		//o steht senkrecht auf a
		//n steht senkrecht auf a
		m_ledPositions = cvCreateMat(16, 3, CV_32F);
		m_vertices = NULL;
		m_faces = NULL;
		m_led = NULL;
	};

	//~CBRDFdata();

	bool Cleanup();
	bool LoadImages();
	void NormaliseImages();
	void PrintImages();
	void PrintNormalisedImages();
	void SubtractAmbientLight();
	bool LoadDarkImage();
	void LoadCameraParameters(std::string filename);
	bool ReadInFile(std::string filename, std::vector<char>* buffer);
	void WriteValue(std::string parameter, std::string value);
	void LoadModel(std::string filename);
	CvMat* CalcPixel2SurfaceMapping();
	void CalcBRDFEquation(CvMat* pixelMap);
	CvMat* GetCosNH(int currentSurface);
	CvMat* GetCosRV(int currentSurface);
	CvMat* GetCosLN(int currentSurface);
	CvMat* GetIntensities(int x, int y, int colorChannel);
	CvMat* SolveEquation(CvMat* phi, CvMat* thetaDash, CvMat* theta, CvMat* I);
	void SaveValuesToSurface(int currentSurface, CvMat* brdf, int colorChannel);
	bool ReadInFileAsLines(std::string filename, std::vector<char*>* buffer);
	int GetNumVertices(vector<char*>* linesInFile);
	void WriteVertices(vector<char*>* linesInFile);
	int GetNumFaces(vector<char*>* linesInFile);
	void WriteFaces(vector<char*>* linesInFile);
	void CalcFaceNormals();
	void ScaleMesh();
	CvMat* GetCameraOrigin();
	CvMat* GetA();
	CvMat* GetO();
	CvMat* GetN();
	double GetCX();
	double GetCY();
	void InitLEDs();
};
