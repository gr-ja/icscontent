#ifndef MYMAZEGENERATOR_HPP
#define MYMAZEGENERATOR_HPP

#include "MazeGenerator.hpp"
#include "Maze.hpp"
#include "Direction.hpp"
#include <random>
#include <vector>
using namespace std;

class myMazeGenerator: public MazeGenerator
{
public:
	vector<Direction> getDirections(int x,int y,Maze& maze);
	void check(int x,int y,vector<Direction> direction,Maze& maze);
	void generateMaze(Maze& maze) override;
	void generatingMaze(int x,int y,Maze& maze);

private:
    vector<vector<bool>> visit;
	vector<int> position = {0,0};
    std::random_device device;
    std::default_random_engine engine{device()};
};

#endif