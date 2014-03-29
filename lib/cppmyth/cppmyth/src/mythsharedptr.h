/*
 * File:   sharedptr.h
 * Author: jlb
 *
 * Created on 24 janvier 2014, 20:42
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
