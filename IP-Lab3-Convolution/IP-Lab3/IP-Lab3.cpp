// IP-Lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Blur.h"
#include "EdgeDetector.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Invalid\n";
		return -1;
	}
	Blur blur;
	EdgeDetector ed;

	Mat srcImage, dstImage;
	string cmd(argv[1]); 

	int result = 0;

	if (argc < 3)
	{
		cout << "Invalid command\n";
		return -1;
	}
	srcImage = imread(argv[2], IMREAD_COLOR);

	if (cmd == "--mean")
	{
		int kW, kH;

		if (argc < 5)
		{
			cout << "Input kernel size\n";
			return -1;
		}
		
		kW = atoi(argv[3]);
		kH = atoi(argv[4]);

		result = blur.BlurImage(srcImage, dstImage, kW, kH, 0);
	}
	else if (cmd == "--median")
	{
		int kW, kH;

		if (argc < 5)
		{
			cout << "Input kernel size\n";
			return -1;
		}

		kW = atoi(argv[3]);
		kH = atoi(argv[4]);

		result = blur.BlurImage(srcImage, dstImage, kW, kH, 1);
	}
	else if (cmd == "--gauss")
	{
		int kW, kH;

		if (argc < 5)
		{
			cout << "Input kernel size\n";
			return -1;
		}

		kW = atoi(argv[3]);
		kH = atoi(argv[4]);

		result = blur.BlurImage(srcImage, dstImage, kW, kH, 2);
	}
	else if (cmd == "--sobel")
	{
		result = ed.DetectEdge(srcImage, dstImage, 3, 3, 0); // kWidth = kHeight = 3
	}
	else if (cmd == "--prewitt")
	{
		result = ed.DetectEdge(srcImage, dstImage, 3, 3, 1); // kWidth = kHeight = 3
	}
	else if (cmd == "--laplace")
	{
		result = ed.DetectEdge(srcImage, dstImage, 3, 3, 2); // kWidth = kHeight = 3
	}

	if (result == 1)
	{
		imshow("Source Image", srcImage);
		imshow("Destination Image", dstImage);

		waitKey(0);
	}
	else
	{
		cout << "Cannot open image\n";
	}

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
