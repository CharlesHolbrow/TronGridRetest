#include "TronGrid.h"



TronGrid::TronGrid()
{
    mPlane.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    mPlane.enableIndices();
    mPlane.disableNormals();
    mPlane.disableColors();
    mGrid.setMode(OF_PRIMITIVE_LINES);
    mGrid.enableIndices();
    resize(10, 30);

    // Slightly raise the grid above the plane to reduce artifacts
    mGridNode.setParent(node);
    mGridNode.move(0, 0.003, 0);
}


TronGrid::~TronGrid()
{
}

void TronGrid::resize(int size, float step)
{
    if (size == mSize && step == mStep) return;
    mGrid.clear();
    mPlane.clear();

    mSize = size;
    mStep = step;
    mGrid.enableColors();
    for (int x = 0; x <= mSize; x++) {
        for (int z = 0; z <= mSize; z++) {
            glm::vec3 v = glm::vec3(x * mStep, 0, z * mStep);
            //if (x == 3) v.y = 1;
            //if (z == 4) v.y += 1;
            mPlane.addVertex(v);
            mGrid.addVertex(v);
            mGrid.addColor(c2);
        }
    }

    // Add lines to the grid
    for (int x = 0; x <= mSize; x++) {
        for (int z = 0; z <= mSize; z++) {
            int q1 = x * (mSize + 1) + z;
            int q2 = q1 + mSize + 1;
            int q3 = q1 + 1;

            if (x != mSize) {
                mGrid.addIndex(q1);
                mGrid.addIndex(q2);
            }
            if (z != mSize) {
                mGrid.addIndex(q1);
                mGrid.addIndex(q3);
            }
        }
    }

    // Add faces to the plane
    for (int x = 0; x < mSize; x++) {
        if (x % 2) {
            for (int z = mSize; z > 0; z--) {
                int q1 = (x + 1) * (mSize + 1) + z;
                int q2 = q1 - 1;
                int q3 = x * (mSize + 1) + z;
                int q4 = q3 - 1;
                
                mPlane.addIndex(q1);
                mPlane.addIndex(q2);
                mPlane.addIndex(q3);
                mPlane.addIndex(q4);
            }
            int q = (mSize + 1) * (x + 1);
            mPlane.addIndex(q);
        }
        else {
            for (int z = 0; z < mSize; z++) {
                int q1 = x * (mSize + 1) + z;
                int q2 = q1 + (mSize + 1);
                int q3 = q2 + 1;
                int q4 = q1;
                mPlane.addIndex(q1);
                mPlane.addIndex(q2);
                mPlane.addIndex(q3);
                mPlane.addIndex(q4);
            }
            int q5 = (mSize + 1) * x + mSize;
            int q6 = (mSize + 1) * (x + 1) + (mSize);
            mPlane.addIndex(q5);
            mPlane.addIndex(q6);
        }
    }
}

void TronGrid::setColor(ofFloatColor c) {
    int i = 0;
    glm::vec3 p0(0, 0, 0);

    // Add lines to the grid
    for (int x = 0; x <= mSize; x++) {
        for (int z = 0; z <= mSize; z++) {
            glm::vec3 p1(x, 0, z);
            glm::vec3 dist = p0 - p1;
            float l = glm::length(dist);
            float v = ofMap(l, 0, 8, 1, 0, true);
            //float a = 
            mGrid.setColor(i, ofFloatColor(1, v, 0, 1./32));
            i++;
        }
    }
}

void TronGrid::draw()
{
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    drawPlane();               // draw the plane, no colors, no alpha
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // draw the grid, but just the colors (not the depth buffer)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Flend func: Additive
    drawGrid();
    ofEnableAlphaBlending();           // Blend func: Restore
}

void TronGrid::drawPlane()
{
    node.transformGL();        // Transform: PUSH
    ofSetColor(c1);
    mPlane.drawFaces();
    node.restoreTransformGL(); // Transform: POP
}

void TronGrid::drawGrid()
{
    // Our depth mask comes from the plane, not the grid. We want want to obey
    // the depth buffer, but do not want to write to it. For API spec, see:
    // https://www.khronos.org/opengl/wiki/GLAPI/glDepthMask
    glDepthMask(GL_FALSE);          // Don't write to the z-buffer.
    mGridNode.transformGL();        // Transform: PUSH
    //ofSetColor(c2);
    mGrid.draw();
    mGridNode.restoreTransformGL(); // Transform: POP
    glDepthMask(GL_TRUE);           // re-enable depth mask
}
