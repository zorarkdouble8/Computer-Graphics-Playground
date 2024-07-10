#pragma once
#include "glm/glm.hpp"
#include "vector"

float vertexs[] = {
    //positions          //colors          
    //Top
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, //BL
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, //BR
    -0.5f, 0.5f, -0.5f,  1.0f, 1.0f, 1.0f, //TL
    0.5f, 0.5f, -0.5f,   1.0f, 1.0f, 1.0f, //TR

    //Bottom (starting at 4)
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //BL
    0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //BR
    -0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //TL
    0.5f, 0.5f, 0.5f,   1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //TR
};

unsigned int indices[] = {
    //top plane
    0, 1, 2,
    3, 2, 1,

    //bottom plane
    4, 5, 6,
    7, 6, 5,

    //z plane (it's normal to)
    4, 1, 5,
    4, 1, 0,

    //-z plane
    6, 7, 3,
    6, 2, 3,

    //x plane
    5, 3, 1,
    5, 3, 7,

    //-x plane
    4, 2, 0,
    4, 2, 6,

};

std::vector<glm::vec3> shapePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(3.0f, 0.0f, 0.0f),
};