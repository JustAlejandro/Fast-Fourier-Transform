R"zzz(
#version 410 core
out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2DArray history;
uniform sampler2DArray world;
uniform mat4 view[16];
uniform mat4 projection;
uniform float aspect;

float distro[16] = float[](0.891,0.794,0.707,0.629,0.561,0.5,0.445,0.397,0.353,0.314,0.280,0.25,0.222,0.198,0.177,0.140);

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
	if(texture(world, vec3(tex_coord, 0)) == vec4(0.0,0.0,0.0,1.0)) return;
	for(int i = 0; i < 16; i++){
		vec4 pos = projection * view[i] * texture(world, vec3(tex_coord, 0));
		pos = toTextureSpace(pos / pos.w);
		if(!(pos.x < 0.0 || pos.x > 1.0 || pos.y > 1.0 || pos.y < 0.0 || 
				length(texture(world, vec3(pos.x,pos.y, i)) - texture(world, vec3(pos.x, pos.y, 0))) > 0.1)){
			color += texture(history, vec3(pos.x,pos.y, i)) * (distro[i]+1.0);
		}
		else{
			color += texture(history, vec3(tex_coord, 0)) * (distro[i]+1.0);
		}
	}
	fragment_color = color / color.w;
	
}
)zzz"
