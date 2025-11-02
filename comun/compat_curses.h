#ifndef COMPAT_CURSES_H
#define COMPAT_CURSES_H

/* Evita warnings por redefinición de macros (p.ej. MOUSE_MOVED) cuando
   headers de Windows se incluyen antes que PDCurses. No modificamos
   headers de terceros: simplemente anulamos la macro si existe antes
   de incluir <curses.h> para que PDCurses la defina sin warning. */

#ifdef MOUSE_MOVED
#undef MOUSE_MOVED
#endif

#include <curses.h>

#endif /* COMPAT_CURSES_H */
