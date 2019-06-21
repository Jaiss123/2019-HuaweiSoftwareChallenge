// ¹¬¿¡Ð¡·Ö¶Ó.cpp : ¶šÒå¿ØÖÆÌšÓŠÓÃ³ÌÐòµÄÈë¿Úµã¡£
//

#include "stdafx.h"
#include"class.h"
#include<fstream> 
#include<string.h>
#include <stdio.h>
#include<map>
#include<set>
#include <time.h>
#include<queue>
#include<fstream>
#include<string>


double car_size;
vector<Car> car;
map<int, Road> road;
vector<Cross> cross;
long unfinished;
long timer = 0;
long nterminateds;
map<int, long> crossTurnNums;
Graph G;


int main(int argc, char *argv[])
{
	vector<Car> car0;
	//std::cout << "Begin" << std::endl;

	//if (argc < 5) {
	//	std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
	//	exit(1);
	//}

	/*std::string carPath(argv[1]);
	std::string roadPath(argv[2]);
	std::string crossPath(argv[3]);
	std::string answerPath(argv[4]);*/

	string carPath = "F:\\4. 华为比赛\\SDK\\SDK_C++\\config\\car.txt";
	string roadPath = "F:\\4. 华为比赛\\SDK\\SDK_C++\\config\\road.txt";
	string crossPath = "F:\\4. 华为比赛\\SDK\\SDK_C++\\config\\cross.txt";
	string answerPath = "F:\\4. 华为比赛\\SDK\\SDK_C++\\config\\presetAnswer.txt";

	std::cout << "carPath is " << carPath << std::endl;
	std::cout << "roadPath is " << roadPath << std::endl;
	std::cout << "crossPath is " << crossPath << std::endl;
	std::cout << "answerPath is " << answerPath << std::endl;
	//初始化
	init_class(car0, carPath);
	init_class1(road, roadPath);
	init_class2(cross, crossPath);

	vector<Car*> a(5);
	for (int i = 0; i < cross.size(); i++)
	{

		for (auto iter = road.begin(); iter != road.end(); iter++) {
			if (cross[i].cross_id == iter->second.road_from)
				iter->second.road_from = i + 1;
			if (cross[i].cross_id == iter->second.road_to)
				iter->second.road_to = i + 1;
		}
		for (int j = 0; j < car0.size(); j++)
		{
			if (cross[i].cross_id == car0[j].car_start)
				car0[j].car_start = i + 1;
			if (cross[i].cross_id == car0[j].car_end)
				car0[j].car_end = i + 1;
		}
		cross[i].cross_id = i + 1;
	}

	sort(car0.begin(), car0.end(), compCarID);
	vector<vector<Car>> groups;

	groups = group_cars(car0, GROUPSIZE);
	int groupSize = groups.size();
	int i = 0;
	G.createGraph();
	long cur_baseTime = 0;
	ofstream myanswer(answerPath);
	myanswer << "#(carld,StartTime,RoadId...)" << endl;
	timer = 0;
	while (i < groupSize)
	{
		car = groups[i];
		unfinished = car.size();
		car_size = car.size();
		timer = 0;
		while (++timer)
		{

			cout << timer << ":" << endl;
			nterminateds = 0;
			crossTurnNums.clear();//清除等待转数组
								  /***************************************************************/
								  //cout << "已经上路车的状态：" << endl;
								  /**************************************************************/
			G.setWeight();
			for (int i = 0; i < car.size(); ++i)
			{
				if (car[i].isStart  && car[i].isFinish != 1)
				{

					if (car[i].isTurnwait == 0)
					{
						int nextRoadID = nextRoadInfo(car[i].cross_id_from, car[i].cross_id_end, car[i].car_end)[2];
						if (nextRoadID == 0)
							car[i].car_next_roadID = car[i].car_road_id;
						else
							car[i].car_next_roadID = nextRoadID;

						//*****************************************************
						//动态权重更新3.23  
						int currentCarStartCross = car[i].cross_id_from;
						int currentCarToCross = car[i].cross_id_end;
						Road roadIncrease = road[nextRoadID];
						//	G.arcWeight[currentCarStartCross][currentCarToCross] = G.arcWeight[currentCarStartCross][currentCarToCross] + 20 * (1 / (double)(roadIncrease.road_length*roadIncrease.road_channel));
						G.arcWeight[currentCarStartCross][currentCarToCross] = G.arcWeight[currentCarStartCross][currentCarToCross] + 1;
						//*****************************************************
					}
					//将所有车置为等待状态
					car[i].car_current_state = 0;
					//car[i].isTurnwait = 0;
					nterminateds++;
				}
			}
			//**********************************************************************************************************************	
			int nterminnated_last = 0;
			int clockflag = 0;

			while (nterminateds > 0)//未终止数
			{
				if (nterminnated_last == nterminateds)
				{
					clockflag = 1;
				}

				nterminnated_last = nterminateds;

				if (clockflag)
				{
					for (int i = 0; i < cross.size(); i++) {
						if (crossTurnNums[cross[i].cross_id] > 0)
							if (cross[i].ClockClear(crossTurnNums[cross[i].cross_id]))
								break;
					}
					clockflag = 0;
				}

				for (map<int, Road>::iterator iter = road.begin(); iter != road.end(); iter++)
				{
					driveAllCarJustOnRoadToEndState(iter->second);
				}

				for (int i = 0; i < cross.size(); i++)
				{
					cross[i].car_in_cross_toend(crossTurnNums[cross[i].cross_id]);
				}

			}

			////出库前对路径进行规划
			G.setWeight();
			for (int j = 0; j< car.size(); j++)
			{
				if (car[j].isStart == 0 && car[j].car_start_time == timer)
				{
					vector<int> nextRoad = nextRoadInfo(car[j].car_start, car[j].car_start, car[j].car_end);
					int nextRoadID = nextRoad[2];

					if (nextRoadID == 0)
						car[j].car_next_roadID = car[j].car_road_id;
					else
						car[j].car_next_roadID = nextRoadID;
					//动态权重更新3.23  
					int currentCarStartCross = car[j].car_start;
					int currentCarToCross = nextRoad[1];
					Road roadIncrease = road[nextRoadID];

					//G.arcWeight[currentCarStartCross][currentCarToCross] = G.arcWeight[currentCarStartCross][currentCarToCross] + 20 * (1 / (double)(roadIncrease.road_length*roadIncrease.road_channel));
					G.arcWeight[currentCarStartCross][currentCarToCross] = G.arcWeight[currentCarStartCross][currentCarToCross] + 1;
					//*****************************************************
				}
			}


			////智能车库的车出库
			for (int j = 0; j < car.size(); j++) {
				if (car[j].isStart == 0 && car[j].car_start_time == timer)
				{
					road[car[j].car_next_roadID].update_new_car(car[j]);
				}
			}
			vector<Car> rr;
			for (int i = 0; i < car.size(); i++)
			{
				if (car[i].isStart == 0)
					rr.push_back(car[i]);
			}
			//所有车均调度完成
			if (unfinished <= 0)
				break;
		}
		for (int j = 0; j < car.size(); ++j)
		{
			myanswer << "(";
			myanswer << car[j].car_id << "," << (car[j].car_start_time + cur_baseTime);
			for (auto &p : car[j].routeID)
			{
				myanswer << "," << p;
			}
			myanswer << ")" << endl;
		}
		cur_baseTime += timer;
		i++;

	}
	myanswer.close();
	cout << cur_baseTime << endl;
	//system("pause");
	return 0;
}


