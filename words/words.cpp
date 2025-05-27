//Разработайте консольный вариант игры «Слова» — лингвистическая игра для 2—4 игроков, в которой необходимо составлять слова с помощью букв, добавляемых определённым образом на квадратное игровое поле. В наиболее популярном варианте игры, который имеет множество компьютерных реализаций, слова составляются посредством переходов от буквы к букве под прямым углом.
//В наиболее популярном варианте игровое поле представляет собой 25 - клеточную квадратную таблицу, ячейки(клетки) центральной строки которой содержат по одной букве, а строка целиком — произвольное 5 - буквенное нарицательное имя существительное в именительном падеже и единственном числе(множественном числе, если слово не имеет единственного числа).Размеры поля, расположение и длина слова могут варьироваться, тем не менее количество пустых клеток в начале игры должно быть чётным, чтобы у обоих игроков было одинаковое количество ходов(слов).
//Во время своего хода игрок может добавить букву в клетку, примыкающую по вертикали / горизонтали(в варианте «Королевский квадрат» также по диагонали) к заполненной клетке таким образом, чтобы получалась неразрывная и несамопересекающаяся прямоугольная ломаная(«змейка») из клетки с добавленной буквой и других заполненных клеток, представляющая собой слово(соответствующее описанным выше требованиям), или пропустить ход(если оба игрока трижды подряд пропускают ход, то фиксируется ничья).В течение игры должны соблюдаться также следующие правила :
//Игроки ходят по очереди(в варианте «Королевский квадрат» для двух игроков ходы двойные, то есть игрок, если не пропускает ход, добавляет подряд две буквы в разные клетки).
//Каждая клетка содержит только одну букву, каждая буква в составленном слове приносит игроку одно очко.
//Слово должно содержаться хотя бы в одном толковом или энциклопедическом словаре(или энциклопедии), при этом запрещаются аббревиатуры, слова с уменьшительно - ласкательными(банька, дочурка, ножик) и размерно - оценочными суффиксами(домина, зверюга, ножища), если такие слова не имеют специального значения, а также слова, не входящие в состав русского литературного языка.
//Слова в одной игре повторяться не могут, даже если это омонимы(в варианте «Королевский квадрат» запрещается составление слов, уже образовавшихся на игровом поле, даже если они не были составлены ни одним из игроков).
//Игра заканчивается тогда, когда либо заполнены все клетки, либо невозможно составить очередное слово согласно указанным выше правилам.Выигрывает тот игрок, который наберёт большее количество очков, кроме случая ничьей после троекратного пропуска хода обоими игроками.

#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <sstream>
#include <codecvt>
#ifdef _WIN32
#include <windows.h>
#endif
#include "player.h"
#include "board.h"
#include "dfs.h"
#include "cns.h"
#include "logger.h"
using namespace std;

int main() {
    //Настройка кодировки консоли для Windows
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    //Настройка локали для работы с широкими символами (wchar_t)
    try {
        std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>());
        std::wcout.imbue(utf8_locale);
        std::wcin.imbue(utf8_locale);
    }
    catch (const exception& e) {
        wcerr << L"Не удалось установить локаль: " << e.what() << endl;
        return 1;
    }
    Logger gameLogger("game_log.txt");
    gameLogger.log(L"Новая игра");
    //Ввод параметров игры
    int numPlayers, fieldSize;
    wstring centerWord;
    bool skip = false;
    wcout << L"Количество игроков (2-4): "; wcin >> numPlayers;
    wcout << L"Размер поля (нечетное, >=3): ";
    wcin >> fieldSize;
    if (fieldSize % 2 == 0) {
        wcout << L"Размер поля должен быть нечетным ";
        return 1;
    }

    wcin.ignore(10000, '\n');
    wcout << L"Стартовое слово (длина <= поле): "; getline(wcin, centerWord);


    vector<Player> players;
    for (int i = 1; i <= numPlayers; ++i) {
        wstring playerName = L"Игрок " + to_wstring(i);
        players.emplace_back(playerName);
        gameLogger.log(playerName + L" присоединился к игре");
    }
    //Инициализация игрового поля
    Board board(fieldSize, fieldSize, centerWord);
    gameLogger.log(L"Создано поле размером " + to_wstring(fieldSize) + L"x" + to_wstring(fieldSize));
    gameLogger.log(L"Центральное слово: " + centerWord);
    gameLogger.logBoard(board.grid);
    int current = 0;
    int totalPasses = 0;


    while (true) {
        clearConsole();
        wcout << L"СЧЁТ:\n";
        for (const auto& p : players) {
            wcout << p.name << L": " << p.score << L" очк";
            // Правильное склонение слова "очко"
            if ((p.score % 10 == 1) && (p.score % 100 != 11))
                wcout << L"о";
            else if ((p.score % 10 >= 2 && p.score % 10 <= 4) && (p.score % 100 < 10 || p.score % 100 >= 20))
                wcout << L"а";
            wcout << L", пропусков подряд: " << p.passCount << L"\n";
        }

        wcout << board.searchText;

        wcout << L"\nТекущее поле:\n";
        board.display();

        Player& pl = players[current];
        gameLogger.log(L"Ход " + pl.name);
        wcout << pl.name << L", ваш ход (x y буква или skip): ";
        wstring line; getline(wcin, line);
        if (line == L"skip") {
            // Обработка пропуска хода
            pl.incrementPass();
            totalPasses++;
            gameLogger.log(pl.name + L" пропускает ход. Всего пропусков: " + to_wstring(pl.passCount));

            // Проверка на ничью (3 пропуска подряд всеми игроками)
            if (totalPasses >= numPlayers * 3) {
                skip = true;
                wcout << L"Ничья по троекратным пропускам!\n";
                gameLogger.log(L"Ничья по троекратным пропускам!");
                break;
            }
        }
        else {
            // Обработка хода с буквой
            wstringstream ss(line);
            int x, y; wchar_t c;
            ss >> x >> y >> c;
            if (board.placeLetter(x, y, c, pl)) {
                // Успешный ход - сброс счетчика пропусков
                pl.resetPass();
                totalPasses = 0;
                gameLogger.log(pl.name + L" поставил '" + wstring(1, c) + L"' на позицию (" + to_wstring(x) + L"," + to_wstring(y) + L")");
                gameLogger.log(board.searchText); // Логируем найденное слово
                gameLogger.logBoard(board.grid);  // Логируем текущее состояние поля
            }
            else {
                // Некорректный ход - повтор ввода
                wcout << L"Нажмите Enter..."; wcin.get();
                continue;
            }
        }
        // Проверка условий окончания игры
        if (board.isFull() || !board.hasMovesLeft()) {
            wcout << L"Игра окончена: нет возможных ходов или поле заполнено!\n";
            gameLogger.log(L"Игра окончена!");
            break;
        }
        // Переход хода к следующему игроку
        current = (current + 1) % numPlayers;
    }
    // Определение победителя
    int best = -1;
    wstring winner;
    for (auto& p : players) {
        gameLogger.log(p.name + L": " + to_wstring(p.score) + L" очков");
        if (p.score > best) {
            best = p.score;
            winner = p.name;
        }
    }
    if (skip == false) {
        wcout << L"Победитель: " << winner << L" с " << best << L" очками!\n";
        gameLogger.log(L"Победитель: " + winner + L" с " + to_wstring(best) + L" очками!");
    }
    else {
        gameLogger.log(L"Игра завершилась вничью");
    }
    return 0;
}