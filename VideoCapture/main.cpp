#include "StdAfx.h"
#include "cvext.hpp"

//#define NO_DEVICE

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
{
#ifdef NO_DEVICE
	cv::VideoCapture cap("video_for_test.avi");
#else
	cv::VideoCapture cap(1401);
#endif
	if (!cap.isOpened())
	{
		MessageBox(0, L"Can't connect to camera", L"Error", MB_ICONERROR);
		return 1;
	}
	cv::namedWindow("wnd", CV_GUI_EXPANDED);
	QWidget* w = reinterpret_cast<QWidget*>(cvGetWindowHandle("wnd"));
	int exp = cap.get(CV_CAP_PROP_EXPOSURE);
	int gain = cap.get(CV_CAP_PROP_GAIN);
	cv::createTrackbar("exposure", "", &exp, 100);
	cv::createTrackbar("gain", "", &gain, 100);
	cvext::createButton("apply", [&cap,&exp,&gain](int state)
	{
		int old_exp = cap.get(CV_CAP_PROP_EXPOSURE);
		int old_gain = cap.get(CV_CAP_PROP_GAIN);
		if (old_exp != exp)
			cap.set(CV_CAP_PROP_EXPOSURE, exp);
		if (old_gain != gain)
			cap.set(CV_CAP_PROP_GAIN, gain);
	});
	cv::displayOverlay("wnd", "C - capture", 10000);

	QString fileNameFormat("%1.tiff");
	QString msgFormat("File saved: %1");
	QString errMsgFormat("Can't save file: %1");
	QString filePath;
	int key;
	int frameNum = 0;
	while (w->isVisible())
	{
		cv::Mat img;
		cap >> img;
#ifdef NO_DEVICE
		if (img.empty())
		{
			cap.set(CV_CAP_PROP_POS_FRAMES, 0);
			cap >> img;
		}
#endif
		cv::imshow("wnd", img);
		key = cv::waitKey(10);
		switch (key)
		{
		case 'c':
		case 'C':
			if (filePath.isEmpty())
			{
				filePath = QFileDialog::getExistingDirectory(nullptr, "Choose folder to save");
				if (filePath.isEmpty())
					break;
			}
			QString fileName = fileNameFormat.arg(frameNum++);
			if (cv::imwrite((filePath + '\\' + fileName).toStdString(), img))
				cv::displayOverlay("wnd", msgFormat.arg(fileName).toStdString(), 1000);
			else
			{
				MessageBox(0, reinterpret_cast<const wchar_t*>(errMsgFormat.arg(fileName).utf16()), L"Error", MB_ICONERROR);
				filePath.clear();
			}
			break;
		}
	}//while (key != 27);
	return 0;
}