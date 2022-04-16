///////////////////////////////////////////////////////////////////////////////////
// About

// 
// Extension Maths3D
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


#include <cstdint>
#include "maths3d.h"
#include <xmmintrin.h>
#include <emmintrin.h>

Matrix4x4f Matrix4x4f_Inversed(const Matrix4x4f& a)
{
  static const uint8_t idxA[12] = { 0x50, 0x60, 0x70, 0x61, 0x71, 0x72, 0xFA, 0xF9, 0xE9, 0xF8, 0xE8, 0xD8 }; // entries 0-6 and 7-11 are mirrors with + 0x88
  // TODO: idxE->J are all only using 4bits per entry, could reduce space by combining
  static const uint8_t idxE[16] = { 5, 1, 13,  9,  4,  0, 12,  8,  4,  0, 12,  8,  4,  0, 12,  8 };
  static const uint8_t idxF[16] = { 6, 2, 14, 10,  6,  2, 14, 10,  5,  1, 13,  9,  5,  1, 13,  9 };
  static const uint8_t idxG[16] = { 7, 3, 15, 11,  7,  3, 15, 11,  7,  3, 15, 11,  6,  2, 14, 10 };
  static const uint8_t idxH[16] = { 6, 6,  5,  5,  6,  6,  5,  5,  7,  7,  4,  4,  8,  8,  3,  3 };
  static const uint8_t idxI[16] = { 7, 7,  4,  4,  9,  9,  2,  2,  9,  9,  2,  2, 10, 10,  1,  1 };
  static const uint8_t idxJ[16] = { 8, 8,  3,  3, 10, 10,  1,  1, 11, 11,  0,  0, 11, 11,  0,  0 };
  // TODO: could use bit flags to signify if positive or negative instead of needing 32-bits for each
  static const float signs[16] = {1.f,-1.f,1.f,-1.f,-1.f,1.f,-1.f,1.f,1.f,-1.f,1.f,-1.f,-1.f,1.f,-1.f,1.f };
  static const float detSigns[6] = { 1.f, -1.f, 1.f, 1.f, -1.f, 1.f };

  Matrix4x4f ret;
  const float* v = a.v;
  float* r = ret.v;
  float s[12];
  for (int i = 0; i < 12; i++)
  {
    const int A = (idxA[i] & 0x0F) >> 0;
    const int B = (idxA[i] & 0xF0) >> 4;
    s[i] = v[A] * v[B] - v[A+4] * v[B+4];
  }
  float det = 0.f;
  for (int i = 0; i < 6; i++)
    det += detSigns[i] * s[i] * s[6+i];
  float invdet = 1.0f / det;
  for (int i = 0; i < 16; i++)
    r[i] = signs[i] * (v[idxE[i]] * s[idxH[i]]
        - v[idxF[i]] * s[idxI[i]] + v[idxG[i]] * s[idxJ[i]]) * invdet;
  return ret;
}

template <bool translate, bool divideByW, bool alignedOutput, int outputStep, bool alignedInput, int inputStep>
void Vector4f_SSETransformStreamGeneric(float* outputStream, const float* inputStream, unsigned count, const Matrix4x4f& transform)
{
  union
  {
    Matrix4x4f m;
    __m128     r[4];
  } xform;

  // Using the union allows us to access the rows of the transform matrix as __m128 SSE values.
  // Also the transform parameter might not be aligned to 128-bits, copying it to the union will fix that
  // as it has to be 128-bit aligned.
  xform.m = transform;
  __m128 R0 = xform.r[0]; // R0 to R4 are the rows of the transform matrix
  __m128 R1 = xform.r[1];
  __m128 R2 = xform.r[2];
  __m128 R3 = xform.r[3];
  for (unsigned i = 0; i < count; ++i)
  {
    // Hint to the CPU that we will access the memory ahead and it should warm the cache to avoid a cache miss
    // when we actually go to use it. A bit of tweaking was required to get the best value to look ahead by.
    _mm_prefetch((const char*)&inputStream[256], _MM_HINT_T0);
    __m128 vals;
    if (alignedInput) // note these 'if's are only on template parameters so aren't in the generated code
    {
      vals = (__m128)_mm_load_si128((const __m128i *)inputStream);
    }
    else
    {
      vals = _mm_loadu_ps(inputStream);
    }
    // vals contains the next x,y,z entry from the input stream
    // We now make a 128-bit X with x copied as x,x,x,x, and same for Y and Z.
    __m128 X = _mm_shuffle_ps(vals,vals,_MM_SHUFFLE(0,0,0,0));
    __m128 Y = _mm_shuffle_ps(vals,vals,_MM_SHUFFLE(1,1,1,1));
    __m128 Z = _mm_shuffle_ps(vals,vals,_MM_SHUFFLE(2,2,2,2));
    // We multiply Z with R2, so Z = z*R2[0], z*R2[1], z*R2[2], z*R2[3]
    Z = _mm_mul_ps(Z,R2);
    if (translate)
    {
      // We add R3, so Z = R3[0]+z*R2[0], R3[1]+z*R2[1], R3[2]+z*R2[2], R3[3]+z*R2[3]
      Z = _mm_add_ps(Z,R3);
    }
    // We multiply Y with R1, so Y = y*R1[0], ...
    Y = _mm_mul_ps(Y,R1);
    // We add Y to Z, so Z = R3[0]+z*R2[0]+y*R1[0] ...
    Z = _mm_add_ps(Z,Y);
    // We multiply X with R0, so X = x*R0[0], ...
    X = _mm_mul_ps(X,R0);
    // We add X to Z, so Z = R3[0] + z*R2[0] + y*R1[0] + x*R0[0], ...
    Z = _mm_add_ps(Z,X);
    // Z now contains our transformed point. For perspective transforms, we need to divide by W.
    if (divideByW)
    {
      Z = _mm_div_ps(Z,_mm_shuffle_ps(Z,Z,_MM_SHUFFLE(3,3,3,3)));
    }
    if (alignedOutput)
    {
      _mm_stream_ps(outputStream,Z); // aligned and cache friendly
    }
    else
    {
      _mm_storeu_ps(outputStream,Z); // unaligned store
    }
    inputStream  += inputStep;
    outputStream += outputStep;
  }
}

template <size_t N>
void Vector4f_SSETransformStream(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<true,false,false,4,false,4>((float*)outputStream, (float*)inputStream, N, transform);
}

template <size_t N>
void Vector4f_SSETransformCoordStream(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<true,true,false,4,false,4>((float*)outputStream, (float*)inputStream, N, transform);
}

template <size_t N>
void Vector4f_SSETransformNormalStream(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<false,false,false,4,false,4>((float*)outputStream, (float*)inputStream, N, transform);
}

template <size_t N>
void Vector4f_SSETransformStreamAligned(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<true,false,true,4,true,4>((float*)outputStream, (float*)inputStream, N, transform);
}

template <size_t N>
void Vector4f_SSETransformCoordStreamAligned(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<true,true,true,4,true,4>((float*)outputStream, (float*)inputStream, N, transform);
}

template <size_t N>
void Vector4f_SSETransformNormalStreamAligned(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<false,false,true,4,true,4>((float*)outputStream, (float*)inputStream, N, transform);
}


