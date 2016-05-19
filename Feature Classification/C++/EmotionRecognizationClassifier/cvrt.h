#pragma once
#include <opencv2\core\core_c.h>
#include <opencv2\ml\ml.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include "tools.h"
#include "CvClassifierDriver.h"
using namespace std;
using namespace cv;

class CvRTDriver:public CvClassifierDriver
{
private:
	CvRTrees* rtree=NULL;
protected:
	void createClassifier(const CvMat* data, const CvMat* responses, const CvMat* missing, float p_weight);
public:
	~CvRTDriver(){
		delete rtree;
	}
	void CvRTClassifyDriver(string &file_name);
};



