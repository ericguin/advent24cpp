#include "common.hh"
#include <limits>
#include <memory>
#include <algorithm>

using Order = std::pair<int, int>;
using Manual = std::vector<int>;

struct Ordering {
    int begin, end, range;

    Ordering(int b, int e) : begin{b}, end{e} {
        range = e - b + 1;
        entries = std::unique_ptr<Entry[]>(new Entry[range]);

        for (int i = 0; i < range; i ++) {
            entries[i].after = std::unique_ptr<bool[]>(new bool[range]);
            entries[i].before = std::unique_ptr<bool[]>(new bool[range]);
            for (int j = 0; j < range; j ++) {
                entries[i].after[j] = false;
                entries[i].before[j] = false;
            }
        }
    }

    void insert(Order o) {
        int X = std::get<0>(o);
        int Y = std::get<1>(o);

        auto ptrX = X - begin;
        auto ptrY = Y - begin;

        entries[ptrY].exists = true;
        entries[ptrX].exists = true;

        entries[ptrY].after[ptrX] = true;
        entries[ptrX].before[ptrY] = true;
    }

    bool good_order(int a, int b) const {
        auto ptrA = a - begin;
        auto ptrB = b - begin;

        return !entries[ptrA].after[ptrB];
    }

    bool is_before(int a, int b) const {
        auto ptrA = a - begin;
        auto ptrB = b - begin;

        return entries[ptrA].before[ptrB];
    }

    bool is_after(int a, int b) const {
        auto ptrA = a - begin;
        auto ptrB = b - begin;

        return entries[ptrA].after[ptrB];
    }

    void print_order() const {
        for (int i = 0; i < range; i ++) {
            if (entries[i].exists) {
                L4 << "Number " << i + begin << " is after: ";
                for (int j = 0; j < range; j ++) {
                    if (entries[i].after[j]) {
                        L4 << "    " << j + begin;
                    }
                }
            }
        }
    }

    struct Entry {
        bool exists{false};
        std::unique_ptr<bool[]> after;
        std::unique_ptr<bool[]> before;
    };
    std::unique_ptr<Entry[]> entries{};
};

inline std::vector<Order> parse_orders(Tokenizer& tok, int& smallest, int& largest) {
    std::vector<Order> ret{};

    smallest = std::numeric_limits<int>::max();
    largest = 0;

    std::optional<std::string_view> cur = tok.next();
    while (cur.has_value() && cur->size() > 0) {
        int X, Y;
        Scanner bit{cur.value()};
        X = sv_to<int>(bit.take_while([](char c) { return c != '|'; }).value()).value();
        bit.skip_wile([](char c) { return c == '|'; });
        Y = sv_to<int>(bit.take_while([](char c) { return c != '|'; }).value()).value();
        ret.emplace_back(X, Y);
        auto big = std::max(X, Y);
        auto smol = std::min(X, Y);

        if (big > largest) largest = big;
        if (smol < smallest) smallest = smol;

        cur = tok.next();
    }

    return ret;
}

inline std::vector<Manual> parse_manuals(Tokenizer& tok) {
    std::vector<Manual> ret{};

    std::optional<std::string_view> cur = tok.next();
    while (cur.has_value()) {
        Tokenizer pages{cur.value(), ','};
        auto& man = ret.emplace_back();
        pages.iter([&](auto l) {
            int page = sv_to<int>(l).value();
            man.push_back(page);

            return true;
        });
        cur = tok.next();
    }

    return ret;
}

inline Ordering gen_ordering(std::vector<Order> const& orders, int lower, int upper) {
    Ordering ret{lower, upper};

    for (auto const& o: orders) {
        ret.insert(o);
    }

    return ret;
}

inline int get_middle(Manual const& man) {
    auto mid = man.begin() + man.size() / 2;
    return *mid;
}


inline auto bad_index(auto begin, auto end, Ordering const& order) {
    for (auto anchor = begin; anchor != end; anchor ++) {
        for (auto cursor = std::next(anchor); cursor != end; cursor++) {
            if (!order.good_order(*anchor, *cursor)) {
                return anchor;
                break;
            }
        }
    }

    return end;
}

inline auto bad_index(Manual const& manual, Ordering const& order) {
    return bad_index(manual.cbegin(), manual.cend(), order);
}

inline int verify_manuals(std::vector<Manual> const& manuals, Ordering const& order) {
    int ret{0};

    for (auto const& manual : manuals) {
        auto bad = bad_index(manual, order);
        if (bad == manual.cend()) ret += get_middle(manual);
    }

    return ret;
}


inline int fix_manual(Manual manual, Ordering const& order) {
    int ret{};
    bool oof{false};

    L1 << "Fixing manual";

    for (auto bad = bad_index(manual.begin(), manual.end(), order); bad != manual.end(); bad = bad_index(manual.begin(), manual.end(), order)) {
        auto log = L2;
        oof = true;
        L2 << ", is bad: " << *bad << ", ";
        
        // are there ones I should be after to my right
        auto most_after = manual.end();
        for (auto cursor = bad; cursor != manual.end(); cursor ++) {
            if (order.is_after(*bad, *cursor)) most_after = cursor;
        }

        if (most_after != manual.end()) {
            L1 << "There's one that shoulda dun been before us " << *most_after;
            std::iter_swap(bad, most_after);
        }
    }

    if (oof) {
        auto log = L1;
        log << "is good now: ";
        for (auto const& i : manual) {
            log << ", " << i;
        }
        ret += get_middle(manual);
    }


    return ret;
}

inline int fix_manuals(std::vector<Manual> const& manuals, Ordering const& order) {
    int ret{};

    for (auto const& manual : manuals) {
        ret += fix_manual(manual, order);
    }

    return ret;
}

inline int part1(std::string const& contents) {
    Tokenizer lines{contents, '\n'};
    lines.greedy = false;

    int lower,upper;
    auto orders = parse_orders(lines, lower, upper);
    auto manuals = parse_manuals(lines);
    auto ordering = gen_ordering(orders, lower, upper);
    auto good = verify_manuals(manuals, ordering);

    return good;
}

inline int part2(std::string const& contents) {
    Tokenizer lines{contents, '\n'};
    lines.greedy = false;
    int ret{};

    int lower,upper;
    auto orders = parse_orders(lines, lower, upper);
    auto manuals = parse_manuals(lines);
    auto ordering = gen_ordering(orders, lower, upper);
    ret += fix_manuals(manuals, ordering);

    return ret;
}
