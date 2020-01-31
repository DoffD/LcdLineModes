# LcdLineModes
 An example showing the use of an LCD line in different 'modes'. The objective is to enable the 'simultaneous' use of an LCD line output by different 'processes'. Default mode: An LCD line is controlled by a state machine 'listening' to an incrementing global variable. Also, the state machine waits for a 'flash' command and its parameters (message and duration). It can then flash the message for the required period of time, then it goes back to displaying the previous incrementing global variable. Yield mode: The same LCD line 'yields' control to other state machines, e.g. a keypad input.
