////////////////////////////////////////////////////////////////////////////////////
// About

//
// Example of applying matrix transformations to 3D points.
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

/// \file example4.cpp
///
/// This is the fourth example program for the Maths3D library.


////////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdint>
#include <cstdio>
#include <vector>
#include "svg.h"
#include "maths3d_pp.h"
#include "maths3d_ext.h"


////////////////////////////////////////////////////////////////////////////////////
// Object

template <size_t N>
struct Object
{
  Vector4f points[N];
};


template <size_t N>
Object<N> Object_Transform(const Object<N>& obj, const Vector4f& scale, const Rotation& rotation, const Vector4f& translation)
{
  const Matrix4x4f scaling = Matrix4x4f_ScaleXYZ(scale);
  const Matrix4x4f rotating = Matrix4x4f_RotateXYZ(rotation);
  const Matrix4x4f translating = Matrix4x4f_TranslateXYZ(translation);
  const Matrix4x4f xform = translating * rotating * scaling;

  // Transform local coordinates in to world coordinates (without perspective divide)
  Object<N> ret;
  Vector4f_SSETransformStream(ret.points, obj.points, xform);
  return ret;
}


template <size_t N>
void Object_LineHelper(const Object<N>& obj, int pnt1, int pnt2, uint32_t color, std::vector<Shape>& shapeList)
{
  Shape shape = { ShapeType::Line, color };
  shape.line.x1 = obj.points[pnt1].x;
  shape.line.y1 = obj.points[pnt1].y;
  shape.line.x2 = obj.points[pnt2].x;
  shape.line.y2 = obj.points[pnt2].y;
  shapeList.push_back(shape);
}


////////////////////////////////////////////////////////////////////////////////////
// Cube

using Cube = Object<8>;


Cube Cube_Unit()
{
  Cube cube;
  for (int pnt = 0; pnt < 8; ++pnt)
  {
    Vector4f point = { 0.0, 0.0, 0.0, 0.0 };
    point.x += (pnt & 1) ? 0.5 : -0.5;
    point.y += (pnt & 2) ? 0.5 : -0.5;
    point.z += (pnt & 4) ? 0.5 : -0.5;
    point.w  = 1.0;
    cube.points[pnt] = point;
  }
  return cube;
}


Cube Cube_Transform(const Cube& cube, const Vector4f& scale, const Rotation& rotation, const Vector4f& translation)
{
  return Object_Transform(cube, scale, rotation, translation);
}


void Cube_Lines(const Cube& cube, std::vector<Shape>& shapeList)
{
  for (int pnt1 = 0; pnt1 < 8; ++pnt1)
  {
    for (int pnt2 = 0; pnt2 < 8; ++pnt2)
    {
      int x = pnt1 ^ pnt2;
      if (x == 1 || x == 2 || x == 4)
      {
        Object_LineHelper(cube, pnt1, pnt2, 0x000000, shapeList);
      }
    }
  }
}


////////////////////////////////////////////////////////////////////////////////////
// Axes

using Axes = Object<4>;


Axes Axes_Unit()
{
  Axes axes;
  axes.points[0] = { 0.0, 0.0, 0.0, 1.0 };
  axes.points[1] = { 1.0, 0.0, 0.0, 1.0 };
  axes.points[2] = { 0.0, 1.0, 0.0, 1.0 };
  axes.points[3] = { 0.0, 0.0, 1.0, 1.0 };
  return axes;
}


Axes Axes_Transform(const Axes& axes, const Vector4f& scale, const Rotation& rotation, const Vector4f& translation)
{
  return Object_Transform(axes, scale, rotation, translation);
}


void Axes_Lines(const Axes& axes, std::vector<Shape>& shapeList)
{
  uint32_t colors[4] = { 0x000000, 0xFF0000, 0x00FF00, 0x0000FF };
  for (int pnt = 1; pnt < 4; ++pnt)
  {
    Shape shape = { ShapeType::Arrow, colors[pnt] };
    shape.arrow.x1 = axes.points[0].x;
    shape.arrow.y1 = axes.points[0].y;
    shape.arrow.x2 = axes.points[pnt].x;
    shape.arrow.y2 = axes.points[pnt].y;
    shapeList.push_back(shape);
  }
}


////////////////////////////////////////////////////////////////////////////////////
// Frustum

using Frustum = Object<9>;


Frustum Frustum_Unit()
{
  Frustum frustum;
  frustum.points[0] = {  0.0,  0.0, -3.0, 1.0 };
  
  frustum.points[1] = {  0.5,  0.5, -1.0, 1.0 };
  frustum.points[2] = { -0.5,  0.5, -1.0, 1.0 };
  frustum.points[3] = { -0.5, -0.5, -1.0, 1.0 };
  frustum.points[4] = {  0.5, -0.5, -1.0, 1.0 };

  frustum.points[5] = {  1.0,  1.0,  1.0, 1.0 };
  frustum.points[6] = { -1.0,  1.0,  1.0, 1.0 };
  frustum.points[7] = { -1.0, -1.0,  1.0, 1.0 };
  frustum.points[8] = {  1.0, -1.0,  1.0, 1.0 };
  return frustum;
}


Frustum Frustum_Transform(const Frustum& frustum, const Vector4f& scale, const Rotation& rotation, const Vector4f& translation)
{
  return Object_Transform(frustum, scale, rotation, translation);
}


void Frustum_Lines(const Frustum& frustum, std::vector<Shape>& shapeList)
{
  for (int pnt = 0; pnt < 3; ++pnt)
  {
    Object_LineHelper(frustum, 0, pnt + 5, 0x000000, shapeList);
    Object_LineHelper(frustum, pnt + 1, pnt + 2, 0x000000, shapeList);
    Object_LineHelper(frustum, pnt + 5, pnt + 6, 0x000000, shapeList);
  }
  Object_LineHelper(frustum, 0, 8, 0x000000, shapeList);
  Object_LineHelper(frustum, 4, 1, 0x000000, shapeList);
  Object_LineHelper(frustum, 8, 5, 0x000000, shapeList);
}


////////////////////////////////////////////////////////////////////////////////////
// Camera

struct Camera
{
  Scalar1f scale;
  Rotation rotation;
  Vector4f translation;
};


Camera Camera_Create(Scalar1f scale, const Rotation& rotation, const Vector4f& translation)
{
  return { scale, rotation, translation };
}


Matrix4x4f Camera_ViewMatrix(const Camera& camera)
{
  // A rotation matrix is a rigid body transform, there is no scaling, so we can
  // use the transpose instead of a full inverse here.
  Matrix4x4f invRotation = Matrix4x4f_Transposed(Matrix4x4f_RotateXYZ(camera.rotation));
  return invRotation * Matrix4x4f_TranslateXYZ(camera.translation) * Matrix4x4f_Scale(camera.scale);
}


////////////////////////////////////////////////////////////////////////////////////
// Main

int main(int argc, const char* argv[])
{
  printf("example4\n");

  float w = 300.0;
  float h = 200.0;
  float aspectRatio = w / h;
  Degrees fieldOfView = { 90.0 };

  // Move world coordinates in to camera space.
  Camera camera = { .scale = 1.5, .rotation = { 5.0, 45.0, 10.0 }, .translation = { 12.0, -1.0f, -10.0f, 0.0f } };
  Matrix4x4f modelViewMatrix = Camera_ViewMatrix(camera);

  // Transforms world to NDC (normalized device coords / clip space).
  Matrix4x4f perspective = Matrix4x4f_PerspectiveFrustum(fieldOfView, aspectRatio, 0.1, 10000.0);
  Matrix4x4f screenScale = Matrix4x4f_ScaleXYZ({ w, -h, 1.0f, 1.0f });
  Matrix4x4f screenCenter = Matrix4x4f_TranslateXYZ({ w/2.f, h/2.f, 0.0f, 0.0f }); // scale and trans converts NDC in to screen space (device coords)

  // Combine the transforms.
  Matrix4x4f xform = screenCenter * screenScale * perspective * modelViewMatrix;

  std::vector<Shape> shapeList;
  int gridSize = 1;
  for (int j = 0; j < gridSize; ++j)
  {
    for (int i = 0; i < gridSize; ++i)
    {
      Cube cube = Cube_Transform(Cube_Unit(), { 2.0, 2.0, 2.0, 1.0 }, Rotation{ .x = 20.0, .y = 40.0, .z = 0.0 }, { i*5.0f, 0.0, j*5.0f, 0.0 });
      Vector4f_SSETransformCoordStream(cube.points, cube.points, xform);
      Cube_Lines(cube, shapeList);
    }
  }

  Axes axes = Axes_Transform(Axes_Unit(), { 5.0, 5.0, 5.0, 1.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0 });
  Vector4f_SSETransformCoordStream(axes.points, axes.points, xform);
  Axes_Lines(axes, shapeList);

  Frustum frustum = Frustum_Transform(Frustum_Unit(), { 3.0, 3.0, 3.0, 1.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0 });
  Vector4f_SSETransformCoordStream(frustum.points, frustum.points, xform);
  Frustum_Lines(frustum, shapeList);

  SVG_Create("cube.svg", w, h, shapeList);
}

