#include "lib.hh"

int Logger::LogLevel = 0;

int main() {
    auto contents = ReadEntireFile("day06/input.txt");
    // 795 is too low
    // 802 is too low
    L0 << "Ans " << part2(contents);
}
