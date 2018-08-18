#version 400

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

uniform float size;

in vec3 positionv[];
in vec3 colorv[];
out vec3 colorg;
out vec2 uvg;

void main()
{
	colorg = colorv[0];
	float halfAmount = 1/size;
	vec2 pos = vec2((positionv[0].x - halfAmount * 2) / (halfAmount * 2),(positionv[0].y + halfAmount)/halfAmount);
	
	float xuv = (int(positionv[0].z) % 16) / 16.0;
	float yuv = 1 - (((int(positionv[0].z) / 16) + 1) / 16.0);

	uvg = vec2(xuv, yuv + 1.0/16.0);
	gl_Position = vec4(pos.x,pos.y,0,1);
	EmitVertex();
	uvg = vec2(xuv + 1.0/16.0, yuv + 1.0/16.0);
	gl_Position = vec4(pos.x + size,pos.y,0,1);
	EmitVertex();
	uvg = vec2(xuv,yuv);
	gl_Position = vec4(pos.x,pos.y - size,0,1);
	EmitVertex();

    EndPrimitive();
	
	uvg = vec2(xuv + 1.0/16.0, yuv + 1.0/16.0);
	gl_Position = vec4(pos.x + size,pos.y,0,1);
	EmitVertex();
	uvg = vec2(xuv, yuv);
	gl_Position = vec4(pos.x,pos.y - size,0,1);
	EmitVertex();
	uvg = vec2(xuv + 1.0/16.0, yuv);
	gl_Position = vec4(pos.x + size,pos.y - size,0,1);
	EmitVertex();

    EndPrimitive();
}