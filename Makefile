CFLAGS  += -c -std=c89 -Wall -Wextra -Werror -O0 `pkg-config --cflags cairo`
LDFLAGS += -L/usr/X11R6/lib -lxcb `pkg-config --libs cairo`

test: cairo_segfault_on_xcb_surface.c
	$(CC) $(CFLAGS) cairo_segfault_on_xcb_surface.c
	$(CC) $(LDFLAGS) -o $@ cairo_segfault_on_xcb_surface.o
