#include "logger.h"
#include <iomanip>

Logger::Logger(const std::string & filename) {
    logFile.open(filename, std::ios::out | std::ios::app);
    if (logFile.is_open()) {
        logFile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));
        logFile << L"--------������ ������--------";
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile << L"--------������ ���������--------\n";
        logFile.close();
    }
}

void Logger::log(const std::wstring& message) {//������ ������
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
}

void Logger::logBoard(const std::vector<std::vector<wchar_t>>& grid) {//��������� ����
    if (!logFile.is_open()) return;

    logFile << L"������� ����:\n";
    //����� ������� ��������
    logFile << L"   ";
    for (size_t j = 0; j < grid[0].size(); ++j) logFile << j << L" ";
    logFile << L"\n";
    // ����� ����� � �������� � ����������
    for (size_t i = 0; i < grid.size(); ++i) {
        logFile << i << L": ";
        for (size_t j = 0; j < grid[i].size(); ++j) {
            logFile << grid[i][j] << L" ";
        }
        logFile << L"\n";
    }
    logFile << L"\n";
}