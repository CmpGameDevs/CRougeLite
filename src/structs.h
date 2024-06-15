#ifndef STRUCTS_H
#define STRUCTS_H

#include <raylib.h>

typedef struct {
  bool isFiring;
} State;

typedef struct {
  int x;
  int y;
  Texture2D texture;
} Player;

typedef struct {
  int x;
  int y;
  int dx;
  int dy;
  int health;
  Texture2D texture;
} Entity;

#endif // STRUCTS_H
