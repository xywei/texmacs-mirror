
/******************************************************************************
* MODULE     : upgradetm.cpp
* DESCRIPTION: upgrade old TeXmacs formats
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license and comes WITHOUT
* ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for more details.
* If you don't have this file, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
******************************************************************************/

#include "convert.hpp"
#include "hashset.hpp"
#include "path.hpp"
#include "vars.hpp"
#include "drd_std.hpp"
#include <stdio.h>

/******************************************************************************
* Retrieve older operator hashmap
******************************************************************************/

static void
rename_feature (hashmap<string,int>& H, string old_name, string new_name) {
  H (old_name)= H [new_name];
  H->reset (new_name);
}

static void
new_feature (hashmap<string,int>& H, string new_name) {
  H->reset (new_name);
}

/*static*/ hashmap<string,int>
get_codes (string version) {
  hashmap<string,int> H (UNKNOWN);
  H->join (STD_CODE);

  if (version_inf ("1.0.2.0", version)) return H;

  new_feature (H, "with_limits");
  new_feature (H, "line_break");
  new_feature (H, "new_line");
  new_feature (H, "line_separator");
  new_feature (H, "next_line");
  new_feature (H, "no_line_break");
  new_feature (H, "no_first_indentation");
  new_feature (H, "enable_first_indentation");
  new_feature (H, "no_indentation_after");
  new_feature (H, "enable_indentation_after");
  new_feature (H, "page_break_before");
  new_feature (H, "page_break");
  new_feature (H, "no_page_break_before");
  new_feature (H, "no_page_break_after");
  new_feature (H, "new_page_before");
  new_feature (H, "new_page");
  new_feature (H, "new_double_page_before");
  new_feature (H, "new_double_page");

  if (version_inf ("1.0.1.25", version)) return H;

  new_feature (H, "active");
  new_feature (H, "var_inactive");
  new_feature (H, "var_active");
  new_feature (H, "attr");

  if (version_inf ("1.0.0.20", version)) return H;

  new_feature (H, "text_at");

  if (version_inf ("1.0.0.19", version)) return H;

  new_feature (H, "find_file");

  if (version_inf ("1.0.0.14", version)) return H;

  rename_feature (H, "paragraph", "para");

  if (version_inf ("1.0.0.5", version)) return H;

  new_feature (H, "var_if");
  new_feature (H, "hide_expand");

  if (version_inf ("1.0.0.2", version)) return H;

  new_feature (H, "superpose");
  new_feature (H, "spline");
  new_feature (H, "var_spline");
  new_feature (H, "cspline");
  new_feature (H, "fill");

  if (version_inf ("0.3.5.2", version)) return H;

  new_feature (H, "raw_data");
  new_feature (H, "include");

  if (version_inf ("0.3.5.1", version)) return H;

  new_feature (H, "var_expand");

  if (version_inf ("0.3.4.12", version)) return H;

  new_feature (H, "range");
  new_feature (H, "is_tuple");
  new_feature (H, "look_up");

  if (version_inf ("0.3.4.11", version)) return H;

  new_feature (H, "float");
  new_feature (H, "datoms");
  new_feature (H, "dlines");
  new_feature (H, "dpages");
  new_feature (H, "pageref");

  if (version_inf ("0.3.4.7", version)) return H;

  rename_feature (H, "matrix", "old_matrix");
  rename_feature (H, "table", "old_table");
  rename_feature (H, "mosaic", "old_mosaic");
  rename_feature (H, "mosaic_item", "old_mosaic_item");

  if (version_inf ("0.3.4.6", version)) return H;

  new_feature (H, "tformat");
  new_feature (H, "twith");
  new_feature (H, "cwith");
  new_feature (H, "tmarker");
  new_feature (H, "row");
  new_feature (H, "cell");
  new_feature (H, "sub_table");

  if (version_inf ("0.3.4.0", version)) return H;

  new_feature (H, "tag");
  new_feature (H, "meaning");

  if (version_inf_eq ("0.3.3.15", version)) return H;

  new_feature (H, "uninit");
  new_feature (H, "error");
  new_feature (H, "surround");
  new_feature (H, "hold");
  new_feature (H, "release");
  new_feature (H, "arg");

  if (version_inf_eq ("0.3.3.0", version)) return H;

  new_feature (H, "with");
  new_feature (H, "macro");
  new_feature (H, "eval");
  new_feature (H, "value");
  new_feature (H, "or");
  new_feature (H, "xor");
  new_feature (H, "and");
  new_feature (H, "not");
  new_feature (H, "over");
  new_feature (H, "divide");
  new_feature (H, "modulo");
  new_feature (H, "length");
  new_feature (H, "date");
  new_feature (H, "equal");
  new_feature (H, "unequal");
  new_feature (H, "less");
  new_feature (H, "lesseq");
  new_feature (H, "greater");
  new_feature (H, "greatereq");
  new_feature (H, "if");
  new_feature (H, "case");
  new_feature (H, "for");
  new_feature (H, "while");
  new_feature (H, "extern");
  new_feature (H, "authorize");

  if (version_inf_eq ("0.3.1.8", version)) return H;

  rename_feature (H, "mosaic item", "mosaic_item");
  rename_feature (H, "<>", "symbol");
  rename_feature (H, ";", "backup");
  rename_feature (H, "'", "quote");
  rename_feature (H, ":=", "assign");
  rename_feature (H, "\\", "apply");
  rename_feature (H, "()", "tuple");
  rename_feature (H, "{,}", "collection");
  rename_feature (H, "->", "associate");
  rename_feature (H, "+", "plus");
  rename_feature (H, "-", "minus");
  rename_feature (H, "x", "times");
  rename_feature (H, "*", "merge");
  rename_feature (H, "nr", "number");
  H ("style")= H ["()"];

  return H;
}

/******************************************************************************
* Old style conversion from TeXmacs strings to TeXmacs trees
******************************************************************************/

static tree
string_to_tree (string s, int& pos, hashmap<string,int> codes) {
  string l ("");
  while ((pos<N(s)) && (s[pos]!='(') && (s[pos]!=',') && (s[pos]!=')')) {
    if ((s[pos]=='\\') && (pos<N(s)-1)) pos++;
    l << s[pos++];
  }
  tree t (l);
  tree_label code= (tree_label) codes [l];
  if ((l == "style") || (code == COLLECTION)) t= tree (code);
  if ((pos<N(s)) && (s[pos]=='(')) {
    if (code != UNKNOWN) t= tree (code);
    else t= tuple (l);
    do {
      pos++;
      t << string_to_tree (s, pos, codes);
    } while ((pos<N(s)) && (s[pos]==','));
    if ((pos<N(s)) && (s[pos]==')')) pos++;
  }
  return t;
}

static tree
un_paragraph (tree t) {
  if (is_atomic (t)) return t;
  if (is_func (t, PARAGRAPH, 1)) return t[0];
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++) r[i]= un_paragraph (t[i]);
    return r;
  }
}

/*static*/ tree
string_to_tree (string s, string version) {
  int pos=0;
  return un_paragraph (string_to_tree (s, pos, get_codes (version)));
}

/******************************************************************************
* Upgrade large delimiters, big operators and primes
******************************************************************************/

tree
upgrade_textual (tree t, path& mode_stack) {
  if (t == "") return t;
  if (is_atomic (t)) {
    int i, n= N(t->label);
    string s;
    tree r (CONCAT);
    for (i=0; i<n; ) {
      if (t->label[i] == '<') {
	int start= i;
	for (i++; i<n; i++)
	  if (t->label[i-1] == '>') break;
	string ss= t->label (start, i);
	if (t->label[i-1] != '>') ss << '>';
	if (starts (ss, "<left-")) {
	  if (s != "") r << s; s= "";
	  r << tree (LEFT, ss (6, N(ss)-1));
	}
	else if (starts (ss, "<mid-")) {
	  if (s != "") r << s; s= "";
	  r << tree (MIDDLE, ss (5, N(ss)-1));
	}
	else if (starts (ss, "<right-")) {
	  if (s != "") r << s; s= "";
	  r << tree (RIGHT, ss (7, N(ss)-1));
	}
	else if (starts (ss, "<big-")) {
	  if (s != "") r << s; s= "";
	  r << tree (BIG, ss (5, N(ss)-1));
	}
	else s << ss;
      }
      else if (((t->label[i] == '\'') || (t->label[i] == '`')) &&
	       (!nil (mode_stack)) && (mode_stack->item == 1))
	{
	  int start= i++;
	  while ((i<n) && (t->label[i] == t->label[i-1])) i++;
	  if (s != "") r << s; s= "";
	  tree_label op= t->label[start] == '`'? LEFT_PRIME: RIGHT_PRIME;
	  r << tree (op, t->label (start, i));
	}
      else s << t->label[i++];
    }
    if (s != "") r << s;
    if (N(r) == 1) return r[0];
    return r;
  }
  else {
    int i, n= arity (t);
    tree r (t, 0);
    for (i=0; i<n; i++) {
      tree u= upgrade_textual (t[i], mode_stack);
      if (is_func (u, SET)) {
	if (u == tree (SET, "mode", "text")) mode_stack= path (0, mode_stack);
	if (u == tree (SET, "mode", "math")) mode_stack= path (1, mode_stack);
	if (u == tree (SET, "mode", "prog")) mode_stack= path (2, mode_stack);
      }
      else if (is_func (u, RESET)) {
	if (u == tree (RESET, "mode"))
	  if (!nil (mode_stack))
	    mode_stack= mode_stack->next;
      }
      else if (is_func (u, BEGIN, 1)) {
	if ((u[0] == "equation") ||
	    (u[0] == "equation*") ||
	    (u[0] == "eqnarray*") ||
	    (u[0] == "leqnarray*"))
	  mode_stack= path (1, mode_stack);
      }
      else if (is_func (u, END, 1)) {
	if ((u[0] == "equation") ||
	    (u[0] == "equation*") ||
	    (u[0] == "eqnarray*") ||
	    (u[0] == "leqnarray*"))
	  if (!nil (mode_stack))
	    mode_stack= mode_stack->next;
      }
      if (is_concat (t) && is_concat (u)) r << A(u);
      else r << u;
    }
    return r;
  }
}

/******************************************************************************
* Upgrade lambda application -> macro expansion and value keyword
******************************************************************************/

tree
upgrade_apply_expand_value (tree t, hashset<string> H) {
  if (is_atomic (t)) return t;
  else {
    int i, n= arity (t);
    tree r (t, n);
    if (is_func (t, APPLY))
      if ((n >= 1) && is_atomic (t[0]) && H->contains (t[0]->label)) {
	if (n == 1) r= tree (VALUE, n);
	else r= tree (EXPAND, n);
      }
    for (i=0; i<n; i++)
      r[i]= upgrade_apply_expand_value (t[i], H);
    return r;
  }
}

typedef char* charp;
static charp apply_expand_value_strings[]= {
  "chapter", "chapter*", "appendix",
  "section", "section*", "subsection", "subsection*",
  "subsubsection", "subsubsection*",
  "paragraph", "paragraph*", "subparagraph", "subparagraph*",
  "footnote", "item*", "overline", "underline",
  "mathord", "mathbin", "mathopen", "mathpunct",
  "mathop", "mathrel", "mathclose", "mathalpha",
  "op", "strong", "em", "tt", "name", "samp", "abbr",
  "dfn", "kbd", "var", "acronym", "person",
  "menu", "submenu", "subsubmenu", "tmdef", "tmref",
  "key", "skey", "ckey", "akey", "mkey", "hkey",
  "include-document", "include-project", "globalize-variable",
  "localize-variable", "assign-variable",
  "gb", "cgb", "gbt", "cgbt", "head", "tail", "hm", "tm", "binom",
  "ma", "mb", "md", "me", "mf", "mg", "mh", "mi", "mj", "mk",
  "mm", "mn", "mu", "mv", "mw", "my", "mz",
  "MA", "MB", "MD", "ME", "MF", "MG", "MH", "MI", "MJ", "MK",
  "MM", "MN", "MU", "MV", "MW", "MY", "MZ",
  ""
};

tree
upgrade_apply_expand_value (tree t) {
  int i;
  hashset<string> H;
  for (i=0; apply_expand_value_strings[i][0] != '\0'; i++)
    H->insert (apply_expand_value_strings[i]);
  return upgrade_apply_expand_value (t, H);
}

/******************************************************************************
* Subroutines for upgrading set/reset -> with, begin/end -> apply
******************************************************************************/

static bool
matching (tree open, tree close) {
  if (is_func (open, SET, 2))
    return is_func (close, RESET, 1) && (open[0] == close[0]);
  if (is_func (open, BEGIN))
    return is_func (close, END, 1) && (open[0] == close[0]);
  return false;
}

static tree
with_replace (tree var, tree val, tree body) {
  if (is_func (body, WITH)) {
    int i, n= N(body);
    tree t= tree (WITH, n+2);
    t[0]= var;
    t[1]= val;
    for (i=0; i<n; i++) t[i+2]= body[i];
    return t;
  }
  else return tree (WITH, var, val, body);
}

static tree
expand_replace (tree begin, tree body) {
  int i, k= N(begin);
  tree expand (EXPAND, k+1);
  for (i=0; i<k; i++) expand[i]= begin[i];
  expand[i]= body;
  return expand;
}

static void
concat_search (tree t, int& i, tree open= "") {
  bool set_reset= (open == "") || is_func (open, SET) || is_func (open, RESET);
  bool begin_end= (open == "") || is_func (open, BEGIN) || is_func (open, END);
  int n= N(t);
  while (i<n) {
    if (set_reset && is_func (t[i], SET, 2)) return;
    if (set_reset && is_func (t[i], RESET, 1)) return;
    if (begin_end && is_func (t[i], BEGIN)) return;
    if (begin_end && is_func (t[i], END, 1)) return;
    i++;
  }
}

static tree
concat_replace (tree t, int i1, int i2) {
  int i;
  tree v (CONCAT);
  for (i=i1+1; i<i2; i++) v << t[i];
  if (N(v)==0) v= "";
  else if (N(v)==1) v= v[0];
  if (is_func (t[i1], SET))
    return with_replace (t[i1][0], t[i1][1], v);
  else return expand_replace (t[i1], v);
}

static tree
document_explode (tree t) {
  int i, n= N(t);
  tree u (t, n);
  for (i=0; i<n; i++)
    if (is_concat (t[i])) u[i]= t[i];
    else u[i]= tree (CONCAT, t[i]);
  return u;
}

static tree
document_contract (tree t) {
  int i, n= N(t);
  tree u (t, n);
  for (i=0; i<n; i++)
    if (N(t[i]) == 0) u[i]= "";
    else if (N(t[i]) == 1) u[i]= t[i][0];
    else u[i]= t[i];
  return u;
}

static void
document_search (tree t, int& i, int& j, tree open= "") {
  bool set_reset= (open == "") || is_func (open, SET) || is_func (open, RESET);
  bool begin_end= (open == "") || is_func (open, BEGIN) || is_func (open, END);
  int n= N(t);
  while (i<n) {
    int k= N(t[i]);
    while (j<k) {
      if (set_reset && is_func (t[i][j], SET, 2)) return;
      if (set_reset && is_func (t[i][j], RESET, 1)) return;
      if (begin_end && is_func (t[i][j], BEGIN)) return;
      if (begin_end && is_func (t[i][j], END, 1)) return;
      j++;
    }
    i++;
    j=0;
  }
}

static void
document_inc (tree doc_t, int& doc_i, int& con_i) {
  con_i++;
  if (con_i == N(doc_t[doc_i])) {
    doc_i++;
    con_i= 0;
  }
}

static void
document_inc (tree& doc, tree& con, tree doc_t, int& doc_i, int& con_i) {
  con_i++;
  if (con_i == N(doc_t[doc_i])) {
    doc << con;
    con= tree (CONCAT);
    doc_i++;
    con_i= 0;
  }
}

static void
document_merge (tree& doc, tree& con, tree doc_t,
		int doc_1, int con_1, int doc_2, int con_2)
{
  int doc_i= doc_1, con_i= con_1;
  while ((doc_i<doc_2) || ((doc_i==doc_2) && (con_i<con_2))) {
    con << doc_t[doc_i][con_i];
    document_inc (doc, con, doc_t, doc_i, con_i);
  }
}

static tree
document_replace (tree doc_t, int doc_1, int con_1, int doc_2, int con_2) {
  tree doc_b (DOCUMENT), con_b (CONCAT);
  int doc_i= doc_1, con_i= con_1;
  document_inc (doc_b, con_b, doc_t, doc_i, con_i);
  document_merge (doc_b, con_b, doc_t, doc_i, con_i, doc_2, con_2);
  doc_b << con_b;
  doc_b= document_contract (doc_b);
  bool flag= (doc_1!=doc_2) || ((con_1==0) && (con_2==N(doc_t[doc_2])-1));
  /*
  if (N(doc_b) != (doc_2-doc_1+1))
    cout << (doc_2-doc_1+1) << ", " << doc_b << "\n";
  */
  if ((!flag) && (N(doc_b)==1)) doc_b= doc_b[0];
  if (is_func (doc_t[doc_1][con_1], SET))
    return with_replace (doc_t[doc_1][con_1][0],
			 doc_t[doc_1][con_1][1],
			 doc_b);
  else return expand_replace (doc_t[doc_1][con_1], doc_b);
}

/******************************************************************************
* Upgrade set/reset -> with, begin/end -> apply
******************************************************************************/

static tree upgrade_set_begin (tree t);

static tree
upgrade_set_begin_default (tree t) {
  int i, n= N(t);
  tree u (t, n);
  for (i=0; i<n; i++)
    u[i]= upgrade_set_begin (t[i]);
  return u;
}

static tree
upgrade_set_begin_concat_once (tree t) {
  // cout << "in : " << t << "\n";
  int i=0, n= N(t);
  tree u (CONCAT);
  while (i<n) {
    int i0=i, i1, i2;
    concat_search (t, i);
    i1= i;
    for (i=i0; i<i1; i++) u << t[i];
    if (i==n) {
      // cout << "  " << i0 << ", " << i1 << "\n";
      break;
    }
    i++;
    concat_search (t, i, t[i1]);
    i2= i;
    // cout << "  " << i0 << ", " << i1 << ", " << i2 << "\n";
    if ((i2<n) && matching (t[i1], t[i2])) {
      u << concat_replace (t, i1, i2);
      i= i2+1;
    }
    else {
      i= i1;
      if (i == i0) u << t[i++];
    }
  }
  // cout << "out: " << u << "\n";
  // cout << "-------------------------------------------------------------\n";
  // fflush (stdout);
  return u;
}

static tree
upgrade_set_begin_concat (tree t) {
  tree u= t;
  do { t= u; u= upgrade_set_begin_concat_once (t); } while (u != t);
  u= upgrade_set_begin_default (u);
  if (N(u) == 1) return u[0];
  return u;
}

static void
upgrade_verbatim_expand (tree& doc, tree& con, tree ins) {
  tree& body= ins[N(ins)-1];
  if (is_document (body) && (N(body)>1)) {
    int n= N(body);
    int start=0, end=n;
    if (body[0] == "") start= 1;
    if (body[n-1] == "") end= n-1;
    body= body (start, end);
    if (start != 0) {
      doc << con;
      con= tree (CONCAT);
    }
    con << ins;
    if (end != n) {
      doc << con;
      con= tree (CONCAT);
    }
  }
  else con << ins;
}

static void
upgrade_abstract_expand (tree& doc, tree& con, tree ins) {
  (void) doc;
  tree& body= ins[N(ins)-1];
  if (is_document (body) && (N(body) > 1) && (body[0] == ""))
    body= body (1, N(body));
  con << ins;
}

static tree
upgrade_set_begin_document_once (tree doc_t) {
  // cout << "in : " << doc_t << "\n";
  int doc_i=0, con_i=0;
  tree doc (DOCUMENT), con (CONCAT);
  while ((doc_i < N(doc_t)) && (con_i < N(doc_t[doc_i]))) {
    int doc_0= doc_i, con_0= con_i;
    document_search (doc_t, doc_i, con_i);
    int doc_1= doc_i, con_1= con_i;
    // cout << "  0: " << doc_0 << ", " << con_0 << "\n";
    // cout << "  1: " << doc_1 << ", " << con_1 << "\n";
    document_merge (doc, con, doc_t, doc_0, con_0, doc_1, con_1);
    if (doc_i == N(doc_t)) break;
    document_inc (doc_t, doc_i, con_i);
    document_search (doc_t, doc_i, con_i, doc_t[doc_1][con_1]);
    int doc_2= doc_i, con_2= con_i;
    // cout << "  2: " << doc_2 << ", " << con_2 << "\n";
    if ((doc_2 < N(doc_t)) &&
	matching (doc_t[doc_1][con_1], doc_t[doc_2][con_2]))
      {
	tree ins= document_replace (doc_t, doc_1, con_1, doc_2, con_2);
	if (is_func (ins, EXPAND, 2)) {
	  if ((ins[0] == "verbatim") || (ins[0] == "code"))
	    upgrade_verbatim_expand (doc, con, ins);
	  else if (ins[0] == "abstract")
	    upgrade_abstract_expand (doc, con, ins);
	  else con << ins;
	}
	else con << ins;
	document_inc (doc, con, doc_t, doc_i, con_i);
      }
    else {
      doc_i= doc_1; con_i= con_1;
      if ((doc_i == doc_0) && (con_i == con_0)) {
	con << doc_t[doc_i][con_i];
	document_inc (doc, con, doc_t, doc_i, con_i);	  
      }
    }
  }
  // cout << "out: " << doc << "\n";
  // cout << "-------------------------------------------------------------\n";
  fflush (stdout);
  return doc;
}

static tree
upgrade_set_begin_document (tree t) {
  tree u= t;
  do {
    t= u;
    u= document_explode (u);
    u= upgrade_set_begin_document_once (u);
    u= document_contract (u);
  } while (u != t);
  u= upgrade_set_begin_default (u);
  return u;
}

static tree
upgrade_set_begin_surround (tree t, tree search, bool& found) {
  if (t == search) {
    found= true;
    return copy (t);
  }
  if (is_func (t, WITH) || is_func (t, EXPAND)) {
    tree u= copy (t);
    u[N(u)-1]= upgrade_set_begin_surround (u[N(u)-1], search, found);
    return u;
  }
  if (is_concat (t)) {
    int i, n= N(t), searching= !found;
    tree left (CONCAT), middle, right (CONCAT);
    for (i=0; i<n; i++) {
      middle= upgrade_set_begin_surround (t[i], search, found);
      if (searching && found) break;
      else left << middle;
    }
    if (i==n) return copy (t);
    for (i++; i<n; i++)
      right << upgrade_set_begin_surround (t[i], search, found);
    if (N(left) == 0) left= "";
    else if (N(left) == 1) left= left[0];
    if (N(right) == 0) right= "";
    else if (N(right) == 1) right= right[0];
    return tree (SURROUND, left, right, middle);
  }
  return copy (t);
}

static tree
upgrade_env_args (tree t, tree env) {
  if (is_atomic (t)) return t;
  else if (is_func (t, APPLY, 1)) {
    int i, k= N(env);
    for (i=0; i<k-2; i++)
      if (t[0] == env[i])
	return tree (ARGUMENT, t[0]);
    return t;
  }
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_env_args (t[i], env);
    return r;
  }
}

static tree
upgrade_set_begin_env (tree t) {
  // cout << "in  : " << t << "\n";
  int i, n= N(t);
  tree u (MACRO, n);
  for (i=0; i<n-2; i++)
    u[i]= upgrade_set_begin (t[i]);
  string s= "body";
  for (i=0; i<n-2; i++)
    if (t[i] == "body") s= "body*";
  u[n-2]= copy (s);

  tree begin= t[n-2], end= t[n-1], body (CONCAT);
  if (begin == "") begin= tree (CONCAT);
  else if (!is_concat (begin)) begin= tree (CONCAT, begin);
  if (end == "") end= tree (CONCAT);
  else if (!is_concat (end)) end= tree (CONCAT, end);
  body << A (begin) << tree (ARGUMENT, copy (s)) << A (end);
  // cout << "mid1: " << body << "\n";
  body= upgrade_set_begin_concat (body);
  body= upgrade_env_args (body, t);
  // cout << "mid2: " << body << "\n";
  bool found= false;
  u[n-1]= upgrade_set_begin_surround (body, tree (ARGUMENT, s), found);
  // cout << "out : " << u << "\n";
  // cout << "-------------------------------------------------------------\n";
  return u;
}

static tree
upgrade_set_begin (tree t) {
  if (is_atomic (t)) return copy (t);
  else {
    if (is_concat (t)) return upgrade_set_begin_concat (t);
    else if (is_document (t)) return upgrade_set_begin_document (t);
    else if (is_func (t, ENVIRONMENT)) return upgrade_set_begin_env (t);
    else return upgrade_set_begin_default (t);
  }
}

static tree
eliminate_set_begin (tree t) {
  if (is_atomic (t)) return t;
  if (is_func (t, SET) || is_func (t, RESET) ||
      is_func (t, BEGIN) || is_func (t, END) ||
      is_func (t, ENVIRONMENT)) return "";

  int i, n= N(t);
  if (is_concat (t)) {
    tree r (CONCAT);
    for (i=0; i<n; i++) {
      tree u= eliminate_set_begin (t[i]);
      if (u != "") r << u;
    }
    if (N(r) == 0) return "";
    if (N(r) == 1) return r[0];
    return r;
  }
  else {
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= eliminate_set_begin (t[i]);
    return r;
  }
}

/******************************************************************************
* Upgrade surround, indentation after and final routine
******************************************************************************/

static bool
expand_needs_surrounding (string s) {
  return
    (s == "maketitle") || (s == "abstract") ||
    (s == "theorem") || (s == "proposition") || (s == "lemma") ||
    (s == "corollary") || (s == "proof") || (s == "axiom") ||
    (s == "definition") || (s == "conjecture") ||
    (s == "remark") || (s == "note") || (s == "example") ||
    (s == "exercise") || (s == "warning") || (s == "convention") ||
    (s == "code") || (s == "quote") ||
    (s == "quotation") || (s == "verse") || (s == "center") ||
    (s == "indent") || (s == "body") || (s == "description") ||
    starts (s, "itemize") || starts (s, "enumerate");
}

static bool
with_needs_surrounding (string s) {
  return
    (s == "paragraph mode") || (s == "paragraph hyphenation") ||
    (s == "paragraph width") || (s == "left margin") ||
    (s == "right margin") || (s == "first indentation") ||
    (s == "last indentation") || (s == "no first indentation") ||
    (s == "no last indentation") || (s == "interline space") ||
    (s == "horizontal ink separation") || (s == "line stretch") ||
    (s == "interparagraph space");
}

static bool
needs_surrounding (tree t) {
  if (is_multi_paragraph (t)) return true;
  if ((is_func (t, APPLY) || is_func (t, EXPAND)) && is_atomic (t[0])) {
    if (t[0] == "verbatim") return (N(t)==2) && is_multi_paragraph (t[1]);
    return expand_needs_surrounding (t[0]->label);
  }
  if (is_func (t, WITH)) {
    int i, n= N(t)-1;
    for (i=0; i<n; i+=2)
      if (is_atomic (t[i]) && with_needs_surrounding (t[i]->label))
	return true;
  }
  return false;
}

static bool
needs_transfer (tree t) {
  return
    is_func (t, EXPAND, 2) &&
    ((t[0] == "equation") || (t[0] == "equation*") ||
     (t[0] == "eqnarray*") || (t[0] == "leqnarray*"));
}

static tree
upgrade_surround (tree t) {
  if (is_atomic (t)) return t;
  int i, n= N(t);
  tree r (t, n);
  for (i=0; i<n; i++) {
    tree u= t[i];
    if (is_document (t) && is_concat (u) && (N(u)>1)) {
      int j, k= N(u);
      for (j=0; j<k; j++)
	if (needs_surrounding (u[j])) {
	  tree before= u (0  , j);
	  tree after = u (j+1, k);
	  tree body  = upgrade_surround (u[j]);
	  if (N(before)==0) before= "";
	  if (N(before)==1) before= before[0];
	  if (N(after )==0) after = "";
	  if (N(after )==1) after = after [0];
	  before= upgrade_surround (before);
	  after = upgrade_surround (after );
	  r[i]= tree (SURROUND, before, after, body);
	  break;
	}
	else if (needs_transfer (u[j])) {
	  tree temp= upgrade_surround (u[j][1]);
	  if (!is_concat (temp)) temp= tree (CONCAT, temp);
	  tree body= u (0, j);
	  body << A (temp) << A (u (j+1, k));
	  r[i]= tree (EXPAND, u[j][0], body);
	  break;
	}
      if (j<k) continue;
    }
    r[i]= upgrade_surround (u);
  }
  return r;
}

static tree
upgrade_indent (tree t) {
  if (is_atomic (t)) return t;
  else if (t == tree (ASSIGN, "no first indentation", "true"))
    return tree (FORMAT, "no indentation after");
  else if (t == tree (ASSIGN, "no first indentation", "false"))
    return tree (FORMAT, "enable indentation after");
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_indent (t[i]);
    return r;
  }
}

static tree
upgrade_new_environments (tree t) {
  t= upgrade_set_begin (t);
  t= eliminate_set_begin (t);
  t= upgrade_surround (t);
  t= upgrade_indent (t);
  return t;
}

/******************************************************************************
* Upgrade items
******************************************************************************/

static tree
upgrade_items (tree t) {
  if (is_atomic (t)) return t;
  else if ((t == tree (APPLY, "item")) || (t == tree (VALUE, "item")))
    return tree (EXPAND, "item");
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_items (t[i]);
    return r;
  }
}

/******************************************************************************
* Upgrade resize
******************************************************************************/

static tree
upgrade_resize_arg (tree t, int type) {
  if (!is_atomic (t)) return "";
  string s= t->label;
  if ((s == "same") || (s == "ink")) return "";
  if (type == 1) s= "l[" * s;
  if (type == 2) s= "b[" * s;
  if (type == 3) s= "r]" * s;
  if (type == 4) s= "t]" * s;
  return s;
}

static tree
upgrade_resize (tree t) {
  if (is_atomic (t)) return t;
  else if (is_func (t, RESIZE, 6)) {
    tree r (RESIZE, t[0]);
    int extend= (t[1] == "extend"? 1: 0);
    r << upgrade_resize_arg (t[2], 1 * extend)
      << upgrade_resize_arg (t[3], 2 * extend)
      << upgrade_resize_arg (t[4], 3 * extend)
      << upgrade_resize_arg (t[5], 4 * extend);
    return r;
  }
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_resize (t[i]);
    return r;
  }
}

/******************************************************************************
* Upgrade tables
******************************************************************************/

static void
handle_mosaic_format (tree& fm, tree t, int i, int j) {
  string align = as_string (t[1]);
  string hspan = as_string (t[2]);
  string vspan = as_string (t[3]);
  string col   = as_string (t[4]);

  string halign= "l";
  string valign= "B";
  if (N(align)>=2) {
    switch (align[0]) {
    case 'n': valign= "t"; break;
    case 'c': valign= "c"; break;
    case '0': valign= "B"; break;
    case 's': valign= "s"; break;
    }
    switch (align[1]) {
    case 'w': halign= "l"; break;
    case '0': halign= "L"; break;
    case 'c': halign= "c"; break;
    case 'e': halign= "r"; break;
    }
  }
  if ((col == "none") || (col == "")) col= "";
  else col= "foreground";

  tree w (CELL_WITH);
  w << as_string (i+1) << as_string (j+1)
    << as_string (i+1) << as_string (j+1);

  if (halign != "l") {
    tree with= copy (w);
    with << "cell halign" << halign;
    fm << with;
  }
  if (valign != "B") {
    tree with= copy (w);
    with << "cell valign" << valign;
    fm << with;
  }
  if (hspan != "1") {
    tree with= copy (w);
    with << "cell hspan" << hspan;
    fm << with;
  }
  if (vspan != "1") {
    tree with= copy (w);
    with << "cell vspan" << vspan;
    fm << with;
  }
  if (col != "") {
    tree with= copy (w);
    with << "cell background" << col;
    fm << with;
  }
}


static tree
upgrade_table (tree t) {
  if (is_atomic (t)) return t;
  else if (is_func (t, OLD_MATRIX) ||
	   is_func (t, OLD_TABLE) ||
	   is_func (t, OLD_MOSAIC) ||
	   (is_func (t, TABLE_FORMAT) && is_func (t[N(t)-1], OLD_MATRIX)))
    {
      tree ft (TABLE_FORMAT);
      if (is_func (t, TABLE_FORMAT)) {
	ft= t (0, N(t)-1);
	t = t [N(t)-1];
      }
      if (is_func (t, OLD_MOSAIC)) {
	tree with (CELL_WITH);
	with << "1" << "-1" << "1" << "-1" << "cell mode" << "c";
	ft << with;
      }

      int i, j;
      int nr_rows= as_int (t[N(t)-1]);
      int nr_cols= as_int (t[N(t)-2]);
      tree tt (TABLE, nr_rows);
      for (i=0; i<nr_rows; i++) {
	tree rt (ROW, nr_cols);
	for (j=0; j<nr_cols; j++) {
	  tree c= upgrade_table (t[i*nr_cols+j]);
	  if (is_func (c, OLD_MOSAIC_ITEM)) {
	    handle_mosaic_format (ft, c, i, j);
	    c= c[0];
	  }
	  rt[j]= tree (CELL, c);
	}
	tt[i]= rt;
      }

      ft << tt;
      tree xt (EXPAND, "tabular*", ft);
      if (is_func (t, OLD_TABLE)) xt[0]= "block*";
      if (is_func (t, OLD_MOSAIC)) xt[0]= "tabular";
      return xt;
    }
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_table (t[i]);
    return r;
  }
}

/******************************************************************************
* Upgrade splits
******************************************************************************/

static tree
upgrade_split (tree t) {
  int i, n= N(t);
  if (is_atomic (t)) return t;
  else if (is_func (t, SURROUND, 3) && is_func (t[0], SPLIT)) {
    tree u= t[2];
    if (!is_concat (u)) u= tree (CONCAT, t[0], u);
    else u= join (tree (CONCAT, t[0]), u);
    return tree (SURROUND, "", upgrade_split (t[1]), upgrade_split (u));
  }
  else if (is_func (t, SURROUND, 3) && is_concat (t[0])) {
    tree r (CONCAT);
    tree split ("");
    for (i=0; i<N(t[0]); i++)
      if (is_func (t[0][i], SPLIT)) split= t[0][i];
      else r << t[0][i];
    tree u= t[2];
    if (split != "") {
      if (!is_concat (u)) u= tree (CONCAT, split, u);
      else u= join (tree (CONCAT, split), u);
    }
    r= tree (SURROUND, upgrade_split (r),
	     upgrade_split (t[1]), upgrade_split (u));
    return r;
  }
  else if (is_concat (t)) {
    tree r (CONCAT);
    tree split ("");
    int nr_rows=1, nr_cols=1, sep=1;
    for (i=0; i<n; i++)
      if (is_func (t[i], SPLIT)) split= t[i];
      else {
	tree u= upgrade_split (t[i]);
	if (u == tree (FORMAT, "line separator")) sep++;
	if (u == tree (FORMAT, "next line")) {
	  nr_cols= max (sep, nr_cols);
	  sep= 1;
	  nr_rows++;
	}
	r << u;
      }
    nr_cols= max (sep, nr_cols);
    if ((split == "") && (nr_cols == 1)) return r;

    if ((nr_cols > 1) || ((split != "") && (nr_rows > 1))) {
      int col=0, row=0;
      tree T (TABLE, nr_rows);
      for (row=0; row<nr_rows; row++) {
	tree R (ROW, nr_cols);
	for (col=0; col<nr_cols; col++) R[col]= tree (CELL, "");
	T[row]= R;
      }

      tree u (CONCAT);
      row= col= 0;
      for (i=0; i<N(r); i++)
	if ((r[i] == tree (FORMAT, "line separator")) ||
	    (r[i] == tree (FORMAT, "next line")))
	  {
	    if (N(u) == 0) u= "";
	    else if (N(u) == 1) u= u[0];
	    T[row][col][0]= u;
	    u= tree (CONCAT);
	    if (r[i] == tree (FORMAT, "line separator")) col++;
	    else {
	      row++;
	      col= 0;
	    }
	  }
	else u << r[i];
      if (N(u) == 0) u= "";
      else if (N(u) == 1) u= u[0];
      T[row][col][0]= u;
      r= T;
    }

    tree tf (TABLE_FORMAT);
    if (split != "") {
      tf << tree (TABLE_WITH, "table hyphen", "y")
	 << tree (TABLE_WITH, "table width", "1par")
	 << tree (TABLE_WITH, "table min cols", as_string (N (split)))
	 << tree (TABLE_WITH, "table max cols", as_string (N (split)))
	 << tree (CELL_WITH, "1", "-1", "1", "1", "cell lsep", "0spc")
	 << tree (CELL_WITH, "1", "-1", "-1", "-1", "cell rsep", "0spc")
	 << tree (CELL_WITH, "1", "-1", "1", "-1", "cell bsep", "0sep")
	 << tree (CELL_WITH, "1", "-1", "1", "-1", "cell tsep", "0sep")
	 << tree (CELL_WITH, "1", "-1", "1", "1", "cell hyphen", "b")
	 << tree (CELL_WITH, "1", "-1", "-1", "-1", "cell hyphen", "t");
      if (split[0] == "right")
	tf << tree (CELL_WITH, "1", "-1", "1", "1", "cell hpart", "1");
      if ((split[N(split)-1] == "left") || (split[N(split)-1] == "justify"))
	tf << tree (CELL_WITH, "1", "-1", "-1", "-1", "cell hpart", "1");
      for (i=0; i<N(split); i++) {
	tree with (CELL_WITH);
	int j= (i==N(split)-1)? -1: i+1;
	with << "1" << "-1" << as_string (j) << as_string (j) << "cell halign";
	if (split[i] == "right") with << "r";
	else if (split[i] == "center") with << "c";
	else with << "l";
	tf << with;
      }
    }
    if (r == tree (CONCAT)) r= "";
    else if (is_func (r, CONCAT, 1)) r= r[0];
    tf << r;
    if ((split != "") && is_func (r, TABLE))
      return tree (EXPAND, "tabular*", tf);
    return tf;
  }
  else {
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_split (t[i]);
    return r;
  }
}

/******************************************************************************
* Upgrade projects
******************************************************************************/

static tree
upgrade_project (tree t) {
  if (is_atomic (t)) return t;
  else if (is_expand (t, "include-document", 1))
    return tree (INCLUDE, t[1]);
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_project (t[i]);
    return r;
  }
}

/******************************************************************************
* Upgrade title
******************************************************************************/

static tree
upgrade_title (tree t, tree& params) {
  if (is_atomic (t)) return t;
  else if (is_func (t, APPLY, 2)) {
    if (t[0] == "title") { params[0]= t[1]; return ""; }
    if (t[0] == "author") { params[1]= t[1]; return ""; }
    if (t[0] == "address") { params[2]= t[1]; return ""; }
    return t;
  }
  else if ((t == tree (APPLY, "maketitle")) ||
	   (t == tree (EXPAND, "maketitle")))
    {
      tree doc (DOCUMENT);
      doc << tree (EXPAND, "title", copy (params[0]));
      doc << tree (EXPAND, "author", copy (params[1]));
      doc << tree (EXPAND, "address", copy (params[2]));
      doc << tree (EXPAND, "title-date", tree (_DATE, ""));
      return tree (EXPAND, "make-title", doc);
    }
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_title (t[i], params);
    return r;
  }
}

static tree
upgrade_title (tree t) {
  tree params (TUPLE, "", "", "");
  return simplify_correct (upgrade_title (t, params));
}

/******************************************************************************
* Upgrade cas
******************************************************************************/

static void
upgrade_cas_search (tree t, tree& style) {
  if (is_atomic (t));
  else if (is_expand (t, "session", 3)) {
    if (!is_atomic (t[1])) return;
    string l= copy (t[1]->label);
    if (l == "scheme") return;
    if (l == "shell") return;
    if (l == "gTybalt") l= "gtybalt";
    if (l == "Macaulay2") l= "macaulay2";
    int i, n= N(style);
    for (i=0; i<n; i++)
      if (style[i] == l) return;
    style << l;
  }
  else {
    int i, n= N(t);
    for (i=0; i<n; i++)
      upgrade_cas_search (t[i], style);
  }
}

static void
set_document_attribute (tree doc, string attr, tree val) {
  int i, n= arity (doc);
  for (i=0; i<n; i++)
    if ((is_func (doc[i], EXPAND, 2) || is_func (doc[i], APPLY, 2)) &&
	(doc[i][0] == attr))
      {
	doc[i][1]= val;
	return;
      }
  doc << tree (EXPAND, attr, val);
}

static tree
upgrade_cas (tree doc) {
  tree style= copy (extract (doc, "style"));
  upgrade_cas_search (doc, style);
  doc= copy (doc);
  set_document_attribute (doc, "style", style);
  return doc;
}

/******************************************************************************
* Upgrade modified symbols
******************************************************************************/

static bool
is_with (tree t, string var, string val) {
  return is_func (t, WITH, 3) && (t[0] == var) && (t[1] == val);
}

static bool
is_alpha (tree t) {
  if (is_compound (t)) return false;
  string s= t->label;
  return (N(s) == 1) && is_alpha (s[0]);
}

static bool
is_alpha_numeric (tree t) {
  if (is_compound (t)) return false;
  string s= t->label;
  return (N(s) == 1) && (is_alpha (s[0]) || is_numeric (s[0]));
}

static bool
is_upper (tree t) {
  if (is_compound (t)) return false;
  string s= t->label;
  return (N(s) == 1) && (s[0] >= 'A') && (s[0] <= 'Z');
}

static bool
is_bold (tree t) {
  if (is_compound (t)) return false;
  if (is_alpha_numeric (t)) return true;
  string s= locase_all (t->label);
  return
    (s == "<alpha>") || (s == "<beta>") || (s == "<gamma>") ||
    (s == "<delta>") || (s == "<epsilon>") || (s == "<zeta>") ||
    (s == "<eta>") || (s == "<theta>") || (s == "<iota>") ||
    (s == "<kappa>") || (s == "<lambda>") || (s == "<mu>") ||
    (s == "<nu>") || (s == "<xi>") || (s == "<omicron>") ||
    (s == "<pi>") || (s == "<rho>") || (s == "<sigma>") ||
    (s == "<tau>") || (s == "<upsilon>") || (s == "<phi>") ||
    (s == "<psi>") || (s == "<chi>") || (s == "<omega>") ||
    (s == "<varepsilon>") || (s == "<vartheta>") || (s == "<varkappa>") ||
    (s == "<varpi>") || (s == "<varrho>") || (s == "<varsigma>") ||
    (s == "<varphi>") || (s == "<backepsilon>") || (s == "<mho>") ||
    (s == "<Backepsilon>") || (s == "<Mho>") || (s == "<ell>");
}

static tree
upgrade_mod_symbol (string prefix, string s) {
  if (N(s) == 1) return "<" * prefix * s * ">";
  else return "<" * prefix * s (1, N(s)-1) * ">";
}

static tree
upgrade_mod_symbols (tree t) {
  if (is_atomic (t)) return t;
  if (is_with (t, "math font series", "bold") && is_bold (t[2]))
    return upgrade_mod_symbol ("b-", t[2]->label);
  else if (is_with (t, "math font", "cal") && is_upper (t[2]))
    return upgrade_mod_symbol ("cal-", t[2]->label);
  else if (is_with (t, "math font", "Euler") && is_alpha (t[2]))
    return upgrade_mod_symbol ("frak-", t[2]->label);
  else if (is_with (t, "math font", "Bbb*") && is_alpha (t[2]))
    return upgrade_mod_symbol ("bbb-", t[2]->label);
  else if (is_with (t, "math font series", "bold") &&
	   is_with (t[2], "math font", "cal") && is_upper (t[2][2]))
    return upgrade_mod_symbol ("b-cal-", t[2][2]->label);
  else if (is_with (t, "math font", "cal") &&
	   is_with (t[2], "math font series", "bold") && is_upper (t[2][2]))
    return upgrade_mod_symbol ("b-cal-", t[2][2]->label);
  else if ((is_func (t, VALUE, 1) || is_func (t, EXPAND, 1) ||
	    is_func (t, APPLY, 1)) && (is_atomic (t[0]))) {
    string s= t[0]->label;
    if ((N(s) == 2) && ((s[0]=='m') && (s[1]>='a') && s[1]<='z'))
      return upgrade_mod_symbol ("frak-", s(1,2));
    if ((N(s) == 2) && ((s[0]=='M') && (s[1]>='A') && s[1]<='Z'))
      return upgrade_mod_symbol ("frak-", s(1,2));
    return t;
  }
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_mod_symbols (t[i]);
    return r;
  }
}

/******************************************************************************
* Upgrading menus in the help
******************************************************************************/

static tree
upgrade_menus_in_help (tree t) {
  if (is_atomic (t)) return t;
  if (is_expand (t, "menu", 1) || is_expand (t, "submenu", 2) ||
      is_expand (t, "subsubmenu", 3) || is_expand (t, "subsubsubmenu", 4)) {
    int i, n= N(t);
    tree r (APPLY, n);
    r[0]= "menu";
    for (i=1; i<n; i++) r[i]= t[i];
    return r;
  }
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_menus_in_help (t[i]);
    return r;
  }
}

static tree
capitalize_sub (tree t) {
  if (is_atomic (t)) return upcase_first (t->label);
  else return t;
}

static tree
upgrade_capitalize_menus (tree t) {
  if (is_atomic (t)) return t;
  if (is_func (t, APPLY) && (t[0] == "menu")) {
    int i, n= N(t);
    tree r (APPLY, n);
    r[0]= "menu";
    for (i=1; i<n; i++) r[i]= capitalize_sub (t[i]);
    return r;
  }
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_capitalize_menus (t[i]);
    return r;
  }
}

/******************************************************************************
* Upgrade branches
******************************************************************************/

static tree
upgrade_traverse_branch (tree t) {
  if (is_atomic (t)) return t;
  else if (is_expand (t, "branch", 3) ||
	   (is_func (t, APPLY, 4) && (t[0] == "branch")))
    return tree (APPLY, t[0], t[1], t[3]);
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_traverse_branch (t[i]);
    return r;
  }
}

/******************************************************************************
* Upgrade sessions
******************************************************************************/

static tree
upgrade_session (tree t) {
  if (is_atomic (t)) return t;
  else if (is_expand (t, "session", 3)) {
    tree u= tree (EXPAND, "session", t[3]);
    tree w= tree (WITH);
    w << PROG_LANGUAGE << t[1] << THIS_SESSION << t[2] << u;
    return w;
  }
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_session (t[i]);
    return r;
  }
}

/******************************************************************************
* Upgrade sessions
******************************************************************************/

static tree
upgrade_formatting (tree t) {
  if (is_atomic (t)) return t;
  else if (is_func (t, FORMAT, 1)) {
    string name= replace (t[0]->label, " ", "_");
    return tree (as_tree_label (name));
  }
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_formatting (t[i]);
    return r;
  }
}

/******************************************************************************
* Upgrade expand
******************************************************************************/

static tree
upgrade_expand (tree t) {
#ifdef WITH_EXTENSIONS
  if (is_atomic (t)) return t;
  else if (is_func (t, EXPAND) && is_atomic (t[0])) {
    int i, n= N(t)-1;
    string s= t[0]->label;
    if (s == "quote") s= s * "-env";
    tree_label l= make_tree_label (s);
    tree r (l, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_expand (t[i+1]);
    return r;
  }
  else if (is_func (t, ASSIGN, 2) &&
	   (t[0] == "quote") &&
	   is_func (t[1], MACRO)) {
    return tree (ASSIGN, t[0]->label * "-env", upgrade_expand (t[1]));
  }
  else {
    int i, n= N(t);
    tree r (t, n);
    for (i=0; i<n; i++)
      r[i]= upgrade_expand (t[i]);
    return r;
  }
#else
  return t;
#endif
}

/******************************************************************************
* Upgrade from previous versions
******************************************************************************/

tree
upgrade_tex (tree t) {
  t= upgrade_apply_expand_value (t);
  t= upgrade_new_environments (t);
  t= upgrade_items (t);
  t= upgrade_table (t);
  t= upgrade_split (t);
  t= upgrade_title (t);
  t= simplify_correct (upgrade_mod_symbols (t));
  t= upgrade_menus_in_help (t);
  t= upgrade_capitalize_menus (t);
  t= upgrade_formatting (t);
  t= upgrade_expand (t);
  return t;
}

tree
upgrade (tree t, string version) {
  if (version_inf (version, "0.3.1.9")) {
    path p;
    t= upgrade_textual (t, p);
  }
  if (version_inf (version, "0.3.3.1"))
    t= upgrade_apply_expand_value (t);
  if (version_inf (version, "0.3.3.20"))
    t= upgrade_new_environments (t);
  if (version_inf (version, "0.3.3.24"))
    t= upgrade_items (t);
  if (version_inf (version, "0.3.4.4"))
    t= upgrade_resize (t);
  if (version_inf_eq (version, "0.3.4.7"))
    t= upgrade_table (t);
  if (version_inf_eq (version, "0.3.4.8"))
    t= upgrade_split (t);
  if (version_inf_eq (version, "0.3.5.6"))
    t= upgrade_project (t);
  if (version_inf_eq (version, "0.3.5.10"))
    t= upgrade_title (t);
  if (version_inf_eq (version, "1.0.0.1"))
    t= upgrade_cas (t);
  if (version_inf_eq (version, "1.0.0.8"))
    t= simplify_correct (upgrade_mod_symbols (t));
  if (version_inf_eq (version, "1.0.0.11"))
    t= upgrade_menus_in_help (t);
  if (version_inf_eq (version, "1.0.0.13"))
    t= upgrade_capitalize_menus (t);
  if (version_inf_eq (version, "1.0.0.19"))
    t= upgrade_traverse_branch (t);
  if (version_inf_eq (version, "1.0.1.20"))
    t= upgrade_session (t);
  if (version_inf_eq (version, "1.0.2.0"))
    t= upgrade_formatting (t);
  if (version_inf_eq (version, "1.0.2.3"))
    t= upgrade_expand (t);
  return t;
}
