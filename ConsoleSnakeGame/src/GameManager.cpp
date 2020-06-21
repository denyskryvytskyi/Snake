#include <ctime>
#include <conio.h>
#include <windows.h>
#include <WinUser.h>

#include "GameManager.h"
#include "GameParams.h"

GameManager::GameManager()
    : mAppleGenCheckpoint()
    , mSpeed()
    , mBonusSpeed()
    , mUpdateDt()
    , mUpdateCheckpoint()
    , mUpdateCheckpointGap()
    , mScore()
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
    mUpdateDt = mGameParams->mSnakeMoveUpdateDt;
    mUpdateCheckpointGap = mGameParams->mSnakeMoveUpdateGap;
    mBonusSpeed = mGameParams->mSnakeBonusSpeed;
    mAppleGenCheckpoint = mGameParams->mAppleGenerateTimeInterval;
    mUpdateCheckpoint = mUpdateDt;

    int width = mGameParams->mWidth;
    int height = mGameParams->mHeight;
    srand(time(0));
    int x = rand() % (width / 2) + width / 3;
    int y = rand() % (height / 2) + height / 3;
    Position startSnakeHeadPos(x, y);

    mSnake->Init(startSnakeHeadPos);
    mMap->Init(width, height);
    // Setup snake head
    mMap->SetCell(positionToMapIndex(startSnakeHeadPos), "snake_head", Cell::ECellState::Snake);

    mScoreRow = mGameParams->mScoreRowIndex;
    mRankRow = mGameParams->mRankRowIndex;
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
    // �������� ������
    // ��������:
    /*
    1. ������� �� ����������� ������
    2. �������� ������ �� Snake's mBody � ������ +-1 * deltaTime ��� ������
    3. ����� ��� ������� �������� ����: ����������� �� ������������� ������
    4. �������� ��� ����� ������������ deltatime
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
        // �������� �������� (����� � ������)
        // ���� ��� �������� - wall - ������ isAlive = false;
        // ���� ��� �������� - ������ - ����� �������� � ����� ��� ���� ����
        ECollisionType collision = checkCollision();
        if (collision == ECollisionType::Wall || collision == ECollisionType::Snake)
        {
            mSnake->SetAlive(false);
            return; // ��� ��� ������ ����� - ��� �� ����� ������ ���������� ����������
        }
        moveSnakeItem(prevItemPos, (*snakeIterator), true);

        // move snake body
        // �������� ��������: ���������� ������ ��������� ������� ���� �� ������� �������������
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
    mMap->Draw(mScore, "snake", mScoreRow, mRankRow);
}

void GameManager::InputHandler()
{
    if (_kbhit())
    {
        char pressedKey = _getch();

        Snake::EDirection newDir = mSnake->GetCurrentDir();
        switch (pressedKey)
        {
        case 'w':
            newDir = Snake::EDirection::Up;
            break;
        case 's':
            newDir = Snake::EDirection::Down;
            break;
        case 'a':
            newDir = Snake::EDirection::Left;
            break;
        case 'd':
            newDir = Snake::EDirection::Right;
            break;
        }
        mSnake->ChangeDirection(newDir);
    }
}

GameManager::ECollisionType GameManager::checkCollision()
{
    // ��������� ��������� ��������� ������ �� �����
    // ���� state == apple || state == wall
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
