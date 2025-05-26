#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include <unordered_set>
#include <locale>
#include <fstream>


// Предварительное объявление класса Player
class Player; 

class Board {
public:
    int rows, cols;
    // Двумерная сетка для хранения букв
    std::vector<std::vector<wchar_t>> grid;
    std::vector<std::wstring> dictionary;
    // Множество уже использованных слов
    std::unordered_set<std::wstring> usedWords;
    std::wstring searchText;
    // Конструктор с параметрами
    Board(int r, int c, const std::wstring& centerWord);

    std::vector<std::wstring> loadDictionary(const std::string& filename);
    bool isCellEmpty(int r, int c) const;
    bool hasAdjacent(int r, int c) const;
    bool placeLetter(int r, int c, wchar_t letter, Player& player);
    bool hasMovesLeft();
    bool isFull() const;
    void display() const;
};

#endif
