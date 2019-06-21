#include "stdafx.h"
#include"class.h"
#include<map>
#include<set>
#include<queue>
#include<string.h>
using namespace std;

extern vector<Car> car;
extern map<int, Road> road;
extern Graph G;

//ŽŽœšÍŒ
void Graph::createGraph()
{
	int fileRoadNum = road.size();
	G.arcNum = 0;
	set<int> set_roadId, set_soptId;
	for (map<int, Road>::iterator iter = road.begin(); iter != road.end(); ++iter)
	{
		set_soptId.insert(iter->second.road_from);
		set_soptId.insert(iter->second.road_to);
		if (iter->second.isDuplex)
			(G).arcNum += 2;
		else
			(G).arcNum++;
	}
	(G).vertexNum = set_soptId.size();

	int num = 1;
	for (set<int>::iterator iter = set_soptId.begin(); iter != set_soptId.end(); ++iter)
	{
		G.vertex[num] = *iter;
		num++;
	}

	for (int i = 1; i <= (G).vertexNum; ++i)
	{
		for (int j = 1; j <= (G).vertexNum; ++j)
		{
			(G).arc[i][j].length = MAXLENGTH;
			(G).arc[i][j].numQueue = 0;
			(G).arc[i][j].roadId = 0;
			(G).arc[i][j].speed = 0;
			G.arcWeight[i][j] = MAXWEIGHT;
		}
	}

	for (map<int, Road>::iterator iter = road.begin(); iter != road.end(); ++iter)
	{
		int road_from = iter->second.road_from;
		int road_to = iter->second.road_to;
		(G).arc[road_from][road_to].length = iter->second.road_length;
		(G).arc[road_from][road_to].numQueue = iter->second.road_channel;
		(G).arc[road_from][road_to].roadId = iter->second.road_id;
		(G).arc[road_from][road_to].speed = iter->second.road_speed;
		if (iter->second.isDuplex)
		{
			(G).arc[road_to][road_from].length = iter->second.road_length;
			(G).arc[road_to][road_from].numQueue = iter->second.road_channel;
			(G).arc[road_to][road_from].roadId = iter->second.road_id;
			(G).arc[road_to][road_from].speed = iter->second.road_speed;
		}
	}
}


//***********************************************************************************
//ÈšÖØ³¢ÊÔ
void Graph::setWeight()
{
	for (int i = 1; i <= (G).vertexNum; ++i)
	{
		for (int j = 1; j <= (G).vertexNum; ++j)
		{
			G.arcWeight[i][j] = MAXWEIGHT;
		}
	}
	for (int i = 1; i <= G.vertexNum; ++i)
	{
		for (int j = 1; j <= G.vertexNum; ++j)
		{
			if (G.arc[i][j].length != MAXLENGTH)//ÓÐÂ·
			{

				int roadLength = G.arc[i][j].length;
				int roadID = G.arc[i][j].roadId;
				Road rInFile = road[roadID];
				int startCross = rInFile.road_from;//³µËùÔÚÂ·¶ÎµÄÆðµã
				int toCross = rInFile.road_to;//³µËùÔÚÂ·¶ÎµÄÖÕµã
				Road r = road[roadID];
				int numChannel = r.road_channel;
				vector<vector<Car*>> carInRoad;
				//ÅÐ¶Ï·œÏòºÍžö·œÏòÉÏµÄÈšÖØ,ÕýÏòÊÇÖžÓëÎÄ±ŸÖÐÏàÍ¬µÄ·œÏò
				if (i == startCross && j == toCross)
					carInRoad = r.car_in_road;//car_in_roadŽæµÄÊÇÕýÏòÖÐµÄ³µ
				else if (j == startCross && i == toCross)
				{
					if (!r.isDuplex)//Èç¹ûµ¥ÏòµÀ
						continue;
					else
						carInRoad = r.car_in_road1;//Ë«ÏòµÀ£¬car_in_road1ŽæµÄÊÇ·ŽÏòÖÐµÄ³µ
				}



				//*****************************
				//ÕÒµ±Ç°Â·ÖÐ×îÐ¡ËÙ¶È³µ
				double posOfSlowCar, speedOfSlowCar, sumOfRoadCar = 0;

				speedOfSlowCar = 10;
				posOfSlowCar = 10;

				for (int k = 0; k < numChannel; ++k)
				{
					vector<Car*> carInChannel = carInRoad[k];
					sumOfRoadCar += carInChannel.size();
					if (carInChannel.size() == 0)
					{
						G.arcWeight[i][j] = 0.5;
						break;
					}
					for (int l = 0; l < carInChannel.size(); ++l)
					{
						if (speedOfSlowCar < carInChannel[l]->car_speed)
						{
							speedOfSlowCar = carInChannel[l]->car_speed;
							posOfSlowCar = carInChannel[l]->car_address;
						}
					}

				}
				int rate = sumOfRoadCar / (double)(r.road_length*r.road_channel);
				if (rate>0.7)
					G.arcWeight[i][j] = posOfSlowCar / speedOfSlowCar + 50 * (sumOfRoadCar / (double)(r.road_length*r.road_channel));
				else if (r.road_channel == 1)
					G.arcWeight[i][j] = posOfSlowCar / speedOfSlowCar + 40 * (sumOfRoadCar / (double)(r.road_length*r.road_channel));
				else if (r.road_channel == 2)
					G.arcWeight[i][j] = posOfSlowCar / speedOfSlowCar + 25 * (sumOfRoadCar / (double)(r.road_length*r.road_channel));
				else
					G.arcWeight[i][j] = posOfSlowCar / speedOfSlowCar + 10 * (sumOfRoadCar / (double)(r.road_length*r.road_channel));
				//*****************************

			}
		}
	}
}



double tempWeight;



//×î¶ÌÂ·Ÿ¶žÄœø
//
int SPFA(int fromCurrentRoadCrossID, int fromCrossID, int toCrossID)
{
	//tempWeight = MAXWEIGHT;
	if (fromCrossID == toCrossID)
	{
		tempWeight = G.arcWeight[fromCrossID][fromCurrentRoadCrossID];
		return toCrossID;
	}


	int nextCrossID = -1;
	tempWeight = G.arcWeight[fromCrossID][fromCurrentRoadCrossID];
	G.arcWeight[fromCrossID][fromCurrentRoadCrossID] = MAXWEIGHT;

	//double (*matrix)[MaxSize]= G.arcWeight;

	bool visited[200];     //标记数组
	double dist[200];         //源点到顶点i的最短距离
	int path[200];         //记录最短路的路径
	int enqueue_num[200];  //记录入队次数

	int vertex_num = G.vertexNum;        //¶¥µãÊý
	int edge_num = G.arcNum;          //±ßÊý
	int source = fromCrossID;            //ÔŽµã
	memset(visited, 0, sizeof(visited));
	memset(enqueue_num, 0, sizeof(enqueue_num));
	for (int i = 1; i <= vertex_num; i++)
	{
		dist[i] = MAXWEIGHT;
		path[i] = source;
	}

	queue<int> Q;
	Q.push(source);
	dist[source] = 0;
	visited[source] = 1;
	enqueue_num[source]++;
	while (!Q.empty())
	{
		int u = Q.front();
		Q.pop();
		visited[u] = 0;
		for (int v = 1; v <= vertex_num; v++)
		{
			if (G.arcWeight[u][v] != MAXWEIGHT)  //uÓëvÖ±œÓÁÚœÓ
			{
				if (dist[u] + G.arcWeight[u][v] < dist[v])
				{
					dist[v] = dist[u] + G.arcWeight[u][v];
					path[v] = u;
					if (!visited[v])
					{
						Q.push(v);
						enqueue_num[v]++;
						if (enqueue_num[v] >= vertex_num)
							return false;
						visited[v] = 1;
					}
				}
			}
		}
	}



	int p = toCrossID;
	vector<int> s;


	while (source != p)  //Â·Ÿ¶Ë³ÐòÊÇÄæÏòµÄ£¬ËùÒÔÏÈ±£ŽæµœÕ»
	{
		s.push_back(p);
		//s.push(p);
		p = path[p];
	}
	nextCrossID = s[s.size() - 1];

	return nextCrossID;
}

vector<int> nextRoadInfo(int fromCurrentRoadCrossID, int fromCrossID, int toCrossID)
{
	int nextCrossID = SPFA(fromCurrentRoadCrossID, fromCrossID, toCrossID);

	int roadID = G.arc[fromCrossID][nextCrossID].roadId;
	G.arcWeight[fromCrossID][fromCurrentRoadCrossID] = tempWeight;
	vector<int> roadInfo;
	roadInfo.push_back(fromCrossID);//ÏÂÒ»Â·¶ÎµÄÆðµã
	roadInfo.push_back(nextCrossID);//ÏÂÒ»Â·¶ÎµÄÖÕµã
	roadInfo.push_back(roadID);//ÏÂÒ»Â·¶ÎµÄID
	return roadInfo;
}
//****************************************************************************************************************8
