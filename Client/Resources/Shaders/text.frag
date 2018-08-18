#version 400

uniform sampler2D Text;

in vec3 colorg;
in vec2 uvg;

out vec4 Color;

void main()
{
	Color = vec4(colorg.xyz*texture(Text,uvg).rgb ,1);
}