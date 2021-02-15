#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

uniform vec4 u_posDim;
uniform vec4 u_texPosDim;

out vec2 v_texPos;

void main()
{
    v_texPos = vec2(u_texPosDim[0] + position[0] * u_texPosDim[2], u_texPosDim[1] + position[1] * u_texPosDim[3]);
    vec2 p = vec2(u_posDim[0] + position[0] * u_posDim[2], u_posDim[1] + position[1] * u_posDim[3]);
	gl_Position = vec4(p, 0, 1);
};




#shader geometry
#version 330 core


layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 v_texPos[];
out vec2 g_texPos;

void main(){

	gl_Position = gl_in[0].gl_Position;
    g_texPos = v_texPos[0];
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
    g_texPos = v_texPos[1];
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
    g_texPos = v_texPos[2];
	EmitVertex();

}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 g_texPos;

uniform sampler2D u_texture;
uniform vec3 u_textColor;


void main()
{
    color = texture(u_texture, g_texPos);
};
