R"zzz(#version 410 core
out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2D sampler;
uniform sampler2D depSten;
uniform sampler2D specular;
uniform sampler2D vs_Normals;
uniform sampler2D vs_Ray;

uniform mat4 projection;

vec4 toTextureSpace(vec4 r){
	r.x = (r.x + 1.0) / 2.0;
	//Aspect ration correction
	r.y = ((r.y + 1.0) / 2.0) * (16.0/9.0);
	r.z = (r.z + 1.0) / 2.0;
	return r;
}

//Generates the ray in view space
vec3 genRay(float d){
	vec4 normal = normalize(texture(vs_Normals,tex_coord));
	vec4 ray = -1.0 * normalize(texture(vs_Ray, tex_coord));
	ray.z = -1.0* ray.z;
	vec3 toRet = reflect(normalize(ray), normal).xyz;
	return toRet;
}

//Here's where we'll trace our SS ray
bool traceRay(vec3 ray, vec4 color){
	//Start position of the ray
	vec3 normal = texture(vs_Normals,tex_coord).xyz;

	//Start postion in view Space
	vec3 viewRay = (texture(vs_Ray,tex_coord)).xyz;
	vec4 start = vec4((viewRay).xyz,1.0);

	//Reflected view space Ray
	vec3 vsRefl = reflect(normalize(viewRay.xyz), normalize(normal));

	//End Postition in view Space
	vec4 end = start + vec4(vsRefl * 10000, 1.0);

	//Start Position in texture Space;
	start = (projection * start);
	start = toTextureSpace(start / start.w);

	//End Position in texture Space
	end = (projection * end);
	end = toTextureSpace(end / end.w);

	vec4 tsRefl = vec4(normalize((end - start).xyz),1.0);
	vec4 pos = start + tsRefl * 0.01;

	bool hit = false;
	int step = 0;
	while(!hit && (pos.x) < 1.0 && (pos.y) < 1.0 && pos.z < 1.0 && pos.z > 0.0 && step < 10000){
		step++;
		pos += tsRefl * 0.0001;
		if(abs(pos.z - (texture(depSten, vec2(pos.x, pos.y))).x) < 0.00004){
			hit = true;
			fragment_color = texture(sampler, vec2(pos.x, pos.y));
		}
	}
	//fragment_color = tsRefl;
	//fragment_color = start;
	return true;
}

void main() {
	//Don't run reflections if specular not specified
	if(texture(specular,tex_coord).x <= 0.01){
		fragment_color = vec4(texture(sampler, tex_coord).xyz, 1.0);
		return;
	}
	//Reflection running will be put in here.
	vec3 ray = (vec4(genRay(texture(depSten, tex_coord).x), 1.0)).xyz;
	vec4 rayCol = vec4(1.0);
	if(traceRay(ray,rayCol)){

	}
	else{
		fragment_color = vec4(ray, 1.0);
	}
		//fragment_color = vec4(ray, 1.0);
		//fragment_color = vec4(texture(sampler, tex_coord).xyz, 1.0);
}
)zzz"
