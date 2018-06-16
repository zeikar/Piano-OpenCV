#pragma once
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>
#include <queue>
#include <iostream>

class FaceDetector {
	cv::VideoCapture cap;
	cv::Mat img;
	cv::CascadeClassifier face_cascade;

	// Container of faces
	std::vector<cv::Rect> faces;

public:
	FaceDetector();
	bool detect(int& currX);
};