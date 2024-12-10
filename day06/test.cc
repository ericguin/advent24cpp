#include "lib.hh"
#include "raylib.h"

int Logger::LogLevel = 4;

constexpr const static auto CELL_SIZE = 20.0f;
constexpr const static auto CELL_HALF = CELL_SIZE / 2.0f;
constexpr const static auto CELL_DIMS = Vector2{CELL_SIZE, CELL_SIZE};

Vector2 iterator_top_left(Grid::Iterator const &it) {
  return {it.col * CELL_SIZE, it.row * CELL_SIZE};
}

Vector2 iterator_center(Grid::Iterator const &it) {
  auto tl = iterator_top_left(it);
  return {tl.x + CELL_HALF, tl.y + CELL_HALF};
}

void draw_arrow(Grid::Iterator::Direction dir, Vector2 cell_center) {
  switch (dir) {
  case Grid::Iterator::UP:
    DrawLineV(cell_center, {cell_center.x, cell_center.y - CELL_HALF}, WHITE);
    break;
  case Grid::Iterator::RIGHT:
    DrawLineV(cell_center, {cell_center.x + CELL_HALF, cell_center.y}, WHITE);
    break;
  case Grid::Iterator::DOWN:
    DrawLineV(cell_center, {cell_center.x, cell_center.y + CELL_HALF}, WHITE);
    break;
  case Grid::Iterator::LEFT:
    DrawLineV(cell_center, {cell_center.x - CELL_HALF, cell_center.y}, WHITE);
    break;
  }
  return;
}

void draw_directions(Grid::Iterator const &it) {
  auto loc = iterator_center(it);
  if (it.value() < '0')
    return;

  if (bit_test(it.value(), 0b0001)) {
    draw_arrow(Grid::Iterator::Direction::UP, loc);
  }
  if (bit_test(it.value(), 0b0010)) {
    draw_arrow(Grid::Iterator::Direction::RIGHT, loc);
  }
  if (bit_test(it.value(), 0b0100)) {
    draw_arrow(Grid::Iterator::Direction::DOWN, loc);
  }
  if (bit_test(it.value(), 0b1000)) {
    draw_arrow(Grid::Iterator::Direction::LEFT, loc);
  }
}

void draw_concern(Grid::Iterator const &it) {
  auto loc = iterator_top_left(it);
  if (bit_test(it.value(), 0b1000000))
    DrawText("?", loc.x, loc.y, 6, BLUE);
}

void draw_outline(Vector2 topleft) {
  Rectangle rect{topleft.x, topleft.y, CELL_DIMS.x, CELL_DIMS.y};
  DrawRectangleLinesEx(rect, 1.0f, DARKGRAY);
}

bool draw_cell(Grid::Iterator it) {
  auto loc = iterator_top_left(it);
  switch (it.value()) {
  case '#':
    DrawRectangleV(loc, CELL_DIMS, RED);
    break;
  default:
    DrawRectangleV(loc, CELL_DIMS, BLACK);
    break;
  }
  draw_outline(loc);
  draw_directions(it);
  draw_concern(it);
  return true;
}

void draw_grid(Grid &grid) { grid.iter_cells(draw_cell); }
void draw_guard(Guard const &guard) {
  auto loc = iterator_center(guard.pos);
  DrawText("G", loc.x, loc.y, 6, GREEN);
}

int main() {
  auto contents = ReadEntireFile("day06/ex01.txt");
  auto grid = make_grid(contents);

  auto guard_start = grid.at(0, 0).value();
  grid.iter_cells([&](auto loc) {
    if (loc.value() == '^') {
      guard_start = loc;
      contents[loc.index(contents)] = '0';
      return false;
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
