#include <ctime>
#include <conio.h>
#include <windows.h>

#include "GameManager.h"
#include "GameParams.h"
#include "defines.h"

GameManager::GameManager()
    : mAppleGenCheckpoint()
    , mSpeed()
    , mBonusSpeed()
    , mUpdateDt()
    , mUpdateCheckpoint()
    , mUpdateCheckpointGap()
    , mScore()
    , mRank()
    , mScoreRowIndex()
    , mRankRowIndex()
{
    mSnake = new Snake();
    mMap = new Map();
    mGameParams = GameParams::GetInstance();
}

GameManager::~GameManager()
{
    if (mMap != nullptr)
    {
        delete mMap;
    }
    if (mSnake != nullptr)
    {
        delete mSnake;
    }
}

void GameManager::Init()
{
    Reset();

    mScoreRowIndex = mGameParams->mScoreRowIndex;
    mRankRowIndex = mGameParams->mRankRowIndex;
}

void GameManager::OnStart()
{
    Position newSnakeStartPos = getRandStartSnakePos();
    mSnake->Init(newSnakeStartPos);
    mMap->Init(mGameParams->mWidth, mGameParams->mHeight);
    // Setup snake head
    mMap->SetCell(positionToMapIndex(newSnakeStartPos), "snake_head", Cell::ECellState::Snake);
}

void GameManager::Update()
{
    if (mAppleGenCheckpoint <= 0)
    {
        mAppleGenCheckpoint = mGameParams->mAppleGenerateTimeInterval;
        generateApple();
    }
    --mAppleGenCheckpoint;

    // Map reconfig
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
        mUpdateCheckpoint = mUpdateDt - mSpeed;
        //
        PositionArray* snake = mSnake->GetBody();
        PositionArray::iterator snakeIterator = snake->begin();
        // calculate head movement and collision
        Position prevItemPos = (*snakeIterator);
        Position headMoveOffset(0, 0);
        switch (mSnake->GetCurrentDir())
        {
        case Snake::EDirection::Up:
            headMoveOffset = Position(0, -1);
            break;
        case Snake::EDirection::Down:
            headMoveOffset = Position(0, 1);
            break;
        case Snake::EDirection::Left:
            headMoveOffset = Position(-1, 0);
            break;
        case Snake::EDirection::Right:
            headMoveOffset = Position(1, 0);
            break;
        }
        (*snakeIterator) += headMoveOffset;
        // Проверка коллизии (стена и яблоко)
        // если тип коллизии - wall - ставим isAlive = false;
        // если тип коллизии - яблоко - нужно добавить в конец еще одно тело
        ECollisionType collision = checkCollision();
        if (collision == ECollisionType::Wall || collision == ECollisionType::Snake)
        {
            mSnake->SetAlive(false);
            return; // так как дальше стена - нам не нужно делать дальнейшие вычисления
        }
        moveSnakeItem(prevItemPos, (*snakeIterator), true);

        // move snake body
        // алгоритм движения: перемещаем каждый следующий элемент тела на позицию передвинутого
        ++snakeIterator;
        for (; snakeIterator != snake->end(); ++snakeIterator)
        {
            swapPositions(&(*snakeIterator), &prevItemPos);
            moveSnakeItem(prevItemPos, (*snakeIterator));
        }

        if (collision == ECollisionType::Apple)
        {
            // add one more body item
            snake->push_back(prevItemPos);
            addSnakeItem(prevItemPos);
            mSpeed += mBonusSpeed;
            mScore++;
        }
    }
    mUpdateCheckpoint -= mUpdateCheckpointGap;
}

void GameManager::Render()
{
    // redraw cells
    mMap->Draw(mScore, "snake", mScoreRowIndex, mRankRowIndex);
}

void GameManager::Reset(bool hardReset /*= false*/)
{
    mUpdateDt = mGameParams->mSnakeMoveUpdateDt;
    mUpdateCheckpointGap = mGameParams->mSnakeMoveUpdateGap;
    mBonusSpeed = mGameParams->mSnakeBonusSpeed;
    mAppleGenCheckpoint = mGameParams->mAppleGenerateTimeInterval;
    mUpdateCheckpoint = mUpdateDt;

    if (hardReset)
    {
        mMap->Reset();
        mSnake->Reset();

        mSpeed = 0.0f;
        mScore = 0;
        mRank = "";
    }
}

void GameManager::InputHandler()
{
    Snake::EDirection newDir = mSnake->GetCurrentDir();
    if (GetAsyncKeyState(KEY_UP) || GetAsyncKeyState(KEY_W))
    {
        newDir = Snake::EDirection::Up;
    }
    if (GetAsyncKeyState(KEY_DOWN) || GetAsyncKeyState(KEY_S))
    {
        newDir = Snake::EDirection::Down;
    }
    if (GetAsyncKeyState(KEY_LEFT) || GetAsyncKeyState(KEY_A))
    {
        newDir = Snake::EDirection::Left;
    }
    if (GetAsyncKeyState(KEY_RIGHT) || GetAsyncKeyState(KEY_D))
    {
        newDir = Snake::EDirection::Right;
    }
    mSnake->ChangeDirection(newDir);
}

GameManager::ECollisionType GameManager::checkCollision()
{
    // проверяем состояние следующей ячейки на карте
    // если state == apple || state == wall
    Cell::ECellState state = mMap->GetCellState(positionToMapIndex(mSnake->GetHeadPos()));

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

int GameManager::positionToMapIndex(const Position& pos) const
{
    return mGameParams->mWidth * pos.GetY() + pos.GetX();
}

void GameManager::generateApple()
{
    srand(time(0));
    // generate apple on map
    while (true)
    {
        int applePosX = rand() % (mGameParams->mWidth - 2) + 1;
        int applePosY = rand() % (mGameParams->mHeight - 2) + 1;

        int appleIndex = positionToMapIndex(Position(applePosX, applePosY));

        if (mMap->GetCellState(appleIndex) == Cell::ECellState::Empty)
        {
            Cell::ECellState state = Cell::ECellState::Apple;
            mMap->SetCell(appleIndex, "apple", state);
            break;
        }
    }
}

void GameManager::swapPositions(Position* prev, Position* next)
{
    Position temp = *prev;
    *prev = *next;
    *next = temp;
}

void GameManager::moveSnakeItem(const Position prevItemPos, const Position nextItemPos, bool isHead)
{
    int prevPosIndex = positionToMapIndex(prevItemPos);
    int nextPosIndex = positionToMapIndex(nextItemPos);

    mMap->SetCell(prevPosIndex, "empty", Cell::ECellState::Empty);

    std::string symbolName = isHead ? "snake_head" : "snake_body";
    mMap->SetCell(nextPosIndex, symbolName, Cell::ECellState::Snake);
}

void GameManager::addSnakeItem(const Position newItemPos)
{
    mMap->SetCell(positionToMapIndex(newItemPos), "snake_body", Cell::ECellState::Snake);
}

Position GameManager::getRandStartSnakePos()
{
    int width = mGameParams->mWidth;
    int height = mGameParams->mHeight;
    srand(time(0));
    int x = rand() % (width / 2) + width / 3;
    int y = rand() % (height / 2) + height / 3;
    Position startSnakeHeadPos(x, y);

    return startSnakeHeadPos;
}
