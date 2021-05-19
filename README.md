Final Project - Viraj Patel

Build
-----

Though I have included the `config.mk` file in this zip file, please rerun

    $ ./configure

to ensure the appropriate `config.mk` is generated. Then run

    $ make

to build the program.

Program Specifics
-----------------

To run this, please run ./pitch and connect an input to it. In the screen recorded video, I used the system_capture as my input. In this way, I was able to use my voice as the input.

In regards to what this project actually does, I attempted to take my spectrm assignment, and using the frequencies we obtained there, create a very rudimentary pitch analyzer. I first obtained the maximum pitch in the for loop inside of `render` for the actual spectrm portion. I decided what frequency I wanted to convert by calculating the maximum magnitude (`max_mag`) that I had in a single render, and used the associated frequency (`mag_freq`) with the maximum magnitude.

To do the actual pitch/note conversion, I used functions from https://github.com/ch98-1/Pitch-detector to help me with this. I primarily worked with the `convert_frequency.c` functions and used the functions in there to take my frequency and transform it into a midi note number (`f_to_midi`). This number was rounded (`f_to_midi_rounded`), and then clamped to a range between 21 and 128 (`clamp_midi_english`). Lastly, the midi number was converted into an actual note (`get_note_name_english`).
