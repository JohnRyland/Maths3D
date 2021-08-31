///////////////////////////////////////////////////////////////////////////////////
// About

// 
// Maths3D
// Maths for Computer Graphics
// 


///////////////////////////////////////////////////////////////////////////////////
// License

// 
// Copyright (c) 2021, John Ryland
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// The views and conclusions contained in the software and documentation are those
// of the authors and should not be interpreted as representing official policies, 
// either expressed or implied, of the cgMaths Project.
// 


///////////////////////////////////////////////////////////////////////////////////
// Documentation

//
// This code requires a modern C++ compiler but is mostly C and avoids using
// classes (with the exception of providing unit type safety discussed below).
// 
// No header or library dependencies.
// 
// The API attempts to be functional in style, with inputs and a returned
// output.
// 
// The inputs are never mutated (modified) and the returned value is a newly
// constructed object. This is to maintain referential transparency. Some
// benefits to this approach are that it is easy to chain together calls to
// these APIs. It also means that they can be safely used in multithreaded
// code. From a performance point of view it assumes that the compiler will be
// able to perform RVO.
// 
// Inputs are passed by const reference when the size of the input would be
// larger than the size of a pointer.
// 
// Changing the scalar type will change the underlying value type used by
// vector and matrix.
//
// A scalar value used to represent an angle can sometimes be used incorrectly
// when an API is not clear if the units are degrees or radians. To avoid this,
// the API itself should document the units with a specific type that encode
// the units. The types Degrees and Radians below do this. These are actually
// classes and so this is C++, but it is worth the developer time it saves by
// ensuring there is no mistakes of this kind. The same could be used for other
// types of units as required, such as meters
// vs feet if the API were to need this.
//
// Both the vector and matrix type contain unions with a 'v' member which can
// be used for iteration of the components instead of needing to write code
// which requires copy paste of the same code for each component. This avoids
// mistakes, such as:
//
//    Vector4f ret;
//    ret.x = a.x + b.x;
//    ret.y = a.y + b.y;
//    ret.y = a.z + b.z;
// 
// Do you see the mistake? Happens all the time. Far better is to write code
// like this:
// 
//    Vector4f ret;
//    for (int i = 0; i < 3; ++i)
//      ret.v[i] = a.v[i] + b.v[i];
//
// Less lines of code, less error prone, and easier to update the formula
// applied to each component.
//
// The API omits providing a 3 component wide vector and instead only provides
// a Vector4f.  This is because when dealing with arrays of these, we will be
// able to optimize better the 4 wide version, particularly if it can be
// aligned to 128-bits and used with a SIMD optimized function to transform an
// array of them. For all the provided functions below, we don't bother
// providing SIMD optimizations as most compilers can auto-vectorize this code
// just fine and so it only will make the code more error prone, and not able
// to be re-compiled with different compiler flags to target different machine
// types or without or without different levels of SIMD support.
//


///////////////////////////////////////////////////////////////////////////////////
// 3D Maths - Scalar

using Scalar1f = float;

inline Scalar1f Scalar1f_Zero()
{
  return 0.0f;
}

inline Scalar1f Scalar1f_One()
{
  return 1.0f;
}

inline Scalar1f Scalar1f_Two()
{
  return 2.0f;
}

// These helpers ensure it is harder to mess up the units and make APIs more
// self-documenting
struct Degrees;
struct Radians;

struct Degrees
{
  Scalar1f value;
  operator Radians() const;
};

struct Radians
{
  Scalar1f value;
  operator Degrees() const;
};

Degrees::operator Radians() const
{
  constexpr float deg2rad = 0.01745329251f; // pi / 180
  return Radians{ value * deg2rad };
}

Radians::operator Degrees() const
{
  constexpr float rad2deg = 57.2957795131f; // 180 / pi
  return Degrees{ value * rad2deg };
}

// As with degrees and radians, so too can distances be confusing.
// Particularly annoying are things where it could be an angle or a distance,
// such as 'elevation' which could mean an altitude or an angle of elevation.
struct Metres;
struct Feet;

struct Metres
{
  Scalar1f value;
  operator Feet() const;
};

// International feet
struct Feet
{
  Scalar1f value;
  operator Metres() const;
};

Metres::operator Feet() const
{
  constexpr float m2ft= 3.2808398950131;
  return Feet{ value * m2ft };
}

Feet::operator Metres() const
{
  constexpr float ft2m = 0.3048;
  return Metres{ value * ft2m };
}


///////////////////////////////////////////////////////////////////////////////////
// 3D Maths - Vector

struct Vector4f
{
  union
  {
    Scalar1f v[4];
    struct
    {
      Scalar1f x, y, z, w;
    };
  };
};

inline Vector4f Vector4f_Set(Scalar1f x, Scalar1f y, Scalar1f z, Scalar1f w)
{
  return Vector4f{ { { x, y, z, w } } };
}

inline Vector4f Vector4f_Replicate(Scalar1f v)
{
  return Vector4f_Set(v, v, v, v);
}

inline Vector4f Vector4f_Zero()
{
  return Vector4f_Replicate(Scalar1f_Zero());
}

inline Vector4f Vector4f_SetX(const Vector4f& vec, Scalar1f x)
{
  return Vector4f_Set(x, vec.y, vec.z, vec.w);
}

inline Vector4f Vector4f_SetY(const Vector4f& vec, Scalar1f y)
{
  return Vector4f_Set(vec.x, y, vec.z, vec.w);
}

inline Vector4f Vector4f_SetZ(const Vector4f& vec, Scalar1f z)
{
  return Vector4f_Set(vec.x, vec.y, z, vec.w);
}

inline Vector4f Vector4f_SetW(const Vector4f& vec, Scalar1f w)
{
  return Vector4f_Set(vec.x, vec.y, vec.z, w);
}

inline Vector4f Vector4f_CrossProduct(const Vector4f& v1, const Vector4f& v2)
{
  return Vector4f_Set(v1.y * v2.z - v1.z * v2.y,
                      v1.x * v2.z - v1.z * v2.x,
                      v1.x * v2.y - v1.y * v2.x,
                      Scalar1f_One());
}

inline Vector4f Vector4f_Multiply(const Vector4f& vec1, const Vector4f& vec2)
{
  return Vector4f_Set(vec1.x*vec2.x, vec1.y*vec2.y, vec1.z*vec2.z, vec1.w*vec2.w);
}

inline Vector4f Vector4f_Add(const Vector4f& vec1, const Vector4f& vec2)
{
  return Vector4f_Set(vec1.x+vec2.x, vec1.y+vec2.y, vec1.z+vec2.z, vec1.w+vec2.w);
}

inline Vector4f Vector4f_Scaled(const Vector4f& vec, Scalar1f scale)
{
  return Vector4f_Multiply(vec, Vector4f_Replicate(scale));
}

inline Scalar1f Vector4f_SumComponents(const Vector4f& vec)
{
  return vec.x + vec.y + vec.z + vec.w;
}

inline Scalar1f Vector4f_DotProduct(const Vector4f& vec1, const Vector4f& vec2)
{
  return Vector4f_SumComponents(Vector4f_Multiply(vec1, vec2));
}

inline Scalar1f Vector4f_LengthSquared(const Vector4f& vec)
{
  return Vector4f_DotProduct(vec, vec);
}

inline Scalar1f Vector4f_Length(const Vector4f& vec)
{
  return ::sqrt(Vector4f_LengthSquared(vec));
}

inline Scalar1f Vector4f_ReciprocalLength(const Vector4f& vec)
{
  return Scalar1f_One() / Vector4f_Length(vec);
}

inline Vector4f Vector4f_Normalized(const Vector4f& vec)
{
  return Vector4f_Scaled(vec, Vector4f_ReciprocalLength(vec));
}


///////////////////////////////////////////////////////////////////////////////////
// 3D Maths - Matrix

struct Matrix4x4f
{
  union
  {
    Vector4f row[4];
    Scalar1f m[4][4];
    Scalar1f v[16];
  };
};

inline Matrix4x4f Matrix4x4f_Set(const Scalar1f v[16])
{
  Matrix4x4f ret;
  for (int i = 0; i < 16; ++i)
    ret.v[i] = v[i];
  return ret;
}

inline Matrix4x4f Matrix4x4f_Zero()
{
  Matrix4x4f ret;
  for (int i = 0; i < 16; ++i)
    ret.v[i] = Scalar1f_Zero();
  return ret;
}

inline Matrix4x4f Matrix4x4f_Identity()
{
  Matrix4x4f ret = Matrix4x4f_Zero();
  ret.v[0] = ret.v[5] = ret.v[10] = ret.v[15] = Scalar1f_One();
  return ret;
}

inline Matrix4x4f Matrix4x4f_TranslateXYZ(const Vector4f& vec)
{
  Matrix4x4f ret = Matrix4x4f_Identity();
  ret.row[3] = Vector4f_SetW(vec, Scalar1f_One());
  return ret;
}

Matrix4x4f Matrix4x4f_Multiply(const Matrix4x4f& m1, const Matrix4x4f& m2)
{
  Matrix4x4f ret = Matrix4x4f_Zero();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      for (int k = 0; k < 4; k++)
        ret.m[i][j] += m1.m[k][j] * m2.m[i][k];
  return ret;
}

Matrix4x4f Matrix4x4f_Transposed(const Matrix4x4f& a)
{
  Matrix4x4f ret;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      ret.m[i][j] = a.m[j][i];
  return ret;
}

Matrix4x4f Matrix4x4f_Scaled(const Matrix4x4f& m, Scalar1f scale)
{
  Matrix4x4f ret = m;
  for (int i = 0; i < 16; i++)
    ret.v[i] = m.v[i] * scale;
  return ret;
}

Matrix4x4f Matrix4x4f_ScaleXYZ(const Vector4f& scale)
{
  Matrix4x4f ret = Matrix4x4f_Zero();
  ret.m[0][0] = scale.x;
  ret.m[1][1] = scale.y;
  ret.m[2][2] = scale.z;
  ret.m[3][3] = scale.w;
  return ret;
}

// Helper for other functions
static Matrix4x4f Matrix4x4f_RotateCommon(Scalar1f angle, int axis)
{
  const Scalar1f sx = ::sin(angle);
  const Scalar1f cx = ::cos(angle);
  Matrix4x4f ret = Matrix4x4f_Zero();
  ret.m[0][2]    = ret.m[1][0]    = ret.m[2][1]    = sx;
  ret.m[0][1]    = ret.m[1][2]    = ret.m[2][0]    = -sx;
  ret.m[axis][0] = ret.m[axis][1] = ret.m[axis][2] = Scalar1f_Zero();
  ret.m[0][axis] = ret.m[1][axis] = ret.m[2][axis] = Scalar1f_Zero();
  ret.m[0][0]    = ret.m[1][1]    = ret.m[2][2]    = cx;
  ret.m[axis][axis] = ret.m[3][3] = Scalar1f_One();
  return ret;
}

inline Matrix4x4f Matrix4x4f_RotateX(Scalar1f x)
{
  return Matrix4x4f_RotateCommon(x, 0);
}

inline Matrix4x4f Matrix4x4f_RotateY(Scalar1f y)
{
  return Matrix4x4f_RotateCommon(y, 1);
}

inline Matrix4x4f Matrix4x4f_RotateZ(Scalar1f z)
{
  return Matrix4x4f_RotateCommon(z, 2);
}

Matrix4x4f Matrix4x4f_PerspectiveFrustum(Radians fieldOfView, Scalar1f aspectRatio, Scalar1f near, Scalar1f far)
{
  const Scalar1f z = Scalar1f_Zero();
  const Scalar1f one = Scalar1f_One();
  const Scalar1f two = Scalar1f_Two();
  const Scalar1f fov = fieldOfView.value;
  const Scalar1f ctan = one / ::tanf(fov / two);
  const Scalar1f inlf = one / (near - far);
  const Scalar1f perspective[16] = { ctan / aspectRatio, z, z, z,
                                     z, ctan, z, z,
                                     z, z, (far + near) * inlf, -one,
                                     z, z, (two * far * near) * inlf, z };
  return Matrix4x4f_Set(perspective);
}

Matrix4x4f Matrix4x4f_OrthographicFrustum(Scalar1f left, Scalar1f right, Scalar1f bottom, Scalar1f top, Scalar1f near, Scalar1f far)
{
  const Scalar1f z = Scalar1f_Zero();
  const Scalar1f two = Scalar1f_Two();
  const Scalar1f width = right - left;
  const Scalar1f height = top - bottom;
  const Scalar1f depth = far - near;
  const Scalar1f a = two / width;
  const Scalar1f b = (right + left) / width;
  const Scalar1f c = two / height;
  const Scalar1f d = (top + bottom) / height;
  const Scalar1f e = -(far + near) / depth;
  Scalar1f ortho[16] = { a, z, z,-b,
                         z, c, z,-d,
                         z, z,-two/depth, e,
                         z, z, z, z };
  return Matrix4x4f_Set(ortho);
}

Vector4f Vector4f_Transform(const Matrix4x4f& m, const Vector4f& vec)
{
  Matrix4x4f trans = Matrix4x4f_Transposed(m);
  return Vector4f_Set(Vector4f_DotProduct(trans.row[0], vec),
                      Vector4f_DotProduct(trans.row[1], vec),
                      Vector4f_DotProduct(trans.row[2], vec),
                      Vector4f_DotProduct(trans.row[3], vec));
}

