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

inline void check_enabled(std::regex const& r, std::string const& h, bool& enabled) {
    auto s = std::sregex_iterator(h.begin(), h.end(), r);
    auto e = std::sregex_iterator();


    for (auto i = s; i != e; ++i) {
        L2 << "Got a dude: " << i->str();
        bool dont = i->str()[2] == 'n';
        L2 << "Was a dont? " << dont;
        if (dont) enabled = false;
        else enabled = true;
    }
}

inline int part2_the_dumb_way(std::string const& contents) {
    std::regex mul{"mul\\(([\\d]{1,3}),([\\d]{1,3})\\)"};
    std::regex donts{"(don't\\(\\)|do\\(\\))"};

    auto muls = std::sregex_iterator(contents.begin(), contents.end(), mul);
    auto mule = std::sregex_iterator();
    auto anchor = contents.begin();

    int res{};

    bool enabled{true};

    for (auto i = muls; i != mule; ++i) {
        auto m = *i;

        auto start = anchor;
        auto end = contents.begin() + m.position();

        std::string test{start, end};

        check_enabled(donts, test, enabled);
        L1 << "Enabled? " << enabled;

        auto l = std::stoi(m[1].str());
        auto r = std::stoi(m[2].str());

        if (enabled) res += l * r;

        anchor = contents.begin() + m.position() + m.length();
    }

    return res;
}
