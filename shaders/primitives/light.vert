#version 460 core

out vec2 tex_coords;
out vec3 normal;
out vec3 frag_pos;

layout (location = 0) in vec3 vertex_pos;
layout (location = 1) in vec3 normal_in;
layout (location = 2) in vec2 texture_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vertex_pos, 1.0);

    frag_pos = vec3(model * vec4(vertex_pos, 1.0));
    tex_coords = texture_coords;
    normal = normal_in;
}