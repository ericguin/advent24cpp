#include "common.hh"
#include <deque>
#include <string_view>

inline bool level_pass(int last, int lev, bool increasing) {
    auto diff = last - lev;
    if (increasing) diff *= -1;
    return  diff <= 3 && diff > 0;
}

inline bool parse_line(std::string_view line, int& safe) {
    Tokenizer levels(line, ' ');
    std::optional<bool> increasing{};
    bool good{true};
    L1 << "Parsing line: " << line;
    auto first = levels.next().value();
    std::deque<int> trail{};
    trail.push_back(sv_to<int>(first).value());
    levels.iter([&](auto l) {
        int lev = sv_to<int>(l).value();
        if (!increasing.has_value()) {
            if (lev > trail.back()) increasing = true;
            else increasing = false;
        }
        if (level_pass(trail.back(), lev, increasing.value())) {
            trail.push_back(lev);
            return true;
        } else {
            good = false;
            return false;
        }
    });
    if (good) safe++;
    return true;
}

inline int Part1(std::string_view in) {
    Tokenizer lines{in, '\n'};
    int safe{};
    lines.iter([&](auto l) {return parse_line(l, safe);});
    return safe;
}

inline int Part2(std::string_view in) {
    Tokenizer lines{in, '\n'};
    int safe{};
    lines.iter([&](auto l) {return parse_line(l, safe);});
    return safe;
}