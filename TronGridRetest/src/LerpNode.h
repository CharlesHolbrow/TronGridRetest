#pragma once

#include "ofMain.h"

class LerpNode : public ofNode
{
public:
    LerpNode();
    ~LerpNode();
    ofNode current;
    ofNode next;
    void lerp(float amount);
    void setNext(ofNode node);
    void setNext(glm::vec3 pos, glm::quat orientation);
    void setNext(glm::vec3 pos, ofNode lookAt);
    void setNext(glm::vec3 pos, glm::vec3 lookAt);
};

