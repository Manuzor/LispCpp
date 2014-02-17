#pragma once

#include <Foundation/Basics.h>

/// \brief This class provides functions to work on raw memory.
class ezMemoryUtils
{
public:
  /// \brief Constructs \a uiCount objects of type T in a raw buffer at \a pDestination.
  ///
  /// You should use 'DefaultConstruct' instead if default construction is needed for trivial types as well.
  template <typename T>
  static void Construct(T* pDestination, size_t uiCount); // [tested]

  /// \brief Constructs \a uiCount objects of type T in a raw buffer at \a pDestination, by creating \a uiCount copies of \a copy.
  template <typename T>
  static void Construct(T* pDestination, const T& copy, size_t uiCount); // [tested]

  /// \brief Constructs \a uiCount objects of type T in a raw buffer at \a pDestination from an existing array of objects at \a pSource by using copy construction.
  template <typename T>
  static void Construct(T* pDestination, const T* pSource, size_t uiCount); // [tested]

  /// \brief Destructs \a uiCount objects of type T at \a pDestination.
  template <typename T>
  static void Destruct(T* pDestination, size_t uiCount); // [tested]

  /// \brief Default constructs \a uiCount objects of type T in a raw buffer at \a pDestination regardless of T being a class, POD or trivial.
  template <typename T>
  static void DefaultConstruct(T* pDestination, size_t uiCount); // [tested]

  /// \brief Copies objects of type T from \a pSource to \a pDestination.
  ///
  /// You should use 'Move' instead of 'Copy', when the source and destination buffer might overlap.
  template <typename T>
  static void Copy(T* pDestination, const T* pSource, size_t uiCount); // [tested]

  /// \brief Moves objects of type T from \a pSource to \a pDestination.
  ///
  /// You should use 'Move' instead of 'Copy', when the source and destination buffer might overlap.
  template <typename T>
  static void Move(T* pDestination, const T* pSource, size_t uiCount); // [tested]

  /// \brief Tests if objects of type T from \a pSource and \a pDestination are equal.
  template <typename T>
  static bool IsEqual(const T* a, const T* b, size_t uiCount = 1); // [tested]

  /// \brief Zeros out buffer of a raw memory.
  template <typename T>
  static void ZeroFill(T* pDestination, size_t uiCount = 1); // [tested]

  /// \brief Compares two buffers of raw memory byte wise.
  template <typename T>
  static ezInt32 ByteCompare(const T* a, const T* b, size_t uiCount = 1); // [tested]

  /// \brief Returns the address stored in \a ptr plus the given byte offset \a iOffset, cast to type \a T.
  ///
  /// This is useful when working with raw memory, to safely modify a pointer without having to take care of the
  /// details of pointer arithmetic.
  template <typename T>
  static T* AddByteOffset(T* ptr, ptrdiff_t iOffset); // [tested]

  /// \brief Returns the address stored in \a ptr plus the given byte offset \a iOffset, cast to type \a const \a T.
  ///
  /// This is useful when working with raw memory, to safely modify a pointer without having to take care of the
  /// details of pointer arithmetic. Also it allows you to modify the pointer without having to do a const_cast.
  template <typename T>
  static const T* AddByteOffsetConst(const T* ptr, ptrdiff_t iOffset); // [tested]

  /// \brief Alignes the pointer \a ptr by moving its address backwards to the previous multiple of \a uiAlignment.  
  template <typename T>
  static T* Align(T* ptr, size_t uiAlignment); // [tested]

  /// \brief Checks whether \a ptr is aligned to a memory address that is a multiple of \a uiAlignment.
  template <typename T>
  static bool IsAligned(const T* ptr, size_t uiAlignment); // [tested]

  /// \brief Reserves the lower 4GB of address space in 64-bit builds to ensure all allocations start above 4GB.
  ///
  /// \note Note that this does NOT reserve 4GB of RAM, only address space.
  ///       This can help to detect pointer truncation. In 32-bit builds it does nothing.
  ///
  /// Currently only implemented on Windows.
  static void ReserveLower4GBAddressSpace();

private:
  template <typename T>
  static void Construct(T* pDestination, size_t uiCount, ezTypeIsPod);
  template <typename T>
  static void Construct(T* pDestination, size_t uiCount, ezTypeIsClass);

  template <typename T>
  static void Construct(T* pDestination, const T& copy, size_t uiCount, ezTypeIsPod);
  template <typename T>
  static void Construct(T* pDestination, const T& copy, size_t uiCount, ezTypeIsClass);

  template <typename T>
  static void Construct(T* pDestination, const T* pSource, size_t uiCount, ezTypeIsPod);
  template <typename T>
  static void Construct(T* pDestination, const T* pSource, size_t uiCount, ezTypeIsClass);

  template <typename T>
  static void Destruct(T* pDestination, size_t uiCount, ezTypeIsPod);
  template <typename T>
  static void Destruct(T* pDestination, size_t uiCount, ezTypeIsClass);

  template <typename T>
  static void Copy(T* pDestination, const T* pSource, size_t uiCount, ezTypeIsPod);
  template <typename T>
  static void Copy(T* pDestination, const T* pSource, size_t uiCount, ezTypeIsClass);

  template <typename T>
  static void Move(T* pDestination, const T* pSource, size_t uiCount, ezTypeIsPod);
  template <typename T>
  static void Move(T* pDestination, const T* pSource, size_t uiCount, ezTypeIsClass);

  template <typename T>
  static bool IsEqual(const T* a, const T* b, size_t uiCount, ezTypeIsPod);
  template <typename T>
  static bool IsEqual(const T* a, const T* b, size_t uiCount, ezTypeIsClass);
};

#include <Foundation/Memory/Implementation/MemoryUtils_inl.h>

