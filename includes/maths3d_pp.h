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

/// \file maths3d_pp.h
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
inline Vector4f& Vector4f::operator=(Scalar1f value)
{
  *this = Vector4f_Replicate(value);
  return *this;
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
inline Vector4f::operator Scalar1f() const
{
  return Vector4f_Length(*this);
}

/// Normalized vector of a vector.
inline Vector4f operator~(const Vector4f& vec)
{
  return Vector4f_Normalized(vec);
}


///////////////////////////////////////////////////////////////////////////////////
// Matrix operators

/// Convert vector to a translation matrix
inline Matrix4x4f& Matrix4x4f::operator=(const Vector4f& vec)
{
  *this = Matrix4x4f_TranslateXYZ(vec);
  return *this;
}

/// Multiply two matrices.
inline Matrix4x4f operator*(const Matrix4x4f& m1, const Matrix4x4f& m2)
{
  return Matrix4x4f_Multiply(m1, m2);
}

/// Transpose of matrix.
inline Matrix4x4f operator~(const Matrix4x4f& m)
{
  return Matrix4x4f_Transposed(m);
}

/// Multiply by a scalar.
inline Matrix4x4f operator*(const Matrix4x4f& m, const Scalar1f& scale)
{
  return Matrix4x4f_Scaled(m, scale);
}

/// Multiply matrix by a vector (transform).
inline Vector4f operator*(const Matrix4x4f& m, const Vector4f& vec)
{
  return Vector4f_Transform(m, vec);
}

