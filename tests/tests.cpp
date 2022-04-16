// 
// Maths3D Tests
// Maths for Computer Graphics
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


#include <cstdio>
#include <cmath>
#include "maths3d_ext.h"


const float epsilon = 0.000001f;

#define FLOAT_COMPARE(a, b) \
    if (fabs(a - b) > epsilon) \
    { \
      printf("values not equal: %f != %f\n", a, b); \
      return -1; \
    }


// Exercise all the scalar functions
int scalar_tests()
{
  printf("Starting scalar tests\n");

  Radians rad = Degrees{ 15.0f };
  Degrees deg = rad;
  FLOAT_COMPARE(deg.value, 15.0f)

  Feet ft = Metres{ 10.0f };
  Metres m = ft;
  FLOAT_COMPARE(m.value, 10.0f)

  return 0;
}

// Exercise all the Vector4f functions (some indirectly)
int vector_tests()
{
  printf("Starting vector tests\n");
  Vector4f v1 = Vector4f_SetW(Vector4f_SetZ(Vector4f_Zero(), 1.0), 2.0);
  Vector4f v2 = Vector4f_SetY(Vector4f_SetX(Vector4f_Zero(), 3.0), 4.0);
  Vector4f v3 = Vector4f_Add(v1, v2);
  Vector4f cross = Vector4f_CrossProduct(v1, v2);
  Vector4f norm1 = Vector4f_Normalized(v3);
  Vector4f norm2 = Vector4f_Normalized(Vector4f_Scaled(v3, 10.0));
  Vector4f expectedCross = Vector4f_Set(-4.0, -3.0, 0.0, 1.0);
  for (int i = 0; i < 4; ++i)
  {
    FLOAT_COMPARE(cross.v[i], expectedCross.v[i])
    FLOAT_COMPARE(norm1.v[i], norm2.v[i])
  }
  return 0;
}

// Exercise all the Matrix4x4f functions (some indirectly)
int matrix_tests()
{
  printf("Starting matrix tests\n");

  // TODO: rotations are angles and should use the Degrees/Radians type
  float rotation = 1.0f;
  Matrix4x4f modelViewProjection;
  modelViewProjection = Matrix4x4f_PerspectiveFrustum(Degrees{15.0}, 1.0f, 0.1, 10000.0);
  modelViewProjection = Matrix4x4f_Multiply(modelViewProjection, Matrix4x4f_TranslateXYZ(Vector4f_Set(0.0f, 0.0f, -100.0f, 1.0f)));
  modelViewProjection = Matrix4x4f_Multiply(modelViewProjection, Matrix4x4f_RotateZ(rotation));
  modelViewProjection = Matrix4x4f_Multiply(modelViewProjection, Matrix4x4f_RotateY(rotation));
  modelViewProjection = Matrix4x4f_Multiply(modelViewProjection, Matrix4x4f_RotateX(rotation));
  modelViewProjection = Matrix4x4f_Multiply(modelViewProjection, Matrix4x4f_ScaleXYZ(Vector4f_Set(5.0, 5.0, 5.0, 1.0)));
  modelViewProjection = Matrix4x4f_Scaled(modelViewProjection, 1.0f);
  modelViewProjection = Matrix4x4f_Transposed(modelViewProjection);
  Vector4f vec = Vector4f_Transform(modelViewProjection, Vector4f_Zero());

  modelViewProjection = Matrix4x4f_OrthographicFrustum(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
  vec = Vector4f_Transform(modelViewProjection, Vector4f_Zero());

  return 0;
}

// Exercise all the extension functions
int extension_tests()
{
  printf("Starting extension tests\n");

  Matrix4x4f modelViewProjection = Matrix4x4f_PerspectiveFrustum(Degrees{15.0}, 1.0f, 0.1, 10000.0);
  Matrix4x4f inv = Matrix4x4f_Inversed(modelViewProjection);
  Matrix4x4f invinv = Matrix4x4f_Inversed(inv);

  // Check that can do inverse or inverse and get back our original matrix within some precision
  for (int i = 0; i < 16; ++i)
  {
    FLOAT_COMPARE(modelViewProjection.v[i], invinv.v[i])
  }

  Vector4f vecs[16];
  for (int i = 0; i < 16; ++i)
  {
    vecs[i] = Vector4f_Set(i, i, i, 1.0);
  }

  Vector4f vecOut[16];
  Vector4f_SSETransformStream(vecOut, vecs, modelViewProjection);
  Vector4f_SSETransformCoordStream(vecOut, vecs, modelViewProjection);
  Vector4f_SSETransformNormalStream(vecOut, vecs, modelViewProjection);
  Vector4f_SSETransformStreamAligned(vecOut, vecs, modelViewProjection);
  Vector4f_SSETransformCoordStreamAligned(vecOut, vecs, modelViewProjection);
  Vector4f_SSETransformNormalStreamAligned(vecOut, vecs, modelViewProjection);

  return 0;
}

int main(int argc, const char* argv[])
{
  printf("Starting Maths3D tests\n");
  return scalar_tests() ||
         vector_tests() ||
         matrix_tests() ||
         extension_tests();
  printf("Finished Maths3D tests\n");
}

