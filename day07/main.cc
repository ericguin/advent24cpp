#include "lib.hh"

int Logger::LogLevel = 1;

int main() {
  auto contents = ReadEntireFile("day07/input.txt");
  auto p1 = part1(contents);
  L0 << "Part 1: " << p1;
}
