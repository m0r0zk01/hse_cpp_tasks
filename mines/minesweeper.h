#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    struct CellInfo {
        bool is_open;
        bool is_mine;
        bool is_marked;
        int neighbour_mines;
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);
    ~Minesweeper();

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

    bool is_game_started;
    bool is_first_move_made;
    bool victory;
    std::time_t start_time;
    std::time_t end_time;

private:
    void ClearField();
    void AllocateField();
    void CountNeighbourMines();
    void ResetTimer();
    void Defeat();
    void Victory();

    bool CheckMove(size_t i, size_t j, int di, int dj);

    struct CellHash {
        size_t operator()(const Cell obj) const;
    };
    struct CellEqual {
        bool operator()(const Cell a, const Cell b) const;
    };

    size_t width_;
    size_t height_;
    size_t open_cells_;
    size_t mines_count_;
    CellInfo** field_;
};
