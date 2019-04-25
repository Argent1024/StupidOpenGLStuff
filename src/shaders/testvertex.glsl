# version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;

uniform mat3 rotation; 
uniform vec3 transition;

out vec3 normal;
out vec2 texcor;

void main() {
    gl_Position = vec4(rotation * v_position + transition, 1);

    normal = normalize(rotation * v_normal);
    texcor = v_texture;
}