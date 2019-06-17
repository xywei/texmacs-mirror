<TeXmacs|1.99.9>

<style|source>

<\body>
  <active*|<\src-title>
    <src-package-dtd|pine-combo|1.0|pine-combo|1.0>

    <\src-purpose>
      Pine theme for presentations and posters.
    </src-purpose>

    <src-copyright|2013--2019|Joris van der Hoeven>

    <\src-license>
      This software falls under the <hlink|GNU general public license,
      version 3 or later|$TEXMACS_PATH/LICENSE>. It comes WITHOUT ANY
      WARRANTY WHATSOEVER. You should have received a copy of the license
      which the software. If not, see <hlink|http://www.gnu.org/licenses/gpl-3.0.html|http://www.gnu.org/licenses/gpl-3.0.html>.
    </src-license>
  </src-title>>

  <use-package|reddish-combo|wood-deco>

  <copy-theme|pine|reddish>

  <select-theme|pine|wood-pine>

  <select-theme|pine|wood-pine-bright-scene>

  <\active*>
    <\src-comment>
      Titles
    </src-comment>
  </active*>

  <select-title-deco|pine|wood-pine>

  <\active*>
    <\src-comment>
      Sessions
    </src-comment>
  </active*>

  <assign|pine-input-color|<pattern|paper-ridged-light.png|*3/5|*3/5|#f4eee8>>

  <assign|pine-fold-bar-color|<pattern|wood-dark.png|*3/5|*3/5|#40200c>>

  <assign|pine-fold-title-color|<pattern|wood-medium.png|*3/5|*3/5|#804018>>

  <\active*>
    <\src-comment>
      Posters
    </src-comment>
  </active*>

  <assign|pine-title-block|<value|wood-pine-block>>

  <assign|pine-framed-block|<value|wood-pine-block>>

  <assign|pine-framed-block*|<value|wood-pine-titled-block>>
</body>

<\initial>
  <\collection>
    <associate|sfactor|7>
  </collection>
</initial>