#include "lib.hh"

int Logger::LogLevel = 1;

int main() {
    auto contents = ReadEntireFile("day03/ex02.txt");
    int res = part2_the_dumb_way(contents);
    L0 << "Answer is: " << res;
}
