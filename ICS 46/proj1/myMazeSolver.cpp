#include "myMazeSolver.hpp"
#include <iostream>
#include <ics46/factory/DynamicFactory.hpp>
using namespace std;

ICS46_DYNAMIC_FACTORY_REGISTER(MazeSolver, myMazeSolver, "Richard's MazeSolver(Required)");

void myMazeSolver::solveMaze(const Maze& maze, MazeSolution& mazeSolution)
{
    mazeSolution.restart();
    pair<int,int> start = mazeSolution.getStartingCell();
    visit.resize(mazeSolution.getWidth(), vector<bool>(mazeSolution.getHeight(), false));
    int x = get<0>(start);
    int y = get<1>(start);
    solvingMaze(x,y,maze,mazeSolution);
}

vector<Direction> myMazeSolver::checkDirections(int x,int y,const Maze& maze)
{
    vector<Direction> direction;
    if (x-1 >= 0)
    {
        if (visit[x-1][y] == false && !(maze.wallExists(x,y,Direction::left)))
        {
            direction.push_back(Direction::left);
        }
    }

    if (x+1 < maze.getWidth())
    {
        if (visit[x+1][y] == false && !(maze.wallExists(x,y,Direction::right)))
        {
            direction.push_back(Direction::right);
        }
    }

    if (y-1 >= 0)
    {
        if (visit[x][y-1] == false && !(maze.wallExists(x,y,Direction::up)))
        {
            direction.push_back(Direction::up);
        }
    }

    if (y+1 < maze.getHeight())
    {
        if (visit[x][y+1] == false && !(maze.wallExists(x,y,Direction::down)))
        {
            direction.push_back(Direction::down);
        }
    }

    return direction;
}

void myMazeSolver::MovingCell(int x,int y,vector<Direction> direction,const Maze& maze,MazeSolution& mazeSolution)
{
    int stop = direction.size()-1;
    std::uniform_int_distribution<int> distribution{0, stop};
    Direction pointingDirection = direction[distribution(engine)];
    int xc = x;
    int yc = y;
    switch (pointingDirection)
    {
    case Direction::up:
        yc--;
        break;
    case Direction::down:
        yc++;
        break;
    case Direction::left:
        xc--;
        break;
    case Direction::right:
        xc++;
        break;

    default:
        break;
    }
    if (visit[xc][yc] == false && !maze.wallExists(x,y,pointingDirection))
    {
        position = {xc,yc};
        mazeSolution.extend(pointingDirection);
    }
	return;
}

void myMazeSolver::solvingMaze(int x,int y,const Maze& maze,MazeSolution& mazeSolution)
{
    visit[x][y] = true;
    vector<Direction> direction = checkDirections(x,y,maze);
    while(mazeSolution.getCurrentCell()!=mazeSolution.getEndingCell())
    {
        cout <<x<<" "<<y<<" "<<direction.size()<<endl;
        if (direction.size()>0)
        {
            MovingCell(x,y,direction,maze,mazeSolution);
            x = position[0];
            y = position[1];
        }
        else
        {
            mazeSolution.backUp();
            x = get<0>(mazeSolution.getCurrentCell());
            y = get<1>(mazeSolution.getCurrentCell());
            position = {x,y};
        }
        visit[x][y] = true;
        direction = checkDirections(x,y,maze);
    }
    return;
}
