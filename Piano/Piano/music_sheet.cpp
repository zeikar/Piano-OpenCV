#include "music_sheet.h"
#include "Constants.h"
cv::Mat piano_view(bool &next_page, int &index, std::vector<cv::Mat> &piano_map)
{
	if (index + 1 != piano_map.size() && next_page)
	{
		index++;
	}
	return piano_map[index];
}

void piano_map_init_video(std::vector<cv::Mat> &piano_map)
{
	//video input type
	cv::VideoCapture video(0);
	if (!video.isOpened())
	{
		std::cout << "웹캠을 열수 없습니다." << std::endl;
		exit(1);
	}
	video.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	video.set(CV_CAP_PROP_FRAME_HEIGHT, 720);//해상도 조절

	cv::Mat frame;

	int key = 0, index = 0;
	int i = 100;
	while (1)
	{
		//cv::setMouseCallback("test", CallBackFunc, NULL);
		cv::resizeWindow(PROJECT_NAME, cv::Size(WINDOW_WIDTH, PIANO_MAP_HEIGHT));
		std::string temp_char;
		video >> frame;//웹캡으로부터 한 프레임을 읽어옴  
		cv::Mat temp_frame = frame.clone();

		temp_char = std::to_string(index);

		//index frame에 추가
		cv::putText(temp_frame, temp_char, cv::Point(10, 40), 2, 1.0, cv::Scalar::all(255));
		
		imshow("test", temp_frame);

		key = cv::waitKey(30);

		if (key == 'a' || key == 'A')//adding map
		{
			piano_map.push_back(frame.clone());
			std::cout << "저장" << std::endl;
			index++;
		}
		else if (key == 'q' || key == 'Q') break;//stop
	}
	video.release();
}

void piano_map_init_DB(std::string &input_file, std::vector<cv::Mat> &piano_map)
{
	//database type
	for (int i = 0; ; i++)
	{
		char char_temp[3];
		itoa(i, char_temp, 10);
		piano_map.push_back(cv::imread(input_file + "/" + input_file + "_" + char_temp + ".jpg"));
		if (!piano_map[i].data)
		{
			piano_map.pop_back();
			break;
		}
	}
	for (int i = 0; i < piano_map.size(); i++)
	{
		cv::resize(piano_map[i], piano_map[i], cv::Size(WINDOW_WIDTH, PIANO_MAP_HEIGHT));
		cv::imshow("test", piano_map[i]);
		cv::waitKey();
	}
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		std::cout << "좌표 = (" << x << ", " << y << ")" << std::endl;
	}
}