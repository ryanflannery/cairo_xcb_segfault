# cairo + xcb segfault test code

i wrote this to help debug and solve an issue where cairo can segfault when
changing the xcb display info being passed to it.

many thanks to Uli Schlachter who finally helped me solve it in:
https://lists.cairographics.org/archives/cairo/2018-November/028791.html
