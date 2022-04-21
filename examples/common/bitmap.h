#pragma once

////////////////////////////////////////////////////////////////////////////////////
// About

//
// Simple BMP image saving
// Minimal code needed to produce an image output
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

/// \file bitmap.h
///
/// Very simple code to take an array of pixels (with a width and height) and
/// output it to a file in BMP format.
///


////////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdint>
#include <cstdio>


////////////////////////////////////////////////////////////////////////////////////
// Image 

/// \brief
/// Simple image which is a buffer of 32-bpp ARGB values in an array.
///
/// Assumes little endian. Offset 0 of pixels is at coordinate (0,0) of
/// the image which is in the lower left. Offset width is at (width,0) and
/// so on up to offset width*height which is the top right at (width,height).
///
/// There is no bounds checking, so it is up to the user to ensure that
/// pixels is a valid pointer to an array of width*height*sizeof(uint32_t) bytes.
struct Image
{
  uint32_t    width;
  uint32_t    height;
  uint32_t*   pixels;
};


////////////////////////////////////////////////////////////////////////////////////
// Bitmap Saving

/// Saves the image to the file named fileName
bool Image_SaveBitmap(const Image& image, const char* fileName);

