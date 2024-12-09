/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define AssertNew(ptr) { if (ptr == NULL) { assert(false); abort(); } }

#ifdef _MSC_VER
#  define INLINE inline
#else
#  define INLINE static inline
#endif

/// @brief Pointeur dans un fichier.
/// Il s'agit du nombre d'octets après le début du fichier.
typedef uint64_t Pointer;

/// @brief Pointeur vers un noeud d'un AVL dans un fichier index (.idx).
typedef uint64_t NodePointer;

/// @brief Pointeur vers une entrée dans un fichier de données (.dat).
typedef uint64_t EntryPointer;

///@brief Equivalent du pointeur NULL pour une position dans un fichier.
#define INVALID_POINTER (uint64_t)(-1)

///@brief Valeur du pointeur nextFreePtr pour une entrée valide dans une table.
#define VALID_ENTRY (uint64_t)(-2)

INLINE int FSeek(FILE *stream, int64_t offset, int origin)
{
#ifdef _MSC_VER
    return _fseeki64(stream, offset, origin);
#else
    return fseek(stream, offset, origin);
#endif
}

INLINE int64_t FTell(FILE *stream)
{
#ifdef _MSC_VER
    return _ftelli64(stream);
#else
    return ftell(stream);
#endif
}