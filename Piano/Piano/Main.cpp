#include "Piano.h"
#include "music_sheet.h"
#include "FaceDetector.h"
#include "Handrecognition.h"

using namespace std;

// 음악 재생률. 10이면 10프레임당 한 음을 재생함.
const int PLAY_RATE = 10;
int NOW_RATE = 0;

int main()
{
	Leap::Controller controller;

	// paste the listener
	MyListener listener;
	controller.addListener(listener);

	Piano piano;
	vector<cv::Mat> piano_map;
	string input_file = "twinkle_twinkle";

	bool next_page = false;
	int index = 0;

	piano_map_init_video(piano_map);//camera capture Ver

	//손명희추가
	//얼굴인식을 위한 클래스
	FaceDetector detector;

	piano_map_init_DB(input_file, piano_map); // DB(폴더)Ver

	//윈도우에 콜백함수를 등록
	cv::setMouseCallback(PROJECT_NAME, CallBackFunc, NULL);

	int tmpX = 0;

	cout << "start the piano system" << endl;

	while (listener.isEnable())
	{
		next_page = detector.detect(tmpX);

		if (next_page)
			cout << "넘김" << endl;

		cv::Mat temp_Map = piano_view(next_page, index, piano_map);

		// 피아노 재생
		// 음 시작
		piano.putFinger(listener.pressNotes());

		piano.renderPiano(temp_Map);
		waitKey(1);
		
		// 피아노 재생
		if (listener.isNoteChanged())
			piano.releaseFinger(listener.realeaseNotes());
	}

	// remove listener before exit program.
	controller.removeListener(listener);

	return 0;
}