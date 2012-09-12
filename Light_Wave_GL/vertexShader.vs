uniform sampler1D vplPosTex;
uniform sampler1D vplNorTex;

varying vec4 ShadowCoord;
varying vec4 direct_color;
varying vec4 indirect_color;

//varying vec4 debugOutput;

void main( ){
    direct_color = vec4(0.0,0.0,0.0,1.0);
    indirect_color = vec4(0.0,0.0,0.0,1.0);

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

	// Calculating The Color from the primary light
	direct_color += gl_Color*DiffuseTermLight*DiffuseTermObj;	
	
	
/////////////////////////////////////////////////////////////////	

    int numLights = lightProperties.z;
	float maxDistance = 8.0;
	
	for(int i=0; i< numLights; i++)
	{
	   //Access the vpl___Tex using this:
	   //texture1D(vpl___Tex,'coord').xyz; gets the 3 coords
	   vec3 vplPosition = texture1D(vplPosTex,i).xyz;
	   vec3 vplNormal = texture1D(vplNorTex,i).xyz;
	   float vplAttenuation = texture1D(vplNorTex,i).w;
       
	   vplPosition = (vplPosition-0.5)*maxDistance*4;
	   vplNormal= (vplNormal-0.5)*maxDistance*4;
	   vplAttenuation= (vplAttenuation-0.5)*maxDistance*4;
       
	   vec3 normalized_vplNormal = normalize(vplNormal);


	   // Calculating The Vector From The Vertex Position To The Light Position and vice versa
	   vertex_to_light_vector = vec3(vplPosition - vertex_in_modelview_space);
	   light_to_vertex_vector = vec3(vertex_in_modelview_space - vplPosition);	
   	
	   // Normalizing Vectors
	   normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);
	   normalized_light_to_vertex_vector = normalize(light_to_vertex_vector);
	   
	   //Distance Vector
	   float distance_vertex_to_light = length(vertex_to_light_vector);
     	
  	   // Reflection term of object
	   DiffuseTermObj = clamp(dot(normalized_normal, normalized_vertex_to_light_vector), 0.0, 1.0);
   	
	   //Reflection term of directional light
	   DiffuseTermLight = dot(normalized_vplNormal, normalized_light_to_vertex_vector);
	   
	   //Clamping between -0.5 and 1 allows for an angle of of 240 degeres (120*2) instead of 180 degrees (90*2)
	   //Normalize larger angle allowances
	   //DiffuseTermLight = (DiffuseTermLight + 0.5)/1.5;
	   DiffuseTermLight = clamp(DiffuseTermLight, 0.0, 1.0);

	   // Calculating The VPL Contribution
	   indirect_color += gl_Color*DiffuseTermObj*DiffuseTermLight*(1-vplAttenuation);///distance_vertex_to_light;
   }	

   //indirect_color = vec4(indirect_color.r/numLights,indirect_color.g/numLights,indirect_color.b/numLights,1);
	//indirect_color = indirect_color/numLights;

	ShadowCoord= gl_TextureMatrix[7] * gl_Vertex;
}