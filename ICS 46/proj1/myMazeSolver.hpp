#ifndef MYMAZESOLVER_HPP
#define MYMAZESOLVER_HPP

#include "MazeSolver.hpp"
#include "MazeSolution.hpp"
#include "Direction.hpp"
#include "Maze.hpp"
#include <random>
#include <vector>
using namespace std;

class myMazeSolver: public MazeSolver
{
public:
    vector<Direction> checkDirections(int x,int y,const Maze& maze);
    void solvingMaze(int x,int y,const Maze& maze,MazeSolution& mazeSolution);
    void solveMaze(const Maze& maze, MazeSolution& mazeSolution) override;
    void MovingCell(int x,int y,vector<Direction> direction,const Maze& maze,MazeSolution& mazeSolution);
private:
    vector<vector<bool>> visit;
	vector<int> position = {0,0};
    std::random_device device;
    std::default_random_engine engine{device()};
};


#endif