#version 330

#define pi 3.14159265358979323846

uniform vec2 resolution_window;

in vec3 fs_normal;
in vec2 fs_uv;
out vec4 frag_color;

void main () {

   vec2 px = gl_FragCoord.xy / resolution_window;

   frag_color = vec4(vec3(fs_uv, 0.0) + 0.01 * fs_normal, 1.0);
}

