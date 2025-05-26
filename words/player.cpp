#include "player.h"

Player::Player(const std::wstring& n) : name(n) {}

void Player::addScore(int points) { score += points; }
void Player::incrementPass() { passCount++; }
void Player::resetPass() { passCount = 0; }