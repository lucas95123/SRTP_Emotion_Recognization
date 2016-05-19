#include "tools.h"

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
		float tmp;
		ss >> tmp;
		label = (int)tmp;
		bool flag = false;
		while (!ss.eof())
		{
			int index;
			float tmp;
			ss >> tmp;
			index = (int)tmp;
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