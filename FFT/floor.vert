R"zzz(
#version 410 core
uniform vec4 light_position;
uniform vec3 camera_position;
in vec4 vertex_position;
in vec4 normal;
out vec4 vs_light_direction;
out vec4 vs_normal;
out vec2 vs_uv;
out vec4 vs_camera_direction;
void main() {
	gl_Position = vertex_position;
	vs_light_direction = light_position - gl_Position;
	vs_camera_direction = gl_Position - vec4(camera_position, 1.0);
	vs_normal = normal;
}
)zzz"
