#pragma once
#include "Convolution.h"

class EdgeDetector
{
public:
	/*
	Hàm phát hiện biên cạnh của ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
	kWidth, kHeight: kích thước kernel
		1: Sobel
		2: Prewitt
		3: Laplace
	Hàm trả về
		0: nếu detect thành công
		1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int sobel(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight) {
		Convolution conv;
		int n = kWidth * kHeight;
		vector<float> kernel_x;
		kernel_x = { 1, 0, -1,2,0,-2,1,0,-1 };
		conv.SetKernel(kernel_x, 3, 3);
		for (int i = 0; i < 9; i++) {
			kernel_x[i] = kernel_x[i] * 1.0 / 4;
		}

		int width = sourceImage.cols, height = sourceImage.rows;
		Mat dstFx;
		dstFx.create(height, width, sourceImage.type());
		conv.DoConvolution(sourceImage, dstFx);

		vector<float> kernel_y;
		kernel_y = { -1,-2,-1,0,0,0,1,2,1 };
		conv.SetKernel(kernel_y, 3, 3);
		for (int i = 0; i < 9; i++) {
			kernel_y[i] = kernel_y[i] * 1.0 / 4;
		}
		Mat dstFy;
		dstFy.create(height, width, sourceImage.type());
		conv.DoConvolution(sourceImage, dstFy);

		int nChannels = sourceImage.channels();
		int size_row_raw = width * nChannels;
		int rem = size_row_raw % 4;
		int widthStep = (rem == 0) ? size_row_raw : size_row_raw + rem;

		uchar* pxData = dstFx.data;
		uchar* pyData = dstFy.data;
		uchar* pData = destinationImage.data;

		uchar* pxRow = dstFx.data;
		uchar* pyRow = dstFy.data;
		uchar* pRow = destinationImage.data;

		float sum;
		for (int i = 0; i < height; i++, pxData += widthStep, pyData += widthStep, pData += widthStep)
		{
			pRow = pData;
			pxRow = pxData;
			pyRow = pyData;
			for (int j = 0; j < width * nChannels; j++, pxRow += 1, pyRow += 1, pRow += 1) {
				sum = sqrt(pxRow[0] * pxRow[0] * 1.0f + pyRow[0] * pyRow[0] * 1.0f);
				pRow[0] = saturate_cast<uchar>(sum);
			}
		}
		return 1;
	}
	int prewitt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight) {
		Convolution conv;
		int n = kWidth * kHeight;
		vector<float> kernel_x;
		kernel_x = { -1, 0, 1,-1,0,1,-1,0,1 };
		conv.SetKernel(kernel_x, 3, 3);
		for (int i = 0; i < 9; i++) {
			kernel_x[i] = kernel_x[i] * 1.0 / 4;
		}

		int width = sourceImage.cols, height = sourceImage.rows;
		Mat dstFx;
		dstFx.create(height, width, sourceImage.type());
		conv.DoConvolution(sourceImage, dstFx);

		vector<float> kernel_y;
		kernel_y = { -1,-1,-1,0,0,0,1,1,1 };
		conv.SetKernel(kernel_y, 3, 3);
		for (int i = 0; i < 9; i++) {
			kernel_y[i] = kernel_y[i] * 1.0 / 4;
		}
		Mat dstFy;
		dstFy.create(height, width, sourceImage.type());
		conv.DoConvolution(sourceImage, dstFy);

		int nChannels = sourceImage.channels();
		int size_row_raw = width * nChannels;
		int rem = size_row_raw % 4;
		int widthStep = (rem == 0) ? size_row_raw : size_row_raw + rem;

		uchar* pxData = dstFx.data;
		uchar* pyData = dstFy.data;
		uchar* pData = destinationImage.data;

		uchar* pxRow = dstFx.data;
		uchar* pyRow = dstFy.data;
		uchar* pRow = destinationImage.data;

		float sum;
		for (int i = 0; i < height; i++, pxData += widthStep, pyData += widthStep, pData += widthStep)
		{
			pRow = pData;
			pxRow = pxData;
			pyRow = pyData;
			for (int j = 0; j < width * nChannels; j++, pxRow += 1, pyRow += 1, pRow += 1) {
				sum = sqrt(pxRow[0] * pxRow[0] * 1.0f + pyRow[0] * pyRow[0] * 1.0f);
				pRow[0] = saturate_cast<uchar>(sum);
			}
		}
		return 1;
	}

	int laplace(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight) {
		Convolution conv;
		int n = kWidth * kHeight;
		vector<float> kernel_x;
		kernel_x = { 0,-1,0,-1,4,-1,0,-1,0 };
		conv.SetKernel(kernel_x, 3, 3);
		for (int i = 0; i < 9; i++) {
			kernel_x[i] = kernel_x[i] * 1.0 / 4;
		}

		int width = sourceImage.cols, height = sourceImage.rows;
		Mat dstFx;
		dstFx.create(height, width, sourceImage.type());
		conv.DoConvolution(sourceImage, dstFx);

		vector<float> kernel_y;
		kernel_y = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
		conv.SetKernel(kernel_y, 3, 3);
		for (int i = 0; i < 9; i++) {
			kernel_y[i] = kernel_y[i] * 1.0 / 4;
		}
		Mat dstFy;
		dstFy.create(height, width, sourceImage.type());
		conv.DoConvolution(sourceImage, dstFy);

		int nChannels = sourceImage.channels();
		int size_row_raw = width * nChannels;
		int rem = size_row_raw % 4;
		int widthStep = (rem == 0) ? size_row_raw : size_row_raw + rem;

		uchar* pxData = dstFx.data;
		uchar* pyData = dstFy.data;
		uchar* pData = destinationImage.data;

		uchar* pxRow = dstFx.data;
		uchar* pyRow = dstFy.data;
		uchar* pRow = destinationImage.data;

		float sum;
		for (int i = 0; i < height; i++, pxData += widthStep, pyData += widthStep, pData += widthStep)
		{
			pRow = pData;
			pxRow = pxData;
			pyRow = pyData;
			for (int j = 0; j < width * nChannels; j++, pxRow += 1, pyRow += 1, pRow += 1) {
				sum = sqrt(pxRow[0] * pxRow[0] * 1.0f + pyRow[0] * pyRow[0] * 1.0f);
				pRow[0] = saturate_cast<uchar>(sum);
			}
		}
		return 1;
	}
	int DetectEdge(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method) {

		destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());

		switch (method) {
		case 0: {
			sobel(sourceImage, destinationImage, kWidth, kHeight);
			break;
		}
		case 1: {
			prewitt(sourceImage, destinationImage, kWidth, kHeight);
			break;
		}
		case 2: {
			laplace(sourceImage, destinationImage, kWidth, kHeight);
			break;
		}
		}
		return 1;
	}

	EdgeDetector() {};
	~EdgeDetector() {};
};

