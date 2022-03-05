#include "minesweeper.h"
#include <algorithm>
#include <random>
#include <tuple>
#include <unordered_set>

std::random_device rd;
std::mt19937_64 rg(rd());

void Minesweeper::ClearField() {
    for (size_t i = 0; i < height_; ++i) {
        delete[] field_[i];
    }
    if (height_) {
        delete[] field_;
    }
}

void Minesweeper::AllocateField() {
    field_ = new CellInfo*[height_];
    for (size_t i = 0; i < height_; ++i) {
        field_[i] = new CellInfo[width_];
        for (size_t j = 0; j < width_; ++j) {
            field_[i][j].is_open = field_[i][j].is_marked = field_[i][j].is_mine = false;
            field_[i][j].neighbour_mines = 0;
        }
    }
}

bool Minesweeper::CheckMove(size_t i, size_t j, int di, int dj) {
    return !((i == 0 && di == -1) || (j == 0 && dj == -1) || (i == height_ - 1 && di == 1) ||
             (j == width_ - 1 && dj == 1));
}

void Minesweeper::CountNeighbourMines() {
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (abs(di) + abs(dj) != 0 && CheckMove(i, j, di, dj)) {
                        if (field_[i + di][j + dj].is_mine) {
                            field_[i][j].neighbour_mines++;
                        }
                    }
                }
            }
        }
    }
}

void Minesweeper::ResetTimer() {
    start_time = end_time = std::time(nullptr);
}

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    height_ = width_ = 0;  // initial
    NewGame(width, height, mines_count);
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    height_ = width_ = 0;  // initial
    NewGame(width, height, cells_with_mines);
}

Minesweeper::~Minesweeper() {
    ClearField();
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    ClearField();
    width_ = width;
    height_ = height;
    open_cells_ = 0;
    mines_count_ = mines_count;

    bool* bombs = new bool[width * height];
    for (size_t i = 0; i < mines_count; ++i) {
        bombs[i] = true;
    }
    std::shuffle(bombs, bombs + width * height, rg);

    AllocateField();
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            field_[i][j].is_mine = bombs[i * width + j];
        }
    }
    CountNeighbourMines();

    delete[] bombs;

    is_game_started = true;
    is_first_move_made = false;
    ResetTimer();
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    ClearField();
    width_ = width;
    height_ = height;
    open_cells_ = 0;
    mines_count_ = cells_with_mines.size();

    AllocateField();
    for (const Cell cell : cells_with_mines) {
        field_[cell.y][cell.x].is_mine = true;
    }
    CountNeighbourMines();

    is_game_started = true;
    is_first_move_made = false;
    ResetTimer();
}

void Minesweeper::Defeat() {
    is_game_started = is_first_move_made = false;
    end_time = std::time(nullptr);
    victory = false;
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            field_[i][j].is_open = true;
        }
    }
}

void Minesweeper::Victory() {
    is_game_started = is_first_move_made = false;
    end_time = std::time(nullptr);
    victory = true;
}

size_t Minesweeper::CellHash::operator()(const Cell obj) const {
    return obj.x + 0x9e3779b9 + (obj.y << 6) + (obj.y >> 2);
}

bool Minesweeper::CellEqual::operator()(const Cell a, const Cell b) const {
    return std::tie(a.x, a.y) == std::tie(b.x, b.y);
}

void Minesweeper::OpenCell(const Cell& cell) {
    if (!is_game_started) {
        return;
    }
    if (!is_first_move_made) {
        is_first_move_made = true;
        ResetTimer();
    }
    if (field_[cell.y][cell.x].is_mine) {
        Defeat();
        return;
    }
    std::unordered_set<Cell, CellHash, CellEqual> used_cells;
    std::vector<Cell> cells_queue;
    cells_queue.push_back(cell);
    used_cells.emplace(cell);
    while (!cells_queue.empty()) {
        Cell cur = cells_queue.back();
        cells_queue.pop_back();
        field_[cur.y][cur.x].is_open = true;
        open_cells_++;
        if (open_cells_ + mines_count_ == width_ * height_) {
            Victory();
            return;
        }
        if (field_[cur.y][cur.x].neighbour_mines) {
            continue;
        }
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (abs(dx) + abs(dy) != 0 && CheckMove(cur.y, cur.x, dy, dx)) {
                    if (!field_[cur.y + dy][cur.x + dx].is_mine && !field_[cur.y + dy][cur.x + dx].is_marked &&
                        used_cells.find({cur.x + dx, cur.y + dy}) == used_cells.end()) {

                        cells_queue.push_back({cur.x + dx, cur.y + dy});
                        used_cells.insert({cur.x + dx, cur.y + dy});
                    }
                }
            }
        }
    }
}

void Minesweeper::MarkCell(const Cell& cell) {
    if (!is_game_started) {
        return;
    }
    if (!is_first_move_made) {
        is_first_move_made = true;
        ResetTimer();
    }
    if (!field_[cell.y][cell.x].is_open) {
        field_[cell.y][cell.x].is_marked = true;
    }
}

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    if (is_game_started) {
        return is_first_move_made ? GameStatus::IN_PROGRESS : GameStatus::NOT_STARTED;
    }
    return victory ? GameStatus::VICTORY : GameStatus::DEFEAT;
}

time_t Minesweeper::GetGameTime() const {
    if (is_game_started) {
        return std::time(nullptr) - start_time;
    }

    return end_time - start_time;
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    RenderedField result(height_);
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            char add;
            if (!field_[i][j].is_open) {
                if (field_[i][j].is_marked) {
                    add = '?';
                } else {
                    add = '-';
                }
            } else {
                if (field_[i][j].is_mine) {
                    add = '*';
                } else {
                    add = field_[i][j].neighbour_mines ? '0' + field_[i][j].neighbour_mines : '.';
                }
            }
            result[i] += add;
        }
    }
    return result;
}