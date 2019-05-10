R"zzz(
#version 410 core
out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2DArray history;
void main() {
	fragment_color = vec4(texture(history, vec3(tex_coord, 0)).xyz, 1.0);
}
)zzz"
