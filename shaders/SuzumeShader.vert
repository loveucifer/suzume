#version 450

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5), // sorry hardcode lol , need to pass if with vertex buffer later but im learning so :3
    vec2(-0.5, 0.5)
  );
void main() {
  gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}
