#include "Snake.h"
#include "Map.h"
#include "GameParams.h"

Snake::Snake()
    : mBody()
    , mCurrentDir(EDirection::Up)
    , mAlive(true)
    , mSpeed(0.0f)
    , mBonusSpeed(0.0f)
    , mUpdateCheckpoint(0.0f)
    , mScore(0)
{
}

Snake::~Snake()
{
}

void Snake::Init(Position headPos)
{
    mBody.push_back(headPos);
    mUpdateCheckpoint = gGameParams.mSnakeMoveUpdateDt;
    mBonusSpeed = gGameParams.mSnakeBonusSpeed;
}

void Snake::Update()
{
    // Движение змейки
    // алгоритм:
    /*
    1. Смотрим на направление змейки
    2. Проходим циклом по Snake's mBody и делаем +-1 * deltaTime для головы
    3. Затем для каждого элемента тела: передвигаем на осободившуюся ячейку
    4. Подумать как лучше обрабатывать deltatime
    */
    if (mUpdateCheckpoint <= 0)
    {
        mUpdateCheckpoint = gGameParams.mSnakeMoveUpdateDt - mSpeed;
        //
        PositionArray::iterator snakeIterator = mBody.begin();
        // calculate head movement and collision
        Position prevItemPos = (*snakeIterator);
        Position headMoveOffset(0, 0);
        switch (mCurrentDir)
        {
        case EDirection::Up:
            headMoveOffset = Position(0, -1);
            break;
        case EDirection::Down:
            headMoveOffset = Position(0, 1);
            break;
        case EDirection::Left:
            headMoveOffset = Position(-1, 0);
            break;
        case EDirection::Right:
            headMoveOffset = Position(1, 0);
            break;
        }
        (*snakeIterator) += headMoveOffset;
        // Проверка коллизии (стена и яблоко)
        // если тип коллизии - wall - ставим isAlive = false;
        // если тип коллизии - яблоко - нужно добавить в конец еще одно тело
        ECollisionType collision = CheckCollision();
        if (collision == ECollisionType::Wall || collision == ECollisionType::Snake)
        {
            mAlive = false;
            return; // так как дальше стена - нам не нужно делать дальнейшие вычисления
        }
        gMap.MoveSnakeItem(prevItemPos, (*snakeIterator), true);

        // move snake body
        // алгоритм движения: перемещаем каждый следующий элемент тела на позицию передвинутого
        ++snakeIterator;
        for (; snakeIterator != mBody.end(); ++snakeIterator)
        {
            SwapPositions(&(*snakeIterator), &prevItemPos);
            gMap.MoveSnakeItem(prevItemPos, (*snakeIterator));
        }

        if (collision == ECollisionType::Apple)
        {
            // add one more body item
            mBody.push_back(prevItemPos);
            gMap.AddSnakeItem(prevItemPos);
            mSpeed += mBonusSpeed;
            mScore++;
        }
    }
    mUpdateCheckpoint -= gGameParams.mSnakeMoveUpdateGap;
}

void Snake::ChangeDirection(EDirection newDir)
{
    // if snake is only head
    if (mBody.size() == 1)
    {
        mCurrentDir = newDir;
        return;
    }

    switch (newDir)
    {
    case EDirection::Up:
        if (mCurrentDir != EDirection::Down)
            mCurrentDir = newDir;
        break;
    case EDirection::Down:
        if (mCurrentDir != EDirection::Up)
            mCurrentDir = newDir;
        break;
    case EDirection::Left:
        if (mCurrentDir != EDirection::Right)
            mCurrentDir = newDir;
        break;
    case EDirection::Right:
        if (mCurrentDir != EDirection::Left)
            mCurrentDir = newDir;
        break;
    }
}

Snake::ECollisionType Snake::CheckCollision()
{
    // проверяем состояние следующей ячейки на карте
    // если state == apple || state == wall
    Cell::ECellState state = gMap.GetCellState(mBody[0]);

    if (state == Cell::ECellState::Apple)
    {
        return ECollisionType::Apple;
    }
    if (state == Cell::ECellState::Wall)
    {
        return ECollisionType::Wall;
    }
    if (state == Cell::ECellState::Snake)
    {
        return ECollisionType::Snake;
    }

    return ECollisionType::None;
}

void Snake::SwapPositions(Position* prev, Position* next)
{
    Position temp = *prev;
    *prev = *next;
    *next = temp;
}

Position::Position()
    : x(0)
    , y(0)
{
}

Position::Position(int posX, int posY)
    : x(posX)
    , y(posY)
{
}
