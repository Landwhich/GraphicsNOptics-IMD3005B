#pragma once

#include "ofMain.h"
#include "AudioAnalyser.h"
#include "Constants.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp {
public:

    ofEasyCam m_camera;
    ofLight m_light;

    ofxAssimpModelLoader m_violin;
    ofxAssimpModelLoader m_string1;
    ofxAssimpModelLoader m_string2;
    ofxAssimpModelLoader m_string3;
    ofxAssimpModelLoader m_string4;
    ofTexture m_violinTexture;

    float drums;
    float lowV;
    float highV;
    float violin;

    ofImage m_drumImage;
    ofImage m_lowVImage;
    ofImage m_highVImage;
    ofImage m_violinImage;

    // check if ranges are on for game points and effects
    bool drumsPlaying;
    bool lowVPlaying;
    bool highVPlaying;
    bool violinPlaying;

    float m_smoothDrumsAnim = 0;
    float m_smoothLowVAnim = 0;
    float m_smoothHighVAnim = 0;
    float m_smoothViolinAnim = 0;

    float m_smoothViolinScale;

    void setViolinScaleState(Constants::SONG_STAGES stage);

    ofTrueTypeFont bearDays;
    float totalScore;
    float songTimeSeconds;
    float m_smoothScale = 1.0f;
    float m_volScale;

	void setup();
	void update();
	void draw();
	void keyPressed(int key);

    bool visualizerState; // 0 for analyser and 1 for visualizer
    AudioAnalyser		m_audioAnalyser;

    float           m_circle1Brightness;
    float           m_circle2Brightness;
    float           m_circle3Brightness;
    float           m_circle4Brightness;
};
