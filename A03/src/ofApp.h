#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxImGui.h"
#include "ImHelpers.h"		//this gives us some extra ofxImGui features (needed for drop-down menus)

#include "CVC.h"

class ofApp : public ofBaseApp{
	public:

		// 3D Scene 
		ofEasyCam m_camera;
		ofLight m_light;
		ofMaterial m_material;


		ofVideoPlayer m_videoPlayer;
		ofVideoGrabber m_videoGrabber;
		bool m_videoResolutionConflict;		//do we need to re-size our video before we process?
		bool m_camResolutionConflict;		//do we need to re-size our video before we process?
		bool m_camPaused;					//pausing our camera feed
		
		ofxCvContourFinder m_contourFinder;

		// store movement data for ps move conts
		ofVec2f m_controller1Move;
		ofVec2f m_controller2Move;
		float m_smoothAngle = 0.0f;
		float m_smoothScale = 1.0f;

		//CV images
		ofxCvColorImage		m_colorImage;
		ofxCvGrayscaleImage	m_grayscaleDiffImage;

		//GUI
		ofxImGui::Gui		m_gui;
		int					m_threshold;	//how closely does the colour have to match to track?
		int					m_numContoursConsidered;
		float				m_minArea;
		float				m_maxArea;

		//we want to select which color we are tracking within the video
		float m_trackedColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	//ofxImgui requires color in this form
		float m_trackedColor2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		//app modes
		CVC::APP_MODE m_appMode;				//current app state
		std::vector<std::string> m_appModes;	//"list"/dynamic array of app states

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);

		//we will do all color CV processing here
		//we are passing a reference here ...
		void processColor(ofxCvColorImage& image);
};
