#version 330

#define pi 3.14159265358979323846

uniform sampler2D texture_sampler;
uniform vec3 light_pos;
uniform vec2 resolution_window;

in vec3 fs_world_pos;
in vec3 fs_normal;
in vec2 fs_uv;
out vec4 frag_color;

void main () {
   vec2 px = gl_FragCoord.xy / resolution_window;

   vec3 n = normalize(fs_normal);
   vec3 l = normalize(light_pos - fs_world_pos);

   float dot_nl = max(0.0, dot(n, l));

   vec4 color = texture(texture_sampler, fs_uv);

   frag_color = vec4(color.rgb * dot_nl, 1.0);
}

