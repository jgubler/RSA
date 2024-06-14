# Playing around with RSA implementation (obviously don't use this code)
Experimenting with RSA implementation for learning purposes, obviously not to be meant for usage in anything else, also limitied to a keygen prototype.

Main things that currently make it cryptographically insecure, aside from my questionable C programming skills:
 - Generated keys are 32-Bit
 - Generation of random numbers is done with `rand()`, seeded with `time(NULL)` (I know, I know...)

This was just to avoid large integer arithmetic and external CSPRGN libraries, as it was never meant for secure production use anyhow.
