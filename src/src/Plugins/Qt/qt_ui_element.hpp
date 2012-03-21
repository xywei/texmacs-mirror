
/******************************************************************************
 * MODULE     : qt_ui_element.hpp
 * DESCRIPTION: User interface proxies
 * COPYRIGHT  : (C) 2010  Massimiliano Gubinelli
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#ifndef QT_UI_ELEMENT_HPP
#define QT_UI_ELEMENT_HPP

#include "qt_widget.hpp"
#include "ntuple.hpp"
#include "promise.hpp"
#include "url.hpp"
#include "object.hpp"
#include "hashmap.hpp"

#include <QAction>


/*! Construction of UI elements / widgets.
 *
 * Most (see the enum types) of the items in the UI are constructed using the
 * factory methods create(). Notable exceptions are the classes 
 * plain_window_widget_rep and refresh_widget_rep.
 *
 * See the documentation of qt_widget_rep for the rationale behind the four
 * methods as_qaction(), get_qmenu(), as_qlayoutmenu(), as_qwidget()
 */
class qt_ui_element_rep: public qt_widget_rep {
public:
  
  enum types {
    horizontal_menu, vertical_menu, horizontal_list, vertical_list,
    tile_menu, minibar_menu, menu_separator, menu_group, 
    pulldown_button, pullright_button, menu_button,
    balloon_widget, text_widget, xpm_widget, toggle_widget,
    enum_widget, choice_widget, scrollable_widget,
    hsplit_widget, vsplit_widget,
    aligned_widget, tabs_widget, wrapped_widget, refresh_widget,
    glue_widget //!< just for non-colored ones (skips qt_glue_widget_rep)
  } ;
  
  types type;
  blackbox load;
  
  QAction* cachedAction;
  
  qt_ui_element_rep (types _type, blackbox _load) 
  : type(_type), load(_load), cachedAction(NULL)  {};

  ~qt_ui_element_rep(); 

  virtual widget make_popup_widget ();
  virtual widget popup_window_widget (string s);
  virtual widget plain_window_widget (string s, command q);
  
  virtual QAction*     as_qaction ();
  virtual QMenu*       get_qmenu();
  virtual QLayoutItem* as_qlayoutitem ();
  virtual QWidget*     as_qwidget ();
  
  operator tree ();

  template<class X1> static widget create (types _type, X1 x1) {
    return tm_new <qt_ui_element_rep> (_type, close_box<X1>(x1));
  }
  
  template <class X1, class X2> 
  static widget create (types _type, X1 x1, X2 x2) {
    typedef pair<X1,X2> T;
    return tm_new <qt_ui_element_rep> (_type, close_box<T> (T (x1,x2)));
  }
  
  template <class X1, class X2, class X3> 
  static widget create (types _type, X1 x1, X2 x2, X3 x3) {
    typedef triple<X1,X2,X3> T;
    return tm_new <qt_ui_element_rep> (_type, close_box<T> (T (x1,x2,x3)));
  }
  
  template <class X1, class X2, class X3, class X4> 
  static widget create (types _type, X1 x1, X2 x2, X3 x3, X4 x4) {
    typedef quartet<X1,X2,X3,X4> T;
    return tm_new <qt_ui_element_rep> (_type, close_box<T> (T (x1,x2,x3,x4)));
  }
  
  template <class X1, class X2, class X3, class X4, class X5> 
  static widget create (types _type, X1 x1, X2 x2, X3 x3, X4 x4, X5 x5) {
    typedef quintuple<X1,X2,X3,X4,X5> T;
    return tm_new <qt_ui_element_rep> (_type, close_box<T> (T (x1,x2,x3,x4,x5)));
  }
  
};


#endif // defined QT_UI_ELEMENT_HPP
