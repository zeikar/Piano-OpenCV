#include <opencv2\opencv.hpp>
#include <string>
#include <vector>
#include "Constants.h"
using namespace std;
using namespace cv;

class Piano
{
public:
	Piano();
	~Piano();

	// 화면에 표시
	void renderPiano();
	// 손가락을 올림
	void putFinger(int pianoKeyboardIndex);
	// 손가락을 뗌
	void releaseFinger(int pianoKeyboardIndex);

private:
	Mat pianoImage;
	Mat fingerImage;

	// 손이 올라가 있는 위치를 기억
	bool isFingerOn[PIANO_SIZE] = { false };
	// 하얀 건반인가
	bool isWhiteKeyboard[PIANO_SIZE] = {
		true, false, true, false, true, true, false, true, false, true, false, true, 
		true, false, true, false, true, true, false, true, false
	};
};

Piano::Piano()
{
	const string pianoFileName = "images/piano.jpg";
	const string fingerFileName = "images/finger.png";

	pianoImage = imread(pianoFileName);
	fingerImage = imread(fingerFileName);

	cout << pianoImage.rows << ' ' << pianoImage.cols << endl;
	cout << fingerImage.rows << ' ' << fingerImage.cols << endl;

	namedWindow(PROJECT_NAME, WINDOW_NORMAL);
	resizeWindow(PROJECT_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
}

Piano::~Piano()
{
}

inline void Piano::renderPiano()
{
	Mat frame = pianoImage;

	// 손이 눌린 것을 화면에 표시
	for (int i = 0; i < PIANO_SIZE; i++)
	{
		if (isFingerOn[i] == false)
		{
			continue;
		}

		int width = i * (pianoImage.cols / PIANO_SIZE);
		int height = pianoImage.rows - fingerImage.rows;

		// 하얀 건반
		if (isWhiteKeyboard[i])
		{			
			frame(cv::Rect(width, height, fingerImage.cols, fingerImage.rows)) -= Scalar(100);
		}
		// 검은 건반
		else
		{
			height -= fingerImage.rows / 3;
			frame(cv::Rect(width, height, fingerImage.cols, fingerImage.rows)) += Scalar(200);
		}
	}

	imshow(PROJECT_NAME, frame);
}

inline void Piano::putFinger(int pianoKeyboardIndex)
{
	isFingerOn[pianoKeyboardIndex] = true;
}

inline void Piano::releaseFinger(int pianoKeyboardIndex)
{
	isFingerOn[pianoKeyboardIndex] = false;
}
