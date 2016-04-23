#pragma once
#include <opencv2\core\core_c.h>
#include <opencv2\ml\ml.hpp>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;
class CvDDriver
{
private:
	CvDTree *dtree = NULL;
	void readData(string &file_name, Mat &data, Mat &responses, Mat &missing);
	void createDtree(const CvMat* data, const CvMat* responses, const CvMat* missing, float p_weight);
public:
	CvDDriver();
	~CvDDriver()
	{
	}
	void createTrainingset(string &file_name);
	void CvDClassifyDriver(string &file_name);
};

