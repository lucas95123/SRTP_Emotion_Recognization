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
class CvDDriver:public CvClassifierDriver
{
private:
	CvDTree *dtree = NULL;
protected:
	void createClassifier(const CvMat* data, const CvMat* responses, const CvMat* missing, float p_weight);
public:
	~CvDDriver(){
		delete dtree;
	}
	void CvDClassifyDriver(string &file_name);
};

