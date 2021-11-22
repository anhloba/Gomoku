#pragma once

#include <algorithm>
#include <map> 
#include <time.h>
#include <chrono>
#include <random>

#include "Common.h"

using namespace std;

namespace AI
{
    class Bot
    {
        enum RiskAssessment
        {
            CLOSE_ONE = 1,      // .ox.
            OPEN_ONE = 10,      // .x.
            CLOSE_TWO = 10,     // .oxx. ; .xxo.
            OPEN_TWO = 100,     // .xx. ; .xx.
            CLOSE_THREE = 100,  // .oxxx. ; .xxxo.
            OPEN_THREE = 1000,  // .xxx. ; .xxx.
            CLOSE_FOUR = 1000,
            OPEN_FOUR = 10000,
            FIVE = 100000,
        };

        struct CacheNode
        {
            int score;
            int depth;
            int Flag;
        };

        struct Restrictions
        {
            int minRows;
            int minColumns;
            int maxRows;
            int maxColumns;
        };
    
    public:
        Bot(int rows, int columns);
        Move FindBestMove();
        void SetGameBoard(const vector<vector<int>> &board);
        void SetAiDepth(int depth);
        bool CheckWin(const vector<vector<int>>& board, int x, int y);

    private:
    
        array<array<int, 9>, 4> GetDirections(const vector<vector<int>>& board, int x, int y);
        bool CheckDirections(array<int, 9> const& arr);
    
        Restrictions GetRestrictions(const vector<vector<int>>& board);
        Restrictions RacalculateRestrictions(Restrictions const& rst, int i, int j);

        int CheckCombination(int blocks, int pieces);
        int EvaluateHorizontals(const vector<vector<int>>& board, int pieceType, Restrictions const& rst);
        int EvaluateVerticals(const vector<vector<int>>& board, int pieceType, Restrictions const& rst);
        int EvaluateDiagonals(const vector<vector<int>>& board, int pieceType, Restrictions const& rst);
        int EvaluateOppositeDiagonals(const vector<vector<int>>& board, int pieceType, Restrictions const& rst);
        int EvaluateBoard(const vector<vector<int>>& board, int pieceType, Restrictions const& rst);
        int EvaluateState(const vector<vector<int>>& board, int player, int hash, Restrictions const& rst);
        int EvaluateDirection(array<int, 9> const& direction, int player);
        int EvaluateMove(const vector<vector<int>>& board, int x, int y, int player);
        int GetSequence(int y, int e);
        int EvaluateSequence(int seq);
    
        void InitHashTable();
        int GetHashOfTheBoard(const vector<vector<int>>& board);
        int UpdateHashOfTheBoard(int hash, int player, int row, int col);
    
        bool IsRemoteCell(const vector<vector<int>>& board, int r, int c);
        vector<Move> MoveGeneration(Restrictions const& rst, const vector<vector<int>>& board, int player);
        int StartNegamax(vector<vector<int>> board, int player, int depth, int a, int b, int hash, Restrictions const& rst, int last_i, int last_j);
        Move StartMTDF(const vector<vector<int>>& board, int f, int depth, Restrictions const& rst);

    public:
    
    private:

        vector<vector<vector<int>>> _hashTable;
        vector<vector<int>> _gameBoard;

        map<int, int> _stateCache;
        map<int, CacheNode> _cache;

        Move _bestMove;

        int _aiDepth;
        int _maxDepth;
        int _boardColumns;
        int _boardRows;
    };

} //end namespace AI
