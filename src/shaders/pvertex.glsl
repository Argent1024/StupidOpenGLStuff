# version 330 core

layout(location = 0) in vec3 v_position;
// layout(location = 1) in vec3 v_color;

out vec3 color;

void main() {
    gl_Position = vec4(v_position, 1);
    color = vec3(1.f, 0.f, 0.f);
}