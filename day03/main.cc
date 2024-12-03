#include "lib.hh"

int Logger::LogLevel = 1;

int main() {
    auto contents = ReadEntireFile("day03/input.txt");
    int res = part1_the_dumb_way(contents);
    L0 << "Answer is: " << res;
    int two = part2_the_dumb_way(contents);
    L0 << "Answer is: " << two;
}
