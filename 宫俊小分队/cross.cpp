#include"stdafx.h"
#include"class.h"
#include<map>
#include<cmath>


extern Graph G;
vector<vector<Car*>> tmp;
extern vector<Car> car;
extern map<int, Road> road;
extern vector<Cross> cross;
extern long nterminateds;
extern long unfinished;
extern long timer;

bool camp(Car* a, Car* b)
{
	return a->car_address > b->car_address;
}
//return 0: ×ªÏòÒòÎªµÈŽýÎÞ·š×ªÏò
//return 1£º×ªÏò³É¹Š×ªÈëÏÂÒ»Â·¿Ú
//return 3£º×ªÏòÒòœ»Íš¶ÂÈûÎÞ·š×ªÈëÏÂÒ»Â·¿Ú£¬µ«ŽË³µ×ŽÌ¬±äÎªÖÕÖ¹×ŽÌ¬
int car_turn_around(Cross cross, Car& car, vector<vector<Car*>>& current_road_car, int channel) {

	Road& NextRoad = road[car.car_next_roadID];
	Road& CurrentRoad = road[car.car_road_id];
	vector<vector<Car*>>& _car_vector = cross.Vector_Of_Road(NextRoad, 1);//È·¶šnextroadµÄµÀÂ·
																		  //vector<vector<Car*>>& _car_vector = cross.Vector_Of_Road(NextRoad, 1);//È·¶šnextroadµÄµÀÂ·

	int DiffCarDistance = CurrentRoad.road_length - car.car_address - 1;

	int i = 0, j = 0;
	int Speed = min(car.car_speed, NextRoad.road_speed);   //ËÙ¶ÈÈ¡ÏÂÒ»µÀÂ·ËÙ¶È£¬³µËÙ£¬µ±Ç°µÀÂ·ËÙ¶È×îÐ¡Öµ
	Speed = min(Speed, NextRoad.road_speed);

	while (i < NextRoad.road_channel)
	{
		if (_car_vector[i].size() == 0)
		{

			car.car_address = Speed - 1 - DiffCarDistance;                //žüÐÂ³µµÄ×ŽÌ¬
			car.cross_id_from = cross.cross_id;
			car.cross_id_end = NextRoad.road_to == cross.cross_id ? NextRoad.road_from : NextRoad.road_to;
			car.car_current_state = 1;
			car.car_road_id = NextRoad.road_id;
			car.crossflag = 0;
			car.routeID.push_back(NextRoad.road_id);
			nterminateds--;
			car.isTurnwait = 0;
			current_road_car[channel].erase(current_road_car[channel].end() - 1);        //œ«³µÔ­ÀŽËùÔÚÂ·µÄÏòÁ¿ÒÆ³ý
			_car_vector[i].insert(_car_vector[i].begin(), &car);    //œ«³µ²åÈëÏàÓŠ³µµÀ

			if (car.preasentFlag) {
				if (car.presentRouteID.size() == 0)
				{
					car.car_next_roadID = car.car_road_id;
				}
				else
				{
					car.car_next_roadID = *car.presentRouteID.begin();
					car.presentRouteID.erase(car.presentRouteID.begin());
				}		
			}

			return 1;
		}

		else if (_car_vector[i][0]->car_address + DiffCarDistance >= Speed)
		{

			car.car_address = Speed - 1 - DiffCarDistance;                //žüÐÂ³µµÄ×ŽÌ¬
			car.cross_id_from = cross.cross_id;
			car.cross_id_end = NextRoad.road_to == cross.cross_id ? NextRoad.road_from : NextRoad.road_to;
			car.car_current_state = 1;
			car.car_road_id = NextRoad.road_id;
			car.crossflag = 0;
			car.routeID.push_back(NextRoad.road_id);
			nterminateds--;
			car.isTurnwait = 0;
			current_road_car[channel].erase(current_road_car[channel].end() - 1);        //œ«³µÔ­ÀŽËùÔÚÂ·µÄÏòÁ¿ÒÆ³ý
			_car_vector[i].insert(_car_vector[i].begin(), &car);    //œ«³µ²åÈëÏàÓŠ³µµÀ

			if (car.preasentFlag) {
				if (car.presentRouteID.size() == 0)
				{
					car.car_next_roadID = car.car_road_id;
				}
				else
				{
					car.car_next_roadID = *car.presentRouteID.begin();
					car.presentRouteID.erase(car.presentRouteID.begin());
				}
			}

			return 1;
		}

		else if (_car_vector[i][0]->car_address + DiffCarDistance< Speed && _car_vector[i][0]->car_current_state == 1)
		{

			if (_car_vector[i][0]->car_address != 0)
			{

				car.car_address = _car_vector[i][0]->car_address - 1;            //žüÐÂ³µµÄ×ŽÌ¬
				car.cross_id_from = cross.cross_id;
				car.cross_id_end = NextRoad.road_to == cross.cross_id ? NextRoad.road_from : NextRoad.road_to;
				car.car_current_state = 1;
				car.car_road_id = NextRoad.road_id;
				car.crossflag = 0;
				car.routeID.push_back(NextRoad.road_id);
				nterminateds--;
				car.isTurnwait = 0;
				current_road_car[channel].erase(current_road_car[channel].end() - 1);
				_car_vector[i].insert(_car_vector[i].begin(), &car);    //œ«³µ²åÈëÏàÓŠ³µµÀ

				if (car.preasentFlag) {
					if (car.presentRouteID.size() == 0)
					{
						car.car_next_roadID = car.car_road_id;
					}
					else
					{
						car.car_next_roadID = *car.presentRouteID.begin();
						car.presentRouteID.erase(car.presentRouteID.begin());
					}
				}

				return 1;
			}
			else j++;
		}
		else return 0;
		i++;
	}

	//µÀÂ·¶ÂÈû ÎÞ·š²åÈë
	if (j == NextRoad.road_channel)
	{
		car.car_address = CurrentRoad.road_length - 1;
		car.car_current_state = 1;
		car.crossflag = 0;
		nterminateds--;

		car.isTurnwait = 1;
		return 2;
	}
}


//µ±Ç°Â·¿Úµ±Ç°Â·µÄ car_in_road Ñ¡Ôñ
vector<vector<Car*>>& Cross::Vector_Of_Road(Road& _road, bool flag) {
	if (_road.isDuplex == 1)         //Ë«ÐÐµÀ
	{
		if (flag == 0)   //³µŽÓµÀÂ·Ê»¡°Ïò¡±Â·¿Ú{
		{
			if (cross_id == _road.road_from)
				return _road.car_in_road1;
			else
				return _road.car_in_road;
		}
		else if (flag == 1)      //³µŽÓµÀÂ·Ê»"Àë"Â·¿Ú
		{
			if (cross_id == _road.road_from)
				return _road.car_in_road;
			else
				return _road.car_in_road1;
		}
	}
	else                            //µ¥ÐÐµÀ
	{
		if (flag == 0)   //³µŽÓµÀÂ·Ê»ÏòÂ·¿Ú
		{
			if (cross_id == _road.road_to)
				return _road.car_in_road;
		}
		else if (flag == 1)
		{
			if (cross_id == _road.road_from)
				return _road.car_in_road;
		}
	}
	return tmp;
}

//¶ÔÂ·¿ÚŽÓÐ¡µœŽóÅÅÐò
vector<int> sort_road_id(Cross cross, vector<int> &a) {

	a[2] = cross.road_down;
	a[3] = cross.road_left;
	a[1] = cross.road_right;
	a[0] = cross.road_up;

	sort(a.begin(), a.end());
	return a;
}

#pragma region MyRegion            //ÅÐ¶ÏÊÇ·ñ¿ÉÒÔ×ªÏò
bool car_turn_flag(Cross cross, Car* car) {

	vector<int> _road;                  //°Ž ÉÏ ÓÒ ÏÂ ×ó Ë³ÐòŽæ·ÅcrossµÄroadid
	_road.push_back(cross.road_up);
	_road.push_back(cross.road_right);
	_road.push_back(cross.road_down);
	_road.push_back(cross.road_left);

	int a, b, i = 0;
	bool flag = true;   //flag1 £ºÊÇ·ñÂú×ã×ªÏò¹æÔò 

	if (car->car_next_roadID == cross.road_up)            //È·¶šÏÂÒ»Â·¿ÚµÄÐòºÅ
		a = 0;
	else if (car->car_next_roadID == cross.road_right)
		a = 1;
	else if (car->car_next_roadID == cross.road_down)
		a = 2;
	else
		a = 3;

	if (car->car_road_id == cross.road_up)                //È·¶šµ±Ç°Â·¿ÚµÄÐòºÅ
		b = 0;
	else if (car->car_road_id == cross.road_right)
		b = 1;
	else if (car->car_road_id == cross.road_down)
		b = 2;
	else
		b = 3;


	if (abs(b - a) == 2)                       //Ö±ÐÐ
	{
		return true;
	}
	else if ((b - a) == 3 || (b - a) == -1)        //×ó×ª
	{
		int c = a + 2 >= 4 ? a - 2 : a + 2;  //Ñ°ÕÒnextÂ·¿ÚµÄ¶ÔÃæ

		if (_road[c] != -1)                  //ÅÐ¶ÏnextÂ·¿ÚµÄ¶ÔÃæÓÐÃ»ÓÐÂ·
		{
			//int m = 0, n = 0, s = 0;
			int flag2 = true;

			if (road[_road[c]].road_to == cross.cross_id || road[_road[c]].isDuplex)
			{
				vector<vector<Car*>> _car_vector;
				_car_vector = cross.Vector_Of_Road(road[_road[c]], 0);


				int max_address_index = -1;
				int max_address = 0;
				//int* point_ = new int[road[_road[c]].road_channel];
				for (int j = 0; j < road[_road[c]].road_channel; j++) {
					//point_[j] = _car_vector[j].size() - 1;

					if (_car_vector[j].size() != 0 && _car_vector[j][_car_vector[j].size() - 1]->car_address > max_address) {
						max_address = _car_vector[j][_car_vector[j].size() - 1]->car_address;
						max_address_index = j;
					}

				}
				if (max_address_index != -1 && _car_vector[max_address_index][_car_vector[max_address_index].size() - 1]->crossflag
					&& (_car_vector[max_address_index][_car_vector[max_address_index].size() - 1]->car_next_roadID == _road[a]
						|| _car_vector[max_address_index][_car_vector[max_address_index].size() - 1]->car_next_roadID == _car_vector[max_address_index][_car_vector[max_address_index].size() - 1]->car_road_id))
				{
					return false;
				}
			}
			else
				return true;
		}
	}
	else                                    //ÓÒ×ªµÄÊ±ºò
	{
		int c = a + 2 >= 4 ? a - 2 : a + 2;  //Ñ°ÕÒnextÂ·¿ÚµÄ¶ÔÃæŽæ²»ŽæÔÚÖ±ÐÐ
		int m = 0, n = 0;
		int flag2 = true;
		if (_road[c] != -1)         //ÅÐ¶ÏÂ·¿ÚŽæ²»ŽæÔÚ
		{

			if (road[_road[c]].road_to == cross.cross_id || road[_road[c]].isDuplex)
			{
				vector<vector<Car*>> _car_vector;
				_car_vector = cross.Vector_Of_Road(road[_road[c]], 0);


				int max_address_index = -1;
				int max_address = 0;
				for (int j = 0; j < road[_road[c]].road_channel; j++) {
					//point_[j] = _car_vector[j].size() - 1;

					if (_car_vector[j].size() != 0 && _car_vector[j][_car_vector[j].size() - 1]->car_address > max_address) {
						max_address = _car_vector[j][_car_vector[j].size() - 1]->car_address;
						max_address_index = j;
					}

				}
				if (max_address_index != -1 && _car_vector[max_address_index][_car_vector[max_address_index].size() - 1]->crossflag
					&& (_car_vector[max_address_index][_car_vector[max_address_index].size() - 1]->car_next_roadID == _road[a]
						|| _car_vector[max_address_index][_car_vector[max_address_index].size() - 1]->car_next_roadID == _car_vector[max_address_index][_car_vector[max_address_index].size() - 1]->car_road_id))
				{
					return false;
				}
			}
		}



		c = a > 0 ? a - 1 : 3;
		if (_road[c] != -1)         //Ñ°ÕÒnextÂ·¿ÚŽæµÄÓÒ±ß²»ŽæÔÚ×ó×ª
		{

			int flag2 = true;
			if (road[_road[c]].road_to == cross.cross_id || road[_road[c]].isDuplex)
			{
				vector<vector<Car*>> _car_vector;
				_car_vector = cross.Vector_Of_Road(road[_road[c]], 0);

				int max_address_index = -1;
				int max_address = 0;
				for (int j = 0; j < road[_road[c]].road_channel; j++) {
					//point_[j] = _car_vector[j].size() - 1;

					if (_car_vector[j].size() != 0 && _car_vector[j][_car_vector[j].size() - 1]->car_address > max_address) {
						max_address = _car_vector[j][_car_vector[j].size() - 1]->car_address;
						max_address_index = j;
					}

				}
				if (max_address_index != -1 && _car_vector[max_address_index][_car_vector[max_address_index].size() - 1]->crossflag
					&& _car_vector[max_address_index][_car_vector[max_address_index].size() - 1]->car_next_roadID == _road[a])
				{
					return false;
				}
			}
			else
				return true;
		}
	}
	return flag;
}
#pragma endregion

/***************************************************************************
CarÎªÐÂŒÓÈëžÃµÀÂ·µÄ³µÁŸ
1.žüÐÂ car µÄÐÅÏ¢
2.œ«car ŒÓÈëµœ*car_in_road ÖÐ

****************************************************************************/
void Cross::car_in_cross_toend(long& counter)
{
	int Roadnum = 0;               //道路标号从0开始
	vector<int> road_id_sort(4);
	road_id_sort = sort_road_id(*this, road_id_sort);		//按照道路id升序排列

	while (Roadnum < 4)                      //遍历每一条路
	{
		int c = 0;
		if (road_id_sort[Roadnum] == -1)
		{
			Roadnum++;
			continue;
		}
		//µ±Â·¿ÚÎªË«Ïò »òÕß Â·¿ÚÎªµ¥Ïò²¢ÇÒÖžÏòÂ·¿Ú¿ÉÒÔœøÐÐÐÐÊ»
		if (road[road_id_sort[Roadnum]].isDuplex || road[road_id_sort[Roadnum]].road_to == cross_id)
		{
			int flag = 1;
			
			while (1)
			{			
				//对所有道路遍历每个道路每条路的头车进行优先级的判断,找出当前优先级最优先车的路  4.22
				for (int i = 0; i < 4; i++)
				{					
					if (road_id_sort[i] == -1)
						continue;
					vector<vector<Car*>> _car_vector = Vector_Of_Road(road[road_id_sort[Roadnum]], 0);         //取出当前道路的车辆集合
					for (int j = 0; j < _car_vector.size(); j++)
					{
						if (_car_vector[j][_car_vector[j].size() - 1]->priorityFlag == 1)
						{
							Roadnum = i;
							goto L1;
						}
					}
				}

				L1:vector<vector<Car*>>& _car_vector = Vector_Of_Road(road[road_id_sort[Roadnum]], 0);         //取出当前道路的车辆集合

				int max_address_index = -1;
				int max_address = 0;
				vector<Car*> priorityCar;
				for (int j = 0; j < _car_vector.size(); j++) {
					if (_car_vector[j].size() != 0 && _car_vector[j][_car_vector[j].size() - 1]->crossflag
						&& _car_vector[j][_car_vector[j].size() - 1]->car_address > max_address)            
					{
						//当该路口有优先级车辆时直接执行 优先级先行
						if (_car_vector[j][_car_vector[j].size() - 1]->priorityFlag)
						{
							priorityCar.push_back(_car_vector[j][_car_vector[j].size() - 1]);
						}
						max_address = _car_vector[j][_car_vector[j].size() - 1]->car_address;
						max_address_index = j;												
					}
				}
				if (max_address_index == -1)
				{
					flag = max_address_index;     //Èôflag == 0 ËµÃ÷Ã¿žö³µµÀµÄµÚÒ»žö¶Œ²»×ªÍä 
					break;
				}

				sort(priorityCar.begin(), priorityCar.end(), camp);    //对优先级车辆按照位置大小排序

				Car* Onecar;
				if (priorityCar[0]!=NULL)
					Onecar = priorityCar[0];
				else
					Onecar = _car_vector[max_address_index][_car_vector[max_address_index].size() - 1];
				
				if (Onecar->car_end == cross_id)
				{
					Onecar->isFinish = 1;
					Onecar->crossflag = 0;
					Onecar->car_current_state = 1;
					nterminateds--;
					unfinished--;
					counter--;
					_car_vector[max_address_index].erase(_car_vector[max_address_index].end() - 1);        //œ«³µÔ­ÀŽËùÔÚÂ·µÄÏòÁ¿ÒÆ³ý
				}
				else
				{
					if ( Onecar->priorityFlag || car_turn_flag(*this, Onecar) )            //满足转向或者具有优先权
					{
						int turnflag = car_turn_around(*this, *(Onecar), _car_vector, max_address_index);
						if (turnflag == 1)  
							counter--;
						else if (turnflag == 2)
						{
							counter--;
							int address = Onecar->car_address;
							for (int n = _car_vector[max_address_index].size() - 2; n >= 0 && _car_vector[max_address_index][n]->crossflag; n--)
							{
								_car_vector[max_address_index][n]->car_address = --address;
								_car_vector[max_address_index][n]->car_current_state = 1;
								_car_vector[max_address_index][n]->crossflag = 0;
								nterminateds--;
								counter--;
							}
						}
						else if (turnflag == 0) return;
					}
					else  break;
				}
			}
			Roadnum++;
		}
		else {
			Roadnum++;
			continue;
		}
	}
}



/**************************************************************************/
bool Cross::ClockClear(long& counter) {

	vector<int> road_id_sort(4);
	road_id_sort = sort_road_id(*this, road_id_sort);

	for (int i = 0; i < 4; i++)
	{
		int road_id = 0;
		int channel = -1;

		if (road_id_sort[i] == -1)
			continue;

		vector<vector<Car*>>& FristRoad = Vector_Of_Road(road[road_id_sort[i]], 0);  //取路口最小车道
		int max_address_index = -1;
		int max_address = 0;

		for (int j = 0; j < FristRoad.size(); j++) {                                //取最小车道的第一辆车																					//point_[j] = _car_vector[j].size() - 1;
			if (FristRoad[j].size() != 0 && FristRoad[j][FristRoad[j].size() - 1]->crossflag
				&& FristRoad[j][FristRoad[j].size() - 1]->car_address > max_address)            //当前的车道容纳量不为0且当前车道第一个车的adress最大且需要转
			{
				max_address = FristRoad[j][FristRoad[j].size() - 1]->car_address;
				max_address_index = j;
			}
		}
		if (max_address_index == -1)
			continue;

		if (car_turn_flag(*this, FristRoad[max_address_index][FristRoad[max_address_index].size() - 1]))
		{
			if (!FristRoad[max_address_index][FristRoad[max_address_index].size() - 1]->isTurnwait)
			{
				channel = max_address_index;
				road_id = i;
			}
			else
				continue;
		}
		else
			continue;


		Road& CurrentRoad = road[road_id_sort[road_id]];
		Car* &Onecar = FristRoad[channel][FristRoad[channel].size() - 1];

		vector<int> allRoadid;
		allRoadid.push_back(road[road_up].road_id);
		allRoadid.push_back(road[road_right].road_id);
		allRoadid.push_back(road[road_down].road_id);
		allRoadid.push_back(road[road_left].road_id);

		int m = i, n = 0;															//判断出路口和进路口的方向
		if (Onecar->car_road_id == road_up)				m = 0;
		else if (Onecar->car_road_id == road_right)		m = 1;
		else if (Onecar->car_road_id == road_down)		m = 2;
		else if (Onecar->car_road_id == road_left)		m = 3;

		if (Onecar->car_next_roadID == road_up)         n = 0;
		else if (Onecar->car_next_roadID == road_right) n = 1;
		else if (Onecar->car_next_roadID == road_down)  n = 2;
		else if (Onecar->car_next_roadID == road_left)	n = 3;

		//确定右转路口的id下标
		int c = 0;
		if (m == 0) c = 3;
		else		c = m - 1;

		Road& NextRoad = road[allRoadid[c]];
		if (NextRoad.road_id != -1 && (NextRoad.road_id != (Onecar->car_road_id) && (NextRoad.road_id != Onecar->car_next_roadID))
			&& (NextRoad.isDuplex == 1 || NextRoad.road_from == cross_id))
		{
			Onecar->crossflag = 0;
			counter--;
			Onecar->car_next_roadID = allRoadid[c];
			G.setWeight();
			for (int i = 0; i < FristRoad.size(); i++)
			{
				for (int j = 0; j < FristRoad[i].size(); j++) {
					if (FristRoad[i][j]->crossflag != 0)
					{
						if (j == FristRoad[i].size() - 1)
						{
							FristRoad[i][j]->crossflag = 0;
							counter--;
						}
						else
						{
							int nextRoadID = nextRoadInfo(FristRoad[i][j]->cross_id_from, FristRoad[i][j]->cross_id_end, FristRoad[i][j]->car_end)[2];
							FristRoad[i][j]->car_next_roadID = nextRoadID == 0 ? FristRoad[i][j]->car_road_id : nextRoadID;
							FristRoad[i][j]->crossflag = 0;
							counter--;
						}

					}
				}
			}
			return 1;
		}
	}
	return 0;
}
