#include <iostream>
#include <ctime>

#include "Map.h"

Map::Map()
    : mWidth(0)
    , mHeight(0)
    , mCells()
{
    mCellSymbolNames = {
        { "wall", '#' },
        { "snake_head", 'x' },
        { "snake_body", 'o' },
        { "apple", '@' },
        { "empty", ' ' },
    };
}

void Map::Init(const int width, const int height)
{
    mWidth = width;
    mHeight = height;

    setupWalls();
}

void Map::Draw(const int score, std::string rank, const int scoreRow, const int rankRow)
{
    std::string scoreStr = "\tScore: " + std::to_string(score);
    std::string rankStr = "\tRank: " + rank;
    std::string map;

    int i = 0;
    int j = 0;
    for (Cell cell : mCells)
    {
        if (i >= mWidth)
        {
            ++j;
            if (j == scoreRow)
            {
                // draw score
                map += scoreStr;
            }
            else if (j == rankRow)
            {
                // draw rank
                map += rankStr;
            }
            map += "\n";
            i = 0;
        }
        map += cell.GetSymbol();
        ++i;
    }
    std::cout << map << std::endl;
}

void Map::Reset()
{
    mCells.clear();
}

Cell::ECellState Map::GetCellState(const int index) const
{
    return mCells[index].GetState();
}

void Map::SetCell(const int index, const std::string symbolName, const Cell::ECellState state)
{
    mCells[index] = Cell(mCellSymbolNames.find(symbolName)->second, state);
}

void Map::setupWalls()
{
    const char symbol = mCellSymbolNames.find("wall")->second;
    Cell::ECellState state = Cell::ECellState::Wall;
    Cell cell(symbol, state);
    bool isSolidRow = false; // all symbols in the row - wall symbols
    /*
    * Configure walls on the map
    * 1. for the first and the last rows - all symbols are wall symbols
    * 2. for other - only first and the last symbols in row are wall symbols
    */
    for (int i = 0; i < mHeight; i++, isSolidRow = false)
    {
        // first and last rows
        if (i == 0 || i == mHeight - 1)
        {
            isSolidRow = true;
        }

        for (int j = 0; j < mWidth; j++)
        {
            if (isSolidRow || j == 0 || j == mWidth - 1)
            {
                mCells.push_back(cell);
            }
            else
            {
                mCells.push_back(Cell());
            }
        }
    }
}

Cell::Cell()
    : mSymbol(' ')
    , mState(ECellState::Empty)
{
}

Cell::Cell(char symbol, ECellState state)
    : mSymbol(symbol)
    , mState(state)
{
}