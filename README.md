Spectrm# Assignment - Viraj Patel

Build
-----

Though I have included the `config.mk` file in this zip file, please rerun

    $ ./configure

to ensure the appropriate `config.mk` is generated. Then run

    $ make

to build the program.

Program Specifics
-----------------

To run this, please run ./pitch and connect an input to it. In the screen recorded video, I used the system_capture as my input.

In this way, I was able to use my voice as the input, which is why you'll see the magnitude jump at random points. I am not sure if the screen record captured my voice, but if it did, I'm sure that will make it easier to understand.

In regards to what this project actually does, I attempted to take my spectrm assignment, and using the frequencies we obtained there, create a very rudimentary pitch analyzer. I first obtained the maximum pitch in the for loop inside of `render` for the actual spectrm portion. I decided what frequency I wanted to use by calculating the maximum magnitude that I had, and used the associated frequency.

I used functions from https://github.com/ch98-1/Pitch-detector to help me with this.
