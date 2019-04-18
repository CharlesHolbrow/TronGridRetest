#pragma once
#include "ofMain.h"

class TronGrid
{
public:
    TronGrid();
    ~TronGrid();
    void draw();
    void drawPlane();
    void drawGrid();
    void resize(int size, float step);
    ofNode node;
    ofColor c1 = ofColor(30, 30, 30);
    ofFloatColor c2 = ofFloatColor(.031, .655, .949, 1./64);
    //ofFloatColor c2 = ofFloatColor(1, 0, 0, 0.7);
private:
    int mSize = 0;
    float mStep = 0;
    ofNode mGridNode;
    ofVboMesh mPlane;
    ofVboMesh mGrid;
};

