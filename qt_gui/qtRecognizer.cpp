#include "qtRecognizer.h"


namespace
{
	const int escKey = 27;
	const int spaceKey = 32;
	const int waitFrame = 35;
}


//public


recognizer::recognizer() :
	_model(cv::face::LBPHFaceRecognizer::create())
{
}


recognizer::~recognizer()
{
}


void recognizer::loadXml(std::string fileName)
{
	_faceCascade.load(fileName);
}


void recognizer::takePicture(int userId)
{
	cv::Mat frame;
	cv::VideoCapture cap(0);
	std::string winName("Cam");

	int count = 0;
	cv::namedWindow(winName);

	int key = cv::waitKey(waitFrame);
	while (key != escKey)
	{
		cap >> frame;

		detectFace(frame, userId, count, key == spaceKey);

		cv::imshow(winName, frame);
		key = cv::waitKey(waitFrame);
	}

	cap.release();
	cv::destroyWindow(winName);
}


void recognizer::train(int userId)
{
	pics_t pics;
	labels_t labels;
	int userCount = 0;
	readPictures(userId, userCount, pics, labels);
	_model->train(pics, labels);
	_model->write("trainer/trainer.yml");
}


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
}


void recognizer::predictFromCam()
{
	_model->read("trainer/trainer.yml");

	cv::Mat frame;
	cv::VideoCapture cap(0);
	std::string winName("Predict");
	cv::HersheyFonts font = cv::FONT_HERSHEY_SIMPLEX;

	cv::namedWindow(winName);
	while (cv::waitKey(waitFrame) != escKey)
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
		
		cv::imshow(winName, frame);
	}

	cap.release();
	cv::destroyWindow(winName);
}


void recognizer::multiPredictFromCam()
{
	_model->read("trainer/multi_trainer.yml");

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
	std::string winName("Multi Predict");
	cv::namedWindow(winName);

	while (cv::waitKey(waitFrame) != escKey)
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

		cv::imshow(winName, frame);
	}

	cap.release();
	cv::destroyAllWindows();
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
