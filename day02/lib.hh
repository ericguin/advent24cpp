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

inline bool test_safe(std::deque<int> const& levels, bool increasing) {
    bool damp = true;
    auto cursor = levels.cbegin() + 1;
    while (cursor != levels.cend()) {
        auto compare = cursor - 1;

        if (!level_pass(*(compare), *cursor, increasing)) {
            if (damp) {
                damp = false;
                L3 << "Damping";
                if (compare == levels.cbegin()) {
                    // assume skip first?
                    L3 << "We are assuming the first value is bad, move on";
                } else if (cursor + 1 == levels.cend()) {
                    L3 << "Assuming last value is bad, move on";
                } else {
                    auto last = compare - 1;
                    L3 << "Re-checking last " << *(last) << " with " << *cursor;
                    auto is_last_bad = level_pass(*(last), *(cursor), increasing);
                    if (!is_last_bad) {
                        auto next = cursor + 1;
                        L3 << "Re-checking next " << *compare << " with " << *next;
                        auto am_i_bad = level_pass(*(compare), *(next), increasing);
                        if (!am_i_bad) {
                            return false;
                        }
                    }
                }
            } else {
                return false;
            }
        }

        cursor ++;
    }
    return true;
}

inline bool parse_line2(std::string_view line, int& safe) {
    Tokenizer levels{line, ' '};
    std::deque<int> parsed_levels{};
    levels.iter([&](auto l) { parsed_levels.push_back(sv_to<int>(l).value()); return true; });

    L1 << "For line: " << line;
    // assume increase

    bool it = test_safe(parsed_levels, true);
    L2 << "Increase test: " << it;

    // assume decrease
    bool dt = test_safe(parsed_levels, false);
    L2 << "Decrease test: " << dt;

    if (it | dt) safe ++;

    L1 << "This line was " << ((it | dt) ? "Safe" : "Unsafe");
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
    lines.iter([&](auto l) {return parse_line2(l, safe);});
    return safe;
}