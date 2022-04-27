////////////////////////////////////////////////////////////////////////////////////
// About

//
// Example of a less basic ray tracer using C++ syntactic sugar
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

/// \file example3.cpp
///
/// This is the third example program for the Maths3D library to show how using
/// the C++ wrapper functions can improve the readability.
///
/// It creates a simple scene containing some spheres and cubes, and then ray traces
/// them with some basic lighting.
///
/// It builds on the second example. \see example2.cpp
///
/// It includes the maths3d_pp.h file for the C++ wrappers. \see maths3d_pp.h


////////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdint>
#include <cstdio>
#include <vector>
#include "bitmap.h"
#include "maths3d_pp.h"


////////////////////////////////////////////////////////////////////////////////////
// Types

struct Ray
{
  Vector4f origin;
  Vector4f lookAt;
};


////////////////////////////////////////////////////////////////////////////////////
// Scene Objects

struct Sphere
{
  Vector4f center;
  Vector4f color;
  Scalar1f radius;
};

struct Cube
{
  Vector4f center;
  Vector4f color;
  Scalar1f radius;
};

struct Light
{
  Vector4f position;
  Vector4f color;
};


////////////////////////////////////////////////////////////////////////////////////
// Scene

struct Scene
{
  std::vector<Sphere> spheres;
  std::vector<Cube>   cubes;
  std::vector<Light>  lights;
};


////////////////////////////////////////////////////////////////////////////////////
// Ray tracer

Vector4f RayDirection(const Ray& ray)
{
  return ~(ray.lookAt - ray.origin);
}

Vector4f PointOfClosestIntercept(const Vector4f& origin, const Vector4f& rayDirection, const Vector4f& point)
{
  return origin + (rayDirection * Scalar1f(rayDirection ^ (point - origin)));
}

template <size_t width, size_t height, int viewDistance>
uint32_t TraceRay(int i, int j, const Scene& scene)
{
  const Vector4f eye{ 0.0f, 0.0f, -viewDistance, 0.0f };
  const Vector4f lookAt{ i - 0.5f * width, j - 0.5f * height, 0.0f, 0.0f };
  const Ray ray{ eye, lookAt };
  const Vector4f rayDirection = RayDirection(ray);
  float closestDistance = 1e10f;
  int closestObjectIndex = -1;
  Vector4f closestIntersection;

  for (int i = 0; i < scene.spheres.size(); ++i)
  {
    const Sphere& sphere = scene.spheres[i];
    const Vector4f pointOnRayPerpendicularToSphereCenter = PointOfClosestIntercept(ray.origin, rayDirection, sphere.center);
    const Vector4f vectorFromSphereCenterToRay = pointOnRayPerpendicularToSphereCenter - sphere.center;
    const Scalar1f distanceFromSphereCenterToRay = vectorFromSphereCenterToRay;

    // See if our point of closest intercept between the ray and the center of the sphere is less than the radius of the sphere.
    if (distanceFromSphereCenterToRay < sphere.radius)
    {
      // Now use Pythagoras' theorem to work out the distance back to the intersection point
      float distanceBack = sqrt(sphere.radius * sphere.radius - distanceFromSphereCenterToRay * distanceFromSphereCenterToRay);

      // Now we trace back along the ray this distance from the closest intercept point
      const Vector4f intersectionPoint = pointOnRayPerpendicularToSphereCenter - (rayDirection * distanceBack);

      float distanceToIntersection = intersectionPoint - ray.origin;

      if (distanceToIntersection < closestDistance)
      {
        closestIntersection = intersectionPoint;;
        closestDistance = distanceToIntersection;
        closestObjectIndex = i;
      }
    }
  }

  if (closestObjectIndex != -1)
  {
    const Sphere& sphere = scene.spheres[closestObjectIndex];
    Vector4f normal = ~(closestIntersection - sphere.center);
    Vector4f color = Vector4f_Zero();
    for (const Light& light : scene.lights)
    {
      Vector4f toLight = ~(closestIntersection - light.position);
      Scalar1f lightIntensity = toLight ^ normal;

      if (lightIntensity < 0.0)
        lightIntensity = 0.0;
      lightIntensity += 0.2;
      if (lightIntensity > 1.0)
        lightIntensity = 1.0;

      color = color + (sphere.color * lightIntensity);
    }
    return ((uint32_t(color.x*255.0)&0xFF) << 16) | ((uint32_t(color.y*255.0)&0xff) << 8) | (uint32_t(color.z*255.0)&0xff); 
  }

  for (int i = 0; i < scene.cubes.size(); ++i)
  {
    // TODO: add cube support
    // if (RayIntersectsCube(ray, scene.spheres[i]))
  }

  // TODO: trace ray to the light to do basic lighting

  // If don't intersect any spheres, then draw a black pixel.
  return 0x000000;
}

/// Applies the ray tracing algorithm to each pixel of the image and saves to a file.
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
  Scene scene = 
  {
    .spheres =
    {
      { { -50, -50,  90 }, { 1.0, 0.0, 0.0 }, 50 },
      { {  60,  20,  50 }, { 0.0, 1.0, 0.0 }, 50 },
      { {   0,  30, 100 }, { 0.0, 0.0, 1.0 }, 70 }
    },
    .cubes =
    {
    },
    .lights =
    {
      { { 20, 100, -10 }, { 1.0, 1.0, 1.0 } }
    }
  };

  RayTracer<320,240,100>(scene, "example3.bmp");
}

