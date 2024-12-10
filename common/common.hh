#pragma once
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <sstream>
#include <string_view>

// haha oh no: https://stackoverflow.com/a/25115163
inline std::ostream &operator<<(std::ostream &dest, __int128_t value) {
  std::ostream::sentry s(dest);
  if (s) {
    __uint128_t tmp = value < 0 ? -value : value;
    char buffer[128];
    char *d = std::end(buffer);
    do {
      --d;
      *d = "0123456789"[tmp % 10];
      tmp /= 10;
    } while (tmp != 0);
    if (value < 0) {
      --d;
      *d = '-';
    }
    int len = std::end(buffer) - d;
    if (dest.rdbuf()->sputn(d, len) != len) {
      dest.setstate(std::ios_base::badbit);
    }
  }
  return dest;
}

struct Logger {
  static int LogLevel;
  int MyLevel;

  Logger(int ml) : MyLevel{ml} {}

  template <typename T> Logger &operator<<(T const &val) {
    if (MyLevel <= LogLevel)
      std::cout << val;

    return *this;
  }

  ~Logger() {
    if (MyLevel <= LogLevel)
      std::cout << "\n";
  }
};

#define L0 Logger{0}
#define L1 Logger{1}
#define L2 Logger{2}
#define L3 Logger{3}
#define L4 Logger{4}

inline std::string ReadEntireFile(std::string const &path) {
  std::fstream stream{path};
  std::ostringstream out{};

  if (bool{stream}) {
    out << stream.rdbuf();

    return out.str();
  }
  L0 << "File " << path << " not found";
  std::exit(1);
}

struct Scanner {
  std::string_view base;
  std::size_t loc = 0;
  Scanner(std::string_view b) : base{b} {}

  bool empty() { return loc >= base.size(); }

  std::size_t skip_wile(std::function<bool(char)> l) {
    if (empty())
      return 0;

    auto anchor = loc;
    while (l(base[loc]) && !empty()) {
      loc++;
    }
    return loc - anchor;
  }

  std::optional<std::string_view> take_while(std::function<bool(char)> l) {
    if (empty())
      return {};

    std::size_t anchor = loc;
    auto c = skip_wile(l);
    return base.substr(anchor, c);
  }

  std::string_view remainder() { return base.substr(loc); }

  bool advance_if(std::function<bool(char)> l) {
    if (empty())
      return false;
    if (l(base[loc]))
      loc++;
    return true;
  }
};

struct Tokenizer {
  Scanner scan;
  char tok;
  bool greedy{true};

  Tokenizer(std::string_view s, char tok) : scan{s}, tok{tok} {}

  std::optional<std::string_view> next() {
    std::optional<std::string_view> ret{};

    if (greedy)
      scan.skip_wile([this](char c) { return c == tok; });
    else
      scan.advance_if([this](char c) { return c == tok; });

    ret = scan.take_while([this](char c) { return c != tok; });

    return ret;
  }

  void iter(std::function<bool(std::string_view)> l) {
    for (auto chunk = next(); chunk.has_value(); chunk = next()) {
      if (!l(chunk.value()))
        break;
    }
  }
};

template <typename T>
inline std::optional<T> sv_to(std::string_view const &sv) {
  T to{};
  auto res = std::from_chars(sv.data(), sv.data() + sv.size(), to);
  if (res.ptr == (sv.data() + sv.size())) {
    return to;
  }
  return {};
}

struct Grid {
  int columns{};
  std::vector<std::string_view> lines{};

  bool position_valid(int col, int row) {
    if (row >= lines.size())
      return false;
    if (col >= lines[0].size())
      return false;
    return true;
  }

  struct Iterator {
    int col, row;
    Grid &parent;

    Iterator &operator=(Iterator const &other) {
      col = other.col;
      row = other.row;
      parent = other.parent;
      return *this;
    }

    char value() const { return parent.lines[row][col]; }

    std::size_t index(std::string const &parent_we_hope) {
      // This is probably less stupid than it looks :)
      // The only way I can think of to reliably index back
      // into the string from whence we generated this string_view
      // is to compare their data pointers, else we might mis
      // calculate the offset based on what characters were
      // excluded during its creation
      auto uh = parent.lines[row].data() + col;
      return uh - parent_we_hope.c_str();
    }

    bool equals(Iterator const &other) {
      return col == other.col && row == other.row;
    }

    enum DirBase { L = 3, H = 2, R = 1, U = 3 << 2, V = 2 << 2, D = 1 << 2 };

    enum Direction {
      LEFT = L | V,
      RIGHT = R | V,
      UP = U | H,
      DOWN = D | H,
      UPLEFT = U | L,
      UPRIGHT = U | R,
      DOWNLEFT = D | L,
      DOWNRIGHT = D | R
    };

    static auto SplitDirection(Direction in) {
      auto dir = static_cast<int>(in);
      auto horiz = dir & 0b11;
      auto vert = (dir >> 2) & 0b11;
      return std::make_tuple(horiz, vert);
    }

    static Direction Rotate90Right(Direction in) {
      // SO MUNTED DEAL WITH IT
      switch (in) {
      case UP:
        return RIGHT;
      case RIGHT:
        return DOWN;
      case DOWN:
        return LEFT;
      case LEFT:
        return UP;
      }

      return in;
    }

    inline static const Direction Directions[] = {
        LEFT, RIGHT, UP, DOWN, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT};

    bool move(Direction dir) {
      int nc = col;
      int nr = row;

      int horiz = 2 - (dir & 0b11);
      int vert = 2 - ((dir >> 2) & 0b11);

      nc += horiz;
      nr += vert;

      if (parent.position_valid(nc, nr)) {
        col = nc;
        row = nr;
        return true;
      }

      return false;
    }

    std::optional<Iterator> move_into(Direction dir) const {
      int nc = col;
      int nr = row;

      int horiz = 2 - (dir & 0b11);
      int vert = 2 - ((dir >> 2) & 0b11);

      nc += horiz;
      nr += vert;

      if (parent.position_valid(nc, nr)) {
        Iterator out = *this;
        out.col = nc;
        out.row = nr;
        return out;
      }

      return {};
    }

    bool scan(Direction dir, std::string_view tgt) {
      auto cursor = tgt.begin();

      while (cursor != tgt.end()) {
        if (*cursor == value()) {
          if (!move(dir))
            return false;
          cursor++;
        } else {
          return false;
        }
      }

      return true;
    }

    int scanall(std::string_view tgt) {
      int ret{0};
      for (auto dir : Directions) {
        if (scan(dir, tgt))
          ret++;
      }
      return ret;
    }
  };

  std::optional<Iterator> at(int col, int row) {
    if (!position_valid(col, row))
      return {};
    return Iterator{col, row, *this};
  }

  void iter_cells(std::function<bool(Iterator)> p) {
    Iterator cursor = at(0, 0).value();

    while (true) {
      if (!p(cursor))
        break;
      if (!cursor.move(Iterator::RIGHT)) {
        if (position_valid(0, cursor.row + 1)) {
          cursor.col = 0;
          cursor.row = cursor.row + 1;
        } else {
          break;
        }
      }
    }
  }
};

template <typename T>
inline bool all_have_value(std::initializer_list<std::optional<T>> Args) {
  bool ret{true};
  for (auto arg : Args) {
    ret &= arg.has_value();
  }
  return ret;
}
