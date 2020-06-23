#pragma once

#include <vector>
#include <unordered_map>
#include <string>

struct Cell
{
    enum class ECellState
    {
        Empty,
        Wall,
        Apple,
        Snake,
    };

public:
    Cell();
    Cell(char symbol, ECellState state = ECellState::Empty);
    //
    char GetSymbol() const { return mSymbol; }
    ECellState GetState() const { return mState; }

private:
    char mSymbol;
    ECellState mState;
};

class Map
{
public:
    Map();
    //
    void Init(const int width, const int height);
    void Draw(const int score, std::string rank, const int scoreRow, const int rankRow);
    void Reset();
    //
    Cell::ECellState GetCellState(const int index) const;
    void SetCell(const int index, const std::string symbolName, const Cell::ECellState state);

private:
    void setupWalls();

private:
    int mWidth;
    int mHeight;
    //
    std::unordered_map<std::string, char> mCellSymbolNames;
    std::vector<Cell> mCells;
};
