#include "draw.hh"
#include "lib.hh"

int Logger::LogLevel = 4;

int main() {
  auto contents = ReadEntireFile("day06/ex01.txt");
  auto grid = make_grid(contents);

  auto guard_start = grid.at(0, 0).value();
  grid.iter_cells([&](auto loc) {
    if (loc.value() == '^') {
      guard_start = loc;
    } else if (loc.value() != '#') {
      contents[loc.index(contents)] = '0';
    }
    return true;
  });

  L1 << "Guard is at: " << guard_start.col << ", " << guard_start.row;

  Guard guard{guard_start, contents};

  InitWindow(800, 450, "raylib [core] example - basic window");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_SPACE)) {
      guard.step();
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_grid(grid);
    draw_guard(guard);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
