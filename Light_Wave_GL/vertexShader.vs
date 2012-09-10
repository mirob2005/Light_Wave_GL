uniform sampler1D vplPosTex;
uniform sampler1D vplNorTex;

varying vec4 ShadowCoord;
varying vec4 color;

//varying vec4 debugOutput;

void main( ){
    color = vec4(0.0,0.0,0.0,1.0);

	gl_FrontColor = gl_Color;
	gl_BackColor = gl_Color;
	
	// Transforming The Vertex
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	// Transforming The Normal To ModelView-Space
	//Rotate camera cause the normals to change
	vec3 normal = gl_NormalMatrix * gl_Normal;
	
	// Transforming The Vertex Position To ModelView-Space
	vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;
	
/////////////////////////////////////////////////////////////////////////////////////////	
	//Primary Lighting Calculations:
	
	//Access Primary Light Position, Normal from Texture 5
	//Use this to extract rows from the matrix (or column as it appears in .cpp):
	//(1.0,0.0,0.0,0.0) = first row
	//(0.0,1.0,0.0,0.0) = 2nd row
	//(0.0,0.0,1.0,0.0) = 3rd row
	//(0.0,0.0,0.0,1.0) = last row
	vec4 extractRow1 = vec4(1.0,0.0,0.0,0.0);
	vec4 extractRow2 = vec4(0.0,1.0,0.0,0.0);
	vec4 extractRow3 = vec4(0.0,0.0,1.0,0.0);
	vec4 extractRow4 = vec4(0.0,0.0,0.0,1.0);
	
	vec4 masterLightPosition = extractRow1*gl_TextureMatrix[5];
	vec4 masterLightNormal = extractRow2*gl_TextureMatrix[5];
	vec4 lightProperties = extractRow3*gl_TextureMatrix[5];
		
	//Light Direction/Normal
	vec3 lightDir = normalize(vec3(masterLightNormal));
	
	// Calculating The Vector From The Vertex Position To The Light Position and vice versa
	vec3 vertex_to_light_vector = vec3(masterLightPosition - vertex_in_modelview_space);
	vec3 light_to_vertex_vector = vec3(vertex_in_modelview_space - masterLightPosition);	
	
    // Normalizing Vectors
	vec3 normalized_normal = normalize(normal);	
	vec3 normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);
	vec3 normalized_light_to_vertex_vector = normalize(light_to_vertex_vector);	
  	
  	// Reflection term of object
	float DiffuseTermObj = clamp(dot(normalized_normal, normalized_vertex_to_light_vector), 0.0, 1.0);
	
	//Reflection term of directional light
	float DiffuseTermLight = clamp(dot(lightDir, normalized_light_to_vertex_vector),0.0,1.0);

	// Calculating The Final Color
	color += gl_Color*DiffuseTermLight*DiffuseTermObj;	
	
	
/////////////////////////////////////////////////////////////////	




	//float maxDistance = 4.0;
    //Access the vpl___Tex using this:
    //texture1D(vpl___Tex,'coord').xyz; gets the 3 coords
    //vec3 vplPosition = texture1D(vplPosTex,0).xyz;
    //vec3 vplNormal = texture1D(vplNorTex,0).xyz;
    
    //vplPosition = (vplPosition-0.5)*(maxDistance*2);
    //vplNormal= (vplNormal-0.5)*(maxDistance*2);
    
    //vec3 normalized_vplNormal = normalize(vplNormal);
   
		
	


	ShadowCoord= gl_TextureMatrix[7] * gl_Vertex;
}