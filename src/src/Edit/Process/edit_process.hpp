
/******************************************************************************
* MODULE     : edit_process.hpp
* DESCRIPTION: Interface for automatically generated content
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license and comes WITHOUT
* ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for more details.
* If you don't have this file, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
******************************************************************************/

#ifndef EDIT_PROCESS_H
#define EDIT_PROCESS_H
#include "editor.hpp"

class edit_process_rep: virtual public editor_rep {
protected:
  bool   math_input;
  string message_l;
  string message_r;
  hashmap<tree,tree> last_prompt;

public:
  edit_process_rep ();
  ~edit_process_rep ();

  void make_session (string lan, string session);
  void start_input ();
  void start_output ();
  void session_message (string l, string r);
  void session_use_math_input (bool flag);
  bool session_is_using_math_input ();
  void session_var_go_up ();
  void session_var_go_down ();
  void session_go_left ();
  void session_go_right ();
  void session_go_up ();
  void session_go_down ();
  void session_go_page_up ();
  void session_go_page_down ();
  void session_remove_backwards ();
  void session_remove_forwards ();
  void session_insert_text_field ();
  void session_insert_input_at (path p);
  void session_insert_input_below ();
  void session_insert_input_above ();
  void session_fold_input ();
  void session_remove_input_backwards ();
  void session_remove_input_forwards ();
  void session_remove_all_outputs ();
  void session_remove_previous_output ();
  void session_split ();
  bool session_complete_try ();

  void generate_bibliography (string bib, string style, string fname);
  void generate_table_of_contents (string toc);
  void generate_index (string idx);
  void generate_glossary (string glo);
  void generate_aux (string which= "");

private:
  void generate_aux_recursively (string which, tree tt, path ip);
};

#endif // defined EDIT_PROCESS_H
