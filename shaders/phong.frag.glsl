
#define NUMBER_OF_LIGHTS 3

uniform sampler2D texture;
varying vec3 eye;
varying vec2 texcoord;
varying vec3 normal;

void main() {
	gl_FragColor = vec4(0);
	vec3 N = normalize(normal);

	// Calculate the fragment color by adding all the lights
	for (int i = 0; i < NUMBER_OF_LIGHTS; i++) {
		// Normalize the light vector and view vector
		vec3 V = normalize(eye);
		vec3 L = normalize(gl_LightSource[i].position.xyz);
		
		// Calculate the diffuse coefficient
		float Kd = max(0.0, dot(L, N));
	
		// Calculate the specular coefficient
		R = reflect(V, N);
		float Ks = pow(max(0.0, dot(L, R), gl_FrontMaterial.shininess);
		
		gl_FragColor += Kd * gl_LightSource[i].diffuse * gl_FrontMaterial.diffuse;
		gl_FragColor += Ks * gl_LightSource[i].specular * gl_FrontMaterial.specular;
		gl_FragColor += gl_LightSource[i].ambient * gl_FrontMaterial.ambient;
	}
}