#include "ColorTransformer.h"

using namespace std;

int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b)
{
	//Lay chieu dai va chieu rong cua anh
	int width = sourceImage.cols, height = sourceImage.rows;

	//Lay channel cua anh
	int SrcChannels = sourceImage.channels();

	//Khoi tao anh gray
	destinationImage = Mat(height, width, CV_8UC3);
	int DstChannels = destinationImage.channels();

	//Dung vong lap lap và lay con tro dau dong
	for (int y = 0; y < height; y++)
	{
		uchar* pSrcRow = (uchar*)sourceImage.ptr<uchar>(y);
		uchar* pDstRow = (uchar*)destinationImage.ptr<uchar>(y);
		//Xet tren tung pixel va lay 3 kenh mau cua anh
		for (int x = 0; x < width; x++, pSrcRow += SrcChannels, pDstRow += DstChannels)
		{
			uchar B = pSrcRow[0] + b;
			uchar G = pSrcRow[1] + b;
			uchar R = pSrcRow[2] + b;
			if (B < pSrcRow[0]) {
				B = 255;
			}
			if (G < pSrcRow[1]) {
				G = 255;
			}
			if (R < pSrcRow[2]) {
				R = 255;
			}
			//uchar gray_val = (uchar)(0.3 * B + 0.59 * G + 0.11 * R + b);
			pDstRow[0] = B;
			pDstRow[1] = G;
			pDstRow[2] = R;
		}

	}
	return 1;
}

int ColorTransformer::ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c)
{
	int nchanel = sourceImage.channels();
	int width = sourceImage.cols * nchanel;
	int height = sourceImage.rows;

	destinationImage = sourceImage.clone();

	for (int y = 0; y < height; y++)
	{
		uchar* pDstRow = destinationImage.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			if (pDstRow[x] * c >= 255)
				pDstRow[x] = 255;
			else if (pDstRow[x] * c < 0)
				pDstRow[x] = 0;
			else
				pDstRow[x] = pDstRow[x] * c;
		}
	}
	return 1;
}

int ColorTransformer::CalcHistogram(const Mat& sourceImage, Mat& histMatrix)
{
	cv::Mat process_image = sourceImage.clone(); // copy matrix
	int nchanel = sourceImage.channels();
	int nl = sourceImage.rows; // number of lines
	int ncol = sourceImage.cols * nchanel;

	switch (nchanel) // initialize histogram matrix
	{
	case 1: histMatrix = Mat::zeros(1, 256, CV_16UC1); break;
	case 3: histMatrix = Mat::zeros(3, 256, CV_16UC1); break;
	default: return 0;
	}

	for (int j = 0; j < nl; j++)
	{
		// get pointer to line j
		uchar* data = process_image.ptr<uchar>(j);
		int step = 0;
		for (int i = 0; i < ncol; i = i + 1)
		{
			if (step > nchanel - 1) step = 0;
			uint16_t* his_data_i = histMatrix.ptr<uint16_t>(step);
			int index = data[i];
			his_data_i[index] += 1;
			step++;
		} // end of line
	}
	return 1;
}

int ColorTransformer::HistogramEqualization(const Mat& sourceImage, Mat& destinationImage)
{
	Mat histImg;

	CalcHistogram(sourceImage, histImg);

	int height = histImg.rows;
	int width = histImg.cols;
	int srcChannels = histImg.channels();

	int listdata[256 * 3];
	int sizeData = 0;
	int listdataTotal[] = {0, 0, 0};
	int listdataTotalSize = height;

    //save histogram to list
	for (int y = 0; y < height; y++)
	{
		uint16_t* data = (uint16_t*)histImg.ptr<uint16_t>(y);
		for (int x = 0; x < width; x++)
		{
			uint16_t v = data[x];

			listdata[y * width + x] = v;
			listdataTotal[y] += v;
			sizeData++;
		}
	}

	float listNewData[256 * 3];
	//chuan hoa ve [0, 1]
	for (int i = 0; i < sizeData / 3; i++)
	{
		listNewData[i] = 1.0 * listdata[i] / listdataTotal[0];
	}
	for (int i = sizeData / 3; i < sizeData / 3 * 2; i++)
	{
		listNewData[i] = 1.0 * listdata[i] / listdataTotal[1];
	}
	for (int i = sizeData / 3 * 2; i < sizeData; i++)
	{
		listNewData[i] = 1.0 * listdata[i] / listdataTotal[2];
	}

	//cong don
	for (int i = 1; i < sizeData; i++)
	{
		if (i % 256 == 0)
		{
			i++;
		}

		listNewData[i] += listNewData[i - 1];
	}

	//Nhan kenh mau
	for (int i = 0; i < sizeData; i++)
	{
		listNewData[i] *= 255;
	}

	int imgHeight = sourceImage.rows;
	int imgWidth = sourceImage.cols;
	int channel = sourceImage.channels();

	destinationImage = Mat(imgHeight, imgWidth, CV_8UC3, Scalar(0, 0, 0));

	//Dung vong lap lap và lay con tro dau dong
	for (int y = 0; y < imgHeight; y++)
	{
		uchar* pRGBRow = (uchar*)sourceImage.ptr<uchar>(y);
		uchar* pGrayRow = (uchar*)destinationImage.ptr<uchar>(y);
		//Xet tren tung pixel va lay 3 kenh mau cua anh
		for (int x = 0; x < imgWidth; x++, pRGBRow += channel, pGrayRow += channel)
		{
			uchar B = pRGBRow[0];
			uchar G = pRGBRow[1];
			uchar R = pRGBRow[2];

			uchar nB = listNewData[B];
			uchar nG = listNewData[256 + G];
			uchar nR = listNewData[256 * 2 + R];

			pGrayRow[0] = nB;
			pGrayRow[1] = nG;
			pGrayRow[2] = nR;
		}

	}

	return 1;
}

int ColorTransformer::DrawHistogram(const Mat& histMatrix, Mat& histImage)
{
    //with, height of hist matrix
	int width = histMatrix.cols;
	int height = histMatrix.rows;
	int srcChannels = histMatrix.channels();

	//with, height of hist image, set width % 255 == 0
	int histWidth = 1275;
	int histHeight = 700;

	histImage = Mat(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0));

	int max = 0;
	
	//array to save histogram to make it easier to poccess
	int BArr[256], GArr[256], RArr[256];
	int BTotal = 0, GTotal = 0, RTotal = 0;

	//save histogram to array
	for (int y = 0; y < height; y++)
	{
		uint16_t* data = (uint16_t*)histMatrix.ptr<uint16_t>(y);
		for (int x = 0; x < width; x++)
		{
			uint16_t v = data[x];
			if (v > max)
			{
				max = v;
			}
			if (y == 0)
			{
				BTotal += v;
				BArr[x] = v;
			}

			if (y == 1)
			{
				GTotal += v;
				GArr[x] = v;
			}

			if (y == 2)
			{
				RTotal += v;
				RArr[x] = v;
			}
		}
	}

	//scale height of histogram image to fit the actual size
	float scale = histHeight * 1.0 / (max + 30);

	//with of each value in histogram image
	int eleWidth = histWidth / 255;

	//draw histogram image
	for (int i = 0; i < 256 - 1; i++)
	{
		line(
			histImage,
			Point(i * eleWidth, -BArr[i] * scale + histHeight),
			Point((i + 1) * eleWidth, -BArr[i + 1] * scale + histHeight),
			Scalar(255, 0, 0),
			2, 8, 0);

		line(
			histImage,
			Point(i * eleWidth, -GArr[i] * scale + histHeight),
			Point((i + 1) * eleWidth, -GArr[i + 1] * scale + histHeight),
			Scalar(0, 255, 0),
			2, 8, 0);

		line(
			histImage,
			Point(i * eleWidth, -RArr[i] * scale + histHeight),
			Point((i + 1) * eleWidth, -RArr[i + 1] * scale + histHeight),
			Scalar(0, 0, 255),
			2, 8, 0);
	}
	return 1;
}

float ColorTransformer::CompareImage(const Mat& image1, Mat& image2)
{
	Mat hsv_image1, hsv_image2;

	CalcHistogram(image1, hsv_image1);
	CalcHistogram(image2, hsv_image2);

	int height = hsv_image1.rows;
	int width = hsv_image1.cols;
	int srcChannels = hsv_image1.channels();

	float diff = 0;

	//array to save histogram to make it easier to poccess
	int listdata1[256 * 3];
	int sizeData1 = 0;
	int data1Total = 0;

	int listdata2[256 * 3];
	int sizeData2 = 0;
	int data2Total = 0;

	//convert histogram to array
	for (int y = 0; y < height; y++)
	{
		uint16_t* data1 = (uint16_t*)hsv_image1.ptr<uint16_t>(y);
		uint16_t* data2 = (uint16_t*)hsv_image2.ptr<uint16_t>(y);
		for (int x = 0; x < width; x++)
		{
			uint16_t v1 = data1[x];
			uint16_t v2 = data2[x];

			listdata1[y * width + x] = v1;
			data1Total += v1;
			sizeData1++;

			listdata2[y * width + x] = v2;
			data2Total += v2;
			sizeData2++;
		}
	}

	//calculate diff
	for (int i = 0; i < sizeData1; i++)
	{
		diff += abs(1.0 * listdata1[i] / data1Total - 1.0 * listdata2[i] / data2Total);
	}

	return diff;
}

ColorTransformer::ColorTransformer()
{
}

ColorTransformer::~ColorTransformer()
{
}
