#include "lib.hh"

int Logger::LogLevel = 0;

int main() {
    auto contents = ReadEntireFile("day04/ex01.txt");
    auto res = part2(contents);
    L0 << "Answer is: " << res;
}
