#include "ofApp.h"

void ofApp::setup() 
{
	ofSetVerticalSync(true);
    ofSetWindowShape( 1280, 900 );

    // jump to a specific time in our song (good for testing)
    // m_audioAnalyser.setPositionMS( 30 * 1000 ); // 30 seconds in (30000 ms)
	// note that this audioAnalyzer uses a modified version of ofFmodSOundPlayer so though it has the same+ functionality it should not be mixed with ofSound / ofFmodSoundPlayer

    // Set up the audio analyser:
    //     int _numBinsPerAverage: size in bins per average number from the full 513 bin spectrum
    //
    //     As an example, if _numBinsPerAverage is set to 10, every 10 bins from the full 513 bin spectrum
    //     will be averaged. You would have 513/10 averages in your set of linear averages.
    //     This would be 52 bins (51 + 1 bins as 10 does not go into 513 evenly).
    //     The reason for doing this is to get a smaller set of frequency ranges to work with. In the full 513
    //     bin spectrum, the each bin is 22,500hz/513 wide -> 43.8 Hz.
    //     If you average them into groups of 10, each bin is 438 Hz wide, which can be easier to work with

    // IMPORTANT TIP: Do not use the 20 that I have here, find a value that works well for your chosen song.
    // You want the lowest value that gives you the ability to clearly isolate the features of your song that
    // are interested in
    m_audioAnalyser.init("lacrimosa.mp3", 40 );
	m_audioAnalyser.setLoop(true);
	m_audioAnalyser.play();
}

void ofApp::update() 
{
    m_audioAnalyser.update();
}


void ofApp::draw() 
{
    ofBackground( ofColor::black );
	ofSetColor(255);

    // Volume Level
    ofSetColor( ofColor::white );
	ofDrawBitmapString( "Volume Level", 140,  50 );

    ofDrawCircle( 100, 100, m_audioAnalyser.getLeftLevel()  * 100.0f );
    ofDrawCircle( 200, 100, m_audioAnalyser.getRightLevel() * 100.0f );
    ofDrawCircle( 300, 100, m_audioAnalyser.getMixLevel()   * 100.0f );

	ofDrawBitmapString( "Left",  80,  170 );
	ofDrawBitmapString( "Right", 180, 170 );
	ofDrawBitmapString( "Mix",   290, 170 );


    // Frequency / FFT information
    m_audioAnalyser.drawWaveform( 40, 300, 1200, 90 );  
    m_audioAnalyser.drawSpectrum( 40, 460, 1200, 128 );

    m_audioAnalyser.drawLinearAverages( 40, 650, 1200, 128 );


    // Get the decibel levels for the frequency bins we are interested in
    float drums = m_audioAnalyser.getLinearAverage( 0 ); 

    // you can add multiple bins together if you notice that two have activity when there's something you would like to track
    float lowV = m_audioAnalyser.getLinearAverage( 3 );
    float highV = m_audioAnalyser.getLinearAverage( 7 );
    float violin = m_audioAnalyser.getLinearAverage( 2 );

    // Advanced: can also get a custom average for a frequency range if you know the frequencies (can get them from mousing over the full spectrum)
    // float customAverage = m_audioAnalyser.getAvgForFreqRange( 128.0f, 300.0f );

    // Draw circles to indicate activity in the frequency ranges we are interested in
    // Must be remapped using the ranges of activity that we want
    m_circle1Brightness = ofMap( drums, 5.0f, 200.0f, 0.0f, 1.0f, true );
    m_circle2Brightness = ofMap( lowV, 0.0f, 100.0f, 0.0f, 1.0f, true );
    m_circle3Brightness = ofMap( highV, 0.0f, 100.0f, 0.0f, 1.0f, true );
    m_circle4Brightness = ofMap( violin, 5.0f, 50.0f, 0.0f, 1.0f, true );

    // Draw circles to indicate activity in the frequency ranges we are interested in
    ofSetColor( ofFloatColor( m_circle1Brightness, 0.0f, 0.0f ) );
    ofDrawCircle( 500, 100, 50 );

    ofSetColor( ofFloatColor( 0.0f, m_circle2Brightness, 0.0f ) );
    ofDrawCircle( 650, 100, 50 );

    ofSetColor( ofFloatColor( 0.0f, 0.0f, m_circle3Brightness ) );
    ofDrawCircle( 800, 100, 50 );

    ofSetColor( ofFloatColor( m_circle4Brightness, m_circle4Brightness, m_circle4Brightness ) );
    ofDrawCircle( 950, 100, 50 );

    ofSetColor( ofColor::white );
    ofDrawBitmapString( "Drum and Brass", 445, 200 );
    ofDrawBitmapString( "Low Vocals", 610, 200 );
    ofDrawBitmapString( "High Vocals", 760, 200 );
    ofDrawBitmapString( "Strings", 925, 200 );

    // song time in seconds. Can use m_soundPlayer.setPositionMS( time ) to jump to a specific time
    float songTimeSeconds = m_audioAnalyser.getPositionMS() / 1000.0f;
	ofDrawBitmapString( "Song time: " + ofToString( songTimeSeconds ), 40, 250 );
}

void ofApp::keyPressed(int key) {
	//hmm wonder what kind of useful shortcuts we could add here to go through song more easily ... look at methods of AudioAnalyser ...
}
