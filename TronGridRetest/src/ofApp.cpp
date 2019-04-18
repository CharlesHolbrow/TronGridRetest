#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    // global of options
    ofBackground(0);
    ofEnableDepthTest();
    ofSetLineWidth(2);
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

    int SIZE = 40;
    float STEP = 1;

    tg.resize(SIZE, STEP);
    tg.node.move(SIZE*STEP/-2., -4, SIZE*STEP/-2.);
    tg.node.setParent(testNode);
	//ofSetFrameRate(90);

    stepper.setStepSize(0.001);
    ofLog() << "maxSamples " << ofFbo::maxSamples();
    lerpCam.setNext(cam);
    //cam.disableMouseInput();
    cam.setFarClip(1000);
    cam.setNearClip(0.01);
    cam.setDistance(5);

    receiver.setup(6969);
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
    tg.c2.r = sin(ofGetElapsedTimef()) * 127 + 127;
    
    // Log slow frames
    double frameDuration = stepper.frameDuration();
    ofSetWindowTitle(ofToString(frameDuration, 3));
    if (frameDuration > 0.020) ofLog() << "Slow Frame: " << frameDuration;

    // cam
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        std::string addr = m.getAddress();
        if (addr == "/controller/0") {
            glm::vec3 pos(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
            glm::quat quat(m.getArgAsFloat(4), m.getArgAsFloat(5), m.getArgAsFloat(6), m.getArgAsFloat(3));
            tn2.setOrientation(quat);
            tn2.setPosition(pos);
            //lerpCam.setNext(pos, quat);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    int steps = 64;
 
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
        // Interpolate the camera
        lerpCam.lerp(float(i + 1) / float(steps));
        cam.setOrientation(lerpCam.getOrientationQuat());
        cam.setPosition(lerpCam.getGlobalPosition());
        cam.begin();

        // rotate the grid
        testNode.rotateDeg(stepper.stepSize * 50 * (sin(ofGetElapsedTimef() * 0.5) + 1), glm::vec3(0, 1, 0));
        
        // Draw the z buffer for each step.  We don't want the plane covering up
        // the grid (as it was drawn on a previous step in this loop)
        glClear(GL_DEPTH_BUFFER_BIT); // Use a different depth buffer on each step

        tg.draw();

        cam.end();
    }
    fbo.end();

    

    //ofClear(0); // clear window
    ofBackgroundGradient(c1, c2);
    ofSetColor(255);
    fbo.draw(0, 0);
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
