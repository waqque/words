#include "board.h"
#include "player.h"
#include "dfs.h"
#include <iostream>
#include <algorithm>
#include <codecvt>

Board::Board(int r, int c, const std::wstring& centerWord) : rows(r), cols(c) {
    // Инициализация пустого поля
    grid.assign(r, std::vector<wchar_t>(c, L' '));
    // Размещение центрального(первого) слова
    int mid = r / 2;
    int len = std::min((int)centerWord.size(), c);
    int offset = (c - len) / 2;
    for (int i = 0; i < len; ++i) {
        grid[mid][offset + i] = centerWord[i];
    }
    dictionary = loadDictionary("dictionary.txt");
}

std::vector<std::wstring> Board::loadDictionary(const std::string& filename) {
    std::vector<std::wstring> dict;
    std::wifstream file(filename);

    if (!file.is_open()) {
        std::wcerr << L"Не удалось открыть файл: " << filename.c_str() << std::endl;
        return dict;
    }
    // Настройка локали для чтения UTF-8
    file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));
    std::wstring word;
    while (getline(file, word)) {
        if (!word.empty()) {
            dict.push_back(word);
        }
    }

    file.close();
    return dict;
}

// Ппустая ли ячейка
bool Board::isCellEmpty(int r, int c) const {
    return (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] == L' ');
}
// Проверка наличия соседних заполненных клеток
bool Board::hasAdjacent(int r, int c) const {
    static int dr[4] = { 1, -1, 0, 0 }; // Смещения по строкам
    static int dc[4] = { 0, 0, 1, -1 }; // Смещения по столбцам
    for (int k = 0; k < 4; ++k) {
        int nr = r + dr[k], nc = c + dc[k];
        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] != L' ') {
            return true;
        }
    }
    return false;
}

bool Board::placeLetter(int r, int c, wchar_t letter, Player& player) {
    if (!isCellEmpty(r, c) || !hasAdjacent(r, c)) {
        std::wcout << L"Ход недопустим: неверная позиция.\n";
        return false;
    }
    // Размещение буквы
    grid[r][c] = letter;
    bool foundWord = false;
    
    // Поиск новых слов в словаре(искл использованные)
    for (const auto& w : dictionary) {
        if (usedWords.count(w)) continue;
        bool found = false;
        for (int i = 0; i < rows && !found; ++i) {
            for (int j = 0; j < cols && !found; ++j) {
                std::vector<std::vector<bool>> vis(rows, std::vector<bool>(cols, false));
                if (dfs(grid, i, j, w, 0, vis) && vis[r][c]) {
                    found = true;
                }
            }
        }
        if (found) {
            usedWords.insert(w); // Добавление слова в использованные
            searchText = L"Найдено слово: " + w + L"\n";
            foundWord = true;
            break;
        }
    }

    player.addScore(1);

    return true;
}
// Проверка наличия возможных ходов
bool Board::hasMovesLeft() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!isCellEmpty(r, c) || !hasAdjacent(r, c)) continue;
            for (const auto& w : dictionary) {
                if (usedWords.count(w)) continue;
                for (wchar_t ch : w) {
                    grid[r][c] = ch; // Временная установка буквы
                    std::vector<std::vector<bool>> vis(rows, std::vector<bool>(cols, false));
                    bool ok = false;
                    for (int i = 0; i < rows && !ok; ++i) {
                        for (int j = 0; j < cols && !ok; ++j) {
                            if (dfs(grid, i, j, w, 0, vis) && vis[r][c]) {
                                ok = true;
                            }
                        }
                    }
                    grid[r][c] = L' ';
                    if (ok) return true;
                }
            }
        }
    }
    return false;
}
// Проверка заполненности поля
bool Board::isFull() const {
    for (auto& row : grid)
        for (auto ch : row)
            if (ch == L' ') return false;
    return true;
}

// Отображение игрового поля
void Board::display() const {
    // Вывод номеров столбцов
    std::wcout << L"   ";
    for (int j = 0; j < cols; ++j) std::wcout << j << L" ";
    std::wcout << L"\n";
    // Вывод строк с номерами и содержимым
    for (int i = 0; i < rows; ++i) {
        std::wcout << i << L": ";
        for (int j = 0; j < cols; ++j) {
            std::wcout << grid[i][j] << L" ";
        }
        std::wcout << L"\n";
    }
}
