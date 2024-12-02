#include <cstddef>
#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <functional>
#include <filesystem>
#include <string_view>

struct Logger {
    static int LogLevel;
    int MyLevel;

    Logger(int ml) : MyLevel{ml} {}

    template <typename T>
    Logger& operator <<(T const& val) {
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

inline std::string ReadEntireFile(std::string path) {
    std::fstream stream{path};
    std::ostringstream out{};

    L1 << "Got path? " << path;
    L1 << "My path is " << std::filesystem::current_path();
    L1 << "What is status: " << stream.is_open();

    out << stream.rdbuf();

    return out.str();
}

struct Scanner {
    std::string_view base;
    std::size_t loc = 0;
    Scanner(std::string_view b) : base{b} {}

    bool empty() {
        return loc >= base.size();
    }

    std::size_t skip_wile(std::function<bool(char)> l) {
        if (empty()) return 0;

        auto anchor = loc;
        while (l(base[loc]) && !empty()) {
            loc++;
        }
        return loc - anchor;
    }

    std::optional<std::string_view> take_while(std::function<bool(char)> l) {
        if (empty()) return {};

        std::size_t anchor = loc;
        auto c = skip_wile(l);
        return base.substr(anchor, c);
    }
};

struct Tokenizer {
    Scanner scan;
    char tok;

    Tokenizer(std::string_view s, char tok): scan{s}, tok{tok} {}

    std::optional<std::string_view> next() {
        std::optional<std::string_view> ret{};

        scan.skip_wile([this](char c) {return c == tok;});
        ret = scan.take_while([this](char c) {return c != tok;});

        return ret;
    }

    void iter(std::function<bool(std::string_view)> l) {
        for (auto chunk = next(); chunk.has_value(); chunk = next())
        {
            if (!l(chunk.value())) break;
        }
    }
};
