#pragma once
#include "Convolution.h"
#include  <math.h>

class Blur
{
public:
	/*
	Hàm làm trơn ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
	kWidth, kHeight: kích thước kernel
	0: Trung bình
	1: Trung vị
	2: Gaussian
	Hàm trả về
	1: nếu detect thành công
	0: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
	{
		Convolution conv;
		vector<float> kernel;
		vector<int> index;
		uchar* I;

		destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());

		switch (method)
		{
		case 0:
		{
			// Proccess with mean filter
			return MeanBlur(sourceImage, destinationImage, kWidth, kHeight);
			break;
		}
		case 1:
		{
			// Proccess with median filter
			return MedianBlur(sourceImage, destinationImage, kWidth, kHeight);
			break;
		}
		case 2:
		{
			return GaussBlur(sourceImage, destinationImage, kWidth, kHeight);
			break;
		}
		}
	};
	Blur() {};
	~Blur() {};

private:
	void Sort(uchar a[], int n)
	{
		for (int i = 0; i < n / 2 + 1; i++)
		{
			for (int j = i + 1; j < n; j++)
			{
				if (a[i] > a[j])
				{
					a[i] ^= a[j];
					a[j] ^= a[i];
					a[i] ^= a[j];
				}
			}
		}
	}

	int MeanBlur(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight)
	{
		if (sourceImage.empty() == true)
			return 0;

		Convolution conv;
		vector<float> kernel;
		vector<int> index;
		int n;
		n = kWidth * kHeight;
		for (int i = 0; i < kWidth * kHeight; i++)
		{
			kernel.push_back(1.0f / n);
		}
		conv.SetKernel(kernel, kWidth, kHeight);
		return conv.DoConvolution(sourceImage, destinationImage);
	}

	int MedianBlur(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight)
	{
		int width = sourceImage.cols, height = sourceImage.rows;
		int nChannels = sourceImage.channels();
		int widthStep = sourceImage.step[0];

		destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());

		uchar* pData = (uchar*)destinationImage.data;
		cv::Mat temp_data = sourceImage.clone();
		uchar* psData = (uchar*)temp_data.data;

		//Tạo bảng offsets
		int kHalfWidth = kWidth >> 1;
		int kHalfHeight = kHeight >> 1;
		vector<int> index;

		int n;
		for (int y = -kHalfHeight; y <= kHalfHeight; y++)
			for (int x = -kHalfWidth; x <= kHalfWidth; x++)
				index.push_back(y * widthStep + x * nChannels);
		n = index.size();

		if (nChannels == 1)
		{
			uchar* I = new uchar[n];

			for (int i = 0; i < height; i++, psData += widthStep, pData += widthStep)
			{
				uchar* pRow = pData;
				uchar* psRow = psData;
				for (int j = 0; j < width; j += 1, psRow += 1, pRow += 1)
				{
					// get value of neighbor pixel
					for (int k = 0; k < n; k++)
					{
						int index_t = index[k];

						if (i * widthStep + j + index_t < 0 || i * widthStep + j + index_t > width * height * nChannels)
						{
							I[k] = 0;
						}
						else
						{
							uchar* p_temp = psRow;
							p_temp += index_t;
							uchar val = *(p_temp);
							I[k] = val;
						}
					}

					Sort(I, n);
					// assign the median to pixel value
					uchar grayscale = I[n / 2];
					pRow[0] = grayscale;
				}
			}
		}

		else
		{
			uchar* I0 = new uchar[n];
			uchar* I1 = new uchar[n];
			uchar* I2 = new uchar[n];

			double sum0, sum1, sum2;
			for (int i = 0; i < height; i++, psData += widthStep, pData += widthStep)
			{
				uchar* pRow = pData;
				uchar* psRow = psData;
				for (int j = 0; j < width; j++, psRow += nChannels, pRow += nChannels)
				{
					sum0 = 0;
					sum1 = 0;
					sum2 = 0;
					//Tính tích chập
					for (int k = 0; k < n; k++)
					{
						int index_t = index[k];

						//blue
						if (i * widthStep + j + index_t < 0 || i * widthStep + j + index_t > width * height * nChannels)
						{
							I0[k] = 0;
						}
						else
						{
							uchar* p_temp = psRow;
							p_temp += index_t;
							uchar val0 = *(p_temp);
							I0[k] = val0;
						}


						//green
						if (i * widthStep + j + index_t + 1< 0 || i * widthStep + j + index_t  + 1> width * height * nChannels)
						{
							I1[k] = 0;
						}
						else
						{
							uchar* p_temp1 = psRow + 1;
							p_temp1 += index_t;
							uchar val1 = *(p_temp1);
							I1[k] = val1;
						}
						//red
						if (i * widthStep + j + index_t + 2 < 0 || i * widthStep + j + index_t + 2> width * height * nChannels)
						{
							I2[k] = 0;
						}
						else
						{
							uchar* p_temp2 = psRow + 2;
							p_temp2 += index_t;
							uchar val2 = *(p_temp2);
							I2[k] = val2;
						}
					}
					Sort(I0, n);
					Sort(I1, n);
					Sort(I2, n);
					// assign the median to pixel value
					uchar blue = I0[n / 2];
					uchar green = I1[n / 2];
					uchar red = I2[n / 2];
					pRow[0] = blue;
					pRow[1] = green;
					pRow[2] = red;
				}
			}
		}

		return 1;
	}

	int GaussBlur(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight)
	{
		if (sourceImage.empty() == true)
			return 0;
		Convolution conv;
		vector<float> kernel;

		int hafl_W, hafl_H;
		float sigma = 0.5f;
		float s = 2 * 3.14 * sigma * sigma; // s = 2 * PI * sigma ^2

		//create kernel for Gaussian
		hafl_W = kWidth / 2;
		hafl_H = kHeight / 2;
		//Tính kernel 
		for (int y = -hafl_H; y <= hafl_H; y++)
		{
			for (int x = -hafl_W; x <= hafl_W; x++)
			{
				float h = expf(-(y * y + x * x) / (2 * sigma * sigma)) / s;
				kernel.push_back(h);
			}
		}

		conv.SetKernel(kernel, kWidth, kHeight);
		return conv.DoConvolution(sourceImage, destinationImage);
	}
};

