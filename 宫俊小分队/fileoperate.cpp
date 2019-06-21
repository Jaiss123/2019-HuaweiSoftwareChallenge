#include "stdafx.h"
#include"class.h"
#include<fstream>
#include<string>
#include <stdio.h>
#include<map>
#include<string.h>
using namespace std;
extern vector<Car> car;

void trim(string &s)
{
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == ' ' || s[i] == ')')
			s.erase(i, 1);
		if (s[i] == '(')
			s[i] = ',';
	}
}

vector<int> Load_data(string &file_path) {

	string text;
	string text1;
	vector<int> data;
	int a, b;

	ifstream inf;  inf.open(file_path);
	while (getline(inf, text1))
	{
		text += text1;
	}
	b = text.find(')');
	text = text.substr(b + 2, text.size());
	trim(text);

	char *buf;
	char *s_input = (char *)text.c_str();
	const char * split = ",";
	// ÒÔ¶ººÅÎª·Öžô·û²ð·Ö×Ö·ûŽ®

	char *p = strtok_s(s_input, split,&buf);
	while (p != NULL)
	{
		// char * -> int
		sscanf_s(p, "%d", &a);
		data.push_back(a);
		p = strtok_s(NULL, split, &buf);
	}

	return data;
}

void init_class(vector<Car> &a, string file_path) {
	vector<int> data = Load_data(file_path);

	for (int i = 0; i < data.size(); i += 5)
	{
		Car _car(data[i], data[i + 1], data[i + 2], data[i + 3], data[i + 4]);
		a.push_back(_car);
	}
}

void init_class2(vector<Cross> &a, string file_path) {
	vector<int> data = Load_data(file_path);

	for (int i = 0; i < data.size(); i += 5)
	{
		Cross cross(data[i], data[i + 1], data[i + 2], data[i + 3], data[i + 4]);
		a.push_back(cross);
	}
}

void init_class1(map<int, Road> &a, string file_path) {
	vector<int> data = Load_data(file_path);

	for (int i = 0; i < data.size(); i += 7)
	{
		Road road(data[i], data[i + 1], data[i + 2], data[i + 3], data[i + 4], data[i + 5], data[i + 6]);

		a.insert(pair<int, Road>(data[i], road));
	}
}
