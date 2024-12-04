#include "lib.hh"

int Logger::LogLevel = 0;

int main() {
    auto contents = ReadEntireFile("day04/input.txt");
    auto res = part1(contents);
    L0 << "Answer is: " << res;
    auto res2 = part2(contents);
    L0 << "Answer is: " << res2;
}
