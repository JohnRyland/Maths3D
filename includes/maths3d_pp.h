#pragma once

///////////////////////////////////////////////////////////////////////////////////
// About

// 
// Maths3D
// Maths for Computer Graphics
// 


///////////////////////////////////////////////////////////////////////////////////
// License

// 
// Copyright (c) 2021-2022, John Ryland
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
// either expressed or implied, of the Maths3D Project.
// 


///////////////////////////////////////////////////////////////////////////////////
// Documentation

/// \file maths3dpp.h
///
/// C++ operator overload support for operations on the Scalar, Vector and Matrix
/// types defined in maths3d.h. \see maths3d.h
///
/// This code requires a modern C++ compiler.
///
/// \see Scalar1f, Vector4f, Matrix4x4f


///////////////////////////////////////////////////////////////////////////////////
// Includes

#include "maths3d.h"


///////////////////////////////////////////////////////////////////////////////////
// Types

class VectorProduct4f
{
public:
  Vector4f lhs;
  Vector4f rhs;
  operator Scalar1f();
  operator Vector4f();
};

inline VectorProduct4f::operator Scalar1f()
{
  return Vector4f_DotProduct(lhs, rhs);
}

inline VectorProduct4f::operator Vector4f()
{
  return Vector4f_CrossProduct(lhs, rhs);
}


///////////////////////////////////////////////////////////////////////////////////
// 3D Maths - Vector

/// assign a scalar
inline Vector4f operator=(Scalar1f v)
{
  return Vector4f_Replicate(v);
}

/// Product of two vectors.
/// When the result is used as a Scalar1f, the dot-product (inner product) is returned.
/// When the result is used as a Vector4f, the cross-product (wedge/exterior product) is returned.
inline VectorProduct4f operator^(const Vector4f& vec1, const Vector4f& vec2)
{
  return { vec1, vec2 };
}

/// Multiply two vectors.
inline Vector4f operator*(const Vector4f& vec1, const Vector4f& vec2)
{
  return Vector4f_Multiply(vec1, vec2);
}

/// Multiply by a scalar.
inline Vector4f operator*(const Vector4f& vec1, const Scalar1f& scale)
{
  return Vector4f_Scaled(vec1, scale);
}

/// Add two vectors.
inline Vector4f operator+(const Vector4f& vec1, const Vector4f& vec2)
{
  return Vector4f_Add(vec1, vec2);
}

/// Subtract two vectors.
inline Vector4f operator-(const Vector4f& vec1, const Vector4f& vec2)
{
  return Vector4f_Subtract(vec1, vec2);
}

/// Length of a vector.
inline Scalar1f operator=(const Vector4f& vec)
{
  return Vector4f_Length(vec);
}

// ~v  which operator?
// inline Vector4f Vector4f_Normalized(const Vector4f& vec)


///////////////////////////////////////////////////////////////////////////////////
// Matrix operators

// m = v
// Matrix4x4f Matrix4x4f_TranslateXYZ(const Vector4f& vec)

// m = m * m
// Matrix4x4f Matrix4x4f_Multiply(const Matrix4x4f& m1, const Matrix4x4f& m2);

// ?
// Matrix4x4f Matrix4x4f_Transposed(const Matrix4x4f& a);

// m = m * s
// Matrix4x4f Matrix4x4f_Scaled(const Matrix4x4f& m, Scalar1f scale);

// m = s
// Matrix4x4f Matrix4x4f_ScaleXYZ(const Vector4f& scale);

// m = m * v
// Vector4f Vector4f_Transform(const Matrix4x4f& m, const Vector4f& vec);

