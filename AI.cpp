#include "AI.h"

namespace AI
{

    const int WIN = numeric_limits<int>::min() + 1;
    const int WINING_STRATEGY = 1999990;
    
    Bot::Bot(int rows, int columns) : _boardRows(rows), _boardColumns(columns)
    {
        InitHashTable();
    }

    void Bot::SetGameBoard(const vector<vector<int>> &board)
    {
        _gameBoard = board;
    }

    void Bot::SetAiDepth(int depth)
    {
        _aiDepth = depth;
    }

#pragma region Direction

    array<array<int, 9>, 4> Bot::GetDirections(const vector<vector<int>>& board, int x, int y)
    {
        array<int, 9> vertical;
        array<int, 9> bSlash;
        array<int, 9> horizontal;
        array<int, 9> slash;
        int vi = 0;
        int bSi = 0;
        int hi = 0;
        int si = 0;
    
        for (int i = -4; i < 5; i++)
        {
            if (x + i >= 0 && x + i <= _boardRows - 1)
            {
                vertical[vi] = board[x + i][y];
                vi++;
                if (y + i >= 0 && y + i <= _boardColumns - 1)
                {
                    bSlash[bSi] = board[x + i][y + i];
                    bSi++;
                }
            }
            if (y + i >= 0 && y + i <= _boardColumns - 1)
            {
                horizontal[hi] = board[x][y + i];
                hi++;
                if (x - i >= 0 && x - i <= _boardRows - 1)
                {
                    slash[si] = board[x - i][y + i];
                    si++;
                }
            }
    
        }
    
        // помечаем выход линии из 9ти ячеек за границы поля
        if (vi != 9)
        {
            vertical[vi] = 2;
        }
        if (bSi != 9)
        {
            bSlash[bSi] = 2;
        }
        if (hi != 9)
        {
            horizontal[hi] = 2;
        }
        if (si != 9)
        {
            slash[si] = 2;
        }
    
        array<array<int, 9>, 4> dir = { vertical, bSlash, horizontal, slash };
        return dir;
    }
    
    bool Bot::CheckDirections(array<int, 9> const& dir)
    {
        for (int i = 0; i < 5; i++)
        {
            if (dir[i] != 0)
            {
                bool isNotValidDirection = dir[i] == 2 || dir[i + 1] == 2 || dir[i + 2] == 2 || dir[i + 3] == 2 || dir[i + 4] == 2;
                if (isNotValidDirection)
                {
                    return false;
                }

                bool isWin = dir[i] == dir[i + 1] && dir[i] == dir[i + 2] && dir[i] == dir[i + 3] && dir[i] == dir[i + 4];
                if (isWin)
                {
                    return true;
                }
                //return isValidDirection && isWin
            }
        }
        return false;
    }

#pragma endregion
    
#pragma region Restrictions

   Bot::Restrictions Bot::GetRestrictions(const vector<vector<int>>& board)
   {
        int minRows = numeric_limits<int>::max() - 1;
        int minColumns = numeric_limits<int>::max() - 1;
        int maxRows = numeric_limits<int>::min() + 1;
        int maxColumns = numeric_limits<int>::min() + 1;
        for (int i = 0; i < _boardRows; i++)
        {
            for (int j = 0; j < _boardColumns; j++)
            {
                if (board[i][j] != 0)
                {
                    minRows = min(minRows, i);
                    minColumns = min(minColumns, j);
                    maxRows = max(maxRows, i);
                    maxColumns = max(maxColumns, j);
                }
            }
        }
        if (minRows - 2 < 0)
        {
            minRows = 2;
        }
        if (maxRows + 2 >= _boardRows)
        {
            maxRows = _boardRows - 3;
        }

        if (minColumns - 2 < 0)
        {
            minColumns = 2;
        }
        if (maxColumns + 2 >= _boardColumns)
        {
            maxColumns = _boardColumns - 3;
        }
        Restrictions rst = { minRows, minColumns, maxRows, maxColumns };
        return rst;
    }
    
    Bot::Restrictions Bot::RacalculateRestrictions(Restrictions const& rst, int i, int j)
    {
        int minRows = rst.minRows;
        int minColumns = rst.minColumns;
        int maxRows = rst.maxRows;
        int maxColumns = rst.maxColumns;
        if (i < minRows)
        {
            minRows = i;
        }
        else if (i > maxRows)
        {
            maxRows = i;
        }

        if (minRows - 2 < 0)
        {
            minRows = 2;
        }
        if (maxRows + 2 >= _boardRows)
        {
            maxRows = _boardRows - 3;
        }

        if (j < minColumns)
        {
            minColumns = j;
        }
        else if (j > maxColumns)
        {
            maxColumns = j;
        }

        if (minColumns - 2 < 0)
        {
            minColumns = 2;
        }
        if (maxColumns + 2 >= _boardColumns)
        {
            maxColumns = _boardColumns - 3;
        }
        Restrictions newRst = { minRows, minColumns, maxRows, maxColumns };
        return newRst;
    }

#pragma endregion
    
#pragma region Evaluations

    int Bot::CheckCombination(int blocks, int pieces)
    {
        if (blocks == 0)
        {
            switch (pieces)
            {
            case 1:
                return OPEN_ONE;
            case 2:
                return OPEN_TWO;
            case 3:
                return OPEN_THREE;
            case 4:
                return OPEN_FOUR;
            default:
                return FIVE;
            }
        }
        else if (blocks == 1)
        {
            switch (pieces)
            {
            case 1:
                return CLOSE_ONE;
            case 2:
                return CLOSE_TWO;
            case 3:
                return CLOSE_THREE;
            case 4:
                return CLOSE_FOUR;
            default:
                return FIVE;
            }
        }
        else
        {
            if (pieces >= 5)
            {
                return FIVE;
            }
            else
            {
                return 0;
            }
        }
    }
    
    int Bot::EvaluateHorizontals(const vector<vector<int>>& board, int pieceType, Restrictions const& rst)
    {
        int score = 0;
        for (int row = rst.minRows; row < rst.maxRows + 1; row++)
        {
            for (int column = rst.minColumns; column < rst.maxColumns + 1; column++)
            {
                if (board[row][column] == pieceType)
                {
                    int block = 0;
                    int piece = 1;
                    // left
                    if (column == 0 || board[row][column - 1] != 0)
                    {
                        block++;
                    }

                    for (column++; column < _boardColumns && board[row][column] == pieceType; column++)
                    {
                        piece++;
                    }

                    // right
                    if (column == _boardColumns || board[row][column] != 0)
                    {
                        block++;
                    }
                    score += CheckCombination(block, piece);
                }
            }
        }
        return score;
    }

    int Bot::EvaluateVerticals(const vector<vector<int>>& board, int pieceType, Restrictions const& rst)
    {
        int score = 0;
        for (int column = rst.minColumns; column < rst.maxColumns + 1; column++)
        {
            for (int row = rst.minRows; row < rst.maxRows + 1; row++)
            {
                if (board[row][column] == pieceType)
                {
                    int block = 0;
                    int piece = 1;
                    // up
                    if (row == 0 || board[row - 1][column] != 0)
                    {
                        block++;
                    }

                    for (row++; row < _boardRows && board[row][column] == pieceType; row++)
                    {
                        piece++;
                    }

                    // down
                    if (row == _boardRows || board[row][column] != 0)
                    {
                        block++;
                    }
                    score += CheckCombination(block, piece);
                }
            }
        }
        return score;
    }

    int Bot::EvaluateDiagonals(const vector<vector<int>>& board, int pieceType, Restrictions const& rst)
    {
        int score = 0;
        int coef = rst.maxColumns - rst.minColumns + rst.maxRows;
        for (int n = rst.minRows; n < coef; n++)
        {
            int r = n;
            int c = rst.minColumns;
            while (r >= rst.minRows && c <= rst.maxColumns)
            {
                if (r <= rst.maxRows)
                {
                    if (board[r][c] == pieceType)
                    {
                        int block = 0;
                        int piece = 1;
                        // left-down
                        if (c == 0 || r == _boardRows - 1 || board[r + 1][c - 1] != 0)
                        {
                            block++;
                        }

                        r--;
                        c++;

                        for (; r >= 0 && c < _boardColumns && board[r][c] == pieceType; r--)
                        {
                            piece++;
                            c++;
                        }

                        // right-up
                        if (r < 0 || c == _boardColumns || board[r][c] != 0)
                        {
                            block++;
                        }
                        score += CheckCombination(block, piece);
                    }
                }
                r--;
                c++;
            }
        }
        return score;
    }

    int Bot::EvaluateOppositeDiagonals(const vector<vector<int>>& board, int pieceType, Restrictions const& rst)
    {
        int score = 0;
        for (int n = rst.minRows - (rst.maxColumns - rst.minColumns); n <= rst.maxRows; n++)
        {
            int r = n;
            int c = rst.minColumns;
            while (r <= rst.maxRows && c <= rst.maxColumns)
            {
                if (r >= rst.minRows && r <= rst.maxRows)
                {
                    if (board[r][c] == pieceType)
                    {
                        int  block = 0;
                        int piece = 1;
                        // left-up
                        if (c == 0 || r == 0 || board[r - 1][c - 1] != 0)
                        {
                            block++;
                        }

                        r++;
                        c++;

                        for (; r < _boardRows && c < _boardColumns && board[r][c] == pieceType; r++)
                        {
                            piece++;
                            c++;
                        }

                        // right-down
                        if (r == _boardRows || c == _boardColumns || board[r][c] != 0)
                        {
                            block++;
                        }
                        score += CheckCombination(block, piece);
                    }
                }
                r++;
                c++;
            }
        }
        return score;
    }

    int Bot::EvaluateBoard(const vector<vector<int>>& board, int pieceType, Restrictions const& rst)
    {
        //переменная для красоты
        int score = EvaluateHorizontals(board, pieceType, rst);
        score += EvaluateVerticals(board, pieceType, rst);
        score += EvaluateDiagonals(board, pieceType, rst);
        score += EvaluateOppositeDiagonals(board, pieceType, rst);
        return score;
    }
    
    int Bot::GetSequence(int y, int e)
    {
        if (y + e == 0)
        {
            return 0;
        }
        else if (y != 0 && e == 0)
        {
            return y;
        }
        else if (y == 0 && e != 0)
        {
            return -e;
        }
        else
        {
            return 33;
        }
    }
    
    int Bot::EvaluateSequence(int sequence)
    {
        switch (sequence)
        {
        case 0:
            return 7;
        case 1:
            return 35;
        case 2:
            return 800;
        case 3:
            return 15000;
        case 4:
            return 800000;
        case -1:
            return 15;
        case -2:
            return 400;
        case -3:
            return 1800;
        case -4:
            return 100000;
        case 33:
            return 0;
        default:
            cout << "Error of evaluating" << endl;
            return 0;
        }
    }
    
    int Bot::EvaluateState(const vector<vector<int>>& board, int player, int hash, Restrictions const& rst)
    {
        int black_score = EvaluateBoard(board, -1, rst);
        int white_score = EvaluateBoard(board, 1, rst);
        int score = 0;
        if (player == -1)
        {
            score = black_score - white_score;
        }
        else
        {
            score = white_score - black_score;
        }
        _stateCache[hash] = score;
        return score;
    }
    
    
    int Bot::EvaluateDirection(array<int, 9> const& direction, int player)
    {
        int score = 0;
        int arr_size = direction.size();
        for (int i = 0; i < arr_size - 4; i++)
        {
            int you = 0;
            int enemy = 0;
            if (direction[i] == 2)
            {
                return score;
            }
            for (int j = 0; j <= 4; j++)
            {
                if (direction[i + j] == 2)
                {
                    return score;
                }
                if (direction[i + j] == player)
                {
                    you++;
                }
                else if (direction[i + j] == -player)
                {
                    enemy++;
                }
            }
            score += EvaluateSequence(GetSequence(you, enemy));
            if ((score >= 800000))
            {
                return WIN;
            }
        }
        return score;
    }
    
    int Bot::EvaluateMove(const vector<vector<int>>& board, int x, int y, int player)
    {
        int score = 0;
        array<array<int, 9>, 4> Directions = GetDirections(board, x, y);
        int temp_score;
        for (int i = 0; i < 4; i++)
        {
            temp_score = EvaluateDirection(Directions[i], player);
            if (temp_score == WIN)
            {
                return WIN;
            }
            else
            {
                score += temp_score;
            }
        }
        return score;
    }
    
#pragma endregion
        
 // https://en.wikipedia.org/wiki/Zobrist_hashing
#pragma region ZobristHashing
    
    mt19937 mt_rand(time(0));

    void Bot::InitHashTable()
    {
        vector<vector<vector<int>>> hashT(_boardRows, vector<vector<int>>(_boardColumns , vector<int>(2)));
        for (int i = 0; i < _boardRows; i++)
        {
            for (int j = 0; j < _boardColumns; j++)
            {
                hashT[i][j][0] = mt_rand();
                hashT[i][j][1] = mt_rand();
            }
        }
        _hashTable = hashT;
    }
    
    int Bot::GetHashOfTheBoard(const vector<vector<int>>& board)
    {
        int h = 0;
        int p;
        for (int i = 0; i < _boardRows; i++)
        {
            for (int j = 0; j < _boardColumns; j++)
            {
                int BoardValue = board[i][j];
                if (BoardValue != 0)
                {
                    if (BoardValue == -1)
                    {
                        p = 0;
                    }
                    else
                    {
                        p = 1;
                    }
                    h = h ^ _hashTable[i][j][p];
                }
            }
        }
        return h;
    }
    
    int  Bot::UpdateHashOfTheBoard(int hash, int player, int row, int col)
    {
        if (player == -1)
        {
            player = 0;
        }
        else
        {
            player = 1;
        }
        hash = hash ^ _hashTable[row][col][player];
        return hash;
    }
    
#pragma endregion
        
#pragma region MDT(f)_Negamax

    bool Bot::CheckWin(const vector<vector<int>>& board, int x, int y)
    {
        array<array<int, 9>, 4> Directions = GetDirections(board, x, y);
        for (int i = 0; i < 4; i++)
        {
            if (CheckDirections(Directions[i]))
            {
                return true;
            }
        }
        return false;
    }

    bool Bot::IsRemoteCell(const vector<vector<int>>& board, int r, int c)
    {
        for (int i = r - 2; i <= r + 2; i++)
        {
            if (i < 0 || i >= _boardRows)
            {
                continue;
            }
            for (int j = c - 2; j <= c + 2; j++)
            {
                if (j < 0 || j >= _boardColumns)
                {
                    continue;
                }
                if (board[i][j] != 0)
                {
                    return false;
                }
            }
        }
        return true;
    }
    
    bool CompareMove(Move const& move1, Move const& move2)
    {
        return move1.score > move2.score;
    }
    
    vector<Move> Bot::MoveGeneration(Restrictions const& rst, const vector<vector<int>>& board, int player)
    {
        vector<Move> evaluationOfMoves;
        for (int i = rst.minRows - 2; i <= rst.maxRows + 2; i++)
        {
            for (int j = rst.minColumns - 2; j <= rst.maxColumns + 2; j++)
            {
                if (board[i][j] == 0 && !IsRemoteCell(board, i, j))
                {
                    Move move;
                    move.i = i;
                    move.j = j;
                    move.score = EvaluateMove(board, i, j, player);
                    evaluationOfMoves.push_back(move);
                    if (move.score == WIN)
                    {
                        if (evaluationOfMoves.size() > 1)
                        {
                            evaluationOfMoves.erase(evaluationOfMoves.begin(), (evaluationOfMoves.end() - 2));
                        }
                        return evaluationOfMoves;
                    }
                }
            }
        }
        sort(evaluationOfMoves.begin(), evaluationOfMoves.end(), CompareMove);
        return evaluationOfMoves;
    }
    
    //https://en.wikipedia.org/wiki/Negamax
    int Bot::StartNegamax(vector<vector<int>> board, int player, int depth, int a, int b, int hash, Restrictions const& rst, int last_i, int last_j)
    {
        const int alphaOrig = a;
        // проверяем сохраненный кеш лучших ходов, чтобы не повторять вычисления
        // если мы зашли в одну и ту же позицию но с разныйх последовательностей ходов
        if ((_cache.count(hash)) && (_cache[hash].depth >= depth))
        {
            int score = _cache[hash].score;
            if (_cache[hash].Flag == 0)
            {
                return score;
            }
            if (_cache[hash].Flag == -1)
            {
                a = max(a, score);
            }
            else if (_cache[hash].Flag == 1)
            {
                b = min(b, score);
            }
            if (a >= b)
            {
                return score;
            }
        }

        //проверка победы противника
        if (CheckWin(board, last_i, last_j))
        {
            return -WINING_STRATEGY;
        }
        if (depth == 0)
        {
            if (_stateCache.count(hash))
            {
                return _stateCache[hash];
            }
            return EvaluateState(board, player, hash, rst);
        }

        vector<Move> availableMoves = MoveGeneration(rst, board, player);
    
        int availableMovesSize = availableMoves.size();
        if (availableMovesSize == 0)
        {
            return 0;
        }
    
        int i;
        int j;
        int newHash;
        int bestValue = numeric_limits<int>::min() + 1;
        int NegamaxResultValue;

        for (int k = 0; k < availableMovesSize; k++)
        {
            i = availableMoves[k].i;
            j = availableMoves[k].j;
    
            newHash = UpdateHashOfTheBoard(hash, player, i, j);
            board[i][j] = player;
            Restrictions newRestrictions = RacalculateRestrictions(rst, i, j);

            //запуск в глубину за разных игроков
            NegamaxResultValue = -StartNegamax(board, -player, depth - 1, -b, -a, newHash, newRestrictions, i, j);
            board[i][j] = 0;
            if (NegamaxResultValue > bestValue)
            {
                bestValue = NegamaxResultValue;
                if (depth == _maxDepth)
                {
                    _bestMove = { i,j,NegamaxResultValue };
                }
            }
            a = max(a, NegamaxResultValue);
            if (a >= b)
            {
                break;
            }
        }

        CacheNode cacheNode;
    
        cacheNode.score = bestValue;
        cacheNode.depth = depth;
        if (bestValue <= alphaOrig)
        {
            cacheNode.Flag = 1;
        }
        else if (bestValue >= b)
        {
            cacheNode.Flag = -1;
        }
        else
        {
            cacheNode.Flag = 0;
        }
        _cache[hash] = cacheNode;
        return bestValue;
    }
    
    // https://en.wikipedia.org/wiki/MTD(f)
    Move Bot::StartMTDF(const vector<vector<int>>& board, int firsrGuess, int depth, Restrictions const& rst)
    {
        int guess = firsrGuess;
        int upperbound = numeric_limits<int>::max() - 1;
        int lowerbound = numeric_limits<int>::min() + 1;
        int best;
        Move lastSuccesful{};
    
        while (lowerbound < upperbound)
        {
            best = max(guess, lowerbound + 1);

            guess = StartNegamax(board, 1, depth, best - 1, best, GetHashOfTheBoard(board), rst, 0, 0);
            lastSuccesful = _bestMove;
    
            guess < best ? (upperbound = guess) : (lowerbound = guess);
        }
    
        return lastSuccesful;
    }
    
    Move Bot::FindBestMove()
    {
        int i = 2;
        Restrictions rst = { 0, 0, _boardRows - 1, _boardColumns - 1 };
        int guessForBest = EvaluateState(_gameBoard, 1, GetHashOfTheBoard(_gameBoard), rst);
        while (i != _aiDepth + 2)
        {
            _maxDepth = i;
            guessForBest = (StartMTDF(_gameBoard, guessForBest, i, GetRestrictions(_gameBoard))).score;
            if (guessForBest > WINING_STRATEGY)
            {
                break;
            }
            i += 2;
        }

        _cache.clear();
        _stateCache.clear();
        return _bestMove;
    }

#pragma endregion

} //end namespace AI