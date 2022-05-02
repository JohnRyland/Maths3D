#pragma once

////////////////////////////////////////////////////////////////////////////////////
// About

//
// SVG shapes
// Convert a simple list of shapes in to an SVG file
//


///////////////////////////////////////////////////////////////////////////////////
// License

//
// Copyright (c) 2022, John Ryland
// All rights reserved.
//
// BSD-2-Clause License. See included LICENSE file for details.
// If LICENSE file is missing, see: https://opensource.org/licenses/BSD-2-Clause
//


////////////////////////////////////////////////////////////////////////////////////
// Includes

#include <vector>
#include <cstdint>


////////////////////////////////////////////////////////////////////////////////////
// Types

struct Line
{
  float  x1, y1;
  float  x2, y2;
};


struct Arrow
{
  float  x1, y1;
  float  x2, y2;
};


struct Circle
{
  float  x, y;
  float  radius;
};


enum class ShapeType
{
  Line,
  Arrow,
  Circle
};


struct Shape
{
  ShapeType type;
  uint32_t  color;
  union
  {
    Line    line;
    Arrow   arrow;
    Circle  circle;
  };
};


////////////////////////////////////////////////////////////////////////////////////
// Functions

void SVG_Create(const char* fileName, float width, float height, const std::vector<Shape>& shapes);


