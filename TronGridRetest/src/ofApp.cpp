#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    // global of options
    ofBackground(0);
    ofEnableDepthTest();
    ofSetLineWidth(3);
    //ofSetVerticalSync(true);

    // fbo setup
    fboSettings.width = ofGetWidth();
    fboSettings.height = ofGetHeight();
    fboSettings.useDepth = true;
    fboSettings.numSamples = 4;
    fboSettings.internalformat = GL_RGBA16F;
    //fboSettings.internalformat = GL_RGBA;
    fbo.allocate(fboSettings);

    fbo.begin();
    ofClear(255);
    fbo.end();

    //ofEnableSmoothing();

    tg.resize(30, 80);
    tg.node.move(-1200, -15, -1200);
    tg.node.setParent(testNode);
	ofSetFrameRate(60);

    stepper.setStepSize(0.002);
    ofLog() << "maxSamples " << ofFbo::maxSamples();
    lerpCam.setNext(cam);
    //cam.disableMouseInput();
}

//--------------------------------------------------------------
void ofApp::update(){

    uint64_t microseconds = ofGetElapsedTimeMicros();
    uint64_t deltaMicroseconds = microseconds - previousMicroseconds;

    // What time did the frame start?
    double frameStart = static_cast<double>(previousMicroseconds * 0.000001);
    // time in seconds
    double frameEnd = static_cast<double>(microseconds * 0.000001);
    // delta time in seconds
    double frameDelta = static_cast<double>(deltaMicroseconds * 0.000001);
    // can I put this here?
    previousMicroseconds = microseconds;

    // Setup the Stepper for this frame
    stepper.advanceFrame(frameDelta);

    //testNode.rotateDeg(30 * frameDelta, glm::vec3(0, 1, 0));
    //tg.node.move(sin(ofGetElapsedTimef()), 0., 0.);
    tg.c2.b = sin(ofGetElapsedTimef() * .1) * 50 + 150;
    //tg.c2.r = sin(ofGetElapsedTimef()) * 127 + 127;
    
    // Log slow frames
    double frameDuration = stepper.frameDuration();
    ofSetWindowTitle(ofToString(frameDuration, 3));
    if (frameDuration > 0.020) ofLog() << "Slow Frame: " << frameDuration;
}

//--------------------------------------------------------------
void ofApp::draw() {
    int steps = stepper.steps;
    int alpha = 255 / steps;

    // Get ready to interpolate the camera
    lerpCam.setNext(cam.getPosition(), cam.getOrientationQuat());

    fbo.begin();
    ofClear(0);
    ofColor c1 = ofColor::darkBlue;
    ofColor c2 = ofColor::black;
    c1.setBrightness(60);
    ofBackgroundGradient(c1, c2);

    // Draw a bunch of steps to the FBO
    for (int i = 0; i < steps; i++) {
        lerpCam.lerp(float(i + 1) / float(steps));
        cam.setOrientation(lerpCam.getOrientationQuat());
        cam.setPosition(lerpCam.getGlobalPosition());
        cam.begin();
        // rotate the grid
        testNode.rotateDeg(stepper.stepsDuration() * 50. / steps * (sin(ofGetElapsedTimef() * 0.5) + 1), glm::vec3(0, 1, 0));
        
        // Draw the z buffer for each step.  We don't want the plane covering up
        // the grid (as it was drawn on a previous step in this loop)
        glClear(GL_DEPTH_BUFFER_BIT); // Use a different depth buffer on each step
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        tg.drawPlane();               // draw the plane, no colors, no alpha
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        // draw the grid, but just the colors (not the depth buffer)
        ofEnableAlphaBlending();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Flend func: Additive
        tg.drawGrid();
        ofEnableAlphaBlending();           // Blend func: Restore
        cam.end();
    }
    fbo.end();

    //ofClear(0); // clear window
    ofBackgroundGradient(c1, c2);
    ofSetColor(255, alpha);
    fbo.draw(0, 0);
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
    case OF_KEY_RETURN:
        if (ofGetKeyPressed(OF_KEY_ALT)) ofToggleFullscreen();
        break;
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    ofLog() << "Window Size: " << w << "x" << h;
    fboSettings.width = w;
    fboSettings.height = h;
    fbo.allocate(fboSettings);
    fbo.begin();
    ofClear(255);
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
