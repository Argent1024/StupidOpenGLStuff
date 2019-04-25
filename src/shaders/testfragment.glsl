# version 330 core

in vec3 viewdir;
in vec3 normal;
in vec2 texcor;

out vec4 color;

uniform sampler2D texture1;

void main() {
    color = abs(dot(normal, vec3(0.f, 0.f, 1.f))) * texture(texture1, texcor);
}

