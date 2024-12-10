#include "draw.hh"
#include "lib.hh"
#include <raylib.h>

int Logger::LogLevel = 0;

int main() {
  auto contents = ReadEntireFile("day06/input.txt");
  // 795 is too low
  // 802 is too low
  L0 << "Ans " << part2(contents);
  constexpr const auto screenWidth = 800.0f;
  constexpr const auto screenHeight = 450.0f;
  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  SetTargetFPS(60);

  auto grid = make_grid(contents);

  Camera2D camera = {0};
  camera.target = Vector2{(grid.lines[0].size() * CELL_SIZE) / 2,
                          (grid.lines.size() * CELL_SIZE) / 2};

  camera.offset = Vector2{screenWidth / 2, screenHeight / 2};
  camera.zoom = 1.0f;

  while (!WindowShouldClose()) {

    if (IsKeyDown(KEY_A))
      camera.target.x -= 5;
    else if (IsKeyDown(KEY_D))
      camera.target.x += 5;

    if (IsKeyDown(KEY_W))
      camera.target.y -= 5;
    else if (IsKeyDown(KEY_S))
      camera.target.y += 5;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);
    {
      draw_grid(grid);
    }
    EndMode2D();

    EndDrawing();
  }

  CloseWindow();
}
