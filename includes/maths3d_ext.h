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


///////////////////////////////////////////////////////////////////////////////////
// Documentation

/// \file maths3d_ext.h
///
/// Extensions to the Maths3D API.
///
/// Transforming large arrays of vectors is performance critical to many 3D pipelines.
/// This file contains SSE optimizations for doing this in a flexible way that this
/// may be useful in a variety of situations. However this is platform specific code,
/// so fallback code may be required.
///
/// It should be a todo that based on a preprocessor define, this SSE optimized code
/// is used or some fallback code is used instead which would need to be written.


///////////////////////////////////////////////////////////////////////////////////
// Includes

#include "maths3d.h"
#include <xmmintrin.h>
#include <emmintrin.h>


/// Transforms arrays of vectors by the transform matrix. 
/// \param translate is a bool to enable or disable applying the translation component of the transform.
/// \param divideByW is a bool to enable or disable applying perspective by dividing by W.
/// \param alignedOutput is a bool to flag if outputStream is aligned to a 128-bit boundary or not.
/// \param outputStep is the width in floats to the next vector element of the outputStream array.
///        This allows the outputStream array to be an array of structures of which the vector is a member.
///        These structures should be 32-bit aligned, and if the size is not a multiple of 128-bits, then
///        alignedOutput shouldn't be set.
/// \param alignedInput is a bool to flag if inputStream is aligned to a 128-bit boundary or not.
/// \param inputStep is the width in floats to the next vector element of the inputStream array.
///        This allows the inputStream array to be an array of structures of which the vector is a member.
///        These structures should be 32-bit aligned, and if the size is not a multiple of 128-bits, then
///        alignedInput shouldn't be set.
/// \param outputStream is the output buffer to put the transformed vectors to.
/// \param inputStream is the input array of vectors to apply the transform to.
/// \param count is the number of vectors to transform.
/// \param transform is the matrix to apply.
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

/// Transforms arrays of vectors by the transform matrix (non-SSE fallback implementation, untested). 
/// \param translate is a bool to enable or disable applying the translation component of the transform.
/// \param divideByW is a bool to enable or disable applying perspective by dividing by W.
/// \param alignedOutput is ignored
/// \param outputStep is the width in floats to the next vector element of the outputStream array.
///        This allows the outputStream array to be an array of structures of which the vector is a member.
///        These structures should be 32-bit aligned, and if the size is not a multiple of 128-bits, then
///        alignedOutput shouldn't be set.
/// \param alignedInput is ignored
/// \param inputStep is the width in floats to the next vector element of the inputStream array.
///        This allows the inputStream array to be an array of structures of which the vector is a member.
///        These structures should be 32-bit aligned, and if the size is not a multiple of 128-bits, then
///        alignedInput shouldn't be set.
/// \param outputStream is the output buffer to put the transformed vectors to.
/// \param inputStream is the input array of vectors to apply the transform to.
/// \param count is the number of vectors to transform.
/// \param transform is the matrix to apply.
template <bool translate, bool divideByW, bool alignedOutput, int outputStep, bool alignedInput, int inputStep>
void Vector4f_TransformStreamGeneric(float* outputStream, const float* inputStream, unsigned count, const Matrix4x4f& transform)
{
  for (unsigned i = 0; i < count; ++i)
  {
    Vector4f Z = Vector4f_Multiply(transform.row[2], Vector4f_Replicate(inputStream[2])); // z
    if (translate)
    {
      Z = Vector4f_Add(transform.row[3], Z);
    }
    // This form of the transform function is tailored for platforms with fused multiply and add instructions
    // and the compiler flags can be configured so that the compiler will be able to emit these.
    // See the Vector4f_Transform function for the alternative form which transposes the transform matrix
    // first and then uses dot products instead. On some platforms where there are optimizations for dot-products
    // then this might be faster instead.
    Z = Vector4f_Add(Z, Vector4f_Multiply(transform.row[1], Vector4f_Replicate(inputStream[1]))); // y
    Z = Vector4f_Add(Z, Vector4f_Multiply(transform.row[0], Vector4f_Replicate(inputStream[0]))); // x
    if (divideByW)
    {
      Z = Vector4f_Multiply(Z, Vector4f_Replicate(Scalar1f_One() / Z.w));
    }
    *(Vector4f*)outputStream = Z;
    inputStream  += inputStep;
    outputStream += outputStep;
  }
}

/// Specialization of Vector4f_SSETransformStreamGeneric for transforming an array of vectors without applying perspective.
/// \see Vector4f_SSETransformStreamGeneric
template <size_t N>
void Vector4f_SSETransformStream(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<true,false,false,4,false,4>((float*)outputStream, (float*)inputStream, N, transform);
}

/// Specialization of Vector4f_SSETransformStreamGeneric for transforming an array of vectors with perspective.
/// \see Vector4f_SSETransformStreamGeneric
template <size_t N>
void Vector4f_SSETransformCoordStream(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<true,true,false,4,false,4>((float*)outputStream, (float*)inputStream, N, transform);
}

/// Specialization of Vector4f_SSETransformStreamGeneric for transforming an array of normal vectors.
/// \see Vector4f_SSETransformStreamGeneric
template <size_t N>
void Vector4f_SSETransformNormalStream(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<false,false,false,4,false,4>((float*)outputStream, (float*)inputStream, N, transform);
}

/// Specialization of Vector4f_SSETransformStreamGeneric for transforming an array of vectors without applying perspective
/// where the arrays of vectors have been prepared to be aligned to a 128-bit boundary.
/// \see Vector4f_SSETransformStreamGeneric
template <size_t N>
void Vector4f_SSETransformStreamAligned(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<true,false,true,4,true,4>((float*)outputStream, (float*)inputStream, N, transform);
}

/// Specialization of Vector4f_SSETransformStreamGeneric for transforming an array of vectors with perspective
/// where the arrays of vectors have been prepared to be aligned to a 128-bit boundary.
/// \see Vector4f_SSETransformStreamGeneric
template <size_t N>
void Vector4f_SSETransformCoordStreamAligned(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<true,true,true,4,true,4>((float*)outputStream, (float*)inputStream, N, transform);
}

/// Specialization of Vector4f_SSETransformStreamGeneric for transforming an array of normal vectors
/// where the arrays of vectors have been prepared to be aligned to a 128-bit boundary.
/// \see Vector4f_SSETransformStreamGeneric
template <size_t N>
void Vector4f_SSETransformNormalStreamAligned(Vector4f (&outputStream)[N], const Vector4f (&inputStream)[N], const Matrix4x4f& transform)
{
  Vector4f_SSETransformStreamGeneric<false,false,true,4,true,4>((float*)outputStream, (float*)inputStream, N, transform);
}

