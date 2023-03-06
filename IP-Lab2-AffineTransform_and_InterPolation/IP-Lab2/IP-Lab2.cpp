// IP-Lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "../../../../../../opencv/build/include/opencv2/highgui.hpp"
#include "GeometricTransformer.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	Mat srcImg, dstImg;
	GeometricTransformer GTF;
	PixelInterpolate* interpolator;
	string Command(argv[1]), Interpolate(argv[2]), fileImg(argv[3]);
	int result = 0;
	srcImg = imread(fileImg, -1);

	if (Command == "--zoom")
	{
		float sx = atof(argv[4]);
		float sy = atof(argv[5]);
		if (Interpolate == "--bl")
		{
			interpolator = new BilinearInterpolate();
			result = GTF.Scale(srcImg, dstImg, sx, sy, interpolator);
		}
		else
		{
			interpolator = new NearestNeighborInterpolate();
			result = GTF.Scale(srcImg, dstImg, sx, sy, interpolator);
		}
	}
	else if (Command == "--resize")
	{
		float newWidth = atof(argv[4]);
		float newHeight = atof(argv[5]);
		if (Interpolate == "--bl")
		{
			interpolator = new BilinearInterpolate();
			result = GTF.Resize(srcImg, dstImg, newWidth, newHeight, interpolator);
		}
		else
		{
			interpolator = new NearestNeighborInterpolate();
			result = GTF.Resize(srcImg, dstImg, newWidth, newHeight, interpolator);
		}
	}
	else if (Command == "--rotate")
	{
		float angle = atof(argv[4]);
		if (Interpolate == "--bl")
		{
			interpolator = new BilinearInterpolate();
			result = GTF.RotateKeepImage(srcImg, dstImg, angle, interpolator);
		}
		else
		{
			interpolator = new NearestNeighborInterpolate();
			result = GTF.RotateKeepImage(srcImg, dstImg, angle, interpolator);
		}
	}
	else if (Command == "--rotateN")
	{
		float angle = atof(argv[4]);
		if (Interpolate == "--bl")
		{
			interpolator = new BilinearInterpolate();
			result = GTF.RotateUnkeepImage(srcImg, dstImg, angle, interpolator);
		}
		else
		{
			interpolator = new NearestNeighborInterpolate();
			result = GTF.RotateUnkeepImage(srcImg, dstImg, angle, interpolator);
		}
	}
	else if (Command == "--flip")
	{
		bool direction = atof(argv[4]);
		if (Interpolate == "--bl")
		{
			interpolator = new BilinearInterpolate();
			result = GTF.Flip(srcImg, dstImg, direction, interpolator);
		}
		else
		{
			interpolator = new NearestNeighborInterpolate();
			result = GTF.Flip(srcImg, dstImg, direction, interpolator);
		}
	}
	if (result)
	{
		imshow("Source Image", srcImg);
		imshow("Image after change", dstImg);
	}
	else
		printf("Could not open or find the image");

	waitKey(0);
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
