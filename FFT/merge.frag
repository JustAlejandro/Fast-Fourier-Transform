R"zzz(#version 410 core
out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2D sampler;
uniform sampler2D SSR;
uniform sampler2D SSAO;
void main() {
	fragment_color = vec4((texture(sampler, tex_coord).xyz + texture(SSR, tex_coord).xyz) * texture(SSAO, tex_coord).x, 1.0);
}
)zzz"
