#pragma once

class Position
{
public:
    Position() : x(0), y(0) {}

    Position(int posX, int posY) : x(posX), y(posY) {}

    int GetX() const { return x; }
    int GetY() const { return y; }

    Position& operator+=(const Position& right)
    {
        x += right.x;
        y += right.y;
        return *this;
    }
    const Position operator+(const Position& right)
    {
        x += right.x;
        y += right.y;
        return *this;
    }

private:
    int x;
    int y;
};

typedef std::vector<Position> PositionArray;
typedef std::vector<Position>* PositionArrayPtr;