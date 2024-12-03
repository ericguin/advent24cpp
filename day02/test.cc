#include "lib.hh"

int Logger::LogLevel = 4;
int main() {
    // auto content = ReadEntireFile("day02/ex1.txt");
    // auto ans = Part2(content);
    // L0 << "answer is " << ans;
    std::string test_line = "5 6 5 8 10 12 17";
    int cnt{};
    parse_line2(test_line, cnt);
}