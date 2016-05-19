#pragma once
#include <opencv2\core\core_c.h>
#include <opencv2\ml\ml.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include "tools.h"
#define CLASSIFY
//#define REGRESS

using namespace std;
using namespace cv;

class CvClassifierDriver
{
protected:
	void readData(string &file_name, Mat &data, Mat &responses, Mat &missing);
	virtual void createClassifier(const CvMat* data, const CvMat* responses, const CvMat* missing, float p_weight);
public:
	virtual ~CvClassifierDriver(){

	}
	void createTrainingset(string &file_name);
};


