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
// Documentation

/// \file svg.cpp
///
/// This takes a list of shapes and converts them in to an SVG file.


////////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdio>
#include "svg.h"


////////////////////////////////////////////////////////////////////////////////////
// Implementation

void SVG_Create(const char* fileName, float width, float height, const std::vector<Shape>& shapes)
{
  FILE* file = fopen(fileName, "wt");
  if (!file)
    return;

  const char* markerStyle = "style=\"fill-rule:evenodd;stroke:#000000;stroke-width:1pt;stroke-opacity:1;fill:#000000;fill-opacity:1\"";
  const char* lineStyle = "style=\"fill:none;stroke-width:1;stroke-linecap:round;stroke-linejoin:round;stroke-opacity:1;stroke:#";

  fprintf(file, R"zz(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
    <svg viewBox="0,0 %f,%f" xmlns:svg="http://www.w3.org/2000/svg" xmlns="http://www.w3.org/2000/svg">)zz", width, height);

  fprintf(file, R"zz(
    <defs>
      <marker id="Arrow1Mend" orient="auto" style="overflow:visible">
        <path d="M 12.5,0.0 L 17.5,-5.0 L 0,0.0 L 17.5,5.0 L 12.5,0.0 z "
           %s
           transform="scale(0.4) rotate(180) translate(0,0)" />
      </marker>
      <marker id="DotM" style="overflow:visible">
        <path d="M -2.5,-1.0 C -2.5,1.76 -4.74,4.0 -7.5,4.0 C -10.26,4.0 -12.5,1.76 -12.5,-1.0 C -12.5,-3.76 -10.26,-6.0 -7.5,-6.0 C -4.74,-6.0 -2.5,-3.76 -2.5,-1.0 z "
           %s
           transform="scale(0.4) translate(7.4, 1)" />
      </marker>
    </defs>)zz", markerStyle, markerStyle);
  
  fprintf(file, "<g id=\"layer1\">\n");

  for (const auto& shape: shapes)
  {
    switch (shape.type)
    {
      case ShapeType::Line:
        fprintf(file, "<path d=\"M %f,%f %f,%f\" %s%06x\" />\n",
            shape.line.x1, shape.line.y1, shape.line.x2, shape.line.y2, lineStyle, shape.color);
        break;
      case ShapeType::Arrow:
        fprintf(file, "<path d=\"M %f,%f %f,%f\" %s%06x;stroke-linecap:butt;marker-start:url(#DotM);marker-end:url(#Arrow1Mend)\" />",
            shape.arrow.x1, shape.arrow.y1, shape.arrow.x2, shape.arrow.y2, lineStyle, shape.color);
        break;
      case ShapeType::Circle:
        fprintf(file, "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" %s%06x;paint-order:stroke fill markers\" />",
            shape.circle.x, shape.circle.y, shape.circle.radius, lineStyle, shape.color);
        break;
    }
  }
  
  fprintf(file, "</g> </svg>\n");
  fclose(file);
}

