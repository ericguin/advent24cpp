#include "lib.hh"

int Logger::LogLevel = 4;

int main() {
    auto contents = ReadEntireFile("day07/ex01.txt");
    part1(contents);
}
