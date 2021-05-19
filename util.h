/* util.h: common utilities */
#ifndef SPECTRM_UTIL_H
#define SPECTRM_UTIL_H

void die(const char *msg);

/* map range [x0,x1] -> [y0,y1] */
float map(float val, float x0, float x1, float y0, float y1);
/* log scale [min,max] to linear scale [0,1] */
float log2lin(float val, float min, float max);

#endif
