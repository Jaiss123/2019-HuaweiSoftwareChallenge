#include "stdafx.h" 
#include"class.h"
#include<map>
#include<set>
#include<queue>
using namespace std;
extern vector<Car> car;
extern map<int, Road> road;
extern long nterminateds;
extern long unfinished;
extern map<int, long> crossTurnNums;

template<typename T>
T min2(const T& num0, const T& num1)
{
	return num0 < num1 ? num0 : num1;
}
//****************************************************************************************************************
vector<vector<Car>> group_cars(vector<Car>& cars, int groupSize)
{
	vector<vector<Car>> res;
	int vec_size = cars.size() / groupSize;
	if (cars.size() % groupSize)
		vec_size++;
	for (int i = 0; i < vec_size; i++)
	{
		vector<Car> vec;
		for (int j = i*groupSize; j < (i + 1)*groupSize && j <cars.size(); j++)
		{
			vec.push_back(cars[j]);
		}
		res.push_back(vec);
	}
	return res;
}
bool compCarID(Car& car1, Car& car2)
{
	return car1.car_id < car2.car_id;
}

bool _CmpByPriorityAndSpeed(Car& car1, Car& car2)
{
	if (car1.priorityFlag != car2.priorityFlag)
		return car1.priorityFlag > car2.priorityFlag;
	else
		return car1.car_speed > car2.car_speed;

}
//****************************************************************************************************************8
void driveAllCarJustOnRoadToEndState(Road& singleRoad)
{
	int forwardChanelSize = singleRoad.car_in_road.size();//?y?ò3μμàêy
	int backwardChanelSize = singleRoad.car_in_road1.size();//?y?ò3μμàêy
	int allChanelSize = forwardChanelSize + backwardChanelSize;//μà?·×ü3μμàêy
															   //±éàú3μμà
	for (int i = 0; i < allChanelSize; i++)
	{
		vector<Car*>* singleChanel;
		if (i < forwardChanelSize)//?ú?y?ò3μμà
			singleChanel = &(singleRoad.car_in_road[i]);
		else//?ú??DD3μμà
			singleChanel = &(singleRoad.car_in_road1[i - forwardChanelSize]);


		int lastIndex = (*singleChanel).size() - 1;
		for (int k = lastIndex; k >= 0; k--)
		{
			Car& oneCar = *((*singleChanel)[k]);

			if (!oneCar.isStart || oneCar.car_current_state)
				continue;
			int curRoadMaxSpeed = singleRoad.road_speed;//μ±?°μà?·?T?ù
			int nextRoadMaxSpeed = road[oneCar.car_next_roadID].road_speed;//??ò?3μμà?T?ù
			int carRealSpeed = min2(oneCar.car_speed, curRoadMaxSpeed);
			int carNextRealSpeed = min2(oneCar.car_speed, nextRoadMaxSpeed);
			int awayCross = singleRoad.road_length - oneCar.car_address - 1;//3μó??·?ú?àà?
			if (k == lastIndex && oneCar.cross_id_end == oneCar.car_end && carRealSpeed > awayCross)
			{
				oneCar.car_current_state = 1;
				nterminateds--;
				oneCar.isFinish = 1;
				unfinished--;
				singleChanel->erase(singleChanel->end() - 1);
				lastIndex--;

				if (oneCar.crossflag == 1)
				{
					crossTurnNums[oneCar.cross_id_end]--;
				}
			}
			else if (!oneCar.crossflag)
			{

				if (carNextRealSpeed > awayCross && (k == lastIndex || (*(*singleChanel)[k + 1]).crossflag))
				{

					oneCar.crossflag = 1;
					if (crossTurnNums.count(oneCar.cross_id_end) > 0)
						crossTurnNums[oneCar.cross_id_end]++;
					else
						crossTurnNums.insert(pair<int, long>(oneCar.cross_id_end, 1));
				}
				else
				{
					if (k == lastIndex)
					{
						if (carRealSpeed > awayCross) {
							oneCar.car_current_state = 1;
							nterminateds--;
							oneCar.car_address += awayCross;
							oneCar.isTurnwait = 1;

						}
						else {

							oneCar.car_address += carRealSpeed;
							oneCar.car_current_state = 1;
							nterminateds--;
						}
					}
					else
					{
						int distanceFromPreCar = (*(*singleChanel)[k + 1]).car_address - (*(*singleChanel)[k]).car_address - 1;//?°oó3μ?à
						distanceFromPreCar = distanceFromPreCar < 0 ? 0 : distanceFromPreCar;//?é??±￡?€2??ü??DD
						if ((*(*singleChanel)[k + 1]).car_current_state)
						{
							oneCar.car_address += min2(carRealSpeed, distanceFromPreCar);
							oneCar.car_current_state = 1;
							nterminateds--;
						}
						else
						{
							if (carRealSpeed <= distanceFromPreCar)
							{
								oneCar.car_address += carRealSpeed;
								oneCar.car_current_state = 1;
								nterminateds--;
							}
						}
					}
				}
			}
		}
	}
}
