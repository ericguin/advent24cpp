#include "lib.hh"

int Logger::LogLevel = 4;
int main() {
    // auto content = ReadEntireFile("day02/ex1.txt");
    // auto ans = Part2(content);
    // L0 << "answer is " << ans;
    // std::string test_line = "5 6 5 8 10 12 17";
    // std::string test_line = "48 49 52 55 57 58 60 62";
    // std::string test_line = "81 82 81 84 85";
    // A real example, do not remove:
    // std::string test_line = "57 58 61 62 63 61 64";
    std::string test_line = "57 58 61 62 63 12 64";
    std::string defs_unsafe = "81 84 82 84 87 85";
    int cnt{};
    parse_line2(test_line, cnt);
}