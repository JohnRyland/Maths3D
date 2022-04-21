////////////////////////////////////////////////////////////////////////////////////
// About

//
// Endian and structure packing helpers
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

/// \file endian.h
///
/// Allows us to map to/from bytes stored in little endian order (irrespective of host machine endianess).
///
/// Also as a side-effect, it allows out of alignment access.
///
/// And another side-effect is that structures of these will be tightly packed, there is no padding
/// as would happen with the normal uint16_t and uint32_t. The padding is added for the reason of
/// alignment, but because these types can be accessed without any particular byte alignment, then
/// padding isn't needed.


////////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdint>


////////////////////////////////////////////////////////////////////////////////////
// Endian and structure packing helpers

/// \brief
/// Drop in replacement for uint16_t which can help simplify dealing with endian conversions.
/// This is to be used in structs for file formats or protocols that are little-endian.
/// \note
/// Not as efficient as using uint16_t for general purpose use in structs, however it is
/// excellent for use in structs used to handle protocols and file formats for serialization
/// and de-serialization. It has the added benefit of making the structs packed.
class uint16_le
{
public:
  uint16_le()                          { }
  uint16_le(uint16_t val)              { operator=(val); }
  operator uint16_t() const            { return uint16_t(bytes[1] << 8U) | bytes[0]; }
  uint16_le& operator=(uint16_t val)   { bytes[1] = val >> 8U; bytes[0] = val & 0xFF; return *this; }
private:
  uint8_t bytes[2];
};

/// \brief
/// Drop in replacement for uint32_t which can help simplify dealing with endian conversions.
/// This is to be used in structs for file formats or protocols that are little-endian.
/// \see uint16_le
class uint32_le
{
public:
  uint32_le()                         { }
  uint32_le(uint32_t val)             { operator=(val); }
  operator uint32_t() const           { return uint32_t(bytes[3] << 24U) | uint32_t(bytes[2] << 16U) | uint32_t(bytes[1] << 8U) | bytes[0]; }
  uint32_le& operator=(uint32_t val)  { bytes[3] = val >> 24U; bytes[2] = val >> 16U; bytes[1] = val >> 8U; bytes[0] = val; return *this; }
private:
  uint8_t bytes[4];
};

/// \brief
/// Drop in replacement for uint32_t which can help simplify dealing with endian conversions.
/// This is to be used in structs for file formats or protocols that are big-endian.
/// \see uint16_le
class uint16_be
{
public:
  uint16_be()                          { }
  uint16_be(uint16_t val)              { operator=(val); }
  operator uint16_t() const            { return uint16_t(bytes[0] << 8U) | bytes[1]; }
  uint16_be& operator=(uint16_t val)   { bytes[0] = val >> 8U; bytes[1] = val & 0xFF; return *this; }
private:
  uint8_t bytes[2];
};

/// \brief
/// Drop in replacement for uint32_t which can help simplify dealing with endian conversions.
/// This is to be used in structs for file formats or protocols that are big-endian.
/// \see uint16_le
class uint32_be
{
public:
  uint32_be()                         { }
  uint32_be(uint32_t val)             { operator=(val); }
  operator uint32_t() const           { return uint32_t(bytes[0] << 24U) | uint32_t(bytes[1] << 16U) | uint32_t(bytes[2] << 8U) | bytes[3]; }
  uint32_be& operator=(uint32_t val)  { bytes[0] = val >> 24U; bytes[1] = val >> 16U; bytes[2] = val >> 8U; bytes[3] = val; return *this; }
private:
  uint8_t bytes[4];
};

