Customising fonts
=================

Fonts are defined in `src/libtm1640/font.h`, which is embedded in the library.

It consists of a 1 dimensional table, defining the LED pattern for that character.

The display uses the ``GFEDCBA`` configuration of digits.  Unsupported charecters are simply set to a space. 

