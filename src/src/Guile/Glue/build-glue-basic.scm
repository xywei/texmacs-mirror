
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : build-glue-basic.scm
;; DESCRIPTION : Building basic glue
;; COPYRIGHT   : (C) 1999  Joris van der Hoeven
;;
;; This software falls under the GNU general public licence and comes WITHOUT
;; ANY WARRENTY WHATSOEVER. See the file $TEXMACS_PATH/LICENCE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(output-copyright "build-glue-basic.scm")

(build
  ""
  "initialize_glue_basic"

  (texmacs-version-release texmacs_version (string string))
  (os-win32? os_win32 (bool))
  (win32-display win32_display (void string))
  (scheme-dialect scheme_dialect (string))
  (get-texmacs-path get_texmacs_path (string))
  (plugin-list plugin_list (scheme_tree))
  (support-ec-fonts? support_ec_fonts (bool))
  (support-tt-fonts? ft_present (bool))
  (use-ec-fonts? use_ec_fonts (bool))
  (use-tt-fonts? use_tt_fonts (bool))
  (set-font-type set_font_type (void int))
  (font-exists-in-tt? tt_font_exists (bool string))
  (eval-system eval_system (string string))
  (var-eval-system var_eval_system (string string))
  (get-locale-language get_locale_language (string))
  (texmacs-time texmacs_time (int))
  (bench-print bench_print (void string))
  (bench-print-all bench_print (void))
  (system-wait system_wait (void string string))
  (set-bibtex-command set_bibtex_command (void string))
  (math-symbol-type math_symbol_type (string string))
  (object->command as_command (command object))

  ;; routines for trees
  (tree->stree tree_to_scheme_tree (scheme_tree tree))
  (stree->tree scheme_tree_to_tree (tree scheme_tree))
  (tree->string coerce_tree_string (string tree))
  (string->tree coerce_string_tree (tree string))
  (tm->tree tree (tree content))
  (tree-atomic? is_atomic (bool tree))
  (tree-compound? is_compound (bool tree))
  (tree-label L (tree_label tree))
  (tree-children A (array_tree tree))
  (tree-arity N (int tree))
  (tree-child tree_ref (tree tree int))
  (tree-set-child! tree_set (void tree int tree))
  (tree-ip obtain_ip (path tree))
  (subtree subtree (tree tree path))
  (tree-range tree_range (tree tree int int))
  (tree-copy copy (tree tree))
  (tree-append tree_append (tree tree tree))
  (tree-label-extension? is_extension (bool tree_label))
  (tree-multi-paragraph? is_multi_paragraph (bool tree))
  (tree-simplify simplify_correct (tree tree))
  (tree-accessible-child? is_accessible_child (bool tree int))
  (tree-accessible-children accessible_children (array_tree tree))
  (tree-load-inclusion load_inclusion (tree url))

  ;; paths
  (path-inf? path_inf (bool path path))
  (path-inf-eq? path_inf_eq (bool path path))
  (path-less? path_less (bool path path))
  (path-less-eq? path_less_eq (bool path path))
  (path-start start (path content path))
  (path-end end (path content path))
  (path-next next_valid (path content path))
  (path-previous previous_valid (path content path))
  (path-next-word next_word (path content path))
  (path-previous-word previous_word (path content path))
  (path-next-tag next_tag (path content path scheme_tree))
  (path-previous-tag previous_tag (path content path scheme_tree))
  (path-next-argument next_argument (path content path))
  (path-previous-argument previous_argument (path content path))

  ;; routines for strings
  (string-number? is_double (bool string))
  (string-search-forwards search_forwards (int string int string))
  (string-search-backwards search_backwards (int string int string))
  (string-replace replace (string string string string))
  (string-slash slash (string string))
  (string-unslash unslash (string string))
  (string-locase-alpha? is_locase_alpha (bool string))
  (upcase-first upcase_first (string string))
  (locase-first locase_first (string string))
  (upcase-all upcase_all (string string))
  (locase-all locase_all (string string))
  (string-union string_union (string string string))
  (string-minus string_minus (string string string))
  (escape-quotes escape_quotes (string string))
  (escape-generic escape_generic (string string))
  (escape-verbatim escape_verbatim (string string))
  (utf8->cork utf8_to_cork (string string))
  (cork->utf8 cork_to_utf8 (string string))
  (utf8->html utf8_to_html (string string))
  (tm->xml-name tm_to_xml_name (string string))
  (old-tm->xml-cdata old_tm_to_xml_cdata (string string))
  (tm->xml-cdata tm_to_xml_cdata (object string))
  (xml-name->tm xml_name_to_tm (string string))
  (old-xml-cdata->tm old_xml_cdata_to_tm (string string))
  (xml-unspace xml_unspace (string string bool bool))

  ;; further conversion routines for trees and strings
  (parse-texmacs texmacs_document_to_tree (tree string))
  (serialize-texmacs tree_to_texmacs (string tree))
  (parse-texmacs-snippet texmacs_to_tree (tree string))
  (serialize-texmacs-snippet tree_to_texmacs (string tree))
  (texmacs->scheme tree_to_scheme (string tree))
  (scheme->texmacs scheme_document_to_tree (tree string))
  (scheme-snippet->texmacs scheme_to_tree (tree string))
  (texmacs->verbatim tree_to_verbatim (string tree))
  (verbatim-snippet->texmacs verbatim_to_tree (tree string))
  (verbatim->texmacs verbatim_document_to_tree (tree string))
  (parse-latex parse_latex (tree string))
  (parse-latex-document parse_latex_document (tree string))
  (latex->texmacs latex_to_tree (tree tree))
  (parse-xml parse_xml (scheme_tree string))
  (parse-html parse_html (scheme_tree string))
  (upgrade-tmml tmml_upgrade (tree scheme_tree))

  ;; routines for urls
  (string->url url (url string))
  (url url (url string string))
  (url-system url_system (url string))
  (url-none url_none (url))
  (url-any url_wildcard (url))
  (url-wildcard url_wildcard (url string))
  (url-parent url_parent (url))
  (url-append url_concat (url url url))
  (url-or url_or (url url url))
  (url->string as_string (string url))
  (url-none? is_none (bool url))
  (url-rooted-web? is_rooted_web (bool url))
  (url-concat? is_concat (bool url))
  (url-or? is_or (bool url))
  (url-ref url_ref (url url int))
  (url-tail tail (url url))
  (url-suffix suffix (string url))
  (url-glue glue (url url string))
  (url-unglue unglue (url url int))
  (url-relative relative (url url url))
  (url-expand expand (url url))
  (url-factor factor (url url))
  (url-delta delta (url url url))

  (url-complete complete (url url string))
  (url-resolve resolve (url url string))
  (url-resolve-in-path resolve_in_path (url url))
  (url-exists? exists (bool url))
  (url-exists-in-path? exists_in_path (bool url))
  (url-exists-in-tex? exists_in_tex (bool url))
  (url-concretize concretize (string url))
  (url-materialize materialize (string url string))
  (url-test? is_of_type (bool url string))
  (url-regular? is_regular (bool url))
  (url-directory? is_directory (bool url))
  (url-link? is_symbolic_link (bool url))
  (url-newer? is_newer (bool url url))
  (url-temp url_temp (url))
  (string-save string_save (void string url))
  (string-load string_load (string url))
  (system-move move (void url url))
  (system-copy copy (void url url))
  (system-append append (void url url))
  (system-remove remove (void url))
  (system-mkdir mkdir (void url))

  ;; connections to extern systems
  (connection-declared? connection_declared (bool string))
  (connection-status connection_status (int string string))
  (connection-start connection_start (string string string bool))
  (connection-eval connection_eval (tree string string content))
  (connection-cmd connection_cmd (tree string string string))
  (connection-write connection_write (void string string content))
  (connection-read connection_read (tree string string string))

  ;; widgets
  (widget-hlist horizontal_list (widget array_widget))
  (widget-named-hlist horizontal_list (widget array_widget array_string))
  (widget-vlist vertical_list (widget array_widget))
  (widget-named-vlist vertical_list (widget array_widget array_string))
  (widget-vmenu vertical_menu (widget array_widget))
  (widget-tile tile (widget array_widget int))
  (widget-named-tile tile (widget array_widget int array_string))
  (widget-harray horizontal_array (widget array_widget int))
  (widget-named-harray horizontal_array (widget array_widget array_string int))
  (widget-switch switch_widget (widget array_widget array_string))
  (widget-switch-init switch_widget (widget array_widget array_string int))
  (widget-optional optional_widget (widget widget))
  (widget-optional-init optional_widget (widget widget bool))
  (widget-glue glue_widget (widget bool bool int int))
  (widget-separator separator_widget (widget int int bool))
  (widget-text text_widget (widget string bool string))
  (widget-xpm xpm_widget (widget url bool))
  (widget-menu-text menu_text_widget (widget string int string bool))
  (widget-command-button-1 command_button (widget widget command bool))
  (widget-command-button-2 command_button (widget widget widget command))
  (widget-command-button-3 command_button
			   (widget widget widget widget command bool bool))
  (widget-pulldown-button pulldown_button (widget widget widget))
  (widget-pullright-button pullright_button (widget widget widget))
  ;;(widget-popup popup_widget (widget widget gravity))
  ;;(widget-canvas canvas_widget (widget widget gravity))
  ;;(widget-text-input input_text_widget (widget command))
  ;;(widget-file-chooser file_chooser_widget (widget command string string))
  (widget-balloon balloon_widget (widget widget widget))
  (widget-pulldown-button-lazy pulldown_button (widget widget make_widget))
  (widget-pullright-button-lazy pullright_button (widget widget make_widget))
  (widget-wait wait_widget (widget int int string))
  (widget-box box_widget (widget scheme_tree string int bool bool))
  (object->make-widget as_make_widget (make_widget object)))
