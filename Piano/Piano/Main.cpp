#include "Piano.h"
#include "music_sheet.h"
#include "FaceDetector.h"
using namespace std;

// 음악 재생률. 10이면 10프레임당 한 음을 재생함.
const int PLAY_RATE = 10;

int main()
{
	int notes[] = {
		0, 4, 7, 12, 16, 12, 7, 4, 0
	};

	Piano piano;
	vector<cv::Mat> piano_map;
	string input_file = "twinkle_twinkle";
	bool next_page = false;
	int index = 0;
	//piano_map_init_video(piano_map);//camera capture Ver

	//손명희추가
	//얼굴인식을 위한 클래스
	FaceDetector detector;

	piano_map_init_DB(input_file, piano_map); // DB(폴더)Ver

	//윈도우에 콜백함수를 등록
	cv::setMouseCallback(PROJECT_NAME, CallBackFunc, NULL);
	int tmpX = 0;
	for (int i = 0; i < 9 * PLAY_RATE; i++)
	{
		cv::Mat temp_Map = piano_view(next_page, index, piano_map);

		// 피아노 재생
		if (i % PLAY_RATE == 0)
		{
			piano.putFinger(notes[i / PLAY_RATE]);
		}
		piano.renderPiano(temp_Map);
		waitKey(1);
		
		// 피아노 재생
		if (i % PLAY_RATE == PLAY_RATE - 1)
		{
			piano.releaseFinger(notes[i / PLAY_RATE]);
		}


		if (!detector.detect(tmpX))
			cout << "qwer";
	
	}

	waitKey();

	return 0;
}