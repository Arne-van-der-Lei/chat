#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 colorv;
out vec3 positionv;

void main()
{
	colorv = color;
	positionv = position;
}