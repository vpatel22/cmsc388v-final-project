/* gui.h: interface for gui */
#ifndef SPECTRM_GUI_H
#define SPECTRM_GUI_H

struct frmbuf;

void gui_init(int width, int height, int scale, int fps);
void gui_finish(void);
/* returns 1 on quit event, 0 otherwise */
int gui_tick(void);
void gui_redraw(struct frmbuf *fb);

#endif
