#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

out vec3 fColor;


void main() {

    fColor = vec3(0.0, 0.1, 0.0);

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    EndPrimitive();
}