#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <cstring>

namespace GDL
{
//! @brief Reads an address at the given position in memory and returns a pointer to that address
//! @param positionInMemory: Position in memory where the address should be read.
//! @return Pointer to read address
inline U8* ReadAddressFromMemory(const U8* positionInMemory)
{
    U8* address = nullptr;
    std::memcpy(&address, positionInMemory, sizeof(void*));
    return address;
}


//! @brief Reads a value of size_t at the given position in memory and returns it
//! @param positionInMemory: Position in memory where the value should be read.
//! @return Read value
inline size_t ReadSizeFromMemory(const void* positionInMemory)
{
    return *static_cast<const size_t*>(positionInMemory);
}


//! @brief Writes a pointer address into the memory at the passed position.
//! @param positionInMemory: Position in memory where the value should be written
//! @param addressToWrite: Address that should be written to the passed position
inline void WriteAddressToMemory(U8* positionInMemory, const void* addressToWrite)
{
    std::memcpy(positionInMemory, &addressToWrite, sizeof(void*));
}

//! @brief Writes a size_t value into the memory at the passed position.
//! @param positionInMemory: Position in memory where the value should be written
//! @param value: Value that should be written to the passed position
inline void WriteSizeToMemory(void* positionInMemory, const size_t value)
{
    size_t* valuePtr = static_cast<size_t*>(positionInMemory);
    *valuePtr = value;
}

} // namespace GDL
