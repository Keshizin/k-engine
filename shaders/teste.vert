#version 450 core

// per-vertex attributes
layout (location = 0) in vec4 vPosition;

// per-instanced attributes
layout (location = 2) in vec4 color;
layout (location = 3) in mat4 model_view_matrix;

uniform mat4 projection_matrix;

out vec4 vs_fs_color;

void main()
{
    vs_fs_color = color;
    gl_Position = projection_matrix * (model_matrix * vPosition);
}
