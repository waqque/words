#ifndef DFS_H
#define DFS_H

#include <vector>
#include <string>

bool dfs(const std::vector<std::vector<wchar_t>>& grid,
    int x, int y,
    const std::wstring& word,
    int idx,
    std::vector<std::vector<bool>>& visited);

#endif