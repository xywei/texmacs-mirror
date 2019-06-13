<TeXmacs|1.99.9>

<style|source>

<\body>
  <active*|<\src-title>
    <src-package-dtd|paper-manila-ornament|1.0|paper-manila-ornament|1.0>

    <\src-purpose>
      Manilla paper ornamenrt for presentations and posters.
    </src-purpose>

    <src-copyright|2013--2019|Joris van der Hoeven>

    <\src-license>
      This software falls under the <hlink|GNU general public license,
      version 3 or later|$TEXMACS_PATH/LICENSE>. It comes WITHOUT ANY
      WARRANTY WHATSOEVER. You should have received a copy of the license
      which the software. If not, see <hlink|http://www.gnu.org/licenses/gpl-3.0.html|http://www.gnu.org/licenses/gpl-3.0.html>.
    </src-license>
  </src-title>>

  <use-package|light-theme>

  <copy-theme|paper-manila|light-ornament>

  <assign|manila-paper|<macro|x|<with-paper-manila|<ornament|<arg|x>>>>>

  <\active*>
    <\src-comment>
      The ornament
    </src-comment>
  </active*>

  <assign|paper-manila-ornament-color|<pattern|paper-manila-medium.png|*3/5|*3/5|#d0d0c0>>

  <assign|paper-manila-ornament-extra-color|<pattern|wood-medium.png|*3/5|*3/5|#804018>>

  <assign|paper-manila-ornament-title-color|<pattern|paper-manila-bright.png|*3/5|*3/5|#f4f4f0>>

  <assign|paper-manila-ornament-sunny-color|#e8e8e0>

  <assign|paper-manila-ornament-shadow-color|#acac90>

  <\active*>
    <\src-comment>
      Colors
    </src-comment>
  </active*>

  <assign|paper-manila-strong-color|#504000>
</body>

<\initial>
  <\collection>
    <associate|sfactor|7>
  </collection>
</initial>