R"zzz(#version 410 core
out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2D sampler;
uniform sampler2D depSten;
uniform sampler2D specular;
uniform sampler2D ss_Normals;
//Something from the WIP shaders that turned out really cool so thought I'd leave it here
vec3 genRay(float d){
	return reflect(vec3(tex_coord.x - 0.5 * 2, tex_coord.y - 0.5 * 2, d), texture(ss_Normals,tex_coord).xyz);
}

void main() {
	//Don't run reflections if specular not specified
	if(texture(specular,tex_coord).x <= 0.01){
		fragment_color = vec4(texture(sampler, tex_coord).xyz, 1.0);
		return;
	}
	//Reflection running will be put in here.
	fragment_color = vec4(1.0,1.0,1.0,1.0);
	fragment_color = vec4(texture(depSten, tex_coord).xyz, 1.0);

	fragment_color = vec4(texture(ss_Normals, tex_coord).xyz, 1.0);
	vec3 ray = genRay(texture(depSten, tex_coord).x);
	fragment_color = vec4(ray, 1.0);
}
)zzz"
