////////////////////////////////////////////////////////////////////////////////////
// About

//
// Endian and structure packing helpers
//


////////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdint>


////////////////////////////////////////////////////////////////////////////////////
// Endian and structure packing helpers

// Allows us to map to/from bytes stored in little endian order (irrespective of host machine endianess).
// Also as a side-effect, it allows out of alignment access.
// And another side-effect is that structures of these will be tightly packed, there is no padding
// as would happen with the normal uint16_t and uint32_t. The padding is added for the reason of
// alignment, but because these types can be accessed without any particular byte alignment, then
// padding isn't needed.
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

