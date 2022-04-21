////////////////////////////////////////////////////////////////////////////////////
// About

//
// Example of a slightly less basic ray tracer
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

/// \file example2.cpp
///
/// This is the second example program for the Maths3D library to show some
/// practical example usage of the vector functions.
///
/// It creates a simple scene containing some spheres and cubes, and then ray traces
/// them with some basic lighting.
///
/// It builds on the first example. \see example1.cpp


////////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdint>
#include <cstdio>
#include "bitmap.h"
#include "maths3d.h"


////////////////////////////////////////////////////////////////////////////////////
// Types

struct Sphere
{
  Vector4f center;
  Scalar1f radius;
  uint32_t color;
};

struct Cube
{
  Vector4f center;
  Scalar1f radius;
  uint32_t color;
};

struct Ray
{
  Vector4f origin;
  Vector4f lookAt;
};

struct Light
{
  Vector4f position;
  uint32_t color;
};


////////////////////////////////////////////////////////////////////////////////////
// Scene

struct Scene
{
  static constexpr int numberOfSpheres = 3;
  static constexpr int numberOfCubes = 3;
  Sphere spheres[numberOfSpheres];
  Cube cubes[numberOfCubes];
  Light light;
};


////////////////////////////////////////////////////////////////////////////////////
// Ray tracer

Vector4f RayDirection(const Ray& ray)
{
  return Vector4f_Normalized(Vector4f_Subtract(ray.lookAt, ray.origin));
}

Vector4f PointOfClosestIntercept(const Ray& ray, const Vector4f& point)
{
  const Vector4f rayDirection = RayDirection(ray);
  const Vector4f fromRayToPoint = Vector4f_Subtract(point, ray.origin);
  const Scalar1f lengthAlongRayToPerpendicularToPoint = Vector4f_DotProduct(rayDirection, fromRayToPoint);
  const Scalar1f& length = lengthAlongRayToPerpendicularToPoint;
  return Vector4f_Add(ray.origin, Vector4f_Scaled(rayDirection, length));
}

bool RayIntersectsSphere(const Ray& ray, const Sphere& sphere)
{
  const Vector4f pointOnRayPerpendicularToSphereCenter = PointOfClosestIntercept(ray, sphere.center);
  const Vector4f vectorFromSphereCenterToRay = Vector4f_Subtract(pointOnRayPerpendicularToSphereCenter, sphere.center);
  const Scalar1f distanceFromSphereCenterToRay = Vector4f_Length(vectorFromSphereCenterToRay);

  // See if our point of closest intercept between the ray and the center of the sphere is less than the radius of the sphere.
  return distanceFromSphereCenterToRay < sphere.radius;
}

template <size_t width, size_t height, int viewDistance>
uint32_t TraceRay(int i, int j, const Scene& scene)
{
  Vector4f eye{ 0.0f, 0.0f, -viewDistance, 0.0f };
  Vector4f lookAt{ i - 0.5f * width, j - 0.5f * height, 0.0f, 0.0f };
  Ray ray{ eye, lookAt };

  for (int i = 0; i < scene.numberOfSpheres; ++i)
  {
    if (RayIntersectsSphere(ray, scene.spheres[i]))
    {
      // TODO: find point on the sphere and find closest object
      return scene.spheres[i].color;
    }
  }

  for (int i = 0; i < scene.numberOfCubes; ++i)
  {
    // TODO: add cube support
    // if (RayIntersectsCube(ray, scene.spheres[i]))
  }

  // TODO: trace ray to the light to do basic lighting

  // If don't intersect any spheres, then draw a black pixel.
  return 0x000000;
}

template <size_t width, size_t height, int viewDistance>
void RayTracer(const Scene& scene, const char* fileName)
{
  uint32_t pixels[width * height];
  Image image{ width, height, pixels };
  for (int j = 0; j < height; ++j)
  {
    for (int i = 0; i < width; ++i)
    {
      pixels[j*width + i] = TraceRay<width,height,viewDistance>(i, j, scene);
    }
  }
  Image_SaveBitmap(image, fileName);
}


////////////////////////////////////////////////////////////////////////////////////
// Main

int main(int argc, const char* argv[])
{
  printf("example1\n");
  Scene scene;
  scene.spheres[0] = Sphere{ { -50, -50, 90 }, 50, 0xFF0000 };
  scene.spheres[1] = Sphere{ {  60,  20, 70 }, 50, 0x00FF00 };
  scene.spheres[2] = Sphere{ {   0,  30, 80 }, 70, 0x0000FF };
  RayTracer<320,240,100>(scene, "example1.bmp");
}

