/***************************************************************************//**
 * @brief Data store and fetch routines.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "connect/ember.h"
#include <assert.h>
#include <string.h>

/**
 * @brief Returns the low byte of the 16-bit value \c n as an \c uint8_t.
 */
#define LOW_BYTE(n)                     ((uint8_t)(n))

/**
 * @brief Returns the high byte of the 16-bit value \c n as an \c uint8_t.
 */
#define HIGH_BYTE(n)                    LOW_BYTE((n) >> 8)

/**
 * @brief Returns the value built from the two \c uint8_t
 * values \c high and \c low.
 */
#define HIGH_LOW_TO_INT(high, low) (        \
    ((uint16_t) (((uint16_t) (high)) << 8)) \
    + ((uint16_t) ((low) & 0xFFu))          \
    )

uint16_t emberFetchLowHighInt16u(const uint8_t *contents)
{
  return HIGH_LOW_TO_INT(contents[1], contents[0]);
}

uint16_t emberFetchHighLowInt16u(const uint8_t *contents)
{
  return HIGH_LOW_TO_INT(contents[0], contents[1]);
}

void emberStoreLowHighInt16u(uint8_t *contents, uint16_t value)
{
  contents[0] = LOW_BYTE(value);
  contents[1] = HIGH_BYTE(value);
}

void emberStoreHighLowInt16u(uint8_t *contents, uint16_t value)
{
  contents[0] = HIGH_BYTE(value);
  contents[1] = LOW_BYTE(value);
}

void emStoreInt32u(bool lowHigh, uint8_t* contents, uint32_t value)
{
  uint8_t ii;
  for (ii = 0; ii < 4; ii++) {
    uint8_t index = (lowHigh ? ii : 3 - ii);
    contents[index] = (uint8_t)(value & 0xFF);
    value = (value >> 8);
  }
}

uint32_t emFetchInt32u(bool lowHigh, const uint8_t* contents)
{
  uint8_t b0;
  uint8_t b1;
  uint8_t b2;
  uint8_t b3;

  if (lowHigh) {
    b0 = contents[3];
    b1 = contents[2];
    b2 = contents[1];
    b3 = contents[0];
  } else {
    b0 = contents[0];
    b1 = contents[1];
    b2 = contents[2];
    b3 = contents[3];
  }
  return ((((uint32_t) ((((uint16_t) b0) << 8) | ((uint16_t) b1)))
           << 16)
          | (uint32_t) ((((uint16_t) b2) << 8) | ((uint16_t) b3)));
}

bool emMemoryByteCompare(const uint8_t *bytes, uint8_t count, uint8_t target)
{
  uint8_t i;
  for (i = 0; i < count; i++, bytes++) {
    if (*bytes != target) {
      return false;
    }
  }
  return true;
}

void emberReverseMemCopy(uint8_t *dest, const uint8_t *src, uint8_t length)
{
  uint8_t i;
  for (i = 0; i < length; i++) {
    dest[i] = src[length - 1 - i];
  }
}

uint16_t emStrlen(const uint8_t * const string)
{
  uint16_t i = 0;

  if (string != NULL) {
    while (string[i] != '\0' && i != 0xFFFF) {
      i++;
    }
    assert(i != 0xFFFF);
  }

  return i;
}

int8_t emStrcmp(const uint8_t *s1, const uint8_t *s2)
{
  uint8_t c1, c2;

  do {
    c1 = (uint8_t) *s1++;
    c2 = (uint8_t) *s2++;
    if (c1 == '\0') {
      return c1 - c2;
    }
  } while (c1 == c2);
  return c1 - c2;
}

//----------------------------------------------------------------
// Returns length of the matching prefix of x and y in bits.

uint16_t emMatchingPrefixBitLength(const uint8_t *x, uint16_t xLength,
                                   const uint8_t *y, uint16_t yLength)
{
  uint16_t length = (xLength < yLength
                     ? xLength
                     : yLength);
  uint16_t count = 0;
  for (; count + 8 <= length && *x == *y; x++, y++, count += 8) {
    ;
  }
  if (count < length) {
    uint8_t mask = 0x80;
    uint8_t same = ~(*x ^ *y);
    for (; count < length && (mask & same); mask >>= 1, count += 1) {
      ;
    }
  }
  return count;
}

// Copy the first 'length' bits of the prefix.

void emBitCopy(uint8_t *to, const uint8_t *from, uint16_t count)
{
  uint8_t fullBytes = count >> 3;
  uint8_t extraBits = count & 0x07;
  memmove(to, from, fullBytes);
  if (extraBits != 0) {
    uint8_t mask = (0xFF >> extraBits);
    to[fullBytes] = ((to[fullBytes] & mask)
                     | (from[fullBytes] & ~mask));
  }
}
