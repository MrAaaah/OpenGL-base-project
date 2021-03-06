#version 330

#define pi 3.14159265358979323846

uniform sampler2D texture_sampler;
uniform sampler2D normal_map_sampler;
uniform sampler2D height_map_sampler;
uniform vec3 view_pos;
uniform vec3 light_pos;
//uniform vec2 resolution_window;
uniform float height_scale;
uniform float uv_scale;
uniform int num_parallax_layers;
uniform int interpolation;


in mat3 tbn;
in vec3 fs_world_pos;
in vec2 fs_uv;
out vec4 frag_color;

void main () {
   //vec2 px = gl_FragCoord.xy / resolution_window;

   vec3 tan_light_pos = tbn * light_pos;
   vec3 tan_world_pos = tbn * fs_world_pos;
   vec3 tan_view_pos = tbn * view_pos;

   vec3 v = normalize(tan_view_pos - tan_world_pos);

   // parallax mapping
   float layer_depth = 1.0 / num_parallax_layers;
   float current_layer_depth = 0.0;
   vec2 offset = v.xy * height_scale;
   vec2 delta_uv = offset / num_parallax_layers;
    
   vec2 uv = uv_scale * vec2(fs_uv.x, fs_uv.y);
   float depth = 1.0 - texture(height_map_sampler, uv).r;
   while (current_layer_depth < depth)
   {
      uv -= delta_uv;
      depth = 1.0 - texture(height_map_sampler, uv).r;
      current_layer_depth += layer_depth;
   }
   if (interpolation == 1)
   {
      // interpolate uv coords with the previous coords
      // (before the "collision")
      vec2 prev_uv = uv + delta_uv;
      float prev_depth = 1.0 - texture(height_map_sampler, prev_uv).r;
      float offset_after_depth = depth - current_layer_depth;
      float offset_before_depth = prev_depth - current_layer_depth + layer_depth;
      float weight = offset_after_depth / (offset_after_depth - offset_before_depth);
      uv = prev_uv * weight + uv * (1.0 - weight);
   }


   if (uv.x > uv_scale || uv.y > uv_scale || uv.x < 0.0 || uv.y < 0.0)
      discard;

   uv.y = 1.0 - uv.y;


   vec3 n = 2.0 * texture(normal_map_sampler, uv).rgb - vec3(1.0);
   vec3 l = normalize(tan_light_pos - tan_world_pos);

   float dot_nl = max(0.0, dot(n, l));

   vec4 color = texture(texture_sampler, uv);

   frag_color = vec4(color.rgb * dot_nl, 1.0);
}

