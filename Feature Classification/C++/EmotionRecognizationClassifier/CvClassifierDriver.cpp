#include "CvClassifierDriver.h"

void CvClassifierDriver::readData(string &file_name, Mat &data, Mat &response, Mat &missing)
{
	ifstream fin(file_name);
	if (!fin.is_open())
	{
		cout << "File not exist" << endl;
		exit(0);
	}
	stringstream ss;
	int row = 0;
	int col = 0;
	findDimension(fin, row, col);
	data = Mat(row, col, CV_32F);
	missing = Mat(row, col, CV_8U);
#ifdef CLASSIFY
	response = Mat(row, 1, CV_32S);
#else
	response = Mat(row, 1, CV_32F);
#endif // CLASSIFY

	
	row = 0;
	col = 0;
	while (!fin.eof())
	{
		col = 0;
		string str;
		getline(fin, str);
		str.erase(0, str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" ") + 1);
		if (str == "")
			break;
		ss.clear();
		ss << str;
		int label;
		float tmp;
		ss >> tmp;
		label=(int)tmp;
#ifdef CLASSIFY
		response.at<int>(cvPoint(col, row)) = label;
#else
		response.at<float>(cvPoint(col, row)) = label;
#endif // CLASSIFY
		while (!ss.eof())
		{
			int index;
			float tmp;
			ss >> tmp;
			index=(int)tmp;
			char comma;
			ss >> comma;
			float value;
			ss >> value;
			while (index != col + 1)
			{
				data.at<float>(cvPoint(col, row)) = -1;
				missing.at<uchar>(cvPoint(col, row)) = 1;
				col++;
			}
			data.at<float>(cvPoint(col, row)) = value;
			missing.at<uchar>(cvPoint(col, row)) = 0;
			col++;
		}
		while (col < data.cols)
		{
			data.at<float>(cvPoint(col, row)) = -1;
			missing.at<uchar>(cvPoint(col, row)) = 1;
		}
		row++;
	}
}

void CvClassifierDriver::createClassifier(const CvMat* data, const CvMat* responses, const CvMat* missing, float p_weight)
{
	return;
}

void CvClassifierDriver::createTrainingset(string &file_name)
{
	cv::Mat data, responses, missing;
	readData(file_name, data, responses, missing);
	CvMat *data_m = cvCreateMat(data.rows, data.cols, data.type());
	CvMat tmp = data;
	cvCopy(&tmp, data_m);
	CvMat *missing_m = cvCreateMat(missing.rows, missing.cols, missing.type());
	tmp = missing;
	cvCopy(&tmp, missing_m);
	CvMat *responses_m = cvCreateMat(responses.rows, responses.cols, responses.type());
	tmp = responses;
	cvCopy(&tmp, responses_m);
	createClassifier(data_m, responses_m, missing_m, 1);
}