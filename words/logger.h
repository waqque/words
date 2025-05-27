#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>

class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();
    void log(const std::wstring& message);//запись текста
    void logBoard(const std::vector<std::vector<wchar_t>>& grid);//состояние игрового поля

private:
    std::wofstream logFile;
};

#endif