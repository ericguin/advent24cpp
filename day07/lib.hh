#include "common.hh"

struct Equation {
    int answer;
    std::vector<int> operands{};

    bool is_possible() {
        int ops{};

        return false;
    }
};

inline Equation parse_line(std::string_view line) {
    Scanner s{line};
    auto answer = sv_to<int>(s.take_while([](char c) { return c != ':';}).value()).value();
    L4 << "Eqn: " << answer;
    s.skip_wile([](char c) { return c == ':';});
    Tokenizer ops{s.remainder(), ' '};
    Equation ret{answer};
    ops.iter([&](auto op) {
        L4 << "    " << op;
        auto opv = sv_to<int>(op).value();
        ret.operands.push_back(opv);
        return true;
    });

    return ret;
}

inline void part1(std::string const& input) {
    Tokenizer lines{input, '\n'};

    lines.iter([&](auto l) {
        parse_line(l);
        return true;
    });
}