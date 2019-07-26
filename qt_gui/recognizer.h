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
	void takePicture();
	void train();
	void predictFromCam();
	void predictFromImage();
	void multiTrain();
	void multiPredictFromCam();

private:
	cv::CascadeClassifier _faceCascade;
	cv::Ptr<cv::face::FaceRecognizer> _model;
	using pics_t = std::vector<cv::Mat>;
	using labels_t = std::vector<int>;
	using faces_t = std::vector<cv::Rect>;

	void detectFace(cv::Mat &src, int id, int &count, bool saveFace);
	void readPictures(int userId, pics_t &pics, labels_t &labels);
};
