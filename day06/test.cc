#include "lib.hh"

int Logger::LogLevel = 4;

int main() {
    auto contents = ReadEntireFile("day06/ex01.txt");
    //L0 << "Part 1: " << part1(contents);
    L0 << "Part 2: " << part2(contents);
    L0 << contents;
}
