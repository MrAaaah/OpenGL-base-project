#version 330

uniform mat4 mvp;

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 fs_world_pos;
out vec3 fs_normal;
out vec2 fs_uv;

void main () {
   fs_world_pos = position;
   fs_normal = normal;
   fs_uv = uv;

   gl_Position = mvp * vec4(position, 1.0);
} 

