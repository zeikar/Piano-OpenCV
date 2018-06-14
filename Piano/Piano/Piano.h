#include <opencv2\opencv.hpp>
#include <string>
#include <vector>
#include <queue>
#include "Constants.h"
#include "fmodex\fmod.h"
using namespace std;
using namespace cv;

class Piano
{
public:
	Piano();
	~Piano();

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
	
	// fmod library for sound effects

	FMOD_SYSTEM *g_System;					// 음악 및 소리 재생
	FMOD_SOUND *g_Sound[PIANO_SIZE];		// 배경음악(효과음)의 개수
	FMOD_CHANNEL *g_Channel[PIANO_SIZE];	// 동시재생될 개수

	void initSound();
	void releaseSound();
};

Piano::~Piano()
{
	releaseSound();
}

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

	// init sound
	initSound();
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
	if (isFingerOn[pianoKeyboardIndex] == false)
	{
		// 소리 재생
		FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[pianoKeyboardIndex], 0, &g_Channel[pianoKeyboardIndex]);
	}

	isFingerOn[pianoKeyboardIndex] = true;
}

inline void Piano::releaseFinger(int pianoKeyboardIndex)
{
	if (isFingerOn[pianoKeyboardIndex] == true)
	{
		// 중지
		FMOD_Channel_Stop(g_Channel[pianoKeyboardIndex]);
	}

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

inline void Piano::initSound()
{
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, PIANO_SIZE, FMOD_INIT_NORMAL, nullptr);

	const char* piano_key_files[] = {
		"piano_keys\\203458__tesabob2001__a3.mp3",
		"piano_keys\\203459__tesabob2001__a-5.mp3",
		"piano_keys\\203460__tesabob2001__a-4.mp3",
		"piano_keys\\203461__tesabob2001__a-3.mp3",
		"piano_keys\\203462__tesabob2001__b4.mp3",
		"piano_keys\\203463__tesabob2001__b3.mp3",
		"piano_keys\\203464__tesabob2001__a5.mp3",
		"piano_keys\\203465__tesabob2001__a4.mp3",
		"piano_keys\\203466__tesabob2001__c-3.mp3",
		"piano_keys\\203467__tesabob2001__b5.mp3",
		"piano_keys\\203468__tesabob2001__f3.mp3",
		"piano_keys\\203469__tesabob2001__f4.mp3",
		"piano_keys\\203470__tesabob2001__e3.mp3",
		"piano_keys\\203471__tesabob2001__e4.mp3",
		"piano_keys\\203472__tesabob2001__d4.mp3",
		"piano_keys\\203473__tesabob2001__d5.mp3",
		"piano_keys\\203474__tesabob2001__f-4.mp3",
		"piano_keys\\203475__tesabob2001__f-5.mp3",
		"piano_keys\\203476__tesabob2001__e5.mp3",
		"piano_keys\\203477__tesabob2001__f-3.mp3",
		"piano_keys\\203478__tesabob2001__c4-middle-c.mp3",
		"piano_keys\\203479__tesabob2001__c3.mp3",
		"piano_keys\\203480__tesabob2001__c-5.mp3",
		"piano_keys\\203481__tesabob2001__c-4.mp3",
		"piano_keys\\203482__tesabob2001__d-4.mp3",
		"piano_keys\\203483__tesabob2001__d-3.mp3",
		"piano_keys\\203484__tesabob2001__c6.mp3",
		"piano_keys\\203485__tesabob2001__c5.mp3",
		"piano_keys\\203486__tesabob2001__d3.mp3",
		"piano_keys\\203487__tesabob2001__d-5.mp3",
		"piano_keys\\203488__tesabob2001__g-3.mp3",
		"piano_keys\\203489__tesabob2001__f5.mp3",
		"piano_keys\\203490__tesabob2001__g-5.mp3",
		"piano_keys\\203491__tesabob2001__g-4.mp3",
		"piano_keys\\203492__tesabob2001__g4.mp3",
		"piano_keys\\203493__tesabob2001__g3.mp3",
		"piano_keys\\203494__tesabob2001__piano-chromatic-scale.mp3",
		"piano_keys\\203495__tesabob2001__g5.mp3",
		"piano_keys\\203499__tesabob2001__f-5.mp3",
		"piano_keys\\203500__tesabob2001__f-4.mp3",
		"piano_keys\\203501__tesabob2001__f-3.mp3",
		"piano_keys\\203502__tesabob2001__a-3.mp3",
		"piano_keys\\68437__pinkyfinger__piano-a.wav",
		"piano_keys\\68438__pinkyfinger__piano-b.wav",
		"piano_keys\\68439__pinkyfinger__piano-bb.wav",
		"piano_keys\\68440__pinkyfinger__piano-c.wav",
		"piano_keys\\68441__pinkyfinger__piano-c.wav",
		"piano_keys\\68442__pinkyfinger__piano-d.wav",
		"piano_keys\\68443__pinkyfinger__piano-e.wav",
		"piano_keys\\68444__pinkyfinger__piano-eb.wav",
		"piano_keys\\68445__pinkyfinger__piano-f.wav",
		"piano_keys\\68446__pinkyfinger__piano-f.wav",
		"piano_keys\\68447__pinkyfinger__piano-g.wav",
		"piano_keys\\68448__pinkyfinger__piano-g.wav"
	};

	const int piano_key_index[PIANO_SIZE] = {
		21, 8,  28, 25, 12, 10, 40, 35, 30, 0, 41, 5, 
		20, 23, 14, 24, 13, 11, 39, 34, 33, 7, 2, 4
	};

	for (int i = 0; i < PIANO_SIZE; i++)
	{
		FMOD_System_CreateStream(g_System, piano_key_files[piano_key_index[i]], FMOD_DEFAULT, 0, &g_Sound[i]);
	}
	
	FMOD_System_Update(g_System);
}

inline void Piano::releaseSound()
{
	for (int i = 0; i < PIANO_SIZE; i++)
	{
		FMOD_Sound_Release(g_Sound[i]);
	}
	FMOD_System_Close(g_System);
	FMOD_System_Release(g_System);
}
