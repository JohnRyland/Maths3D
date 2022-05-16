////////////////////////////////////////////////////////////////////////////////////
// About

//
// Simple BMP saving
//


////////////////////////////////////////////////////////////////////////////////////
// License

//
// Copyright (c) 2022, John Ryland
// All rights reserved.
//
// BSD-2-Clause License. See included LICENSE file for details.
// If LICENSE file is missing, see: https://opensource.org/licenses/BSD-2-Clause
//


////////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdio>
#include "endian.h"
#include "bitmap.h"


////////////////////////////////////////////////////////////////////////////////////
// Bitmap (BMP) Image Format Header

struct BitmapFileHeader
{
  uint8_t     magic[2];
  // Normally here between 'magic' and 'fileSize' the compiler would need to add
  // padding if fileSize was a uint32_t, but it doesn't need to when it is a uint32_le.
  uint32_le   fileSize;
  uint32_le   reserved;
  uint32_le   imageDataFileOffset;
  uint32_le   headerSize;   // expect 40 or 108 (124?)
  uint32_le   width;
  uint32_le   height;
  uint16_le   planes;       // 1
  uint16_le   bitsPerPixel; // 24 or 32
  uint32_le   format;       // expect 0 = uncompressed RGB, 3 = ARGB
  uint32_le   imageSize;    // can be 0 for BI_RGB
  uint32_le   horzRes;
  uint32_le   vertRes;
  uint32_le   paletteSize;
  uint32_le   importantColors;
};

static_assert(sizeof(BitmapFileHeader) == 54,
              "Unexpected Bitmap header size, compiler might be adding padding");


////////////////////////////////////////////////////////////////////////////////////
// Image 

bool Image_SaveBitmap(const Image& image, const char* fileName)
{
  FILE* file = fopen(fileName, "wb");
  if (!file)
  {
    printf("Couldn't open %s for writing\n", fileName);
    return false;
  }

  int bytesPerPixel = 4;
  size_t pixelDataSize = (size_t)image.width * image.height * bytesPerPixel;

  // Save the header
  BitmapFileHeader header = { 0 };
  header.magic[0] = 'B';
  header.magic[1] = 'M';
  header.fileSize = pixelDataSize + sizeof(header);
  header.imageDataFileOffset = sizeof(header);
  header.headerSize = 40;
  header.width = image.width;
  header.height = image.height;
  header.planes = 1;
  header.bitsPerPixel = bytesPerPixel * 8;
  header.format = 0;
  header.imageSize = 0;
  header.horzRes = 0;
  header.vertRes = 0;
  header.importantColors = 0;
  printf("Saving image %s, size: %i x %i, header: %lu\n",
         fileName, (uint32_t)header.width, (uint32_t)header.height, sizeof(header));

  // Write the image header
  bool success = fwrite((void*)&header, 1, sizeof(header), file) == sizeof(header);

  // Write the pixel data
  success = success && fwrite((void*)image.pixels, 1, pixelDataSize, file) == pixelDataSize;

  fclose(file);

  if (!success)
  {
    printf("Error while writing image %s\n", fileName);
  }
  return success;
}

