#include <opencv2\opencv.hpp>
#include <string>
#include <vector>
#include <queue>
#include "Constants.h"
using namespace std;
using namespace cv;

class Piano
{
public:
	Piano();

	// 화면에 표시
	void renderPiano(cv::Mat &piano_map);
	// 손가락을 올림
	void putFinger(int pianoKeyboardIndex);
	// 손가락을 뗌
	void releaseFinger(int pianoKeyboardIndex);

	// 건반을 색칠
	void fillKeyboard(int x, int y, int color, int srcColor, Mat& frame, int maxY, int minY);

private:
	// 피아노 화면
	Mat pianoImage;
	// 피아노 엣지
	Mat edgePiano;

	// 손이 올라가 있는 위치를 기억
	bool isFingerOn[PIANO_SIZE] = { false };
	// 하얀 건반인가
	bool isWhiteKeyboard[PIANO_SIZE] = {
		true, false, true, false, true, true, false, true, false, true, false, true, 
		true, false, true, false, true, true, false, true, false, true, false, true
	};
};

Piano::Piano()
{
	const string pianoFileName = "images/piano.png";

	pianoImage = Mat::zeros(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
	//pianoImage = Scalar(255, 255, 255);

	Mat pianoImageFromFile = imread(pianoFileName);

	cout << pianoImage.rows << ' ' << pianoImage.cols << endl;
	cout << pianoImageFromFile.rows << ' ' << pianoImageFromFile.cols << endl;

	pianoImageFromFile.copyTo(pianoImage(
		cv::Rect(pianoImage.cols - pianoImageFromFile.cols, pianoImage.rows - pianoImageFromFile.rows,
			pianoImageFromFile.cols, pianoImageFromFile.rows)));

	namedWindow(PROJECT_NAME, WINDOW_NORMAL);

	cv::resizeWindow(PROJECT_NAME, cv::Size(WINDOW_WIDTH, WINDOW_HEIGHT));

	// canny
	threshold(pianoImage, edgePiano, 96, 255, THRESH_BINARY);
}

inline void Piano::renderPiano(cv::Mat &piano_map)
{
	Mat frame;
	pianoImage.copyTo(frame);

	// 손이 눌린 것을 화면에 표시
	for (int i = 0; i < PIANO_SIZE; i++)
	{
		if (isFingerOn[i] == false)
		{
			continue;
		}

		int width = i * (pianoImage.cols / PIANO_SIZE) + 30;
		int height = pianoImage.rows - 50;

		// 하얀 건반
		if (isWhiteKeyboard[i])
		{
			//frame(cv::Rect(width, height, fingerImage.cols, fingerImage.rows)) -= Scalar(100);
			fillKeyboard(width, height, 50, 255, frame, 1000, 0);
		}
		// 검은 건반
		else
		{
			//height -= fingerImage.rows / 3;
			//frame(cv::Rect(width, height, fingerImage.cols, fingerImage.rows)) += Scalar(200);
			fillKeyboard(width + 5, height - 200, 70, 0, frame, 450, 255);
		}
	}

	piano_map.copyTo(frame(
		cv::Rect(0, 0, piano_map.cols, piano_map.rows)));

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

inline void Piano::fillKeyboard(int x, int y, int color, int srcColor, Mat& frame, int maxY, int minY)
{
	Mat tempEdge;
	edgePiano.copyTo(tempEdge);

	int direction[][2] = {-1, 0, 0, -1, 0, 1, 1, 0};
	
	queue<Point> que;

	que.push(Point(x, y));

	while (!que.empty())
	{
		Point current = que.front();
		que.pop();

		for (int d = 0; d < 4; d++)
		{
			int nx = current.x + direction[d][0];
			int ny = current.y + direction[d][1];

			if (nx < 0 || nx >= edgePiano.cols || ny < 0 || ny >= edgePiano.rows
				|| ny <= minY || ny >= maxY)
			{
				continue;
			}

			if (tempEdge.at<Vec3b>(ny, nx)[1] == srcColor)
			{
				que.push(Point(nx, ny));

				frame.at<Vec3b>(ny, nx)[0] = 255;
				frame.at<Vec3b>(ny, nx)[1] = color;
				frame.at<Vec3b>(ny, nx)[2] = 255;
				tempEdge.at<Vec3b>(ny, nx)[1] = color;
			}
		}
	}
}
