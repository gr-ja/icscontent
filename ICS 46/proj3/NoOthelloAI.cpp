#include "NoOthelloAI.hpp"
#include <ics46/factory/DynamicFactory.hpp>

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, deyuq::NoOthelloAI, "Fanscinating AI(Required)");

deyuq::NoOthelloAI::NoOthelloAI()
{
    
}

std::vector<std::pair<int,int>> deyuq::NoOthelloAI::validMove(std::unique_ptr<OthelloGameState> s)
{
    std::vector<std::pair<int,int>> valid;
    for (int x = 0; x < s->board().width(); x++)
    {
        for (int y = 0; y < s->board().height(); y++)
        {
            if(s->isValidMove(x,y))
            {
                std::pair <int,int> point (x,y);
                valid.push_back(point);
            }
        }   
    }
    return valid;
}

int deyuq::NoOthelloAI::eval(std::unique_ptr<OthelloGameState> s)
{
    if (s->isBlackTurn())
    {
        return s->blackScore() - s->whiteScore();
    }
    else
    {
        return s->whiteScore() - s->blackScore();
    }
}

int deyuq::NoOthelloAI::search(std::unique_ptr<OthelloGameState> s, int depth, bool myTurn)
{
    if (depth == 0 || s->isGameOver())
    {
        return eval(s->clone());
    }
    else
    {
        if (myTurn)
        {
            std::vector<std::pair<int,int>> valid = validMove(s->clone());
            int max = 0;
            int tmp = 0;
            for (auto& i : valid)
            {
                std::unique_ptr<OthelloGameState> state = s->clone();
                state->makeMove(i.first, i.second);
                tmp = search(state->clone(), depth - 1, false);
                if (max < tmp)
                {
                    max = tmp;
                }
            }
            return max;
        }
        else
        {
            std::vector<std::pair<int,int>> valid = validMove(s->clone());
            int min = 0;
            int tmp = 0;
            for (auto& i : valid)
            {
                std::unique_ptr<OthelloGameState> state = s->clone();
                state->makeMove(i.first, i.second);
                tmp = search(state->clone(), depth - 1, true);
                if (min > tmp)
                {
                    min = tmp;
                }
            }
            return min;
        }
    }
}

std::pair<int, int> deyuq::NoOthelloAI::chooseMove(const OthelloGameState& state)
{
    std::unique_ptr<OthelloGameState> s = state.clone();
    std::vector<std::pair<int,int>> valid = validMove(s->clone());
    int position = 0;
    int max = 0;
    for (int i = 0; i < valid.size(); i++)
    {
        s->makeMove(valid[i].first, valid[i].second);
        if (search(s->clone(),8,true) > max)
        {
            max = search(s->clone(),8,true);
            position = i;
        }
        s = state.clone();
    }
    return valid[position];
}