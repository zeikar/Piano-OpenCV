#include "FaceDetector.h"

FaceDetector::FaceDetector() {
	cap = cv::VideoCapture(0);
	cap.open(0);
	face_cascade.load("haarcascade_frontalface_alt2.xml");
}

bool FaceDetector::detect(int& currX) {
	// Image from camera to Mat
	cap >> img;

	// obtain input image from source
	cap.retrieve(img, CV_CAP_OPENNI_BGR_IMAGE);

	// Just resize input image if you want
	resize(img, img, cv::Size(1000, 640));


	// Detect faces
	face_cascade.detectMultiScale(img, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(140, 140));

	//Show the results
	// Draw circles on the detected faces

	int length;	//radius of the face
	for (int i = 0; i < faces.size(); i++) {
		cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		length = faces[i].width*0.2;
		if (abs(currX - center.x) > length) {
		currX = center.x;
			return true;
		}
//			std::cout << "true" << std::endl;
		currX = center.x;
		return false;
	}
}