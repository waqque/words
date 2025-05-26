#include "dfs.h"
#include <vector>
#include <string>

bool dfs(const std::vector<std::vector<wchar_t>>& grid,
    int x, int y,
    const std::wstring& word,
    int idx,
    std::vector<std::vector<bool>>& visited) {
    // Проверка совпадения текущей буквы
    if (grid[x][y] != word[idx]) return false;
    // Если дошли до конца слова - круто
    if (idx + 1 == word.size()) return true;
    // Помечаем текущую клетку как посещенную
    visited[x][y] = true;
    // Возможные направления движения (вверх, вниз, влево, вправо)
    static int dx[4] = { 1, -1, 0, 0 };
    static int dy[4] = { 0, 0, 1, -1 };
    // Проверка всех соседних клеток
    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d], ny = y + dy[d];
        // Проверка границ поля
        if (nx >= 0 && nx < (int)grid.size() &&
            ny >= 0 && ny < (int)grid[0].size() &&
            !visited[nx][ny]) {
            if (dfs(grid, nx, ny, word, idx + 1, visited)) {
                return true;
            }
        }
    }
    // снятие отметки о посещении
    visited[x][y] = false;
    return false;
}
