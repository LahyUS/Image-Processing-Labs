// IP-Lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ColorTransformer.h"
#include "Converter.h"
#include "../../../../../../opencv/build/include/opencv2/highgui.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "Invalid command\n";
        return -1;
    }

    Converter converter;
    ColorTransformer colorTrans;

    //input path
    string inputPath = argv[2];


    
    //command
    string command = argv[1];

    if (command == "--rgb2gray")
    {
        Mat srcImg = imread(inputPath, IMREAD_COLOR);

        if (srcImg.empty())
        {
            cout << "Can't open or read image\n";
        }

        Mat destImg;

        if (converter.Convert(srcImg, destImg, 0) == 0)
        {
            imshow("Image RGB", srcImg);
            imshow("Image GRAY", destImg);
            waitKey();
        }
        else 
        {
            cout << "Cannot convert file\n";
        }
    }
    else if (command == "--gray2rgb")
    {
        Mat srcImg = imread(inputPath, IMREAD_GRAYSCALE);

        if (srcImg.empty())
        {
            cout << "Can't open or read image\n";
        }

        Mat destImg;

        if (converter.Convert(srcImg, destImg, 1) == 0)
        {
            imshow("Image DEST", srcImg);
            imshow("Image RGB", destImg);
            waitKey();
        }
        else
        {
            cout << "Cannot convert file\n";
        }
    }
    else if (command == "--rgb2hsv")
    {
        Mat srcImg = imread(inputPath, IMREAD_COLOR);

        if (srcImg.empty())
        {
            cout << "Can't open or read image\n";
        }

        Mat destImg;

        if (converter.Convert(srcImg, destImg, 2) == 0)
        {
            imshow("Image RGB", srcImg);
            imshow("Image HSV", destImg);
            waitKey();
        }
        else
        {
            cout << "Cannot convert image\n";
        }
    }
    else if (command == "--hsv2rgb")
    {
        Mat srcImg = imread(inputPath, IMREAD_COLOR);

        if (srcImg.empty())
        {
            cout << "Can't open or read image\n";
        }

        Mat destImg;

        if (converter.Convert(srcImg, destImg, 3) == 0)
        {
            imshow("Image HSV", srcImg);
            imshow("Image RGB", destImg);
            waitKey();
        }
        else
        {
            cout << "Cannot convert image\n";
        }
    }
    else if (command == "--bright")
    {
        //get argument
        int b;
        try
        {
            b = atoi(argv[3]);
        }
        catch(Exception e)
        {
            cout << "Invalid brigthness\n";
            return -1;
        }

        Mat srcImg = imread(inputPath, IMREAD_COLOR);


        if (srcImg.empty())
        {
            cout << "Can't open or read image\n";
        }

        Mat destImg;

        if (colorTrans.ChangeBrighness(srcImg, destImg, (short)b) == 1)
        {
            imshow("Source image", srcImg);
            imshow("Image after change", destImg);
            waitKey();
        }
        else
        {
            cout << "Cannot change brightness\n";
        }
    }
    else if (command == "--contrast")
    {
        //get argument
        float c;
        try
        {
            c = atof(argv[3]);
        }
        catch(Exception e)
        {
            cout << "Invalid contrast\n";
            return -1;
        }

        Mat srcImg = imread(inputPath, IMREAD_COLOR);


        if (srcImg.empty())
        {
            cout << "Can't open or read image\n";
        }

        Mat destImg;

        if (colorTrans.ChangeContrast(srcImg, destImg, c) == 1)
        {
            imshow("Source image", srcImg);
            imshow("Image after change", destImg);
            waitKey();
        }
        else
        {
            cout << "Cannot change brightness\n";
        }

    }
    else if (command == "--hist")
    {
        Mat srcImg = imread(inputPath, IMREAD_COLOR);

        if (srcImg.empty())
        {
            cout << "Can't open or read image\n";
        }

        Mat destImg;

        if (colorTrans.CalcHistogram(srcImg, destImg) == 1)
        {
            imshow("Source image", srcImg);
            imshow("Histogram of image", destImg);
            waitKey();
        }
        else
        {
            cout << "Cannot calculate histogram\n";
        }
    }
    else if (command == "--equalhist")
    {
        Mat srcImg = imread(inputPath, IMREAD_COLOR);

        if (srcImg.empty())
        {
            cout << "Can't open or read image\n";
        }

        Mat destImg;

        if (colorTrans.HistogramEqualization(srcImg, destImg) == 1)
        {
            imshow("Source image", srcImg);
            imshow("Histogram of image", destImg);
            waitKey();
        }
        else
        {
            cout << "Cannot draw new image\n";
        }
    }
    else if (command == "--drawhist")
    {
        Mat srcImg = imread(inputPath, IMREAD_COLOR);

        if (srcImg.empty())
        {
            cout << "Can't open or read image\n";
        }

        Mat hist;

        Mat destImg;

        if (colorTrans.CalcHistogram(srcImg, hist) == 0)
        {
            cout << "Cannot calculate histogram\n";
            return -1;
        }

        if (colorTrans.DrawHistogram(hist, destImg) == 1)
        {
            imshow("Source image", srcImg);
            imshow("Image after change", destImg);
            waitKey();
        }
        else
        {
            cout << "Can't draw histogram\n";
        }

    }
    else if (command == "--compare")
    {
        string secondPath = argv[3];

        Mat srcImg = imread(inputPath, IMREAD_COLOR);
        Mat destImg = imread(secondPath, IMREAD_COLOR);


        if (srcImg.empty() || destImg.empty())
        {
            cout << "Can't open or read image\n";
        }
        
        cout << "Do khac biet: " << colorTrans.CompareImage(srcImg, destImg) << endl;
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
