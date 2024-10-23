/*
    //////////////////////
    msWlib
    By BG2CNT.
    //////////////////////
    Maxmod wav streamer made using BlocksDS examples.

    Limitation: Only supports mono.
*/

#ifndef MSW_H
#define MSW_H

/*
    Loads and start WAV streaming.

    @param Location Location to wav in nitroFS.

    @return Returns 1 if failed.
*/
bool MSWLoadWav(const char* Location);

/*
    End wav streaming :)
*/
void MSWEndWav();

#endif