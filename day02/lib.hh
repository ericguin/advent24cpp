#include "common.hh"
#include <deque>
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

inline int adjacent_count_if(std::deque<int> const& diffs, std::function<bool(int , int)> p) {
    int ret{};
    auto m = diffs.cbegin();
    while (m != diffs.cend()) {
        m = std::adjacent_find(m, diffs.cend(), p);
        ret++;
    }

    return ret - 1;
}

inline bool test_safe(std::deque<int> const& levels) {
    // calculate diffs
    std::deque<int> diffs{};
    constexpr const static int max_diff = 3;
    constexpr const static int min_diff = 1;

    std::adjacent_difference(levels.cbegin(), levels.cend(), std::back_inserter(diffs));
    diffs.pop_front();
    {
        auto l = L4;
        l << "Diffs are: ";
        std::for_each(diffs.cbegin(), diffs.cend(),
                      [&](auto d) { l << d << " "; });
    }
    auto too_big = std::count_if(diffs.cbegin(), diffs.cend(), [](auto d){ return std::abs(d) > max_diff; });
    auto too_small = std::count_if(diffs.cbegin(), diffs.cend(), [](auto d){ return std::abs(d) < min_diff; });
    auto increase = std::count_if(diffs.cbegin(), diffs.cend(), [](auto d) { return d > 0; });
    auto decrease = std::count_if(diffs.cbegin(), diffs.cend(), [](auto d) { return d < 0; });
    auto bad_slopes = std::min(increase, decrease);

    auto bad_count = too_big + too_small + bad_slopes;

    L4 << "Bad counts: " << too_big << " " << too_small << " " << bad_slopes;

    return bad_count <= 1;
}

inline bool parse_line2(std::string_view line, int& safe) {
    Tokenizer levels{line, ' '};
    std::deque<int> parsed_levels{};
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