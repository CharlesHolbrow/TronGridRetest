#include "LerpNode.h"



LerpNode::LerpNode()
{
}


LerpNode::~LerpNode()
{
}


void LerpNode::lerp(float amount)
{
    glm::quat q1 = current.getOrientationQuat();
    glm::quat q2 = next.getOrientationQuat();
    setOrientation(glm::slerp(q1, q2, amount));
    glm::vec3 v1 = current.getPosition();
    glm::vec3 v2 = next.getPosition();
    setPosition(glm::lerp(v1, v2, amount));
}

void LerpNode::setNext(ofNode node)
{
    current = *this;
    next = node;
}

void LerpNode::setNext(glm::vec3 pos, glm::quat orientation)
{
    current = *this;
    next.setPosition(pos);
    next.setOrientation(orientation);
}

void LerpNode::setNext(glm::vec3 pos, ofNode lookAt)
{
    current = *this;
    next.setPosition(pos);
    next.lookAt(lookAt, glm::vec3(0, 1, 0));
}

void LerpNode::setNext(glm::vec3 pos, glm::vec3 lookAt)
{
    current = *this;
    next.setPosition(pos);
    next.lookAt(lookAt, glm::vec3(0, 1, 0));
}
