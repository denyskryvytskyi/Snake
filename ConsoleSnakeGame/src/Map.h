#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Snake.h"

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

    char GetSymbol() const { return mSymbol; }
    void SetSymbol(char symbol) { mSymbol = symbol; }
    void SetState(ECellState state) { mState = state; }
    ECellState GetState() const { return mState; }

private:
    char mSymbol;
    ECellState mState;
};


class Map
{
public:
    Map();
    ~Map();

    void Init(Position snakeHeadPos);
    void Update();
    void Draw(const int score);
    //
    void MoveSnakeItem(Position prevItemPos, Position nextItemPos, bool isHead = false);
    void AddSnakeItem(Position newItemPos);
    Cell::ECellState GetCellState(Position snakeHeadPos) const;

private:
    void SetupWalls();
    void SetupStartSnakePosition(const Position& snakeHeadPos);
    int PositionToMapIndex(const Position& pos) const;
    void GenerateApple();

private:
    int mWidth;
    int mHeight;

    std::unordered_map<std::string, char> mCellSymbolNames;
    std::vector<Cell> mCells;

    float mAppleGenTimeCheckpoint;
};

extern Map gMap;
