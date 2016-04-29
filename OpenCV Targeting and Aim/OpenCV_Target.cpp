#include "OpenCV_Target.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>
#include <iostream>

using namespace cv;

OpenCV_Target::OpenCV_Target(std::string windowName, char port, char imtyp = 0, char minimumWidth = 80, char minimumHeight = 80, char maximumWidth = 200, char maximumHeight = 200)
{
	VideoCapture cap(port);
	cap.open(port);
	cascadeName = imageType[imtyp];
	loaded = detector.load(cascadeName);
	minW = minimumWidth;
	minH = minimumHeight;
	maxW = minimumWidth;
	maxH = maximumHeight;
	minObjectSize = new Size(minW, minH);
	maxObjectSize = new Size(maxW, maxH);
	OpenCV_Target::windowName = windowName;
}

OpenCV_Target::~OpenCV_Target()
{
} 

bool OpenCV_Target::isConnected()
{
	if (!cap.isOpened())
		return false;
	else
		return true;
}

void OpenCV_Target::detectTarget()
{
	
	cap >> img;

	Mat greyImage;
	cvtColor(img, greyImage, CV_BGR2GRAY);
	std::vector<Rect> found;
	found.clear();

	detector.detectMultiScale(greyImage, found, scaleStep, groundTheshold, 0 | 2, *minObjectSize, *maxObjectSize);

	if (found.size() > 0)
	{
		for (int i = 0; i <= found.size() - 1; i++)
			rectangle(img, found[i].br(), found[i].tl(), Scalar(0, 0, 0), 2, 8, 0);
	}

	TL = found[1].tl();
	BR = found[1].br();
	CENTER[1] = (TL[1] + BR[1]) / 2;
	CENTER[2] = (TL[2] + BR[2]) / 2;

	imshow(windowName, img);
}

void OpenCV_Target::detectTargetThread()//beta don't use
{
	std::thread(threadableTargetDetection);
}

void OpenCV_Target::threadableTargetDetection()//beta don't use
{
	while (true)
	{
		cap >> img;

		Mat greyImage;
		cvtColor(img, greyImage, CV_BGR2GRAY);
		std::vector<Rect> found;
		found.clear();

		detector.detectMultiScale(greyImage, found, scaleStep, groundTheshold, 0 | 2, *minObjectSize, *maxObjectSize);

		if (found.size() > 0)
		{
			for (int i = 0; i <= found.size() - 1; i++)
				rectangle(img, found[i].br(), found[i].tl(), Scalar(0, 0, 0), 2, 8, 0);
		}
		TL = found[1].tl();
		BR = found[1].br();
		CENTER[1] = (TL[1] + BR[1]) / 2;
		CENTER[2] = (TL[2] + BR[2]) / 2;
		imshow(windowName, img);
	}
}