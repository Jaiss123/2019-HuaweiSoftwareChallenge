#pragma once

#ifndef _CLASS_H
#define _CLASS_H


#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<map>
#include<string>
#define MaxSize 2000
#define MAXLENGTH 1000
#define MAXWEIGHT 10000
#define SPFAMAXSIZE 2000
#define GROUPSIZE 62000


using namespace std;
class Road;

/************************车辆类******************************/
class Car
{
public:
	friend class Road;
	Car() {}
	Car(int car_id, int car_start, int car_end, int car_speed, int car_star_time, bool preasent = 0, bool priority = 0)
		:car_id(car_id), car_start(car_start), car_end(car_end), car_speed(car_speed), car_start_time(car_star_time), preasentFlag(preasent), priorityFlag(priority) {
		isFinish = 0;
		isStart = 0;
		crossflag = 0;
		isTurnwait = 0;
	}


public:
	int car_id;     //car ID号
	int car_start;  //car 出发地
	int car_end;    // car 目的地
	int car_speed;    //car 速度
	int car_start_time;  //car 开始时间

public:
	int car_address;	//car 在路段中的位置
	int car_road_id;    //car 实时所在路段
	int isFinish;    //是否已经完成调度
	int isStart;	//是否已经开始调度
	vector<int> routeID;  //行车路线  
	int cross_id_from;
	int cross_id_end;
	int crossflag;
	//int car_current_speed;
	bool car_current_state;   //车每时刻的完成状态 0 等待 1终止

public:
	int car_next_roadID;
	int isTurnwait;
	bool preasentFlag;			//预置标志
	bool priorityFlag;				//优先级标志
	vector<int> presentRouteID;  //预置行车路线  
};




/************************路口类******************************/
class Cross
{
public:

	Cross() {}
	Cross(int cross_id, int road_up, int road_right, int road_down, int road_left)
		:cross_id(cross_id), road_up(road_up), road_right(road_right), road_down(road_down), road_left(road_left) {}

	void car_in_cross_toend(long& counter);
	vector<vector<Car*>>& Vector_Of_Road(Road& road, bool flag);
	bool ClockClear(long& counter);

public:
	int cross_id;
	int road_up;
	int road_right;
	int road_down;
	int road_left;


};


/************************道路类******************************/
class Road
{
	friend class Cross;
	friend class Car;

public:
	Road() {}
	Road(int road_id, int road_length, int road_speed, int road_channel, int road_from, int road_to, int isDuplex)
		:road_id(road_id), road_length(road_length), road_speed(road_speed), road_channel(road_channel), road_from(road_from), road_to(road_to), isDuplex(isDuplex) {

		car_in_road.resize(road_channel);
		if (isDuplex)
		{
			car_in_road1.resize(road_channel);
		}

	}

	void update_new_car(Car &car);       //对车库中新加的Car进行更新


public:
	int road_id;
	int road_length;
	int road_speed;
	int road_channel;
	int road_from;
	int road_to;
	int isDuplex;

	vector<vector<Car*>> car_in_road;
	vector<vector<Car*>> car_in_road1;
};

//******************************************************************************************
//í????ó

//áú?ó???ó??·?μ?μà?·D??￠
struct RoadPathInfo
{
	int speed;
	int numQueue;
	int length;
	int roadId;
	//int fromVertexID;
	//int toVertexID;
	//int weight;
};
//í?
class Graph
{
public:
	int vertex[MaxSize];
	RoadPathInfo arc[MaxSize][MaxSize];
	double arcWeight[MaxSize][MaxSize];
	int vertexNum, arcNum;
public:
	void createGraph();
	void  setWeight();
};

int SPFA(int fromCurrentRoadCrossID, int fromCrossID, int toCrossID);
vector<int> nextRoadInfo(int fromCurrentRoadCrossID, int fromCrossID, int toCrossID);
void driveAllCarJustOnRoadToEndState(Road& singleRoad);
void init_class(vector<Car> &a, string file_path);
void init_class2(vector<Cross> &a, string file_path);
void init_class1(map<int, Road> &a, string file_path);
//void outfile();
vector<vector<Car>> group_cars(vector<Car>& cars, int groupSize);
bool compCarID(Car& car1, Car& car2);
bool _CmpByPriorityAndSpeed(Car& car1, Car& car2);
#endif // _CLASS_