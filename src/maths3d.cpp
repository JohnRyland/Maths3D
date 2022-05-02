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
// Includes

#include <cstdint>
#include "maths3d.h"


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

Matrix4x4f Matrix4x4f_RotateCommon(Radians angle, int axis)
{
  const Scalar1f sx = ::sin(angle.value);
  const Scalar1f cx = ::cos(angle.value);
  Matrix4x4f ret = Matrix4x4f_Zero();
  ret.m[0][2]    = ret.m[1][0]    = ret.m[2][1]    = sx;
  ret.m[0][1]    = ret.m[1][2]    = ret.m[2][0]    = -sx;
  ret.m[axis][0] = ret.m[axis][1] = ret.m[axis][2] = Scalar1f_Zero();
  ret.m[0][axis] = ret.m[1][axis] = ret.m[2][axis] = Scalar1f_Zero();
  ret.m[0][0]    = ret.m[1][1]    = ret.m[2][2]    = cx;
  ret.m[axis][axis] = ret.m[3][3] = Scalar1f_One();
  return ret;
}

/*
// Helper for creating off-center projection matrices if required
Matrix4x4f Matrix4x4f_Frustum(Scalar1f left,   Scalar1f right,
                              Scalar1f bottom, Scalar1f top,
                              Scalar1f near,   Scalar1f far)
{
  const Scalar1f one = Scalar1f_One();
  const Scalar1f two = Scalar1f_Two();
  const Scalar1f invWidth = one / (right - left);
  const Scalar1f invHeight = one / (top - bottom);
  const Scalar1f invDepth = one / (far - near);

  Matrix4x4f frustum = Matrix4x4f_Zero();
  frustum.m[0][0] = two * near * invWidth;
  frustum.m[1][1] = two * near * invHeight;
  frustum.m[2][0] = (right + left) * invWidth;
  frustum.m[2][1] = (top + bottom) * invHeight;
  frustum.m[2][2] = -(far + near) * invDepth;
  frustum.m[2][3] = -one;
  frustum.m[3][2] = -two * far * near * invDepth;
  return frustum;
}

Matrix4x4f Matrix4x4f_PerspectiveFrustum_alt(Radians fieldOfView, Scalar1f aspectRatio, Scalar1f near, Scalar1f far)
{
  float scale = ::tan(fieldOfView.value * 0.5) * near;
  float right = aspectRatio * scale;
  return Matrix4x4f_Frustum(-right, right, -scale, scale, near, far);
}
*/

Matrix4x4f Matrix4x4f_PerspectiveFrustum(Radians fieldOfView, Scalar1f aspectRatio, Scalar1f near, Scalar1f far)
{
  const Scalar1f one = Scalar1f_One();
  const Scalar1f two = Scalar1f_Two();
  const Scalar1f fov = fieldOfView.value;
  const Scalar1f invTan = one / ::tanf(fov / two);
  const Scalar1f negInvDepth = one / (near - far);

  Matrix4x4f perspective = Matrix4x4f_Zero();
  perspective.m[0][0] = invTan / aspectRatio;
  perspective.m[1][1] = invTan;
  perspective.m[2][2] = (far + near) * negInvDepth;
  perspective.m[2][3] = -one;
  perspective.m[3][2] = (two * far * near) * negInvDepth;
  return perspective;
}

Matrix4x4f Matrix4x4f_OrthographicFrustum(Scalar1f left, Scalar1f right, Scalar1f bottom, Scalar1f top, Scalar1f near, Scalar1f far)
{
  Matrix4x4f ortho = Matrix4x4f_Zero();
  const Scalar1f leftBottomNear[3] = { left, bottom, near };
  const Scalar1f rightTopFar[3] = { right, top, far };
  for (int i = 0; i < 3; ++i)
  {
    Scalar1f scale = Scalar1f_One() / (rightTopFar[i] - leftBottomNear[i]);
    ortho.m[i][3] = -(rightTopFar[i] + leftBottomNear[i]) * scale; // The translation component
    ortho.m[i][i] = Scalar1f_Two() * scale;                        // The scaling (diagonal component)
  }
  ortho.m[2][2] = -ortho.m[2][2];                                  // This changes the z direction
  return ortho;
}

// Helper function for getting the 3x3 sub-matrix of a 4x4 and calculating the determinant of the 3x3 matrix
Scalar1f Matrixi4x4f_3x3_Determinant(const Matrix4x4f& a, int row0, int row1, int row2, int col0, int col1, int col2)
{
  return a.m[row0][col0] * a.m[row1][col1] * a.m[row2][col2]
       + a.m[row0][col1] * a.m[row1][col2] * a.m[row2][col0]
       + a.m[row0][col2] * a.m[row1][col0] * a.m[row2][col1]
       - a.m[row0][col2] * a.m[row1][col1] * a.m[row2][col0]
       - a.m[row0][col1] * a.m[row1][col0] * a.m[row2][col2]
       - a.m[row0][col0] * a.m[row1][col2] * a.m[row2][col1];
}

Scalar1f Matrix4x4f_Determinant(const Matrix4x4f& a)
{
  return a.m[0][0] * Matrixi4x4f_3x3_Determinant(a, 1, 2, 3,  1, 2, 3)
       - a.m[1][0] * Matrixi4x4f_3x3_Determinant(a, 0, 2, 3,  1, 2, 3)
       + a.m[2][0] * Matrixi4x4f_3x3_Determinant(a, 0, 1, 3,  1, 2, 3)
       - a.m[3][0] * Matrixi4x4f_3x3_Determinant(a, 0, 1, 2,  1, 2, 3);
}

Matrix4x4f Matrix4x4f_Adjugate(const Matrix4x4f& a)
{
  Matrix4x4f ret = Matrix4x4f_Zero();
  int rowsCols[4][3] = { { 1,2,3 }, { 0,2,3 }, { 0,1,3 }, { 0,1,2 } };
  for (int j = 0; j < 4; ++j)
  {
    for (int i = 0; i < 4; ++i)
    {
      ret.m[j][i] = Matrixi4x4f_3x3_Determinant(a,
          rowsCols[j][0], rowsCols[j][1], rowsCols[j][2],
          rowsCols[i][0], rowsCols[i][1], rowsCols[i][2]);
    }
  }
  return ret;
}

Vector4f Vector4f_Transform(const Matrix4x4f& m, const Vector4f& vec)
{
  Matrix4x4f trans = Matrix4x4f_Transposed(m);
  return Vector4f_Set(Vector4f_DotProduct(trans.row[0], vec),
                      Vector4f_DotProduct(trans.row[1], vec),
                      Vector4f_DotProduct(trans.row[2], vec),
                      Vector4f_DotProduct(trans.row[3], vec));
}

