#include "cvrt.h"

void CvRTDriver::createClassifier(const CvMat* data, const CvMat* responses, const CvMat* missing, float p_weight)
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

	rtree = new CvRTrees;
	rtree->train(data, CV_ROW_SAMPLE, responses, 0, 0, 0, missing,
		CvRTParams(20, // max depth
		10, // min sample count
		0.01f, // regression accuracy: N/A here
		false, // compute surrogate split, as we have missing data
		10, // max number of categories (use sub-optimal algorithm for larger numbers)
		0, // the array of priors
		false,  // calculate variable importance
		0,       // number of variables randomly selected at node and used to find the best split(s).
		210,     // max number of trees in the forest
		0.01f,                // forest accuracy
		CV_TERMCRIT_ITER | CV_TERMCRIT_EPS // termination cirteria
		));
}

void CvRTDriver::CvRTClassifyDriver(string &file_name)
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
		float r = rtree->predict(&sample, &mask);
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