/* audio.h: interface for audio */
#ifndef SPECTRM_AUDIO_H
#define SPECTRM_AUDIO_H

struct spectrm_t;

void audio_init(struct spectrm_t *sp);
void audio_finish(void);

#endif
