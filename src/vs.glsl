#version 330

uniform mat4 mvp;

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 uv;

out mat3 tbn;
out vec3 fs_world_pos;
out vec2 fs_uv;

void main () {
   fs_world_pos = position;
   fs_uv = uv;
   
   vec3 binormal = normalize(cross(normal, tangent));

   tbn = transpose(mat3(tangent, binormal, normal));

   gl_Position = mvp * vec4(position, 1.0);
} 

