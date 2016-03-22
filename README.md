# RSA
Implementing RSA for learning purposes, obviously still in very early stages.

For now, I am working on a keygen prototype.

Things that currently make it cryptographically unsecure:
 - Generated keys are currently 32-Bit
 - Generation of random numbers is done with `rand()`, seeded with `time(NULL)` (I know, I know...)

The reasons are that I didn't want to  deal with large integer arithmetic and external CSPRGN libraries right now.
