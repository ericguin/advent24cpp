#include "common.hh"

struct Guard {
    Grid::Iterator pos;
    std::string& map;
    Grid::Iterator::Direction dir{Grid::Iterator::Direction::UP};

    void mark_direction() {
        using d = Grid::Iterator::Direction;
        switch (dir) {
            case d::UP:
                map[pos.index(map)] |= 0b0001;
                break;
            case d::RIGHT:
                map[pos.index(map)] |= 0b0010;
                break;
            case d::DOWN:
                map[pos.index(map)] |= 0b0100;
                break;
            case d::LEFT:
                map[pos.index(map)] |= 0b1000;
                break;
        }
    }

    void walk() {
        while (true) {
            if (pos.value() == '.' || pos.value() == '^') map[pos.index(map)] = '0';
            mark_direction();
            auto next = pos.move_into(dir);
            if (next.has_value()) {
                if (next->value() == '#') {
                    dir = Grid::Iterator::Rotate90Right(dir);
                } else {
                    pos = next.value();
                }
            } else {
                return;
            }
        }
    }
};

inline Grid make_grid(std::string& contents) {
    Tokenizer lines{contents, '\n'};
    Grid ret{};
    lines.iter([&](auto l) {
            ret.lines.push_back(l);
            return true;
            });
    return ret;
}

inline int part1(std::string& contents) {
    auto grid = make_grid(contents);
    auto guard_start = grid.at(0, 0).value();
    grid.iter_cells([&](auto loc) {
        if (loc.value() == '^') {
            guard_start = loc;
            return false;
        }
        return true;
    });

    L1 << "Guard is at: " << guard_start.col << ", " << guard_start.row;

    Guard guard{guard_start, contents};

    guard.walk();

    int res{};
    grid.iter_cells([&](auto loc) {
        if (loc.value() >= '0' && loc.value() <= '?') {
            res++;
        }
        return true;
    });

    return res;
}

inline int part2(std::string& contents) {
    auto grid = make_grid(contents);
    auto guard_start = grid.at(0, 0).value();
    grid.iter_cells([&](auto loc) {
        if (loc.value() == '^') {
            guard_start = loc;
            return false;
        }
        return true;
    });

    L1 << "Guard is at: " << guard_start.col << ", " << guard_start.row;

    Guard guard{guard_start, contents};

    guard.walk();

    int res{};
    constexpr static const char test1 = '0' | 0b0011;
    constexpr static const char test2 = '0' | 0b0110;
    constexpr static const char test3 = '0' | 0b1100;
    constexpr static const char test4 = '0' | 0b1001;
    L1 << test1 << test2 << test3 << test4;
    grid.iter_cells([&](auto loc) {
        auto c = loc.value();
        if (c == test1 || c == test2 || c == test3 || c == test4) {
            res++;
        }
        return true;
    });

    return res;
}