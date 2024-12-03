#include "common.hh"
#include <regex>

inline int part1_the_dumb_way(std::string const& contents) {
    std::regex mul{"mul\\(([\\d]{1,3}),([\\d]{1,3})\\)"};

    auto muls = std::sregex_iterator(contents.begin(), contents.end(), mul);
    auto mule = std::sregex_iterator();

    int res{};

    for (auto i = muls; i != mule; ++i) {
        auto m = *i;
        auto l = std::stoi(m[1].str());
        auto r = std::stoi(m[2].str());

        res += l * r;
    }

    return res;
}