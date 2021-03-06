// main.cpp
//
// ICS 46 Spring 2017
// Project #4: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.

#include "Digraph.hpp"
#include "InputReader.hpp"
#include "RoadMap.hpp"
#include "RoadMapReader.hpp"
#include "RoadMapWriter.hpp"
#include "RoadSegment.hpp"
#include "Trip.hpp"
#include "TripMetric.hpp"
#include "TripReader.hpp"
#include <iostream>
#include <iomanip>

void print(double times)
{
	int mins, hours;
	double sec;
	hours = times/3600;
	mins= (times-hours*3600)/60;
	sec = times-hours*3600-mins*60;

	if(hours == 0)
	{
		if(mins == 0)
		{
			std::cout<< "total time: "<< std::fixed << std::setprecision(1) <<  sec  <<" secs";
		}
		else
		{
			std::cout<< "total time: "<< mins << " mins " << std::fixed << std::setprecision(1) <<  sec << " secs";
		}

	}
	else
	{
		std::cout<< "total time: "<< hours <<" hrs " << mins <<" mins " << std::fixed << std::setprecision(1) << sec << " secs";
	}
	
}
 

int main() 
{
	std::function<double(RoadSegment)> Miles = [](const RoadSegment& info)
	{
		return info.miles;
	};

	std::function<double(RoadSegment)> Times = [](const RoadSegment& info)
	{
		return info.miles/info.milesPerHour*3600;
	};

	InputReader in(std::cin);

	RoadMapReader rmr;
	RoadMap rm = rmr.readRoadMap(in);
	if (!rm.isStronglyConnected())
	{
		std::cout << "not strongly connected";
		return 0;
	}
	TripReader tr;
	std::vector<Trip> tp = tr.readTrips(in);

	for (int i = 0; i < tp.size(); ++i)
	{

		std::vector<int> dir;
		dir.push_back(tp[i].endVertex);
		int curr = tp[i].endVertex;
		if (tp[i].metric == TripMetric::Time)
		{
			std::cout << "Shortest driving time from " << std::fixed << std::setprecision(1) << rm.vertexInfo(tp[i].startVertex) << " to " << rm.vertexInfo(tp[i].endVertex) << std::endl;

			std::map<int, int> shortp = rm.findShortestPaths(tp[i].startVertex, Times);
			double sumTime = 0.00;
			while (1)
			{
				dir.push_back(shortp.at(curr));
				curr = shortp.at(curr);
				if (curr == tp[i].startVertex)
					break;
			}
			
			int fromVertex = dir.back();
			std::cout << "  Begin at " << rm.vertexInfo(fromVertex) << std::endl;
			dir.pop_back();

			while (dir.size())
			{
				int toVertex = dir.back();
				double time = Times(rm.edgeInfo(fromVertex, toVertex));
				std::cout << "  Continue to " << std::fixed << std::setprecision(1) << rm.vertexInfo(toVertex) << " (" << rm.edgeInfo(fromVertex, toVertex).miles << " miles @ " << std::fixed << std::setprecision(1) << rm.edgeInfo(fromVertex, toVertex).milesPerHour << "mph = ";
				print(time);
				std::cout << ")" << std::endl;
				sumTime += time;
				fromVertex = toVertex;
				dir.pop_back();
			}
			std::cout << "Total time: ";
			print(sumTime);
			std::cout << std::endl;
		}
		else
		{


			std::cout << "Shortest distance from " << rm.vertexInfo(tp[i].startVertex) << " to " << rm.vertexInfo(tp[i].endVertex) << std::endl;
			std::map<int, int> shortp = rm.findShortestPaths(tp[i].startVertex, Miles);
			double sumMiles = 0.00;
			while (1)
			{
				dir.push_back(shortp.at(curr));
				curr = shortp.at(curr);
				if (curr == tp[i].startVertex)
					break;
			}

			int fromVertex = dir.back();
			std::cout << "  Begin at " << rm.vertexInfo(fromVertex) << std::endl;
			dir.pop_back();

			while (dir.size())
			{   
				int toVertex = dir.back();
				std::cout << "  Continue to " << rm.vertexInfo(toVertex) << " (" << std::fixed << std::setprecision(1) << rm.edgeInfo(fromVertex, toVertex).miles << " miles)" << std::endl;
				sumMiles += rm.edgeInfo(fromVertex, toVertex).miles;
				fromVertex = toVertex;
				dir.pop_back();
			}
			std::cout << "Total distance: " << std::fixed << std::setprecision(1) << sumMiles << " miles" << std::endl;
		}
		std::cout << std::endl; //next trip
	}

	return 0;
}