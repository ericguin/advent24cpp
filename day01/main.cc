#include "lib.hh"

int Logger::LogLevel = 0;

int main() {
    auto content = ReadEntireFile("day01/input.txt");
    auto lists = ParseLists(content);
    auto part1 = Part1(lists);
    L0 << "Part 1 answer: " << part1;
    auto part2 = Part2(lists);
    L0 << "Part 2 answer: " << part2;
}