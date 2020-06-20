#include <iostream>
#include <ctime>

#include "Map.h"
#include "GameParams.h"

Map::Map()
    : mWidth(0)
    , mHeight(0)
    , mCells()
    , mAppleGenTimeCheckpoint(0)
{
    mCellSymbolNames = {
        { "wall", '#' },
        { "snake_head", 'x' },
        { "snake_body", 'o' },
        { "apple", '@' },
        { "empty", ' ' },
    };
}

Map::~Map()
{
}

void Map::Init(Position snakeHeadPos)
{
    mWidth = gGameParams.mWidth;
    mHeight = gGameParams.mHeight;

    SetupWalls();
    SetupStartSnakePosition(snakeHeadPos);
}

void Map::Update()
{
    if (mAppleGenTimeCheckpoint <= 0)
    {
        mAppleGenTimeCheckpoint = gGameParams.mAppleGenerateTimeInterval;
        GenerateApple();
    }
    --mAppleGenTimeCheckpoint;
}

void Map::Draw(const int score)
{
    std::string scoreStr = "\tScore: " + std::to_string(score);
    std::string map;

    int i = 0;
    int j = 0;
    for (Cell cell : mCells)
    {
        if (i >= mWidth)
        {
            // score calculations
            ++j;
            if (j == mHeight / 2)
            {
                map += scoreStr;
            }
            // continue map calc
            map += "\n";
            i = 0;
        }
        map += cell.GetSymbol();
        ++i;
    }
    std::cout << /*scoreStr << std::endl <<*/ map << std::endl;
}

void Map::MoveSnakeItem(Position prevItemPos, Position nextItemPos, bool isHead /* = false*/)
{
    // передвигаем кусочек змейки на указанную позицию
    int prevPosIndex = PositionToMapIndex(prevItemPos);
    int nextPosIndex = PositionToMapIndex(nextItemPos);

    mCells[prevPosIndex].SetState(Cell::ECellState::Empty);
    mCells[prevPosIndex].SetSymbol(mCellSymbolNames.find("empty")->second);

    std::string symbolName = isHead ? "snake_head" : "snake_body";
    mCells[nextPosIndex].SetState(Cell::ECellState::Snake);
    mCells[nextPosIndex].SetSymbol(mCellSymbolNames.find(symbolName)->second);
}

void Map::AddSnakeItem(Position newItemPos)
{
    char symbol = mCellSymbolNames.find("snake_body")->second;
    Cell snakeItem(symbol, Cell::ECellState::Snake);
    mCells[PositionToMapIndex(newItemPos)] = snakeItem;
}

Cell::ECellState Map::GetCellState(Position snakeHeadPos) const
{
    int index = PositionToMapIndex(snakeHeadPos);

    return mCells[index].GetState();
}

void Map::SetupWalls()
{
    // set walls
    /*
    #########################
    #                       #
    #                       #
    #                       #
    #                       #
    #                       #
    #                       #
    #                       #
    #                       #
    #########################
    */
    char symbol = mCellSymbolNames.find("wall")->second;
    Cell::ECellState state = Cell::ECellState::Wall;
    Cell cell(symbol, state);
    bool isSolidRow = false;
    // 1. Вынести сетап стен в отдельную функцию
    // 2. Все настройки делаем в двойном цикле. Если индекс внешнего цикла == 0 или == height - 1, то все ячейки строки- стена,
    // иначе - для j == 0 и j == width - 1 ставим wall, а для остальных - empty.
    for (int i = 0; i < mHeight; i++, isSolidRow = false)
    {
        // first and last rows
        if (i == 0 || i == mHeight - 1)
        {
            isSolidRow = true;
        }
        // middle rows
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

void Map::SetupStartSnakePosition(const Position& snakeHeadPos)
{
    int snakeHeadIndex = PositionToMapIndex(snakeHeadPos);
    mCells[snakeHeadIndex] = Cell(mCellSymbolNames.find("snake_head")->second, Cell::ECellState::Snake);
}

int Map::PositionToMapIndex(const Position& pos) const
{
    return mWidth * pos.GetY() + pos.GetX();
}

void Map::GenerateApple()
{
    srand(time(0));
    // generate apple on map
    while (true)
    {
        int applePosX = rand() % (mWidth - 2) + 1;
        int applePosY = rand() % (mHeight - 2) + 1;

        int appleIndex = PositionToMapIndex(Position(applePosX, applePosY));
        if (mCells[appleIndex].GetState() == Cell::ECellState::Empty)
        {
            mCells[appleIndex].SetState(Cell::ECellState::Apple);
            mCells[appleIndex].SetSymbol(mCellSymbolNames.find("apple")->second);
            break;
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

