#include <maxmod9.h>
#include <nds.h>

#include "msW.h"

#define DATA_ID 0x61746164
#define FMT_ID  0x20746d66
#define RIFF_ID 0x46464952
#define WAVE_ID 0x45564157

typedef struct WAVHeader {
    // "RIFF" chunk descriptor
    uint32_t chunkID;
    uint32_t chunkSize;
    uint32_t format;
    // "fmt" subchunk
    uint32_t subchunk1ID;
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    // "data" subchunk
    uint32_t subchunk2ID;
    uint32_t subchunk2Size;
} WAVHeader_t;
static WAVHeader_t wavHeader = {0};


static FILE *wavFile = NULL;
static mm_stream stream = {0};


static mm_word streamingCallback(mm_word length, mm_addr dest, mm_stream_formats format) {
    // This is mono not stereo :P
    int res = 0;
    res = fread(dest, 2, length, wavFile);

    if(feof(wavFile)) {
        // Loop back when song ends
        fseek(wavFile, sizeof(WAVHeader_t), SEEK_SET);
        res = fread(dest, 2, length, wavFile);
    }
    // Return the number of words streamed so that if this number is less than
    // "length", the rest will be buffered and used in the next stream
    return res;
}

static int checkWAVHeader(const WAVHeader_t header) {
    if(header.chunkID != RIFF_ID) {
        printf("Wrong RIFF_ID %lx\n", header.chunkID);
        return 1;
    }

    if(header.format != WAVE_ID) {
        printf("Wrong WAVE_ID %lx\n", header.format);
        return 1;
    }

    if(header.subchunk1ID != FMT_ID) {
        printf("Wrong FMT_ID %lx\n", header.subchunk1ID);
        return 1;
    }

    if(header.subchunk2ID != DATA_ID) {
        printf("Wrong Subchunk2ID %lx\n", header.subchunk2ID);
        return 1;
    }

    return 0;
}


static mm_stream_formats getMMStreamType(uint16_t numChannels, uint16_t bitsPerSample) {
    if (numChannels == 1) {
        if (bitsPerSample == 8) {
            return MM_STREAM_8BIT_MONO;
        } else {
            return MM_STREAM_16BIT_MONO;
        }
    } else if (numChannels == 2) {
        if (bitsPerSample == 8) {
            return MM_STREAM_8BIT_STEREO;
        } else {
            return MM_STREAM_16BIT_STEREO;
        }
    }
    return MM_STREAM_8BIT_MONO;
}


bool msW::loadWav(const char* path) {
    if(path == NULL or path == nullptr) return 1;
    wavFile = fopen(path, "rb");

    if(wavFile == nullptr or wavFile == NULL) {
        return 1;
    }
    wavHeader = {0};

    if(fread(&wavHeader, 1, sizeof(WAVHeader_t), wavFile) == 0) {
        return 1;
    }
    if(checkWAVHeader(wavHeader) != 0) {
        return 1;
    }

    stream.sampling_rate = wavHeader.sampleRate;
    stream.buffer_length = 16000;
    stream.callback      = streamingCallback;
    stream.format        = getMMStreamType(wavHeader.numChannels,
                                           wavHeader.bitsPerSample);
    stream.timer         = MM_TIMER0;
    stream.manual        = true;
    mmStreamOpen(&stream);

    return 0;
}


void msW::endWav() {
    if(wavFile == NULL) return;
    mmStreamClose();
    fclose(wavFile);
    wavFile = NULL;
}
