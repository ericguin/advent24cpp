#include "lib.hh"

int Logger::LogLevel = 0;

int main() {
    auto contents = ReadEntireFile("day05/input.txt");
    L0 << "Part 1";
    auto p1 = part1(contents);
    L0 << "Part 1: " << p1;
    auto p2 = part2(contents);
    L0 << "Part 2: " << p2;
}
