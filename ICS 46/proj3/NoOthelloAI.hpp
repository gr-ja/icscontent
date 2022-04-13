#include "OthelloAI.hpp"
#include <vector>

namespace deyuq
{
    class NoOthelloAI: OthelloAI
    {
    public:
        NoOthelloAI();
        std::vector<std::pair<int,int>> validMove(std::unique_ptr<OthelloGameState> s);
        int search(std::unique_ptr<OthelloGameState> s, int depth, bool myTurn = false);
        int eval(std::unique_ptr<OthelloGameState> s);
        std::pair <int, int> chooseMove(const OthelloGameState& state);
    };
}