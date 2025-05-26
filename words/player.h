#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
    std::wstring name;
    int score = 0;
    int passCount = 0;

    Player(const std::wstring& n);
    void addScore(int points);
    void incrementPass();
    void resetPass();
};

#endif