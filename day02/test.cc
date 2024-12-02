#include "lib.hh"

int Logger::LogLevel = 4;
int main() {
    auto content = ReadEntireFile("day02/ex1.txt");
    auto ans = Part2(content);
    L0 << "answer is " << ans;
}