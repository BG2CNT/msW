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

namespace msW {
    
    /*
        Loads and start WAV streaming.
    
        @param Location Location to wav in nitroFS.
    
        @return Returns 1 if failed.
    */
    bool loadWav(const char* Location);
    
    /*
        End wav streaming :)
    */
    void endWav();

}
    
#endif