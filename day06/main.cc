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
  char buf[512];

  while (!WindowShouldClose()) {

    auto speed = 5;
    if (IsKeyDown(KEY_LEFT_SHIFT))
      speed = 1;

    if (IsKeyDown(KEY_A))
      camera.target.x -= speed;
    else if (IsKeyDown(KEY_D))
      camera.target.x += speed;

    if (IsKeyDown(KEY_W))
      camera.target.y -= speed;
    else if (IsKeyDown(KEY_S))
      camera.target.y += speed;

    Vector2 vcross_start{screenWidth / 2, 0};
    Vector2 vcross_end{screenWidth / 2, screenHeight};

    Vector2 hcross_start{0, screenHeight / 2};
    Vector2 hcross_end{screenWidth, screenHeight / 2};

    Vector2 coords{camera.target.x, camera.target.y};

    auto it = grid.at(coords.x / CELL_SIZE, coords.y / CELL_SIZE);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);
    {
      draw_grid(grid);
      if (it.has_value())
        DrawRectangleLines(it->col * CELL_SIZE + 5, it->row * CELL_SIZE + 5,
                           CELL_SIZE - 10, CELL_SIZE - 10, BLUE);
    }
    EndMode2D();

    DrawLineV(vcross_start, vcross_end, GREEN);
    DrawLineV(hcross_start, hcross_end, GREEN);

    if (it.has_value()) {
      auto val = it->value();
      auto hasU = Guard::has_direction(val, Grid::Iterator::Direction::UP);
      auto hasR = Guard::has_direction(val, Grid::Iterator::Direction::RIGHT);
      auto hasD = Guard::has_direction(val, Grid::Iterator::Direction::DOWN);
      auto hasL = Guard::has_direction(val, Grid::Iterator::Direction::LEFT);
      auto isQ = bit_test(val, 0b1 << 6);
      auto ch = val > ' ' && val <= 127 ? val : '?';
      sprintf(buf,
              "Target coords: %d, %d\nGrid idx: %d, %d\nDirections: "
              "%d|%d|%d|%d\nSuspect: %d\nValue: %c",
              (int)coords.x, (int)coords.y, it->col, it->row, hasU, hasR, hasD,
              hasL, isQ, ch);
      DrawText(buf, 0, 0, 20, BLUE);
    }

    EndDrawing();
  }

  CloseWindow();
}
