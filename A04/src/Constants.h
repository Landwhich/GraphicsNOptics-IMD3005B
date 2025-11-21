#pragma once

#include "ofMain.h"

namespace Constants {
    static const int WINDOW_WIDTH = 1280;
    static const int WINDOW_HEIGHT = 900;

    enum SONG_STAGES{
        START    = 0,
        OUT      = 1,
        FULLOUT  = 2,
        FULL     = 3,
        REST     = 4
    };

    // enum VISAULIZER_STATE {
    //     ANALYSER    = 0,
    //     VISUALIZER  = 1
    // }
}