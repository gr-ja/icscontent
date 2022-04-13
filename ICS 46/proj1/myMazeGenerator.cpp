#include "myMazeGenerator.hpp"
#include <ics46/factory/DynamicFactory.hpp>
using namespace std;

ICS46_DYNAMIC_FACTORY_REGISTER(MazeGenerator,myMazeGenerator,"Richard's MazeGenerator(Required)");

void myMazeGenerator::generateMaze(Maze& maze)
{
	maze.addAllWalls();
    visit.resize(maze.getWidth(), vector<bool>(maze.getHeight(), false));
	generatingMaze(0,0,maze);
}

vector<Direction> myMazeGenerator::getDirections(int x,int y,Maze& maze)
{
	vector<Direction> direction;
    if (x-1 >= 0)
    {
        if (visit[x-1][y] == false)
        {
            direction.push_back(Direction::left);
        }
    }

    if (x+1 < maze.getWidth())
    {
        if (visit[x+1][y] == false)
        {
            direction.push_back(Direction::right);
        }
    }

    if (y-1 >= 0)
    {
        if (visit[x][y-1] == false)
        {
            direction.push_back(Direction::up);
        }
    }

    if (y+1 < maze.getHeight())
    {
        if (visit[x][y+1] == false)
        {
            direction.push_back(Direction::down);
        }
    }

    return direction;
}

void myMazeGenerator::check(int x,int y,vector<Direction> direction,Maze& maze)
{
    int stop = direction.size()-1;
    std::uniform_int_distribution<int> distribution{0, stop};
    Direction changeDirection = direction[distribution(engine)];
    int xc = x;
    int yc = y;
    switch (changeDirection)
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
    if (visit[xc][yc] == false && maze.wallExists(x,y,changeDirection))
    {
        maze.removeWall(x,y,changeDirection);
        position = {xc,yc};
    }
	return;
}

void myMazeGenerator::generatingMaze(int x,int y,Maze& maze)
{
    visit[x][y] = true;
    vector<Direction> direction = getDirections(x,y,maze);
    while(direction.size()>0)
    {
        check(x,y,direction,maze);
        generatingMaze(position[0],position[1],maze);
        direction = getDirections(x,y,maze);
    }
    return;
}