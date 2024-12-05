#include "lib.hh"

int Logger::LogLevel = 0;

int main() {
    auto contents = ReadEntireFile("day05/input.txt");
    auto p1 = part1(contents);
    L0 << "Part 1: " << p1;
}
