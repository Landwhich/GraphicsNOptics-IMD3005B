#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class SmokeParticle {
public:
    ofVec2f pos;
    ofVec2f vel;
    float life;
    float maxLife;

    SmokeParticle(ofVec2f startPos) {
        pos = startPos;
        vel = ofVec2f(ofRandom(-1, 1), ofRandom(-3, -1));
        maxLife = life = ofRandom(1.0, 2.5);
    }

    void update(float dt) {
        pos += vel;
        life -= dt;
    }
    
    void draw() {
        float alpha = ofMap(life, 0, maxLife, 0, 255);
        ofSetColor(200, 200, 200, alpha);
        ofDrawCircle(pos, ofMap(life, 0, maxLife, 5, 20));
    }

    bool isDead() const {
        return life <= 0;
    }
};


class ofApp : public ofBaseApp{
	public:
		
	ofCamera camera; 
	ofLight light;

	ofTrueTypeFont rocketFont;

	ofImage rocketImage;

	// entity models
	ofxAssimpModelLoader sol;
	ofxAssimpModelLoader mars; // too lazy to get unique planet models
	ofxAssimpModelLoader mars2;
	ofxAssimpModelLoader mars3;
	ofxAssimpModelLoader mars4;
	ofxAssimpModelLoader phobos;
	ofxAssimpModelLoader deimos;

	// entity textures
	ofTexture marsTexture;
	ofTexture moonTexture;
	ofTexture solTexture;

	// entity attributes
	ofVec2f m_solPos;
	ofVec2f m_marsPos;
	ofVec2f m_marsPos2;
	ofVec2f m_marsPos3;
	ofVec2f m_marsPos4;
	float m_rotFast;
	float m_rotMed;
	float m_rotSlow;
	ofVec2f m_phobosPos;
	ofVec2f m_deimosPos;

	ofVec2f m_rocketPos;
	std::vector<SmokeParticle> smokeParticles;

	ofVec2f m_mousePos;

	void setup();
	void update();
	void draw();

	void mousePressed(int x, int y, int button);
	
	// void keyPressed(int key);
	// void keyReleased(int key);
	// void mouseMoved(int x, int y);
	// void mouseDragged(int x, int y, int button);
	
	// void mouseReleased(int x, int y, int button);
	// void windowResized(int w, int h);
	// void dragEvent(ofDragInfo dragInfo);
	// void gotMessage(ofMessage msg);

	ofShader shader;
	ofPlanePrimitive plane;


};
