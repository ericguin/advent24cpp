#include <algorithm>
#include <charconv>
#include <cstdint>
#include <string_view>
#include <vector>
#include <ranges>
#include "common.hh"

using ListT = std::vector<int32_t>;

struct Lists {
    ListT first{};
    ListT second{};

    void sort() {
        std::sort(first.begin(), first.end(), [](auto a, auto b) { return a <= b; });
        std::sort(second.begin(), second.end(), [](auto a, auto b) { return a <= b; });
    }
};

inline bool parse_line(std::string_view l, Lists& lists) {
    L1 << "Got a line " << l;
    Scanner ls{l};
    auto first = ls.take_while([](char c) { return c != ' ';}).value();
    ls.skip_wile([](char c){ return c == ' ';});
    auto second = ls.take_while([](char c) { return c != ' ';}).value();
    L1 << "Got the values: " << first << " and " << second;
    std::int32_t fi, si;
    auto fc = std::from_chars(first.data(), first.data() + first.size(), fi);
    auto sc = std::from_chars(second.data(), second.data() + second.size(), si);
    L1 << "Inted those out to: " << fi << " and " << si;
    lists.first.push_back(fi);
    lists.second.push_back(si);
    return true;
}

inline Lists ParseLists(std::string_view content) {
    Lists ret{};
    Tokenizer lines{content, '\n'};

    lines.iter([&](std::string_view l) {return parse_line(l, ret);});

    ret.sort();

    return ret;
}

inline int Part1(Lists const& l) {
    int ans{};

    for (auto pair : std::views::zip(l.first, l.second)) {
        L1 << "Parsing pair: " << std::get<0>(pair) << " - " << std::get<1>(pair);
        auto part = std::abs(std::get<0>(pair) - std::get<1>(pair));
        L1 << "Result is: " << part;
        ans += part;
    }

    return ans;
}

inline int Part2(Lists const& l) {
    int ans{};

    for (auto first : l.first) {
        int count{};

        for (auto second: l.second) {
            if (second == first) count ++;
        }

        ans += first * count;
    }

    return ans;
}