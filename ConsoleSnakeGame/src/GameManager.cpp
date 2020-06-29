#include <ctime>
#include <windows.h>

#include "GameManager.h"
#include "GameParams.h"
#include "defines.h"

GameManager::GameManager()
    : mAppleGenCheckpoint(0.0f)
    , mBonusSpeed(0.0f)
    , mSpeedTimer()
    , mScore(0)
    , mRank()
    , mScoreRowIndex(0)
    , mRankRowIndex(0)
    , mInputHanlderEnable(true)
    , mDemoMode(false)
{
    mSnake = new Snake();
    mMap = new Map();

    mRankNames = {
        {2, "Worm"},
        {4, "Viper"},
        {6, "Cobra"},
        {8, "Python"},
        {10, "Anaconda"}
    };
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
    mSpeedTimer.Init();

    GameParams gameParams = GameParams::instance();
    mScoreRowIndex = gameParams.mScoreRowIndex;
    mRankRowIndex = gameParams.mRankRowIndex;
}

void GameManager::OnStart(bool isDemoMode)
{
    mDemoMode = isDemoMode;
    GameParams gameParams = GameParams::instance();

    Position newSnakeStartPos = getRandStartSnakePos();
    mSnake->Init(newSnakeStartPos, mDemoMode);
    mMap->Init(gameParams.mWidth, gameParams.mHeight);
    // Setup snake head
    mMap->SetCell(positionToMapIndex(newSnakeStartPos), "snake_head", Cell::ECellState::Snake);
}

void GameManager::Update(float dt)
{
    // calculate current snake rank
    calcRank();

    // generate apple after timout
    if (mAppleGenCheckpoint <= 0)
    {
        mAppleGenCheckpoint = GameParams::instance().mAppleGenerateTimeInterval;
        generateApple();
    }
    --mAppleGenCheckpoint;

    /*
    * Map update
    * 1. Check if is it time to update snake movement
    * 2. Check snake direction
    * 3. Move all snake's body items
    * 4. Check collision
    */
    if (mSpeedTimer.IsOver())
    {
        // snake movement update timer takes into account snake speed
        mSpeedTimer.Restart(mSnake->GetSpeed());
        //
        PositionArray* snake = mSnake->GetBody();
        PositionArray::iterator snakeIterator = snake->begin();
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
        // Move snake head
        (*snakeIterator) += headMoveOffset;
        // Check collision of snake head with wall or apple
        ECollisionType collision = checkCollision();
        if (collision == ECollisionType::Wall || collision == ECollisionType::Snake)
        {
            mSnake->SetAlive(false);
            // no further calculations needed
            return;
        }
        // update snake head cell on the map
        moveSnakeItem(prevItemPos, (*snakeIterator), true);

        // snake body movement
        ++snakeIterator;
        for (; snakeIterator != snake->end(); ++snakeIterator)
        {
            // swap positions of current item and previously moved
            swapPositions(&(*snakeIterator), &prevItemPos);
            // update snake body item cell on the map
            moveSnakeItem(prevItemPos, (*snakeIterator));
        }

        // if an apple was eaten
        if (collision == ECollisionType::Apple)
        {
            snake->push_back(prevItemPos);
            addSnakeItem(prevItemPos);
            mSnake->IncreaseSpeed(mBonusSpeed);
            mScore++;
        }

        mSnake->Update();
        // enable input handling
        mInputHanlderEnable = true;
    }
    mSpeedTimer.OnTick(dt);
}

void GameManager::Render()
{
    // redraw cells
    mMap->Draw(mScore, mRank, mScoreRowIndex, mRankRowIndex);
}

void GameManager::Reset(bool hardReset /*= false*/)
{
    GameParams gameParams = GameParams::instance();

    mBonusSpeed = gameParams.mSnakeBonusSpeed;
    mAppleGenCheckpoint = gameParams.mAppleGenerateTimeInterval;

    if (hardReset)
    {
        mMap->Reset();
        mSnake->Reset();

        mScore = 0;
        mRank = "";
    }
}

void GameManager::InputHandler()
{
    if (!mInputHanlderEnable || mDemoMode)
        return;

    Snake::EDirection currentDir = mSnake->GetCurrentDir();
    Snake::EDirection newDir = currentDir;
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
    if (newDir != currentDir)
    {
        mSnake->ChangeDirection(newDir);

        // if direction was changed - disable input handling until update will be done
        mInputHanlderEnable = false;
    }
}

GameManager::ECollisionType GameManager::checkCollision()
{
    // get state of the next map cell (new snake head position)
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

// Convert (x;y) position of snake body item to map cell format
int GameManager::positionToMapIndex(const Position& pos) const
{
    return GameParams::instance().mWidth * pos.GetY() + pos.GetX();
}

void GameManager::generateApple()
{
    GameParams gameParams = GameParams::instance();
    srand(time(0));
    // try generate apple position on map
    while (true)
    {
        int applePosX = rand() % (gameParams.mWidth - 2) + 1;
        int applePosY = rand() % (gameParams.mHeight - 2) + 1;

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

// Generate random position of snake head on the map
Position GameManager::getRandStartSnakePos()
{
    GameParams gameParams = GameParams::instance();
    int width = gameParams.mWidth;
    int height = gameParams.mHeight;
    srand(time(0));
    int x = rand() % (width / 2) + width / 3;
    int y = rand() % (height / 2) + height / 3;
    Position startSnakeHeadPos(x, y);

    return startSnakeHeadPos;
}

void GameManager::calcRank()
{
    for (auto rankName : mRankNames)
    {
        if (mScore <= rankName.first)
        {
            mRank = rankName.second;
            return;
        }
    }
}

SpeedTimer::SpeedTimer()
    : mWaitTime(0.0f)
    , mCurrentTime(0.0f)
{
}

void SpeedTimer::Init()
{
    GameParams gameParams = GameParams::instance();
    mWaitTime = gameParams.mSnakeMoveUpdateWaitTime;

    Restart();
}

void SpeedTimer::Restart(float bonusTime /* =0.0f*/)
{
    mCurrentTime = mWaitTime - bonusTime;
}
