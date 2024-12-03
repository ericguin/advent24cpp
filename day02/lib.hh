#include "common.hh"
#include <vector>
#include <numeric>
#include <string_view>
#include <algorithm>

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
    std::vector<int> trail{};
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

inline int adjacent_count_if(std::vector<int> const& diffs, std::function<bool(int , int)> p) {
    int ret{};
    auto m = diffs.cbegin();
    while (m != diffs.cend()) {
        m = std::adjacent_find(m, diffs.cend(), p);
        ret++;
    }

    return ret - 1;
}

inline bool test_safe(std::vector<int> const& levels, bool recurse = true) {
    // calculate diffs
    std::vector<int> diffs{};
    constexpr const static int max_diff = 3;
    constexpr const static int min_diff = 1;

    std::adjacent_difference(levels.cbegin(), levels.cend(), std::back_inserter(diffs));
    diffs.erase(diffs.begin());
    {
        auto l = L4;
        l << "Diffs are: ";
        std::for_each(diffs.cbegin(), diffs.cend(),
                      [&](auto d) { l << d << " "; });
    }

    int to_remove{0};
    bool increasing{diffs[0] >= 0};

    for (auto dp = diffs.begin(); dp != diffs.end(); ++dp) {
        auto diff = *dp;
        if (diff == 0) to_remove ++;
        else if (abs(diff) > max_diff) {
            auto next = dp + 1;
            to_remove++;
            if (next != diffs.end() && dp != diffs.begin()) {
                if (abs(*next + diff) <= max_diff) {
                    *next += diff;
                } else {
                    // Impossible!
                    return false;
                }
            }
        } else if (diff < 0 && increasing) {
            to_remove ++;
            increasing = false;
        } else if (diff > 0 && !increasing) {
            to_remove ++;
            increasing = true;
        }

        if (to_remove >= 2) return false;
    }

    return true;
}

inline bool parse_line2(std::string_view line, int& safe) {
    Tokenizer levels{line, ' '};
    std::vector<int> parsed_levels{};
    levels.iter([&](auto l) { parsed_levels.push_back(sv_to<int>(l).value()); return true; });

    L1 << "For line: " << line;
    // assume increase

    bool it = test_safe(parsed_levels);

    L1 << "This line was " << ((it) ? "Safe" : "Unsafe");
    if (it) safe ++;
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
