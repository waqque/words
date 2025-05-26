#include "dfs.h"
#include <vector>
#include <string>

bool dfs(const std::vector<std::vector<wchar_t>>& grid,
    int x, int y,
    const std::wstring& word,
    int idx,
    std::vector<std::vector<bool>>& visited) {
    if (grid[x][y] != word[idx]) return false;
    if (idx + 1 == word.size()) return true;
    visited[x][y] = true;
    static int dx[4] = { 1, -1, 0, 0 };
    static int dy[4] = { 0, 0, 1, -1 };
    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx >= 0 && nx < (int)grid.size() &&
            ny >= 0 && ny < (int)grid[0].size() &&
            !visited[nx][ny]) {
            if (dfs(grid, nx, ny, word, idx + 1, visited)) {
                return true;
            }
        }
    }
    visited[x][y] = false;
    return false;
}