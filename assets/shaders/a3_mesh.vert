#version 430 core

layout(location = 0) in vec3 vertex_position;
//layout(location = 1) in vec3 vertex_normals;
//layout(location = 2) in vec3 vertex_tcoords;

void main()
{
    vec4 pos = vec4(vertex_position, 1.0);
    gl_Position = pos;
}
