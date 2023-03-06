#pragma once
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"


using namespace cv;
using namespace std;

class Convolution
{
	vector<float> _kernel;// lưu giá trị của ma trận kernel dùng để tích chập
	int _kernelWidth;//chiều rộng kernel
	int _kernelHeight;//chiều dài kernel
public:
	//trả về kernel 
	vector<float> GetKernel() { return _kernel; };
	//set kernel, chiều rộng, chiều dài kernel phải là số lẻ
	void SetKernel(vector<float> kernel, int kWidth, int kHeight) {
		_kernel = kernel;
		_kernelWidth = kWidth;
		_kernelHeight = kHeight;
	};

	/*
	Hàm tính convolution của 1 ảnh xám với kernel được xác định trước
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	1: nếu tính thành công
	0: nếu tính thất bại (không đọc được ảnh input,...)
	*/
	int DoConvolution(const Mat& sourceImage, Mat& destinationImage)
	{
		if (sourceImage.empty())
			return 0;

		int width = sourceImage.cols, height = sourceImage.rows;
		int nChannels = sourceImage.channels();
		int widthStep = sourceImage.step[0];

		destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());

		uchar* pData = (uchar*)destinationImage.data;
		cv::Mat temp_data = sourceImage.clone();
		uchar* psData = (uchar*)temp_data.data;

		//Tạo bảng offsets
		int kHalfWidth = _kernelWidth >> 1;
		int kHalfHeight = _kernelHeight >> 1;
		vector<int> offsets;

		int n;
		for (int y = -kHalfHeight; y <= kHalfHeight; y++)
			for (int x = -kHalfWidth; x <= kHalfWidth; x++)
				offsets.push_back(y * widthStep * nChannels + x * nChannels);
		n = offsets.size();

		double sum;

		// source image is a grayscale image
		if (nChannels == 1)
		{
			for (int i = 0; i < height; i++, psData += widthStep, pData += widthStep)
			{
				uchar* pRow = pData;
				uchar* psRow = psData;
				for (int j = 0; j < width; j++, psRow += nChannels, pRow += nChannels)
				{
					sum = 0;
					//Tính tích chập
					for (int k = 0; k < n; k++)
					{
						int index = offsets[k];

						if (i * width + j + index > 0 && i * widthStep + j + index < width * height * nChannels)
						{
							uchar* p_temp = psRow;
							p_temp += index;
							if (i * width + j + index > 0)
							{
								uchar val = *(p_temp);
								sum += val * _kernel[n - 1 - k];
							}
						}
					}
					pRow[0] = saturate_cast<uchar>(sum);
				}
			}
		}

		// source image is a color image
		else
		{
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
						int index = offsets[k];

						if (i * width + j + index > 0 && i * widthStep + j + index < width * height * nChannels)
						{
							//blue
							uchar* p_temp = psRow;
							p_temp += index;
							uchar val0 = *(p_temp);
							sum0 += val0 * _kernel[n - 1 - k];

						}
						if (i * width + j + index + 1 > 0 && i * widthStep + j + index + 1 < width * height * nChannels)
						{
							//green
							uchar* p_temp1 = psRow + 1;
							p_temp1 += index;
							uchar val1 = *(p_temp1);
							sum1 += val1 * _kernel[n - 1 - k];

						}
						if (i * width + j + index + 2 > 0 && i * widthStep + j + index + 2 < width * height * nChannels)
						{
							//red
							uchar* p_temp2 = psRow + 2;
							p_temp2 += index;
							uchar val2 = *(p_temp2);
							sum2 += val2 * _kernel[n - 1 - k];
						}
					}
					pRow[0] = saturate_cast<uchar>(sum0);
					pRow[1] = saturate_cast<uchar>(sum1);
					pRow[2] = saturate_cast<uchar>(sum2);
				}
			}
		}

		return 1;
	};

	Convolution() {};
	~Convolution() {};
};

