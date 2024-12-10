#include "common.hh"

constexpr static const char test1 = '0' | 0b0011;
constexpr static const char test2 = '0' | 0b0110;
constexpr static const char test3 = '0' | 0b1100;
constexpr static const char test4 = '0' | 0b1001;

struct Guard {
  Grid::Iterator pos;
  std::string &map;
  Grid::Iterator::Direction dir{Grid::Iterator::Direction::UP};

  void mark_direction() {
    using d = Grid::Iterator::Direction;
    switch (dir) {
    case d::UP:
      map[pos.index(map)] |= 0b0001;
      break;
    case d::RIGHT:
      map[pos.index(map)] |= 0b0010;
      break;
    case d::DOWN:
      map[pos.index(map)] |= 0b0100;
      break;
    case d::LEFT:
      map[pos.index(map)] |= 0b1000;
      break;
    }
  }

  static bool has_direction(char c, Grid::Iterator::Direction dir) {
    using d = Grid::Iterator::Direction;
    if ((c & 0b10000) == 0)
      return false;
    switch (dir) {
    case d::UP:
      return (c & 0b0001) > 0;
      break;
    case d::RIGHT:
      return (c & 0b0010) > 0;
      break;
    case d::DOWN:
      return (c & 0b0100) > 0;
      break;
    case d::LEFT:
      return (c & 0b1000) > 0;
      break;
    }

    return false;
  }

  static bool has_been_visited(char c) { return (c & 0b1111) > 0; }

  void mark_near_miss() {
    auto if_i_turn = Grid::Iterator::Rotate90Right(dir);
    // We probably need to iterate until we hit an obstacle or
    // leave the square
    auto that_cell = pos.move_into(if_i_turn);

    if (that_cell.has_value()) {
      if (has_direction(that_cell->value(), if_i_turn)) {
        auto in_front_of_me = pos.move_into(dir);
        if (in_front_of_me.has_value() &&
            !has_been_visited(in_front_of_me->value())) {
          map[in_front_of_me->index(map)] |= 0b1 << 6;
        }
      }
    }
  }

  void mark_direct_hit() {}

  void place_possible_mark() {}

  bool step() {
    if (pos.value() == '.' || pos.value() == '^')
      map[pos.index(map)] = '0';
    auto next = pos.move_into(dir);
    if (next.has_value()) {
      auto nextpos = pos;
      if (next->value() == '#') {
        dir = Grid::Iterator::Rotate90Right(dir);
      } else {
        nextpos = next.value();
      }

      mark_direction();
      mark_near_miss();
      mark_direct_hit();

      pos = nextpos;
    } else {
      return false;
    }

    return true;
  }

  void walk() {
    while (step()) {
    }
  }
};

inline Grid make_grid(std::string &contents) {
  Tokenizer lines{contents, '\n'};
  Grid ret{};
  lines.iter([&](auto l) {
    ret.lines.push_back(l);
    return true;
  });
  return ret;
}

inline int part1(std::string &contents) {
  auto grid = make_grid(contents);
  auto guard_start = grid.at(0, 0).value();
  grid.iter_cells([&](auto loc) {
    if (loc.value() == '^') {
      guard_start = loc;
      return false;
    }
    return true;
  });

  L1 << "Guard is at: " << guard_start.col << ", " << guard_start.row;

  Guard guard{guard_start, contents};

  guard.walk();

  int res{};
  grid.iter_cells([&](auto loc) {
    if (loc.value() >= '0') {
      res++;
    }
    return true;
  });

  return res;
}

inline bool bit_test(char c, char t) { return ((c & t) == t); }

inline int part2(std::string &contents) {
  auto grid = make_grid(contents);
  auto guard_start = grid.at(0, 0).value();
  grid.iter_cells([&](auto loc) {
    if (loc.value() == '^') {
      guard_start = loc;
      return false;
    }
    return true;
  });

  L1 << "Guard is at: " << guard_start.col << ", " << guard_start.row;

  Guard guard{guard_start, contents};

  guard.walk();

  int res{};
  L1 << test1 << test2 << test3 << test4;
  grid.iter_cells([&](auto loc) {
    auto c = loc.value();
    // if (bit_test(c, test1) || bit_test(c, test2) || bit_test(c, test3) ||
    //     bit_test(c, test4)) {
    //   res++;
    // }
    if ((c & 0b1000000) > 0) {
      if (loc.col == guard_start.col && loc.row == guard_start.row)
        return true;
      res++;
    }
    return true;
  });

  return res;
}
