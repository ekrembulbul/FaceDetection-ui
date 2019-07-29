#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <vector>


class recognizer
{
public:
	recognizer();
	~recognizer();
	void loadXml(std::string fileName);
	void takePicture(int userId);
	void train(int userId);
	void multiTrain(int userCount);
	void predictFromCam();
	void predictFromImage();
	void multiPredictFromCam(std::vector<std::string> &names);

private:
	cv::CascadeClassifier _faceCascade;
	cv::Ptr<cv::face::FaceRecognizer> _model;
	using pics_t = std::vector<cv::Mat>;
	using labels_t = std::vector<int>;
	using faces_t = std::vector<cv::Rect>;

	void detectFace(cv::Mat &src, int id, int &count, bool saveFace);
	void readPictures(int userId, int &count, pics_t &pics, labels_t &labels);
};
