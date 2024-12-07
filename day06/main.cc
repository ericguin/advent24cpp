#include "lib.hh"

int Logger::LogLevel = 0;

int main() {
    auto contents = ReadEntireFile("day06/input.txt");
    L0 << "Part 1: " << part1(contents);
}
