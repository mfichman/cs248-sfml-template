varying vec3 eye;
varying vec2 texcoord;
varying vec3 normal;

void main() {

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

	eye = vec3(gl_ModelViewMatrix * gl_Vertex);
	texcoord = gl_MultiTexCoord0.st;
	normal = gl_NormalMatrix * gl_Normal;
}
