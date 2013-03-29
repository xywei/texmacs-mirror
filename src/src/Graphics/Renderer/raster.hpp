
/******************************************************************************
* MODULE     : raster.hpp
* DESCRIPTION: Raster pictures
* COPYRIGHT  : (C) 2013  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#ifndef RASTER_H
#define RASTER_H
#include "picture.hpp"

template<class C> class raster;

template<class C>
class raster_rep: public picture_rep {
public:
  picture_kind kind;
  int w, h;
  int ox, oy;
  C* a;

public:
  raster_rep (picture_kind kind2, int w2, int h2, int ox2, int oy2):
    kind (kind2), w (w2), h (h2), ox (ox2), oy (oy2), a (NULL) {
      if (w * h != 0) a= tm_new_array<C> (w * h); }
  ~raster_rep () { if (w * h != 0) tm_delete_array (a); }

  picture_kind get_type () { return kind; }
  void* get_handle () { return (void*) this; }

  int get_width () { return w; }
  int get_height () { return h; }
  int get_origin_x () { return ox; }
  int get_origin_y () { return oy; }

  color get_pixel (int x, int y) {
    if (0 > x || 0 > y || x >= w || y >= h) return 0;
    else return (color) (a [y*w + x]);
  }

  void set_pixel (int x, int y, color c) {
    if (0 > x || 0 > y || x >= w || y >= h);
    else a [y*w + x]= C (c);
  }
};

/******************************************************************************
* Low level routines for raster manipulation
******************************************************************************/

template<class C> void
hide_alpha (C* d, const C* s, int w, int h) {
  for (int y=0; y<h; y++) {
    int o= y * w;
    for (int x=0; x<w; x++)
      d[o+x]= hide_alpha (s[o+x]);
  }
}

template<class C> void
show_alpha (C* d, const C* s, int w, int h) {
  for (int y=0; y<h; y++) {
    int o= y * w;
    for (int x=0; x<w; x++)
      d[o+x]= show_alpha (s[o+x]);
  }
}

template<class D, class S1, class S2> void
convolute (D* d, const S1* s1, const S2* s2,
           int s1w, int s1h, int s2w, int s2h) {
  if (s1w * s1h == 0) return;
  int dw= s1w + s2w - 1, dh= s1h + s2h - 1;
  for (int i=0; i<dw*dh; i++) d[i]= D ((color) 0);
  S1* temp= tm_new_array<S1> (s1w * s1h);
  hide_alpha (temp, s1, s1w, s1h);
  for (int y1=0; y1<s1h; y1++)
    for (int y2=0; y2<s2h; y2++) {
      int o1= y1 * s1w, o2= y2 * s2w, o= (y1 + y2) * dw;
      for (int x1=0; x1<s1w; x1++)
        for (int x2=0; x2<s2w; x2++)
          d[o+x1+x2] += s1[o1+x1] * s2[o2+x2];
    }
  show_alpha (d, d, dw, dh);
  tm_delete_array (temp);
}

template<class C> void
gaussian (C* d, int R, float r) {
  int w= 2*R+1, h= 2*R+1;
  float lambda= 1.0 / (2.0 * acos (0.0) * r * r);
  double sq_r= r*r;
  for (int y=0; y<h; y++) {
    double sq_y= (y-R)*(y-R);
    for (int x=0; x<w; x++) {
      double sq_x= (x-R)*(x-R);
      d[y*w+x]= C (lambda * ((float) exp (- (sq_x + sq_y) / sq_r)));
    }
  }
}

template<class C, class F> void
blur (C* d, const C* s, int w, int h, int R, float r) {
  int tw= 2*R+1, th= 2*R+1;
  F* temp= tm_new_array<F> (tw * th);
  gaussian (temp, R, r);
  convolute (d, s, temp, w, h, tw, th);
  tm_delete_array (temp);  
}

/******************************************************************************
* Low level composition
******************************************************************************/

template<composition_mode M, class D, class S>
struct composer {
  static inline void op (D& dest, const S& src) { (void) dest; (void) src; }
};

template<class D, class S>
struct composer<compose_source,D,S> {
  static inline void op (D& dest, const S& src) { dest= src; }
};

template<class D, class S>
struct composer<compose_source_over,D,S> {
  static inline void op (D& dest, const S& src) { source_over (dest, src); }
};

template<class D, class S>
struct composer<compose_towards_source,D,S> {
  static inline void op (D& dest, const S& src) { towards_source (dest, src); }
};

template<composition_mode M, class D, class S> void
compose (D* d, const S& s, int w, int h, int wd) {
  for (int y=0; y<h; y++, d += wd)
    for (int x=0; x<w; x++)
      composer<M,D,S>::op (d[x], s);
}

template<composition_mode M, class D, class S> void
compose (D* d, const S* s, int w, int h, int wd, int ws) {
  for (int y=0; y<h; y++, d += wd, s +=ws)
    for (int x=0; x<w; x++)
      composer<M,D,S>::op (d[x], s[x]);
}

#endif // defined RASTER_H
