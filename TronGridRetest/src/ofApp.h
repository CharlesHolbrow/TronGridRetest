#pragma once

#include "ofMain.h"
#include "stepper.h"
#include "TronGrid.h"
#include "LerpNode.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        ofEasyCam cam;
        ofShader shaderOne;
        ofShader shaderTwo;
        ofShader shaderThree;
        ofFbo fbo;
        ofNode testNode;
        LerpNode lerpCam;

        double previousMicroseconds = 0;
        Stepper stepper;
        TronGrid tg;

        ofNode tn2; // test node 2

        ofxOscReceiver receiver;
        float trigger0;

    private:
        ofFbo::Settings fboSettings;
};
