#include "qtRecognizer.h"
#include <qdebug.h>
#include <Windows.h>


namespace
{
	const int escKey = 27;
	const int spaceKey = 32;
	const int waitFrame = 35;
}


//public


recognizer::recognizer(QObject *parent) :
	QObject(parent), _model(cv::face::LBPHFaceRecognizer::create()), _isThreadRunning(true)
{
	loadXml(std::string("haarcascade-frontalface-default.xml"));
	connect(this, &recognizer::signalStopTrain, this, &recognizer::stopTrain);
}


recognizer::~recognizer()
{
}


void recognizer::loadXml(std::string &fileName)
{
	_faceCascade.load(fileName);
}


void recognizer::startTakePicture(int userId)
{
	_isThreadRunning = true;
	_t = std::thread(&recognizer::takePicture, this, userId);
}


void recognizer::stopTakePicture()
{
	_isThreadRunning = false;
	if (_t.joinable()) _t.join();
}


// private
void recognizer::takePicture(int userId)
{
	cv::Mat frame;
	cv::VideoCapture cap(0);

	int count = 0;

	int key = cv::waitKey(waitFrame);
	while (_isThreadRunning)
	{
		cap >> frame;
		detectFace(frame, userId, count, key == spaceKey);

		emit readyImage(frame);
		                   

		key = cv::waitKey(waitFrame);
	}

	emit readyImage(cv::Mat());
	cap.release();
}


void recognizer::startTrain(int userId)
{
	_t = std::thread(&recognizer::train, this, userId);
}

void recognizer::stopTrain()
{
	if (_t.joinable()) _t.join();
	emit signalGuiEnable();
	qDebug() << __FUNCTION__ ": " << GetCurrentThreadId();
}


// private
void recognizer::train(int userId)
{
	pics_t pics;
	labels_t labels;
	int userCount = 0;
	readPictures(userId, userCount, pics, labels);
	_model->train(pics, labels);
	_model->write("trainer/trainer.yml");

	emit signalStopTrain();
	qDebug() << __FUNCTION__ ": " << GetCurrentThreadId();
}


void recognizer::startMultiTrain(std::map<int, std::string> &idName)
{
	_t = std::thread(&recognizer::multiTrain, this, idName);
}


// private
void recognizer::multiTrain(std::map<int, std::string> &idName)
{
	std::ofstream outFile("dataset/names.txt");
	for (auto &i : idName)
	{
		outFile << i.first << ":" << i.second << std::endl;
	}
	outFile.close();

	pics_t pics;
	labels_t labels;
	int count = 0;
	for (auto &i : idName)
	{
		readPictures(i.first, count, pics, labels);
	}

	_model->train(pics, labels);
	_model->write("trainer/multi_trainer.yml");

	emit signalStopTrain();
}


void recognizer::startPredictFromCam()
{
	_isThreadRunning = true;
	_t = std::thread(&recognizer::predictFromCam, this);
}


void recognizer::stopPredictFromCam()
{
	_isThreadRunning = false;
	if (_t.joinable()) _t.join();
}


// private
void recognizer::predictFromCam()
{
	_model->read("trainer/trainer.yml");
	emit signalStatePred();

	cv::Mat frame;
	cv::VideoCapture cap(0);
	cv::HersheyFonts font = cv::FONT_HERSHEY_SIMPLEX;

	while (_isThreadRunning)
	{
		cap >> frame;

		cv::Mat gray;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		
		faces_t faces;
		_faceCascade.detectMultiScale(gray, faces);

		for (auto & face : faces)
		{
			cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2);

			int label;
			double conf;
			_model->predict(cv::Mat(gray, face), label, conf);
			cv::putText(frame, std::to_string(conf), cv::Point(face.x + 5, face.y - 5), font, 1, (0, 0, 255), 2);
		}
		
		emit readyImage(frame);
	}
	emit readyImage(cv::Mat());

	cap.release();
}


void recognizer::startMultiPredictFromCam()
{
	_isThreadRunning = true;
	_t = std::thread(&recognizer::multiPredictFromCam, this);
}


void recognizer::stopMultiPredictFromCam()
{
	_isThreadRunning = false;
	if (_t.joinable()) _t.join();
}


//private
void recognizer::multiPredictFromCam()
{
	_model->read("trainer/multi_trainer.yml");
	emit signalStateMultiPred();

	std::map<int, std::string> idName;
	std::string tmpStr;
	std::ifstream inFile("dataset/names.txt");
	while (inFile >> tmpStr)
	{
		size_t found = tmpStr.find(":");
		if (found != std::string::npos)
		{
			std::stringstream mFirst(std::string(tmpStr.substr(0, found)));
			int mFirstInt;
			mFirst >> mFirstInt;
			idName.insert({ mFirstInt, tmpStr.substr(found + 1) });
		}
	}

	cv::Mat frame;
	cv::VideoCapture cap(0);

	while (_isThreadRunning)
	{
		cap >> frame;

		cv::Mat gray;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		
		faces_t faces;
		_faceCascade.detectMultiScale(gray, faces);

		for (auto & face : faces)
		{
			cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2);

			int label;
			double conf;
			_model->predict(cv::Mat(gray, face), label, conf);

			int threshold = 60;
			std::string s;
			if (conf < threshold) s = idName[label];
			else s = "unknown";

			cv::HersheyFonts font = cv::FONT_HERSHEY_SIMPLEX;
			cv::putText(frame, s, cv::Point(face.x + 5, face.y - 5), font, 1, (0, 0, 255), 2);
		}
		
		emit readyImage(frame);
	}
	emit readyImage(cv::Mat());

	cap.release();
}

/*
void recognizer::predictFromImage()
{
	//std::cout << "[INFO] Predicting faces. It will take a few seconds. Wait..." << std::endl;
	_model->read("trainer/trainer.yml");

	std::multimap<double, int> confs;
	int count = 0;
	while (true)
	{
		std::stringstream fileName;
		fileName << "pred_image/" << count << ".jpg";
		cv::Mat pic = cv::imread(fileName.str(), cv::IMREAD_GRAYSCALE);
		if (pic.data == NULL) break;

		faces_t faces;
		_faceCascade.detectMultiScale(pic, faces);

		for (auto &face : faces)
		{
			double conf;
			int label;
			_model->predict(cv::Mat(pic, face), label, conf);
			confs.insert({ conf, count });
		}

		count++;
	}

	//for (auto &i: confs)
	//{
	//	std::cout << i.second << ". " << i.first << std::endl;
	//}
	//std::cout << "[INFO] Prediction done." << std::endl;
}
*/

//private


void recognizer::detectFace(cv::Mat &src, int id, int &count, bool saveFace)
{
	cv::Mat gray;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

	faces_t faces;
	_faceCascade.detectMultiScale(gray, faces);

	for (auto & face : faces)
	{
		cv::rectangle(src, face, cv::Scalar(0, 0, 255), 2);

		if (saveFace)
		{
			std::stringstream fileName;
			fileName << "dataset/user" << id << "." << count << ".jpg";
			cv::imwrite(fileName.str(), cv::Mat(gray, face));
			count++;
		}
	}
}


void recognizer::readPictures(int userId, int &count, pics_t &pics, labels_t &labels)
{
	while (true)
	{
		std::stringstream fileName;
		fileName << "dataset/user" << userId << "." << count << ".jpg";
		cv::Mat pic = cv::imread(fileName.str(), cv::IMREAD_GRAYSCALE);
		if (pic.data == NULL)
		{
			count = 0;
			return;
		}

		pics.push_back(pic);
		labels.push_back(userId);

		count++;
	}
}
