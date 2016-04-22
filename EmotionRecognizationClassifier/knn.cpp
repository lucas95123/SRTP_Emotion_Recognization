//
//  knn.cpp
//  EmotionRecongnzaiton
//
//  Created by KUAN LU on 16/4/22.
//  Copyright © 2016年 KUAN LU. All rights reserved.
//

#include "knn.h"

bool sort_dist(void *D1, void*D2)
{
	Distance *d1 = (Distance *)D1;
	Distance *d2 = (Distance *)D2;
	if (d1->distance > d2->distance)
		return false;
	else
		return true;
}

int KNNDriver::kNNClassify(vector<double> &vec_value)
{
	vector<Distance *> vec_L2;
	for (auto node : vec_node)
	{
		if (vec_value.size() != node->size())
			throw new exception("Dimension not same");
		double tmp_sum = 0;
		for(int i = 0; i < node->size(); i++)
			tmp_sum += pow((node->getCoordinate()[i]-vec_value[i]),2);
		vec_L2.push_back(new Distance(node->getLabel(),tmp_sum));
	}
	sort(vec_L2.begin(), vec_L2.end(),sort_dist);
	map<int, int> map_min_dist;
	for (int i = 0; i < K; i++)
	{
		if (map_min_dist.find(vec_L2[i]->label) == map_min_dist.end())
			map_min_dist[vec_L2[i]->label] = 1;
		else
			map_min_dist[vec_L2[i]->label] += 1;
	}
	int max = 0;
	int max_label = 0;
	for (auto dist : map_min_dist)
	{
		if (dist.second > max)
		{
			max = dist.second;
			max_label = dist.first;
		}
	}
	return max_label;
}

void KNNDriver::createTrainingset(string &file_name)
{
	parseFile(file_name, vec_node);
}

void KNNDriver::KNNClassifyDriver(string &file_name)
{
	vector<Node *> vec_test;
	parseFile(file_name, vec_test);
	ofstream fout(file_name + ".result");
	int success = 0;
	int fail = 0;
	for (auto sample : vec_test)
	{
		int result=kNNClassify(sample->getCoordinate());
		if (result == sample->getLabel())
			success++;
		else
			fail++;
		fout << result << endl;
	}
	cout << "Among " << success + fail << " cases, there are " << success << " success and " << fail << " fail" << endl;
	cout <<"The rate is "<< ((double)success) / (success + fail) << endl;	
}

void KNNDriver::parseFile(string &file_name, vector<Node *> &vec_result)
{
    ifstream fin(file_name);
	if (!fin.is_open())
	{
		cout << "File not exist" << endl;
		exit(0);
	}
	stringstream ss;
    while(!fin.eof())
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
		Node *new_node = new Node(label);
		bool flag = false;
		while (!ss.eof())
		{
			int index;
			ss >> index;
			char comma;
			ss >> comma;
			double value;
			ss >> value;
			while (index != new_node->size()+1)
				new_node->addValue(0);
			new_node->addValue(value);
		}
		vec_result.push_back(new_node);
    }
	dimension = vec_result[0]->size();
	num_sample = vec_result.size();
}