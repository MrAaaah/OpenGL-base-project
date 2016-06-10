#version 330

#define pi 3.14159265358979323846

uniform sampler2D texture_sampler;
uniform sampler2D normal_map_sampler;
uniform vec3 light_pos;
uniform vec2 resolution_window;

in mat3 tbn;
in vec3 fs_world_pos;
in vec2 fs_uv;
out vec4 frag_color;

void main () {
   vec2 px = gl_FragCoord.xy / resolution_window;

   vec2 uv = vec2(fs_uv.x, 1.0 - fs_uv.y);

   vec3 n = 2.0 * texture(normal_map_sampler, uv).rgb - vec3(1.0);
   vec3 l = tbn * normalize(light_pos - fs_world_pos);

   float dot_nl = max(0.0, dot(n, l));

   vec4 color = texture(texture_sampler, uv);

   frag_color = vec4(color.rgb * dot_nl, 1.0);
}

