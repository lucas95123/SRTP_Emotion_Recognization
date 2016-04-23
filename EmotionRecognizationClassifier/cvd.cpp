#include "cvd.h"

void findDimension(ifstream &fin, int &row, int &col)
{
	row = 0;
	stringstream ss;
	while (!fin.eof())
	{
		string str;
		getline(fin, str);
		str.erase(0, str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" ") + 1);
		if (str == "")
			break;
		ss.clear();
		ss << str;
		int label;
		ss >> label;
		bool flag = false;
		while (!ss.eof())
		{
			int index;
			ss >> index;
			char comma;
			ss >> comma;
			double value;
			ss >> value;
			if (index > col)
				col = index;
		}
		row++;
	}
	fin.clear();
	fin.seekg(0);
}

CvDDriver::CvDDriver()
{
}

void CvDDriver::createDtree(const CvMat* data, const CvMat* responses, const CvMat* missing, float p_weight)
{
	CvMat* var_type;
	int i, success = 0, fail = 0;
	float priors[] = { 1, p_weight };

	var_type = cvCreateMat(data->cols + 1, 1, CV_8U);
	cvSet(var_type, cvScalarAll(CV_VAR_CATEGORICAL)); // all the variables are categorical

	dtree = new CvDTree;
	dtree->train(data, CV_ROW_SAMPLE, responses, 0, 0, 0, missing,
		CvDTreeParams(8, // max depth
		10, // min sample count
		0, // regression accuracy: N/A here
		true, // compute surrogate split, as we have missing data
		15, // max number of categories (use sub-optimal algorithm for larger numbers)
		10, // the number of cross-validation folds
		false, // use 1SE rule => smaller tree
		true, // throw away the pruned tree branches
		priors // the array of priors, the bigger p_weight, the more attention
		// to the poisonous mushrooms
		// (a mushroom will be judjed to be poisonous with bigger chance)
		));
}

void CvDDriver::readData(string &file_name, Mat &data, Mat &response, Mat &missing)
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
	response = Mat(row, 1, CV_32S);
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
		ss >> label;
		response.at<int>(cvPoint(col, row)) = label;
		while (!ss.eof())
		{
			int index;
			ss >> index;
			char comma;
			ss >> comma;
			float value;
			ss >> value;
			while (index != col+1)
			{
				data.at<float>(cvPoint(col, row)) = -1;
				missing.at<uchar>(cvPoint(col, row)) = 1;
				col++;
			}
			data.at<float>(cvPoint(col, row)) = value;
			missing.at<uchar>(cvPoint(col, row)) = 0;
			col++;
		}
		row++;
	}
}

void CvDDriver::createTrainingset(string &file_name)
{
	cv::Mat data,responses,missing;
	readData(file_name, data, responses, missing);
	CvMat *data_m=cvCreateMat(data.rows,data.cols,data.type());
	CvMat tmp = data;
	cvCopy(&tmp, data_m);
	CvMat *missing_m = cvCreateMat(missing.rows, missing.cols, missing.type());
	tmp = missing;
	cvCopy(&tmp, missing_m);
	CvMat *responses_m = cvCreateMat(responses.rows, responses.cols, responses.type());
	tmp = responses;
	cvCopy(&tmp, responses_m);
	createDtree(data_m, responses_m, missing_m, 1);

	//dtree->train(data, CV_ROW_SAMPLE, responses, 0, 0, var_type, missing,
	//	CvDTreeParams(8, // max depth
	//	10, // min sample count
	//	0, // regression accuracy: N/A here
	//	true, // compute surrogate split, as we have missing data
	//	15, // max number of categories (use sub-optimal algorithm for larger numbers)
	//	10, // the number of cross-validation folds
	//	true, // use 1SE rule => smaller tree
	//	true, // throw away the pruned tree branches
	//	priors // the array of priors, the bigger p_weight, the more attention
	//	// to the poisonous mushrooms
	//	// (a mushroom will be judjed to be poisonous with bigger chance)
	//	));
}

void CvDDriver::CvDClassifyDriver(string &file_name)
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
	// compute hit-rate on the training database, demonstrates predict usage.
	int success = 0;
	int fail = 0;
	for (int i = 0; i < data_m->rows; i++)
	{
		CvMat sample, mask;
		cvGetRow(data_m, &sample, i);
		cvGetRow(missing_m, &mask, i);
		double r = dtree->predict(&sample, &mask)->value;
		if (r == responses_m->data.i[i])
			success++;
		else
			fail++;
	}
	cout << "Among " << success + fail << " cases, there are " << success << " success and " << fail << " fail" << endl;
	cout << "The rate is " << ((double)success) / (success + fail) << endl;
}