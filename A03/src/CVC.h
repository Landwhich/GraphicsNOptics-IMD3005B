#pragma once

#include "ofMain.h"

namespace CVC {
    static const int APP_WINDOW_WIDTH       = 1280;
    static const int APP_WINDOW_HEIGHT      = 720;
    static const int APP_DESIRED_FRAMERATE  = 60;
    static const int CAM_DESIRED_FRAMERATE  = 30;

    static const std::string VIDEO_PATH_COLOR = "tupperware.mov";
    static const int VIDEO_WIDTH    = 320;      //we will use these values for BOTH cam and video
    static const int VIDEO_HEIGHT   = 240;

    static const int GAME_SCREEN_WIDTH  = 640;
    static const int GAME_SCREEN_HEIGHT = 480;

    static const int VIDEO_BORDER_SIZE  = 10;

    typedef enum TYPE {
        APP_VIDEO,            //live feed
        APP_CAM,           //simulated feed
        APP_GAME
    } APP_MODE;
};
