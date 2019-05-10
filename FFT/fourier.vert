R"zzz(
#version 410 core
uniform vec4 light_position;
uniform vec3 camera_position;
uniform vec3 trans[15];
uniform float buckets[15];
in vec4 vertex_position;
in vec4 normal;
out vec4 vs_normal;
out vec2 vs_uv;
out vec4 vs_camera_direction;
out vec4 vs_local;
void main() {
	vs_local = (vertex_position - vec4(0.5,0.5,0.5,0.0)) * 2.0;
	vec4 pos = vertex_position + vec4(trans[gl_InstanceID], 0.0);
	pos.y = pos.y * buckets[gl_InstanceID];
	gl_Position = pos;
	vs_camera_direction = vec4(camera_position, 1.0) - gl_Position;
	vs_normal = normal;
}
)zzz"
