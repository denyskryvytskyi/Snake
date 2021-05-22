#include <ctime>
#include <windows.h>

#include "GameManager.h"
#include "GameParams.h"
#include "defines.h"

GameManager::GameManager()
    : mBonusSpeed(0.0f)
    , mSpeedTimer()
    , mAppleGenerationTimer()
    , mScore(0)
    , mRank()
    , mScoreRowIndex(0)
    , mRankRowIndex(0)
    , mInputHanlderEnable(true)
    , mDemoMode(false)
{
    mSnake = new Snake();
    mMap = new Map();
    mGameParams = GameParams::instance();

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

    mSpeedTimer.Init(mGameParams->mSnakeMoveUpdateWaitTime);
    mAppleGenerationTimer.Init(mGameParams->mAppleGenerationTimeInterval);

    mScoreRowIndex = mGameParams->mScoreRowIndex;
    mRankRowIndex = mGameParams->mRankRowIndex;
}

void GameManager::OnStart(bool isDemoMode)
{
    mDemoMode = isDemoMode;

    Position newSnakeStartPos = getRandStartSnakePos();
    mSnake->Init(newSnakeStartPos, mDemoMode, mGameParams->mSnakeMaxSpeed);
    mMap->Init(mGameParams->mWidth, mGameParams->mHeight);
    // setup snake head
    mMap->SetCell(positionToMapIndex(newSnakeStartPos), "snake_head", Cell::ECellState::Snake);
    // generate forst apple
    generateApple(); 
    if (mDemoMode)
    {
        SetSnakeDemoMode();
    }
}

void GameManager::Update(float dt)
{
    // calculate current snake rank
    calcRank();

    // generate apple after timout
    if (mAppleGenerationTimer.IsOver())
    {
        mAppleGenerationTimer.Restart();
        generateApple();
    }
    mAppleGenerationTimer.OnTick(dt);

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
        case ESnakeDirection::Up:
            headMoveOffset = Position(0, -1);
            break;
        case ESnakeDirection::Down:
            headMoveOffset = Position(0, 1);
            break;
        case ESnakeDirection::Left:
            headMoveOffset = Position(-1, 0);
            break;
        case ESnakeDirection::Right:
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
            // find new apple for ai mode
            if (mDemoMode)
            {
                SetSnakeDemoMode();
            }
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
    mBonusSpeed = mGameParams->mSnakeBonusSpeed;

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

    ESnakeDirection currentDir = mSnake->GetCurrentDir();
    ESnakeDirection newDir = currentDir;
    if (GetAsyncKeyState(KEY_UP) || GetAsyncKeyState(KEY_W))
    {
        newDir = ESnakeDirection::Up;
    }
    if (GetAsyncKeyState(KEY_DOWN) || GetAsyncKeyState(KEY_S))
    {
        newDir = ESnakeDirection::Down;
    }
    if (GetAsyncKeyState(KEY_LEFT) || GetAsyncKeyState(KEY_A))
    {
        newDir = ESnakeDirection::Left;
    }
    if (GetAsyncKeyState(KEY_RIGHT) || GetAsyncKeyState(KEY_D))
    {
        newDir = ESnakeDirection::Right;
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
    return mGameParams->mWidth * pos.GetY() + pos.GetX();
}

void GameManager::generateApple()
{
    srand(time(0));
    // try generate apple position on map
    while (true)
    {
        int applePosX = rand() % (mGameParams->mWidth - 2) + 1;
        int applePosY = rand() % (mGameParams->mHeight - 2) + 1;

        Position applePos = Position(applePosX, applePosY);
        mApplesPos.push_back(applePos);

        int appleIndex = positionToMapIndex(applePos);

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
    int width = mGameParams->mWidth;
    int height = mGameParams->mHeight;
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

void GameManager::SetSnakeDemoMode()
{
    // get last generated apple
    auto it = mApplesPos.end() - 1;
    // send apple pos to snake AI
    mSnake->SetAIApplePos(*it);
    mApplesPos.erase(it);
}

SnakeTimer::SnakeTimer()
    : mWaitTime(0.0f)
    , mCurrentTime(0.0f)
{
}

void SnakeTimer::Init(float waitTime)
{
    mWaitTime = waitTime;
    Restart();
}

void SnakeTimer::Restart(float bonusTime /* =0.0f*/)
{
    mCurrentTime = mWaitTime - bonusTime;
}
