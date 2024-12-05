#include "lib.hh"

int Logger::LogLevel = 4;

int main() {
    auto contents = ReadEntireFile("day05/ex01.txt");
    auto res = part2(contents);
    L0 << "My answer is: " << res;
}
