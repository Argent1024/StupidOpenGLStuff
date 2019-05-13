# version 330 core

in vec3 normal;
in vec2 texcor;

out vec4 color;

uniform sampler2D texture1;

void main() {
    //color = vec4(normal, 1.f);
    //color = vec4(1.f, 0.f, 0.f, 1.f);
    color = abs(dot(normal, vec3(0.f, 0.f, 1.f))) * texture(texture1, texcor);
}

