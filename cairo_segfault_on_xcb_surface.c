/*
 * Test demonstrating how a second call to cairo_xcb_surface_create() bombs.
 * Note I've stripped down most of the setup/etc to be as minimal as possible.
 * Error checking on all xcb/cairo calls previous show no errors when present.
 */
#include <stdio.h>
#include <unistd.h>

#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>

xcb_visualtype_t*
get_root_visual(xcb_screen_t *screen)
{
   xcb_depth_iterator_t i = xcb_screen_allowed_depths_iterator(screen);
   for (; i.rem; xcb_depth_next(&i)) {
      if (i.data->depth != 32)
         continue;

      xcb_visualtype_iterator_t vi;
      vi = xcb_depth_visuals_iterator(i.data);
      for (; vi.rem; xcb_visualtype_next(&vi)) {
         return vi.data;
      }
   }

   return NULL;
}

void
test()
{
   xcb_connection_t *x = xcb_connect(NULL, NULL);
   xcb_drawable_t    w = xcb_generate_id(x);
   xcb_screen_t     *s = xcb_setup_roots_iterator(xcb_get_setup(x)).data;
   xcb_visualtype_t *v = get_root_visual(s);

   xcb_create_window(
         x,
         32,                        /* force 32 bit */
         w,
         s->root,
         0, 0, 0, 0,                /* x,y,w,h */
         0,                         /* border width */
         XCB_WINDOW_CLASS_INPUT_OUTPUT,
         s->root_visual,
         0, NULL);

   cairo_surface_t *cs = cairo_xcb_surface_create(
         x,
         w,
         v,
         1000,
         1000);
   cairo_t *c = cairo_create(cs);

   sleep(1);

   cairo_destroy(c);
   cairo_surface_destroy(cs);
   xcb_destroy_window(x, w);
   xcb_disconnect(x);
}

int
main()
{
   printf("first call...\n");
   test();
   printf("second call...\n");
   test(); /* segfaults on cairo_xcb_surface_create() */

   return 0;
}
