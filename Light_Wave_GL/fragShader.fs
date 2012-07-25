uniform sampler2D ShadowMap;
varying vec3 normal;
varying vec3 vertex_to_light_vector;
varying vec3 light_to_vertex_vector;
varying vec3 lightDir;

varying vec4 ShadowCoord;

void main( ){
	
	vec4 color = vec4(0.0,0.0,0.0,1.0);

	// Scaling The Input Vector To Length 1
	vec3 normalized_normal = normalize(normal);
	vec3 normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);
	vec3 normalized_light_to_vertex_vector = normalize(light_to_vertex_vector);

	// Reflection term of object
	float DiffuseTermObj = clamp(dot(normalized_normal, normalized_vertex_to_light_vector), 0.0, 1.0);
	
	//Reflection term of directional light
	float DiffuseTermLight = abs(dot(lightDir, normalized_light_to_vertex_vector));

	// Calculating The Final Color
	//color += gl_Color*DiffuseTermLight*DiffuseTermObj;
	//color += gl_Color*DiffuseTermLight;
	color = gl_Color * DiffuseTermLight * DiffuseTermLight;
	//gl_FragColor = color;

	vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;
	
	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0001;
	
	float distanceFromLight = texture2D(ShadowMap,shadowCoordinateWdivide.st).z;
	
	float shadow = 1.0;
 	if (ShadowCoord.w >= 0.0)
 		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
  	
  	gl_FragColor =	 shadow * color;  
}
