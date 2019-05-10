R"zzz(
#version 410 core
out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2DArray history;
uniform sampler2DArray world;
uniform mat4 view[16];
uniform mat4 projection;
uniform float aspect;

float distro[8] = float[](0.891,0.707,0.561,0.445,0.353,0.280,0.222,0.177);

vec4 toTextureSpace(vec4 r){
	r.x = (r.x + 1.0) / 2.0;
	//Aspect ration correction
	r.y = ((r.y + 1.0) / 2.0) * aspect;
	r.z = (r.z + 1.0) / 2.0;
	return r;
}

void main() {
	fragment_color = vec4(texture(history, vec3(tex_coord, 0)).xyz, 1.0);
	vec4 color =  vec4(0.0,0.0,0.0,0.0);
	for (int i = 0; i < 8; i++) {
		vec4 pos = projection * view[i] * texture(world, vec3(tex_coord, i));
		pos = toTextureSpace(pos / pos.w);
		if(!(pos.x < 0.0 || pos.x > 1.0 || pos.y > 1.0 || pos.y < 0.0)){
			color += texture(history, vec3(tex_coord, i)) * distro[i];
		}
		else{
			color += texture(history, vec3(pos.x,pos.y, 0)) * distro[i];
		}
	}
	fragment_color = color / color.w;
}
)zzz"
