#include "stdafx.h"
#include"class.h"
#include<vector>
#define MaxTime 80

extern double car_size;
extern long unfinished;
extern vector<Cross> cross;

/***************************************************************************
Car为新加入该道路的车辆
1.更新 car 的信息
2.将car 加入到*car_in_road 中

****************************************************************************/
//double car_size = car.size();
void Road::update_new_car(Car& car) {

	double index = (double)unfinished / car_size;
	vector<vector<Car*>>* _car_vector = NULL;

	if (car.car_start == road_from)
		_car_vector = &car_in_road;
	else if (car.car_start == road_to && isDuplex)
		_car_vector = &car_in_road1;

	int i = 0, j = 0;
	//int flag = 0;  //标志插入是否完成
	int Speed = min(car.car_speed, road_speed);
	//int sum = 0;
	double load_rate = 0.3;

	while (i < road_channel)
	{	
		if (((*_car_vector)[i].size() != 0 && ((*_car_vector)[i][0]->car_address < 2               //预置车辆不可判断路况
			|| (double)(*_car_vector)[i].size() / (double)road_length > load_rate))&& !car.preasentFlag) {
			car.car_start_time += index * MaxTime < 1 ? 1 : index * MaxTime;
			return;
		}
		if ((*_car_vector)[i].size() == 0 || (*_car_vector)[i][0]->car_address >= Speed)
		{
			car.car_address = Speed - 1;                //更新车的状态
			car.cross_id_from = car.car_start;
			car.cross_id_end = road_to == car.car_start ? road_from : road_to;
			car.isStart = 1;
			car.car_current_state = 1;
			car.car_road_id = road_id;
			car.routeID.push_back(car.car_next_roadID);

			(*_car_vector)[i].insert((*_car_vector)[i].begin(), &car);    //将车插入相应车道

			if (car.preasentFlag) {
				car.car_next_roadID = *car.presentRouteID.begin();
				car.presentRouteID.erase(car.presentRouteID.begin());
			}
			break;
		}
		else if ((*_car_vector)[i][0]->car_address < Speed)
		{

			if ((*_car_vector)[i][0]->car_address != 0)
			{
				car.car_address = (*_car_vector)[i][0]->car_address - 1;            //更新车的状态
				car.cross_id_from = car.car_start;
				car.cross_id_end = road_to == car.car_start ? road_from : road_to;;
				car.isStart = 1;
				car.car_current_state = 1;
				car.car_road_id = road_id;
				car.routeID.push_back(car.car_next_roadID);

				(*_car_vector)[i].insert((*_car_vector)[i].begin(), &car);    //将车插入相应车道

				if (car.preasentFlag) {
					car.car_next_roadID = *car.presentRouteID.begin();
					car.presentRouteID.erase(car.presentRouteID.begin());
				}
				break;
			}
			else
			{
				j++;
			}
		}
		i++;
	}
	if (j == road_channel)
	{
		if (car.preasentFlag)          //预置车辆时间+1
		{
			car.car_start_time += 1;
		}
		else
		{
			car.car_start_time += index * MaxTime < 1 ? 1 : index * MaxTime;
		}
	}

}
