#include "lib.hh"

int Logger::LogLevel = 3;

int main() {
    auto contents = ReadEntireFile("day07/ex01.txt");
    auto p1 = part1(contents);;
    L0 << "Part 1: " << p1;
}
