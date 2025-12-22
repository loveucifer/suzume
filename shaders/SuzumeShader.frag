#version 450

//layout(location = 2) in vec3 fragColor;

layout(location = 0) out vec4 OutColor;

layout(push_constant) uniform Push {
  vec2 offset;
  vec3 color;
} push;

void main() {
  OutColor = vec4(push.color, 1.0);
}
