#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTextCord;

uniform float time;

out vec2 textCord;
out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPos.x * time, aPos.y * time, aPos.z, 1.0);
    vertexColor = vec4(aCol.xyz, 1.0);
    textCord = aTextCord;
};