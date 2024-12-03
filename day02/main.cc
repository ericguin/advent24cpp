#include "lib.hh"

int Logger::LogLevel = 1;

int main() {
    auto content = ReadEntireFile("day02/input.txt");
    // auto ans = Part1(content);
    // L0 << "answer is " << ans;
    auto ans = Part2(content);
    L0 << "answer is " << ans;
}