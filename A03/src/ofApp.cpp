#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //set up app
    ofSetWindowShape(CVC::APP_WINDOW_WIDTH, CVC::APP_WINDOW_HEIGHT);
    ofSetFrameRate(CVC::APP_DESIRED_FRAMERATE);

    // --- 3D scene setup ---
    ofEnableDepthTest();
    m_light.setup();
    m_light.setPosition(300, 300, 600);
    m_material.setShininess(64);

    //set up cam
    //m_videoGrabber.listDevices();     //to find other cameras attached
    m_videoGrabber.setDeviceID(0);      //assuming first and only camera attached
    m_videoGrabber.setDesiredFrameRate(CVC::CAM_DESIRED_FRAMERATE);
    m_videoGrabber.initGrabber(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
    m_camResolutionConflict = ( ((int)m_videoGrabber.getWidth() != CVC::VIDEO_WIDTH) ||
                                ((int)m_videoGrabber.getHeight() != CVC::VIDEO_HEIGHT));

    //set up video
    m_videoPlayer.load(CVC::VIDEO_PATH_COLOR);
    m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
    m_videoPlayer.play();

    //want to check to see if video being loaded in matches the video size consts we wish to process at
    m_videoResolutionConflict = ( ((int)m_videoPlayer.getWidth() != CVC::VIDEO_WIDTH) || 
                                  ((int)m_videoPlayer.getHeight() != CVC::VIDEO_HEIGHT));

    //allocate CV images
    m_colorImage.allocate(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
    m_grayscaleDiffImage.allocate(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);

    //init vars
    m_threshold             = 30;
    m_numContoursConsidered = 1;
    m_minArea               = 10.0f;
    m_maxArea               = (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT);
    m_controller1Move       = {0.0f, 0.0f};
    m_controller2Move       = {0.0f, 0.0f};

    //gui
    m_gui.setup();

    //set up app states
    m_appMode = CVC::APP_MODE::APP_CAM;
    m_appModes.push_back("APP_VIDEO");
    m_appModes.push_back("APP_CAM");
    m_appModes.push_back("APP_GAME");
}

//--------------------------------------------------------------
void ofApp::update(){
    switch (m_appMode) {
        case CVC::APP_MODE::APP_VIDEO: {
            m_videoPlayer.update();

            //only process if new frame
            if (m_videoPlayer.isFrameNew() == true) {
                m_colorImage.setFromPixels(m_videoPlayer.getPixels());

                //resize, if res conflict
                if (m_videoResolutionConflict == true) {
                    m_colorImage.resize(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
                }

                processColor(m_colorImage);
            }
        }
            break;
        
        case CVC::APP_MODE::APP_CAM: {
            if (m_camPaused == false) {
                m_videoGrabber.update();

                if (m_videoGrabber.isFrameNew() == true) {
                    m_colorImage.setFromPixels(m_videoGrabber.getPixels());
                }

                //resize, if res conflict
                if (m_camResolutionConflict == true) {
                    m_colorImage.resize(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
                }

                processColor(m_colorImage);
            }
            
        }
            break;

        case CVC::APP_MODE::APP_GAME: {
            if (!m_camPaused) {
                m_videoGrabber.update();

                if (m_videoGrabber.isFrameNew()) {
                    m_colorImage.setFromPixels(m_videoGrabber.getPixels());
                }

                //resize, if res conflict
                if (m_camResolutionConflict) {
                    m_colorImage.resize(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
                }
                
                m_threshold             = 30;
                m_numContoursConsidered = 2;
                m_minArea               = 10.0f;

                //hardcoded for simplicity of demo
                // m_trackedColor = { 0.0f, 0.0f, 0.0f, 0.0f };
                // m_trackedColor2 = { 0.0f, 0.0f, 0.0f, 0.0f };

                processColor(m_colorImage);

                if (m_contourFinder.nBlobs >= 2){
                    m_controller1Move.set(m_contourFinder.blobs[0].boundingRect.getCenter());
                    m_controller2Move.set(m_contourFinder.blobs[1].boundingRect.getCenter());
                }

                ofVec2f currVec = m_controller2Move - m_controller1Move;
                float currAngle = atan2(currVec.y, currVec.x);
                // smooth rotation for stability
                float angleDeg = ofRadToDeg(currAngle);
                m_smoothAngle = ofLerp(m_smoothAngle, angleDeg, 0.1f);

                float currDist = currVec.length();
                m_smoothScale = ofLerp(m_smoothScale, currDist, 0.1f);

                m_smoothAngle = fmodf(m_smoothAngle, 360.0f);
                if (abs(currDist - m_smoothScale) < 2.0f) 
                    m_smoothScale = currDist;

                // m_rotationX = 0.9f * m_rotationX + 0.1f * mappedTilt;
            }
            
        }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //draw videos
    ofSetColor(255, 255, 255);
    m_grayscaleDiffImage.draw(CVC::VIDEO_BORDER_SIZE, CVC::VIDEO_BORDER_SIZE);
    m_colorImage.draw(CVC::VIDEO_WIDTH + CVC::VIDEO_BORDER_SIZE * 2.0f, CVC::VIDEO_BORDER_SIZE);

    //draw contours
    static ofVec2f contourCenter;
    static float contourArea;

    ofPushMatrix();
    {
        ofTranslate(CVC::VIDEO_BORDER_SIZE, CVC::VIDEO_BORDER_SIZE);

        //loop through blobs and draw them with some debug info.
        for (int i = 0; i < m_contourFinder.nBlobs; i++) {
            contourCenter.set(  m_contourFinder.blobs[i].boundingRect.getCenter().x, 
                                m_contourFinder.blobs[i].boundingRect.getCenter().y );
            contourArea = m_contourFinder.blobs[i].area;

            //draw contour
            m_contourFinder.blobs[i].draw();

            //draw center
            ofSetColor(ofColor::coral);
            ofDrawCircle(contourCenter.x, contourCenter.y, 5.0f);

            //draw text info.
            ofDrawBitmapString( "Centre: " + ofToString(contourCenter.x) + "," + ofToString(contourCenter.y),
                                m_contourFinder.blobs[i].boundingRect.getMaxX() + CVC::VIDEO_BORDER_SIZE,
                                contourCenter.y );
            ofDrawBitmapString( "Area: " + ofToString(contourArea),
                                m_contourFinder.blobs[i].boundingRect.getMaxX() + CVC::VIDEO_BORDER_SIZE,
                                contourCenter.y + 20.0f );
        }
    }
    ofPopMatrix();

    ofPushStyle();
    {
        ofViewport(CVC::VIDEO_BORDER_SIZE, CVC::VIDEO_BORDER_SIZE * 2.0f + CVC::VIDEO_HEIGHT, CVC::GAME_SCREEN_WIDTH, CVC::GAME_SCREEN_HEIGHT);
        ofEnableDepthTest();
        m_camera.begin();
        // m_camera.disableMouseInput();

        m_light.enable();
        m_material.begin();

        ofPushMatrix();
        {
            ofSetColor(255);
            ofTranslate(0, 0, 0);
            ofRotateDeg(m_smoothAngle, 0, 0, 1); // rotate around Z-axis
            float scaleFactor = ofMap(m_smoothScale, 50, 400, 0.5, 3.0, true);
            ofScale(scaleFactor, scaleFactor, scaleFactor);
            ofDrawBox(0, 0, 0, 100);
        }
        ofPopMatrix();

        m_material.end();
        m_light.disable();
        ofDisableLighting();

        m_camera.end();
        ofDisableDepthTest();
    }
    ofPopStyle();

    ofViewport(0, 0, ofGetWidth(), ofGetHeight());
    ofSetupScreenOrtho();
    ofSetColor(255);

    //draw gui
    m_gui.begin();
    {
        ImGui::Text("OpenCV Color Lesson");
        ImGui::SliderInt("Threshold", &m_threshold, 0, 255);
        ImGui::SliderInt("Num of Contours", &m_numContoursConsidered, 0, 30);
        ImGui::SliderFloat("Min. Area", &m_minArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));
        ImGui::SliderFloat("Max. Area", &m_maxArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));

        ImGui::Separator(); //you've got to keep them separated

        ImGui::Text("\n Please select app state, thank you! Or not ... in that case, no thank you!");
        static int currentListBoxIndex = 0;
        if (ofxImGui::VectorCombo("App Mode", &currentListBoxIndex, m_appModes)) {
            m_appMode = (CVC::APP_MODE)currentListBoxIndex;
        }
        ImGui::ColorEdit3("Selected Color", (float *)m_trackedColor);
        ImGui::ColorEdit3("Selected Color 2", (float *)m_trackedColor2);
        ImGui::Text("\n Instructions: \npress spacebar to toggle pause \nright-click on video to select colour");
    }
    m_gui.end();

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //each key on a keyboard ha sa keycode associated with it
    //32 = spacebar
    if (key == ' ') {
        switch (m_appMode) {
        case CVC::APP_MODE::APP_VIDEO: {
            //set the opposite of what pause state it alrready is
            m_videoPlayer.setPaused(!m_videoPlayer.isPaused());
        }
            break;
        case CVC::APP_MODE::APP_CAM: {
            m_camPaused = !m_camPaused;
        }
            break;
        case CVC::APP_MODE::APP_GAME: {
            m_camPaused = !m_camPaused;
        }
            break;
        default: 
            break;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //let's do this in a bit 
    if (button == OF_MOUSE_BUTTON_RIGHT) {
        //select the colour from the image
        //we CAN't SELECT color from outside the image (may get an indes out of array error)
        ofRectangle videoRect = ofRectangle( CVC::VIDEO_WIDTH + CVC::VIDEO_BORDER_SIZE * 2,
            CVC::VIDEO_BORDER_SIZE,
            CVC::VIDEO_WIDTH,
            CVC::VIDEO_HEIGHT 
        );

        //step 1: clamping screenspace to image space
        int convertX = ofClamp(x, videoRect.getMinX(), videoRect.getMaxX());
        int convertY = ofClamp(y, videoRect.getMinY(), videoRect.getMaxY());

        //step2: mapping screenspace to image space (by shift coordinate origin
        convertX -= videoRect.getMinX();
        convertY -= videoRect.getMinY();

        //get the color using x + y * width function ...
        const int index = (convertX + convertY * m_colorImage.getWidth()) * m_colorImage.getPixels().getNumChannels();
        m_trackedColor2[0] = m_colorImage.getPixels()[index + 0] / 255.0f; //red, divide by 255.0f as m_trackedColor is a float color [0.0f, 1.0f]
        m_trackedColor2[1] = m_colorImage.getPixels()[index + 1] / 255.0f; //green
        m_trackedColor2[2] = m_colorImage.getPixels()[index + 2] / 255.0f; //blue
    }

    else if (button == OF_MOUSE_BUTTON_LEFT){
        ofRectangle videoRect = ofRectangle( CVC::VIDEO_WIDTH + CVC::VIDEO_BORDER_SIZE * 2,
            CVC::VIDEO_BORDER_SIZE,
            CVC::VIDEO_WIDTH,
            CVC::VIDEO_HEIGHT );

        //step 1: clamping screenspace to image space
        int convertX = ofClamp(x, videoRect.getMinX(), videoRect.getMaxX());
        int convertY = ofClamp(y, videoRect.getMinY(), videoRect.getMaxY());

        //step2: mapping screenspace to image space (by shift coordinate origin
        convertX -= videoRect.getMinX();
        convertY -= videoRect.getMinY();

        //get the color using x + y * width function ...
        const int index = (convertX + convertY * m_colorImage.getWidth()) * m_colorImage.getPixels().getNumChannels();
        m_trackedColor[0] = m_colorImage.getPixels()[index + 0] / 255.0f; //red, divide by 255.0f as m_trackedColor is a float color [0.0f, 1.0f]
        m_trackedColor[1] = m_colorImage.getPixels()[index + 1] / 255.0f; //green
        m_trackedColor[2] = m_colorImage.getPixels()[index + 2] / 255.0f; //blue
    }
}

void ofApp::processColor(ofxCvColorImage& image) {
    const int numChannelsPerPixel   = image.getPixels().getNumChannels(); //shouild be 3 for red, green, blue channels
    const int numChannels           = CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT * numChannelsPerPixel;
    float pixel[3]                  = { 0.0f, 0.0f, 0.0f };

    //loop through color image pixels
    //check if pixel color matches our tracked color
    //if true, set our diff image pixel color to white
    //if false, set our diff image pixel color to black
    for (int i = 0; i < numChannels; i+=numChannelsPerPixel) {
        pixel[0] = image.getPixels()[i + 0]; //red
        pixel[1] = image.getPixels()[i + 1]; //green
        pixel[2] = image.getPixels()[i + 2]; //blue

        //check if tracked color matches pixel color
        //m_trackedColor is a float color [0.0, 1.0] so we need to convert back to a byte color [0,255]
        if (    ((abs(pixel[0] - m_trackedColor[0] * 255.0f) < m_threshold) &&
                (abs(pixel[1] - m_trackedColor[1] * 255.0f) < m_threshold) &&
                (abs(pixel[2] - m_trackedColor[2] * 255.0f) < m_threshold)) ||
            
                ((m_appMode == CVC::APP_MODE::APP_GAME) &&
                (abs(pixel[0] - m_trackedColor2[0] * 255.0f) < m_threshold) &&
                (abs(pixel[1] - m_trackedColor2[1] * 255.0f) < m_threshold) &&
                (abs(pixel[2] - m_trackedColor2[2] * 255.0f) < m_threshold))) 
        {
            //COLOR THE SAME _ YAAYYYYYYY!!
            m_grayscaleDiffImage.getPixels()[i / numChannelsPerPixel] = 255;
        }
        else 
        {
            //NOT THE SAME - BOOO
            m_grayscaleDiffImage.getPixels()[i / numChannelsPerPixel] = 0;
        }
    }

    //update cv image
    m_grayscaleDiffImage.flagImageChanged(); //tells the compy that the image has changed on the CPU RAM and should be moved to GPU RAM
    
    //find contours/blobs
    m_contourFinder.findContours(m_grayscaleDiffImage, m_minArea, m_maxArea, m_numContoursConsidered, false, true);
}