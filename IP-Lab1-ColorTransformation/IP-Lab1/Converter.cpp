#include "Converter.h"

int Converter::RGB2GrayScale(const Mat& sourceImage, cv::Mat& destinationImage)
{
	//Lay chieu dai va chieu rong cua anh
	int width = sourceImage.cols, height = sourceImage.rows;
	//Lay channel cua anh
	int rgbChannels = sourceImage.channels();
	int grayChannels = destinationImage.channels();
	//Khoi tao anh gray
	destinationImage = cv::Mat(height, width, CV_8UC1);
	//Dung vong lap lap và lay con tro dau dong
	for (int y = 0; y < height; y++)
	{
		uchar* pRGBRow = (uchar*)sourceImage.ptr<uchar>(y);
		uchar* pGrayRow = destinationImage.ptr<uchar>(y);
		//Xet tren tung pixel va lay 3 kenh mau cua anh
		for (int x = 0; x < width; x++, pRGBRow += rgbChannels, pGrayRow += grayChannels)
		{
			uchar B = pRGBRow[0];
			uchar G = pRGBRow[1];
			uchar R = pRGBRow[2];
			//Tinh gia tri gray
			uchar gray_val = (uchar)((B + G + R) / 3);
			pGrayRow[0] = gray_val;
		}

	}

	return 1;
}

int Converter::GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage)
{
	int width = sourceImage.cols, height = sourceImage.rows;
	destinationImage = Mat(height, width, CV_8UC3, Scalar(0, 0, 0));

	int srcChannels = sourceImage.channels();
	int dstChannels = destinationImage.channels();

	for (int y = 0; y < height; y++) {
		uchar* pSrcRow = (uchar*)sourceImage.ptr<uchar>(y);
		uchar* pDstRow = destinationImage.ptr<uchar>(y);

		for (int x = 0; x < width; x++, pSrcRow += srcChannels, pDstRow += dstChannels) {

			int B = pSrcRow[0];
			int G = B;
			int R = G;
			pDstRow[0] = B;
			pDstRow[1] = G;
			pDstRow[2] = R;
		}
	}
	return 1;
}

int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL) {
		return 0;
	}
	int width = sourceImage.cols;
	int height = sourceImage.rows;
	int srcchannel = sourceImage.channels();

	destinationImage = Mat(height, width, CV_8UC3);
	int dstchannel = destinationImage.channels();


	if (height != destinationImage.rows || width != destinationImage.cols || dstchannel != destinationImage.channels()) {
		return 0;
	}

	int srcwidthStep = sourceImage.step[0];
	int dstwidthStep = destinationImage.step[0];


	uchar* pSrcData = sourceImage.data;
	uchar* pDstData = destinationImage.data;

	for (int i = 0; i < height; i++, pSrcData += srcwidthStep, pDstData += dstwidthStep) {
		uchar* pSrcRow = pSrcData;
		uchar* pDstRow = pDstData;
		for (int j = 0; j < width; j++, pSrcRow += srcchannel, pDstRow += dstchannel) {
			uchar blue = pSrcRow[0];
			uchar green = pSrcRow[1];
			uchar red = pSrcRow[2];

			float fBlue = float(blue) / 255;
			float fGreen = float(green) / 255;
			float fRed = float(red) / 255;

			float max, min, H, S, V;
			max = min = H = S = V = 0;
			if (blue == red && red == green) {
				max = min = fBlue;
			}
			else {
				/* blue>red */
				if (blue > red) {
					/* red>green */
					if (red > green) {
						/* blue>red>green */
						max = fBlue;
						min = fGreen;
					}
					/* green>red */
					else {
						if (blue > green) {
							/* blue>green>red */
							max = fBlue;
							min = fRed;
						}
						/* green>blue */
						else {
							/* green>blue>red */
							max = fGreen;
							min = fRed;
						}
					}
				}
				/* blue<red */
				else {
					/* blue>green */
					if (blue > green) {
						/* red>blue>green */
						max = fRed;
						min = fGreen;
					}
					/* green>blue */
					else {
						/* green>red */
						if (green > red) {
							/* green>red>blue */
							max = fGreen;
							min = fBlue;
						}
						/* green<red */
						else {
							/* red>green>blue */
							max = fRed;
							min = fBlue;
						}
					}
				}
			}

			V = max;
			if (max == 0) {
				S = 0;
			}
			else {
				S = (max - min) / max;
			}

			if (max == min) {
				H = 0;
			}
			else {
				if (max == fRed) {
					H = 60 * (fGreen - fBlue) / (max - min);
				}
				else if (max == fGreen) {
					H = 60 * (2 + (fBlue - fRed) / (max - min));
				}
				else if (max == fBlue) {
					H = 60 * (4 + (fRed - fGreen) / (max - min));
				}
			}
			if (H < 0) {
				H += 360;
			}

			uchar iV = uchar(V * 255);
			uchar iS = uchar(S * 255);
			uchar iH = uchar(H / 2);

			pDstRow[0] = iH;
			pDstRow[1] = iS;
			pDstRow[2] = iV;
		}
	}
	return 1;
}

int Converter::HSV2RGB(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL) {
		return 0;
	}
	int width = sourceImage.cols;
	int height = sourceImage.rows;
	int srcchannel = sourceImage.channels();

	destinationImage = Mat(height, width, CV_8UC3);
	int dstchannel = destinationImage.channels();

	if ((height != destinationImage.rows) || (width != destinationImage.cols) || (dstchannel != destinationImage.channels())) {
		return 0;
	}

	int srcwidthStep = sourceImage.step[0];
	int dstwidthStep = destinationImage.step[0];

	uchar* pSrcData = sourceImage.data;
	uchar* pDstData = destinationImage.data;

	for (int i = 0; i < height; i++, pSrcData += srcwidthStep, pDstData += dstwidthStep) {
		uchar* pSrcRow = pSrcData;
		uchar* pDstRow = pDstData;
		for (int j = 0; j < width; j++, pSrcRow += srcchannel, pDstRow += dstchannel) {
			float fH = float(pSrcRow[0] * 2);
			float fS = float(pSrcRow[1]) / 255;
			float fV = float(pSrcRow[2]) / 255;



			float fC = fS * fV;
			float fHComma = fH / 60;
			float fX = fC * (1 - abs((int(fHComma) % 2 - 1)));

			float fR1, fG1, fB1;
			fR1 = fG1 = fB1 = 0;
			if (fHComma >= 0 && fHComma <= 1) {
				fR1 = fC;
				fG1 = fX;
				fB1 = 0;
			}
			else if (fHComma <= 2) {
				fR1 = fX;
				fG1 = fC;
				fB1 = 0;
			}
			else if (fHComma <= 3) {
				fR1 = 0;
				fG1 = fC;
				fB1 = fX;
			}
			else if (fHComma <= 4) {
				fR1 = 0;
				fG1 = fX;
				fB1 = fC;
			}
			else if (fHComma <= 5) {
				fR1 = fX;
				fG1 = 0;
				fB1 = fC;
			}
			else if (fHComma <= 6) {
				fR1 = fC;
				fG1 = 0;
				fB1 = fX;
			}

			float m = fV - fC;
			float fR, fG, fB;
			fR = fR1 + m;
			fG = fG1 + m;
			fB = fB1 + m;
			pDstRow[0] = uchar(fB * 255);
			pDstRow[1] = uchar(fG * 255);
			pDstRow[2] = uchar(fR * 255);
		}
	}
	return 1;
}

int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type)
{
	switch (type)
	{
	case 0:
		RGB2GrayScale(sourceImage, destinationImage);
		break;
	case 1:
		GrayScale2RGB(sourceImage, destinationImage);
		break;
	case 2:
		RGB2HSV(sourceImage, destinationImage);
		break;
	case 3:
		HSV2RGB(sourceImage, destinationImage);
		break;
	default:
		break;
	}
	return 0;
}

Converter::Converter()
{
}

Converter::~Converter()
{
}
