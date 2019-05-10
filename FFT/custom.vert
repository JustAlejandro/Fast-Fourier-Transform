R"zzz(
#version 410 core
uniform vec3 camera_position;
in vec4 vertex_position;
in vec3 normal;
out vec4 vs_normal;
out vec2 vs_uv;
out vec4 vs_camera_direction;
void main() {
	gl_Position = vertex_position;
	vs_camera_direction = gl_Position - vec4(camera_position, 1.0);
	vs_normal = vec4(normal,0.0);
}
)zzz"
