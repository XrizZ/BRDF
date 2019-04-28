uniform vec3 test;
uniform vec3 brdf_vecR;
uniform vec3 brdf_vecG;
uniform vec3 brdf_vecB;

uniform float cosPhi;

//work in progress! NOT the final version of the shader..

void main()
{
	//ambient light	
	vec3 amb;
	amb[0] = 0.5;
	amb[1] = 0.2;
	amb[2] = 0.2;

	// x_i = p[0]*cos(phi) + p[1]*(cos(phi)^p[2])
	float brdfR = amb[0] + brdf_vecR[0] * cosPhi + brdf_vecR[1] * (pow(cosPhi, brdf_vecR[2]));
	float brdfG = amb[1] + brdf_vecG[0] * cosPhi + brdf_vecG[1] * (pow(cosPhi, brdf_vecG[2]));
	float brdfB = amb[2] + brdf_vecB[0] * cosPhi + brdf_vecB[1] * (pow(cosPhi, brdf_vecB[2]));

	gl_FragColor = vec4(abs(cosPhi), brdfG, brdfB, 1.0);
}