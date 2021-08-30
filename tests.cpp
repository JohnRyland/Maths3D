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


#include <cmath>
#include "maths3d_ext.h"


// Exercise all the Vector4f functions (some indirectly)
int vector_tests()
{
  Vector4f v1 = Vector4f_SetW(Vector4f_SetZ(Vector4f_Zero(), 1.0), 2.0);
  Vector4f v2 = Vector4f_SetY(Vector4f_SetX(Vector4f_Zero(), 3.0), 4.0);
  Vector4f v3 = Vector4f_Add(v1, v2);
  Vector4f cross = Vector4f_CrossProduct(v1, v2);
  Vector4f norm1 = Vector4f_Normalized(v3);
  Vector4f norm2 = Vector4f_Normalized(Vector4f_Scaled(v3, 10.0));
  Vector4f expectedCross = Vector4f_Set(-4.0, -3.0, 0.0, 1.0);
  for (int i = 0; i < 4; ++i)
  {
    if (cross.v[i] != expectedCross.v[i])
      return -1;
    if (norm1.v[i] != norm2.v[i])
      return -1;
  }
  return 0;
}

// Exercise all the Matrix4x4f functions (some indirectly)
int matrix_tests()
{
  // TODO
  return 0;
}

// Exercise all the extension functions
int extension_tests()
{
  // TODO
  return 0;
}

int main(int argc, const char* argv[])
{
  return vector_tests() ||
         matrix_tests() ||
         extension_tests();
}

