#include "common.hh"

inline Grid make_grid(std::string const& contents) {
    Tokenizer lines{contents, '\n'};
    Grid ret{};
    lines.iter([&](auto l) {
            ret.lines.push_back(l);
            return true;
            });
    return ret;
}

inline int part1(std::string const& contents) {
    Grid grid = make_grid(contents);
    int ret{};

    grid.iter_cells([&](Grid::Iterator c) { if (c.value() == 'X') ret += c.scanall("XMAS"); return true; });

    return ret;
}
