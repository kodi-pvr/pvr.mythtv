/*
 *      Copyright (C) 2014 Jean-Luc Barriere
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301 USA
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#ifndef ATOMIC_H
#define	ATOMIC_H

#ifdef	__cplusplus
extern "C" {
#endif

#if defined _MSC_VER
#if !defined CC_INLINE
#define CC_INLINE __inline
#endif
#else
#if !defined CC_INLINE
#define CC_INLINE inline
#endif
#endif

#if defined _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
typedef volatile long atomic_t;
#elif defined __APPLE__
#include <libkern/OSAtomic.h>
typedef volatile int32_t atomic_t;
#else
typedef volatile int atomic_t;
#endif

#if defined __arm__ && (!defined __thumb__ || defined __thumb2__)
/* The __ARM_ARCH define is provided by gcc 4.8.  Construct it otherwise.  */
#ifndef __ARM_ARCH
#ifdef __ARM_ARCH_2__
#define __ARM_ARCH 2
#elif defined (__ARM_ARCH_3__) || defined (__ARM_ARCH_3M__)
#define __ARM_ARCH 3
#elif defined (__ARM_ARCH_4__) || defined (__ARM_ARCH_4T__)
#define __ARM_ARCH 4
#elif defined (__ARM_ARCH_5__) || defined (__ARM_ARCH_5E__) \
        || defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5TE__) \
        || defined(__ARM_ARCH_5TEJ__)
#define __ARM_ARCH 5
#elif defined (__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) \
        || defined (__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) \
        || defined (__ARM_ARCH_6K__) || defined(__ARM_ARCH_6T2__)
#define __ARM_ARCH 6
#elif defined (__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) \
        || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) \
        || defined(__ARM_ARCH_7EM__)
#define __ARM_ARCH 7
#else
#warning could not detect ARM architecture
#endif
#endif
#endif

/**
 * Atomically incremente a reference count variable.
 * \param valp address of atomic variable
 * \return incremented reference count
 */
static CC_INLINE long atomic_increment(atomic_t *valp)
{
  atomic_t __val;

#if defined _MSC_VER
  __val = InterlockedIncrement(valp);

#elif defined __APPLE__
  __val = OSAtomicIncrement32(valp);

#elif defined ANDROID
  __val = __sync_add_and_fetch(valp, 1);

#elif defined __mips__
  int temp, amount = 1;
  __asm__ volatile (
    "    .set   arch=r4000\n"
    "1:  ll     %0, %1\n"       /* load old value */
    "    addu   %2, %0, %3\n"   /* calculate new value */
    "    sc     %2, %1\n"       /* attempt to store */
    "    beqzl  %2, 1b\n"       /* spin if failed */
    "    .set   mips0\n"
    : "=&r" (__val), "=m" (*valp), "=&r" (temp)
    : "r" (amount), "m" (*valp));
  /* __val is the old value, so normalize it. */
  __val += amount;

#elif defined __i386__ || defined __i486__ || defined __i586__ || defined __i686__ || defined __x86_64__
  __asm__ volatile (
    "lock xaddl %0, (%1);"
    : "=r" (__val)
    : "r" (valp), "0" (1)
    : "cc", "memory"
    );
  /* __val is the old value, so normalize it. */
  ++__val;

#elif defined __powerpc__ || defined __ppc__ || defined __ppc64__
  int amount = 1;
  __asm__ volatile (
    "1:  lwarx   %0,0,%1\n"
    "    add     %0,%2,%0\n"
    "    dcbt    %0,%1\n"
    "    stwcx.  %0,0,%1\n"
    "    bne-    1b\n"
    "    isync\n"
    : "=&r" (__val)
    : "r" (valp), "r" (amount)
    : "cc", "memory");

#elif defined __sparc__ || defined __sparc64__
  atomic_t __old, __new = *valp;
  do
  {
    __old = __new;
    __new = __old + 1;
    /* compare and swap: if (*a == b) swap(*a, c) else c = *a */
    __asm__ volatile (
      "cas [%2], %3, %0"
      : "=&r" (__new)
      : "" (__new), "r" (valp), "r" (__old)
      : "memory");
  }
  while (__new != __old);
  __val = __old + 1;

#elif (defined __ARM_ARCH && __ARM_ARCH == 7)
  int amount = 1;
  __asm__ volatile (
    "    dmb     ish\n"           /* Memory barrier */
    "1:  ldrex   %0, [%1]\n"
    "    add     %0, %0, %2\n"
    "    strex   r1, %0, [%1]\n"
    "    cmp     r1, #0\n"
    "    bne     1b\n"
    "    dmb     ish\n"           /* Memory barrier */
    : "=&r" (__val)
    : "r" (valp), "r" (amount)
    : "r1", "memory");

#elif (defined __ARM_ARCH && __ARM_ARCH == 6)
  int amount = 1;
  __asm__ volatile (
    "mcr p15, 0, %0, c7, c10, 5"  /* Memory barrier */
    : : "r" (0) : "memory");
  __asm__ volatile (
    "1:  ldrex   %0, [%1]\n"
    "    add     %0, %0, %2\n"
    "    strex   r1, %0, [%1]\n"
    "    cmp     r1, #0\n"
    "    bne     1b\n"
    : "=&r" (__val)
    : "r" (valp), "r" (amount)
    : "r1");
  __asm__ volatile (
    "mcr p15, 0, %0, c7, c10, 5"  /* Memory barrier */
    : : "r" (0) : "memory");

#elif (defined __ARM_ARCH && __ARM_ARCH < 6)
  int tmp1, tmp2;
  int amount = 1;
  __asm__ volatile (
    "0:  ldr     %0, [%3]\n"
    "    add     %1, %0, %4\n"
    "    swp     %2, %1, [%3]\n"
    "    cmp     %0, %2\n"
    "    swpne   %0, %2, [%3]\n"
    "    bne     0b\n"
    : "=&r" (tmp1), "=&r" (__val), "=&r" (tmp2)
    : "r" (valp), "r" (amount)
    : "cc", "memory");

#elif defined __aarch64__
  unsigned long tmp;
  int amount = 1;
  __asm__ volatile (
    "    dmb     ish\n"           /* Memory barrier */
    "1:  ldxr    %w0, %2\n"
    "    add     %w0, %w0, %w3\n"
    "    stlxr   %w1, %w0, %2\n"
    "    cbnz    %w1, 1b\n"
    "    dmb     ish\n"           /* Memory barrier */
    : "=&r" (__val), "=&r" (tmp), "+Q" (*valp)
    : "Ir" (amount)
    : "memory");

#elif defined HAS_BUILTIN_SYNC_ADD_AND_FETCH
  /*
   * Don't know how to atomic increment for a generic architecture
   * so try to use GCC builtin
   */
  __val = __sync_add_and_fetch(valp, 1);

#else
#warning unknown architecture, atomic increment is not...
  __val = ++(*valp);

#endif
  return __val;
}

/**
 * Atomically decrement a reference count variable.
 * \param valp address of atomic variable
 * \return decremented reference count
 */
static CC_INLINE long atomic_decrement(atomic_t *valp)
{
  atomic_t __val;

#if defined _MSC_VER
  __val = InterlockedDecrement(valp);

#elif defined __APPLE__
  __val = OSAtomicDecrement32(valp);

#elif defined ANDROID
  __val = __sync_sub_and_fetch(valp, 1);

#elif defined __mips__
  int temp, amount = 1;
  __asm__ volatile (
    "    .set   arch=r4000\n"
    "1:  ll     %0, %1\n"       /* load old value */
    "    subu   %2, %0, %3\n"   /* calculate new value */
    "    sc     %2, %1\n"       /* attempt to store */
    "    beqzl  %2, 1b\n"       /* spin if failed */
    "    .set   mips0\n"
    : "=&r" (__val), "=m" (*valp), "=&r" (temp)
    : "r" (amount), "m" (*valp));
  /* __val is the old value, so normalize it */
  __val -= sub;

#elif defined __i386__ || defined __i486__ || defined __i586__ || defined __i686__ || defined __x86_64__
  __asm__ volatile (
    "lock xaddl %0, (%1);"
    : "=r" (__val)
    : "r" (valp), "0" (-1)
    : "cc", "memory"
    );
  /* __val is the pre-decrement value, so normalize it */
  --__val;

#elif defined __powerpc__ || defined __ppc__ || defined __ppc64__
  int amount = 1;
  __asm__ volatile (
    "1:  lwarx   %0,0,%1\n"
    "    subf    %0,%2,%0\n"
    "    dcbt    %0,%1\n"
    "    stwcx.  %0,0,%1\n"
    "    bne-    1b\n"
    "    isync\n"
    : "=&r" (__val)
    : "r" (valp), "r" (amount)
    : "cc", "memory");

#elif defined __sparc__ || defined __sparc64__
  atomic_t __old, __new = *valp;
  do
  {
    __old = __new;
    __new = __old - 1;
    /* compare and swap: if (*a == b) swap(*a, c) else c = *a */
    __asm__ volatile (
      "cas [%2], %3, %0"
      : "=&r" (__new)
      : "" (__new), "r" (valp), "r" (__old)
      : "memory");
  }
  while (__new != __old);
  __val = __old - 1;

#elif (defined __ARM_ARCH && __ARM_ARCH == 7)
  int amount = 1;
  __asm__ volatile (
    "    dmb     ish\n"           /* Memory barrier */
    "1:  ldrex   %0, [%1]\n"
    "    sub     %0, %0, %2\n"
    "    strex   r1, %0, [%1]\n"
    "    cmp     r1, #0\n"
    "    bne     1b\n"
    "    dmb     ish\n"           /* Memory barrier */
    : "=&r" (__val)
    : "r" (valp), "r" (amount)
    : "r1", "memory");

#elif (defined __ARM_ARCH && __ARM_ARCH == 6)
  int amount = 1;
  __asm__ volatile (
    "mcr p15, 0, %0, c7, c10, 5"  /* Memory barrier */
    : : "r" (0) : "memory");
  __asm__ volatile (
    "1:  ldrex   %0, [%1]\n"
    "    sub     %0, %0, %2\n"
    "    strex   r1, %0, [%1]\n"
    "    cmp     r1, #0\n"
    "    bne     1b\n"
    : "=&r" (__val)
    : "r" (valp), "r" (amount)
    : "r1");
  __asm__ volatile (
    "mcr p15, 0, %0, c7, c10, 5"  /* Memory barrier */
    : : "r" (0) : "memory");

#elif (defined __ARM_ARCH && __ARM_ARCH < 6)
  int tmp1, tmp2;
  int amount = -1;
  __asm__ volatile (
    "0:  ldr     %0, [%3]\n"
    "    add     %1, %0, %4\n"
    "    swp     %2, %1, [%3]\n"
    "    cmp     %0, %2\n"
    "    swpne   %0, %2, [%3]\n"
    "    bne     0b\n"
    : "=&r" (tmp1), "=&r" (__val), "=&r" (tmp2)
    : "r" (valp), "r" (amount)
    : "cc", "memory");

#elif defined __aarch64__
  unsigned long tmp;
  int amount = 1;
  __asm__ volatile (
    "    dmb     ish\n"           /* Memory barrier */
    "1:  ldxr    %w0, %2\n"
    "    sub     %w0, %w0, %w3\n"
    "    stlxr   %w1, %w0, %2\n"
    "    cbnz    %w1, 1b\n"
    "    dmb     ish\n"           /* Memory barrier */
    : "=&r" (__val), "=&r" (tmp), "+Q" (*valp)
    : "Ir" (amount)
    : "memory");

#elif defined HAS_BUILTIN_SYNC_SUB_AND_FETCH
  /*
   * Don't know how to atomic decrement for a generic architecture
   * so try to use GCC builtin
   */
  __val = __sync_sub_and_fetch(valp, 1);

#else
#warning unknown architecture, atomic deccrement is not...
  __val = --(*valp);

#endif
  return __val;
}

#ifdef	__cplusplus
}
#endif

#endif	/* ATOMIC_H */
