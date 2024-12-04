#include "lib.hh"

int Logger::LogLevel = 1;

int main() {
    auto contents = ReadEntireFile("day04/tiny.txt");
    auto res = part1(contents);
    L0 << "Answer is: " << res;
}
