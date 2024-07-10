#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexColor;

uniform mat4 modelTrans;
uniform mat4 worldTrans;
uniform mat4 viewTrans;
uniform mat4 projTrans;

out vec4 fragColor;

void main()
{
    gl_Position = projTrans * viewTrans * worldTrans * modelTrans * vec4(vertexPos.xyz, 1.0);
    fragColor = vec4(vertexColor.xyz, 1.0);
}