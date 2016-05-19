//
//  knn.hpp
//  EmotionRecongnzaiton
//
//  Created by KUAN LU on 16/4/22.
//  Copyright © 2016年 KUAN LU. All rights reserved.
//

#ifndef knn_hpp
#define knn_hpp

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream> 
#include <sstream>
#include <algorithm>
#include "tools.h"
using namespace std;

class Distance{
public:
	double distance;
	int label;
	Distance(int lab, double dist) :label(lab), distance(dist){
	}
};

class Node{
private:
    vector<double> vec_value;
    int label;
public:
    Node(int label){
        this->label=label;
    }
    void addValue(double value)
    {
        vec_value.push_back(value);
    }
	void setlabel(int label)
	{
		this->label = label;
	}
	int getLabel()
	{
		return label;
	}
	vector<double> &getCoordinate()
	{
		return vec_value;
	}
	int size()
	{
		return vec_value.size();
	}
};

class KNNDriver {
private:
    vector<Node *> vec_node;
	int kNNClassify(vector<double> &vec_value);
	int K;
	int dimension;
	int num_sample;
public:
	KNNDriver(int K)
	{
		this->K = K;
	}
    ~KNNDriver()
	{
        for(auto node:vec_node)
            delete node;
    }
    void createTrainingset(string &file_name);
    void KNNClassifyDriver(string &file_name);
    void parseFile(string &file_name,vector<Node *> &vec_result);
};

#endif /* knn_hpp */
