#include "common.hh"

struct Visualizer {
    struct Attempt {
        Grid::Iterator start;
        std::vector<Grid::Iterator> tasted{};

        void draw() {
            start.parent.iter_cells([&](Grid::Iterator it) {
                if (it.equals(start)) {
                    std::cout << "Found start\n";
                }

                return true;
            });
        }
    };
    std::vector<Attempt> attempts{};
};

inline Grid make_grid(std::string const& contents) {
    Tokenizer lines{contents, '\n'};
    Grid ret{};
    lines.iter([&](auto l) {
            ret.lines.push_back(l);
            return true;
            });
    return ret;
}

inline int attempt(Grid::Iterator it, Visualizer& vis) {
    std::string_view tgt{"XMAS"};
    int res{};

    auto vis_attempt = vis.attempts.emplace_back(it);
    L1 << "Running attempt: " << it.col << ":" << it.row;

    for (auto dir : Grid::Iterator::Directions) {
        auto cursor = it;
        auto curchar = std::next(tgt.begin());

        L2 << "Direction is: " << dir;

        while (curchar != tgt.end() && cursor.move(dir) && cursor.value() == *curchar) {
            L2 << "Tasted " << *curchar << " at " << cursor.col << ":" << cursor.row;
            vis_attempt.tasted.push_back(it);
            curchar++;
        }

        if (curchar == tgt.end()) res++;
    }

    return res;
}

inline int attempt2(Grid::Iterator it, Visualizer& vis) {
    int res{};

    auto vis_attempt = vis.attempts.emplace_back(it);

    auto upl = it.move_into(Grid::Iterator::UPLEFT);
    auto upr = it.move_into(Grid::Iterator::UPRIGHT);
    auto dnl = it.move_into(Grid::Iterator::DOWNLEFT);
    auto dnr = it.move_into(Grid::Iterator::DOWNRIGHT);

    bool possible = all_have_value({upl, upr, dnl, dnr});

    if (possible) {
        if (((upl->value() == 'S' && dnr->value() == 'M') ||
                (upl->value() == 'M' && dnr->value() == 'S'))
                &&
                ((upr->value() == 'S' && dnl->value() == 'M') ||
                (upr->value() == 'M' && dnl->value() == 'S'))) {
            res++;
        }
    }

    return res;
}

inline int part1(std::string const& contents) {
    Grid grid = make_grid(contents);
    Visualizer vis{};
    int ret{};

    grid.iter_cells([&](Grid::Iterator c) { if (c.value() == 'X') ret += attempt(c, vis); return true; });

    return ret;
}

inline int part2(std::string const& contents) {
    Grid grid = make_grid(contents);
    Visualizer vis{};
    int ret{};

    grid.iter_cells([&](Grid::Iterator c) { if (c.value() == 'A') ret += attempt2(c, vis); return true; });

    return ret;
}
