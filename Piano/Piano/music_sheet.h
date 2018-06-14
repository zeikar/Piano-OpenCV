#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <opencv2\opencv.hpp>

cv::Mat piano_view(bool &next_page, int &index, std::vector<cv::Mat> &piano_map);
void piano_map_init_video(std::vector<cv::Mat> &piano_map);
void piano_map_init_DB(std::string & input_file, std::vector<cv::Mat>& piano_map);
void CallBackFunc(int event, int x, int y, int flags, void* userdata);

