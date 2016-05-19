#include "cvd.h"

void CvDDriver::createClassifier(const CvMat* data, const CvMat* responses, const CvMat* missing, float p_weight)
{
	CvMat* var_type;
	int i, success = 0, fail = 0;
	float priors[] = { 1, p_weight };

	var_type = cvCreateMat(data->cols + 1, 1, CV_8U);

#ifdef CLASSIFY
	cvSet(var_type, cvScalarAll(CV_VAR_CATEGORICAL)); // all the variables are categorical
#else
	cvSet(var_type, cvScalarAll(CV_VAR_NUMERICAL)); // all the variables are categorical
#endif
	
	dtree = new CvDTree;
	dtree->train(data, CV_ROW_SAMPLE, responses, 0, 0, 0, missing,
		CvDTreeParams(8, // max depth
		10, // min sample count
		0.01f, // regression accuracy: N/A here
		true, // compute surrogate split, as we have missing data
		15, // max number of categories (use sub-optimal algorithm for larger numbers)
		0, // the number of cross-validation folds
		false, // use 1SE rule => smaller tree
		true, // throw away the pruned tree branches
		0 // the array of priors, the bigger p_weight, the more attention
		));
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
#ifdef CLASSIFY
		if (r == responses_m->data.i[i])
			success++;
		else
			fail++;
#else
		if (r == responses_m->data.fl[i])
			success++;
		else
			fail++;
#endif // CLASSIFY
	}
	cout << "Among " << success + fail << " cases, there are " << success << " success and " << fail << " fail" << endl;
	cout << "The rate is " << ((double)success) / (success + fail) << endl;
}