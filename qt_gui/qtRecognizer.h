#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <vector>


class recognizer
{
	using pics_t = std::vector<cv::Mat>;
	using labels_t = std::vector<int>;
	using faces_t = std::vector<cv::Rect>;

public:
	recognizer();
	~recognizer();
	void loadXml(std::string &fileName);
	void takePicture(int userId);
	void train(int userId);
	void multiTrain(std::map<int, std::string> &idName);
	void predictFromCam();
	void multiPredictFromCam();
	//void predictFromImage();

private:
	void detectFace(cv::Mat &src, int id, int &count, bool saveFace);
	void readPictures(int userId, int &count, pics_t &pics, labels_t &labels);
	
	cv::CascadeClassifier _faceCascade;
	cv::Ptr<cv::face::FaceRecognizer> _model;
};
