// ****** SOUND STUFF ******
#include "Audio.h"
#include "SD.h"
#include "FS.h"
 
// Digital I/O used
#define SD_CS          5
#define SPI_MOSI      23    // SD Card
#define SPI_MISO      19
#define SPI_SCK       18
 
#define I2S_DOUT      25
#define I2S_BCLK      27    // I2S
#define I2S_LRC       26

#define NUM_SOUNDS     8
#define VOLUME        25

const char* file_names[] = {
    "aua.wav",
    "bomb.wav",
    "das_wars.wav",
    "explosion.wav",
    "hit1.wav",
    "hit2.wav",
    "kabumm.wav",
    "lecker_lecker.wav",
};

