#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <vector>
#include <atomic>
#include <QObject>
#include <QImage>


class recognizer : public QObject
{
	Q_OBJECT

	using pics_t = std::vector<cv::Mat>;
	using labels_t = std::vector<int>;
	using faces_t = std::vector<cv::Rect>;

public:
	recognizer(QObject *parent = Q_NULLPTR);
	~recognizer();
	void loadXml(std::string &fileName);
	void startTakePicture(int userId);
	void stopTakePicture();
	void startTrain(int userId);
	void startMultiTrain(std::map<int, std::string> &idName);
	void startPredictFromCam();
	void stopPredictFromCam();
	void startMultiPredictFromCam();
	void stopMultiPredictFromCam();

signals:
	void readyImage(cv::Mat frame);
	void signalStopTrain();
	void signalGuiEnable();
	void signalStatePred();
	void signalStateMultiPred();

public slots:
	void stopTrain();

private:
	void takePicture(int userId);
	void train(int userId);
	void multiTrain(std::map<int, std::string> &idName);
	void predictFromCam();
	void multiPredictFromCam();
	void detectFace(cv::Mat &src, int id, int &count, bool saveFace);
	void readPictures(int userId, int &count, pics_t &pics, labels_t &labels);
	
	cv::CascadeClassifier _faceCascade;
	cv::Ptr<cv::face::FaceRecognizer> _model;
	std::thread _t;
	std::atomic<bool> _isThreadRunning;
};
