#include <iostream>
#include <string>
#include "svm.h"
#include "knn.h"
#include "cvd.h"
#include "cvrt.h"
using namespace std;

void exit_with_help()
{
	printf(
		"Usage: emo-recogn [options] training_set_file testing_set_file\n"
		"options:\n"
		"-t classifier type: (default 0 svm)\n"
		"0 -- SVM/n"
		"1 -- KNN/n"
		"2 -- ID3/n"
		"3 -- Random Forest"
		);
	exit(1);
}

int main(int argc, char** argv)
{
	int mode = 0;
	string training_set_file;
	string testing_set_file;
	if (argc == 3)
	{
		mode = 0;
		training_set_file = argv[1];
		testing_set_file = argv[2];
	}
	else if (argc == 5)
	{
		if (strcmp(argv[1], "-t"))
			exit_with_help();
		mode = atoi(argv[2]);
		training_set_file = argv[3];
		testing_set_file = argv[4];
	}
	else
		exit_with_help();
	if (mode == 0)
	{
		cout << "SVM Classifier"<<endl;
		SVMDriver svm_driver;
		svm_driver.createTrainingset(training_set_file);
		svm_driver.SVMClassifyDriver(testing_set_file);
	}
	else if (mode == 1)
	{
		cout << "KNN Classifier" << endl;
		cout<< "Please input K: ";
		int K = 0;
		cin >> K;
		KNNDriver knn_driver(K);
		knn_driver.createTrainingset(training_set_file);
		knn_driver.KNNClassifyDriver(testing_set_file);
	}
	else if (mode==2)
	{
		cout << "Decision Tree Classifier" << endl;
		CvDDriver cvd_driver;
		cvd_driver.createTrainingset(training_set_file);
		cvd_driver.CvDClassifyDriver(testing_set_file);
	}
	else if (mode == 3)
	{
		cout << "Random Forest Classifier" << endl;
		CvRTDriver cvrt_driver;
		cvrt_driver.createTrainingset(training_set_file);
		cvrt_driver.CvRTClassifyDriver(testing_set_file);
	}
}