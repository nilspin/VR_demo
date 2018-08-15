//Debug shader to check whether plane triangles are being generated properly
#version 430

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

//in vec3 v_position[];


uniform mat4 MVP;


void main()
{
	vec4 pos[3];

	//transform vertices to clip space
	pos[0] = MVP * gl_in[0].gl_Position;
	pos[1] = MVP * gl_in[1].gl_Position;
	pos[2] = MVP * gl_in[2].gl_Position;


	gl_Position = MVP*pos[0];
	EmitVertex();

	gl_Position = MVP*pos[1];
	EmitVertex();

	gl_Position = MVP*pos[1];
	EmitVertex();

	gl_Position = MVP*pos[2];
	EmitVertex();

	gl_Position = MVP*pos[2];
	EmitVertex();

	gl_Position = MVP*pos[0];
	EmitVertex();

    EndPrimitive();
}

