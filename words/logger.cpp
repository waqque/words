#include "logger.h"
#include <iomanip>

Logger::Logger(const std::string & filename) {
    logFile.open(filename, std::ios::out | std::ios::app);
    if (logFile.is_open()) {
        logFile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));
        logFile << L"--------Сессия начата--------";
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile << L"--------Сессия завершена--------\n";
        logFile.close();
    }
}

void Logger::log(const std::wstring& message) {//запись текста
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
}

void Logger::logBoard(const std::vector<std::vector<wchar_t>>& grid) {//состояние поля
    if (!logFile.is_open()) return;

    logFile << L"Текущее поле:\n";
    //вывод номеров столбцов
    logFile << L"   ";
    for (size_t j = 0; j < grid[0].size(); ++j) logFile << j << L" ";
    logFile << L"\n";
    // вывод строк с номерами и содержимым
    for (size_t i = 0; i < grid.size(); ++i) {
        logFile << i << L": ";
        for (size_t j = 0; j < grid[i].size(); ++j) {
            logFile << grid[i][j] << L" ";
        }
        logFile << L"\n";
    }
    logFile << L"\n";
}