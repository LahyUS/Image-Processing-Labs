#include "GeometricTransformer.h"

PixelInterpolate::PixelInterpolate()
{
}

PixelInterpolate::~PixelInterpolate()
{
}

uchar BilinearInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels, int i)
{
	int x = (int)tx;
	int y = (int)ty;

	// get an uchar address of data array
	uchar* pA11 = pSrc + (y * srcWidthStep * nChannels + x * nChannels) + i;
	uchar* pA12 = pSrc + (y * srcWidthStep * nChannels + x * nChannels + 3) + i;
	uchar* pA21 = pSrc + ((y + 1) * srcWidthStep * nChannels + x * nChannels) + i;
	uchar* pA22 = pSrc + ((y + 1) * srcWidthStep * nChannels + x * nChannels + 3) + i;

	// get value at index
	uchar val1 = *pA11;
	uchar val2 = *pA12;
	uchar val3 = *pA21;
	uchar val4 = *pA22;

	// calculate value
	uchar U1 = ((tx - x) * val1 + ((x + 1) - tx) * val2) / (1);
	uchar U2 = ((tx - x) * val3 + ((x + 1) - tx) * val4) / (1);
	uchar U = ((ty - y) * U1 + ((ty + 1) - ty) * U2) / (1);

	return U;
}

BilinearInterpolate::BilinearInterpolate()
{
}

BilinearInterpolate::~BilinearInterpolate()
{
}

uchar NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels, int i)
{
	int x = (int)tx, y = (int)ty;
	uchar pDstRow;
	try
	{
		uchar* pSrcRow = pSrc + (y * srcWidthStep * nChannels + x * nChannels) + i;
		pDstRow = *pSrcRow;
	}
	catch (char* e)
	{
		return 255;
	}

	return pDstRow;
}

NearestNeighborInterpolate::NearestNeighborInterpolate()
{
}

NearestNeighborInterpolate::~NearestNeighborInterpolate()
{
}

void AffineTransform::Translate(float dx, float dy)
{
	Mat temp;
	temp.create(3, 3, CV_32FC1);
	int width = temp.cols, height = temp.rows;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (x == y)
				temp.at<float>(x, y) = 1;
			else
				temp.at<float>(x, y) = 0;
	//Gán các giá trị dx, dy tại (0, 2), (1, 2)
	temp.at<float>(0, 2) = dx;
	temp.at<float>(1, 2) = dy;
	//Nhân matrix gốc với ma trận tịnh tiến
	_matrixTransform = _matrixTransform * temp;
}

void AffineTransform::Rotate(float angle)
{
	//Tính sinA, cosA và chuyển góc angle về radian
	angle = angle * 3.14;
	float sinAlpha = sin(angle), cosAlpha = cos(angle);
	Mat temp;
	temp.create(3, 3, CV_32FC1);
	int width = temp.cols, height = temp.rows;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (x == y)
				temp.at<float>(x, y) = 1;
			else
				temp.at<float>(x, y) = 0;
	//Gán các giá trị cosA, -sinA, sinA, cosA  tại (0, 0), (0, 1), (1, 0), (1, 1)
	temp.at<float>(0, 0) = cosAlpha;
	temp.at<float>(0, 1) = -sinAlpha;
	temp.at<float>(1, 0) = sinAlpha;
	temp.at<float>(1, 1) = cosAlpha;
	//Nhân matrix gốc với ma trận tịnh tiến
	_matrixTransform = _matrixTransform * temp;
}

void AffineTransform::Scale(float sx, float sy)
{
	Mat temp;
	temp.create(3, 3, CV_32FC1);
	int width = temp.cols, height = temp.rows;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (x == y)
				temp.at<float>(x, y) = 1;
			else
				temp.at<float>(x, y) = 0;
	//Gán các giá trị sx, sy tại(0, 0), (1, 1)
	temp.at<float>(0, 0) = sx;
	temp.at<float>(1, 1) = sy;
	//Nhân matrix gốc với ma trận tịnh tiến
	_matrixTransform = _matrixTransform * temp;
}

void AffineTransform::TransformPoint(float& x, float& y)
{
	float P[3] = { x, y, 1 }, P1[3] = { 0, 0, 0 }; //Tạo 2 ma trận điểm ảnh nguồn và đích
	   //Nhân ma trận nguồn với ma trận chuyển đổi
	for (int i = 0; i < 3; i++)
	{
		float* data = (float*)_matrixTransform.ptr<float>(i);

		for (int j = 0; j < 3; j++)
		{
			float g = _matrixTransform.at<float>(i, j);
			//P1[i] = P[i] + P[j] * _matrixTransform.at<float>(i, j);
			P1[i] = P1[i] + P[j] * data[j];
		}
	}
	//Gán lại giá trị mới cho điểm ảnh
	x = P1[0];
	y = P1[1];
}

AffineTransform::AffineTransform()
{
	//Khởi tạo matrix 3x3 type 32F chanels = 1;
	_matrixTransform.create(3, 3, CV_32FC1);
	int width = _matrixTransform.cols, height = _matrixTransform.rows;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			if (x == y)
			{
				_matrixTransform.at<float>(x, y) = 1;
			}
			else
			{
				_matrixTransform.at<float>(x, y) = 0;

			}
		}
	}	
}

AffineTransform::~AffineTransform()
{
}

int GeometricTransformer::Transform(const Mat& beforeImage, Mat& afterImage, AffineTransform* transformer, PixelInterpolate* interpolator)
{
	cv::Mat process_image = afterImage.clone(); // copy matrix
	int nchanel = process_image.channels();
	int width = process_image.cols; // number of lines
	int height = process_image.rows;

	int beWidth = beforeImage.cols;
	int beHeight = beforeImage.rows;

	//Dung vong lap lap và lay con tro dau dong
	for (int y = 0; y < height; y++)
	{
		uchar* aftRow = (uchar*)afterImage.ptr<uchar>(y);
		//Xet tren tung pixel va lay 3 kenh mau cua anh
		for (int x = 0; x < width; x++, aftRow += nchanel)
		{
			for (int i = 0; i < nchanel; i++)
			{
				//calculate point in beforeImage
				float newX = x;
				float newY = y;
				transformer->TransformPoint(newX, newY);

				if (newX >= beWidth || newY >= beHeight || newX < 0 || newY < 0)
				{
					aftRow[i] = 255;
				}
				else
				{
					//get color
					aftRow[i] = interpolator->Interpolate(newX, newY, beforeImage.data, beWidth, nchanel, i);
				}
			}
		}

	}
	return 0;
}

int GeometricTransformer::RotateKeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	float w = srcImage.cols;
	float h = srcImage.rows;

	AffineTransform* af = new AffineTransform();
	dstImage = Mat(w, h, CV_8UC3);

	float dgcheo = sqrt(w * w + h * h);

	//calculate sin of given angle sum with center angle
	float sinA = abs(sin(angle * 3.14));
	float cosA = abs(cos(angle * 3.14));
	float sinB = h / dgcheo;
	float cosB = w / dgcheo;
	float sinAB = sinA * cosB + sinB * cosA;

	//give destImage a new size
	dstImage = Mat(dgcheo * sinAB, dgcheo * sinAB, CV_8UC3);

	float a = w / 2;
	float b = h / 2;

	float a1 = dstImage.cols / 2;
	float b1 = dstImage.rows / 2;

	af->Translate(-a1 + a, -b1 + b);
	af->Translate(a1, b1);
	af->Rotate(angle);
	af->Translate(-a1, -b1);

	
	Transform(srcImage, dstImage, af, new NearestNeighborInterpolate());
	return 1;
}

int GeometricTransformer::RotateUnkeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	int H = srcImage.rows;
	int W = srcImage.cols;
	AffineTransform* af = new AffineTransform();
	dstImage = Mat(H, W, CV_8UC3);

	float a = srcImage.cols / 2;
	float b = srcImage.rows / 2;
	af->Translate(a, b);
	af->Rotate(angle);
	af->Translate(-a, -b);
	Transform(srcImage, dstImage, af, new NearestNeighborInterpolate());
	return 1;
}

int GeometricTransformer::Scale(const Mat& srcImage, Mat& dstImage, float sx, float sy, PixelInterpolate* interpolator)
{
	int H = srcImage.rows;
	int W = srcImage.cols;
	AffineTransform* af = new AffineTransform();
	dstImage = Mat(H, W, CV_8UC3);

	af->Scale(1 / sx, 1 / sy);

	Transform(srcImage, dstImage, af, new NearestNeighborInterpolate());

	return 1;
}

int GeometricTransformer::Resize(const Mat& srcImage, Mat& dstImage, int newWidth, int newHeight, PixelInterpolate* interpolator)
{
	AffineTransform* af = new AffineTransform();
	dstImage = Mat(newHeight, newWidth, CV_8UC3);

	float a = (float)newWidth / srcImage.cols;
	float b = (float)newHeight / srcImage.rows;

	af->Scale(1 / a, 1 / b);
	Transform(srcImage, dstImage, af, new NearestNeighborInterpolate());

	return 1;
}

int GeometricTransformer::Flip(const Mat& srcImage, Mat& dstImage, bool direction, PixelInterpolate* interpolator)
{
	int H = srcImage.rows;
	int W = srcImage.cols;
	AffineTransform* af = new AffineTransform();
	dstImage = Mat(H, W, CV_8UC3);

	if (direction == 1)
	{
		af->Scale(1, -1);
		af->Translate(0, -H);
	}
	else
	{
		af->Scale(-1, 1);
		af->Translate(-W, 0);
	}

	Transform(srcImage, dstImage, af, new NearestNeighborInterpolate());
	return 1;
}

GeometricTransformer::GeometricTransformer()
{
}

GeometricTransformer::~GeometricTransformer()
{
}
