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

#ifndef MYTHSHAREDPTR_H
#define	MYTHSHAREDPTR_H

#include "atomic.h"

namespace Myth
{

  template<class T>
  class shared_ptr
  {
  public:

    shared_ptr() : p(), c() { }

    explicit shared_ptr(T* s) : p(s), c(new atomic_t(1)) { }

    shared_ptr(const shared_ptr& s) : p(s.p), c(s.c)
    {
      if (c)
        atomic_increment(c);
    }

    shared_ptr& operator=(const shared_ptr& s)
    {
      if (this != &s)
      {
        reset();
        p = s.p;
        c = s.c;
        if (c)
          atomic_increment(c);
      }
      return *this;
    }

    ~shared_ptr()
    {
      reset();
    }

    void reset()
    {
      if (c)
      {
        if (*c == 1)
          delete p;
        if (!atomic_decrement(c))
          delete c;
      }
      c = 0;
      p = 0;
    }

    void reset(T* s)
    {
      if (p != s)
      {
        reset();
        if (s)
        {
          p = s;
          c = new atomic_t(1);
        }
      }
    }

    T *get() const
    {
      return (c) ? p : 0;
    }

    void swap(shared_ptr<T>& s)
    {
      T *tmp_p = p;
      atomic_t *tmp_c = c;
      p = s.p;
      c = s.c;
      s.p = tmp_p;
      s.c = tmp_c;
    }

    T *operator->() const
    {
      return get();
    }

    T& operator*() const
    {
      return *get();
    }

    operator bool() const
    {
      return p != 0;
    }

    bool operator!() const
    {
      return p == 0;
    }

  protected:
    T *p;
    atomic_t *c;
  };

}

#endif	/* MYTHSHAREDPTR_H */
