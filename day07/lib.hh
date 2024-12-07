#include "common.hh"
#include <cmath>

struct Equation {
    __int128 answer;
    std::vector<__int128> operands{};

    auto operate(bool op, __int128 lhs, __int128 rhs) {
        if (op) {
            return rhs + lhs;
        } else {
            return rhs * lhs;
        }
    }

    bool is_possible() {
        bool possible{false};
        int count = std::pow(operands.size() - 1, 2);

        for (int i = 0; i < count; i ++) {
            auto lhs = operands[0];
            for (int j = 1; j < operands.size(); j ++) {
                auto op = (i >> (j - 1)) & 0b1;
                lhs = operate(op, lhs, operands[j]);
            }
            if (lhs == answer) {
                possible = true;
            } else {
            }
        }

        return possible;
    }
};

inline Equation parse_line(std::string_view line) {
    Scanner s{line};
    auto answer = sv_to<__int128>(s.take_while([](char c) { return c != ':';}).value()).value();
    s.skip_wile([](char c) { return c == ':';});
    Tokenizer ops{s.remainder(), ' '};
    Equation ret{answer};
    ops.iter([&](auto op) {
        auto opv = sv_to<__int128>(op).value();
        ret.operands.push_back(opv);
        return true;
    });

    return ret;
}

inline auto part1(std::string const& input) {
    Tokenizer lines{input, '\n'};
    __int128 ret{};

    lines.iter([&](auto l) {
        auto eqn = parse_line(l);
        if (eqn.is_possible()) ret += eqn.answer;
        return true;
    });

    return ret;
}