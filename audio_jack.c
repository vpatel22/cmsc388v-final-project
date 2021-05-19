/* audio_jack.c: jack audio backend */
#include <stdio.h>
#include <jack/jack.h>
#include "spectrm.h"
#include "rb.h"
#include "util.h"

static jack_client_t *client = NULL;
static jack_port_t *port_in = NULL;

static int
on_process(jack_nframes_t nframes, void *arg)
{
  const sample_t *in;
  ringbuf *rb = arg;
  in = jack_port_get_buffer(port_in, nframes);

  rb_write(rb, in, nframes*sizeof(sample_t));
  return 0;
}

static void
on_shutdown(void *arg)
{
  die("jack server is down, exiting...");
}

void
audio_init(spectrm_t *sp)
{
  client = jack_client_open("pitch", JackNoStartServer, NULL);
  if (!client)
    die("fail to open jack client");

  sp->sr = jack_get_sample_rate(client);
  snprintf(sp->nyq_str, 16, "%dHz", (int)sp->sr/2);

  port_in = jack_port_register(client, "in", JACK_DEFAULT_AUDIO_TYPE,
                               JackPortIsInput, 0);
  if (!port_in)
    die("fail to register jack input port");

  jack_on_shutdown(client, on_shutdown, NULL);
  if (jack_set_process_callback(client, on_process, sp->rb))
    die("fail to set up jack process callback");

  if (jack_activate(client))
    die("fail to activate jack client");
}

void
audio_finish(void)
{
  jack_deactivate(client);
  jack_client_close(client);
}
