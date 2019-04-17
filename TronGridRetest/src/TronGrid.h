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
    ofColor c2 = ofColor(8, 167, 242, 180);
private:
    int mSize = 0;
    float mStep = 0;
    ofNode mGridNode;
    ofVboMesh mPlane;
    ofVboMesh mGrid;
};

