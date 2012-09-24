#version 120

uniform sampler1D vplPosTex;
uniform sampler1D vplNorTex;

varying vec3 normalized_normal,normalized_vertex_to_light_vector,lightDir,normalized_light_to_vertex_vector;
varying vec3 normalized_viewing_position_vector;

varying vec4 ShadowCoord;
varying vec4 indirect_color;

void main( ){
    
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
	vec4 cameraPosition = extractRow4*gl_TextureMatrix[5];
		
	//Light Direction/Normal
	lightDir = normalize(vec3(masterLightNormal));
	
	// Calculating The Vector From The Vertex Position To The Light Position and vice versa
	vec3 vertex_to_light_vector = vec3(masterLightPosition - vertex_in_modelview_space);
	vec3 light_to_vertex_vector = vec3(vertex_in_modelview_space - masterLightPosition);
	
	// Calculating The Vector From The Vertex Position To The Camera Position
	vec3 vertex_to_camera_vector = vec3(cameraPosition - vertex_in_modelview_space);
	
    // Normalizing Vectors
	normalized_normal = normalize(normal);	
	normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);
	normalized_light_to_vertex_vector = normalize(light_to_vertex_vector);
	normalized_viewing_position_vector = normalize(vertex_to_camera_vector);
		
	
   /////////////////////////////////////////////////////////////////
	//Indirect Lighting Calculations:

    int numLights = int(lightProperties.z);
	float maxDistance = 4.0;
	
	for(int i=1; i< numLights+1; i++)
	{
	   float texCoord = (float(i)/numLights);

	   vec3 vplPosition = texture1D(vplPosTex,texCoord).rgb;
	   vec3 vplNormal = texture1D(vplNorTex,texCoord).rgb;
	   float vplAttenuation = texture1D(vplNorTex,texCoord).a;
       
	   vplPosition = (vplPosition-0.5)*maxDistance*4.0;
	   vplNormal= (vplNormal-0.5)*maxDistance*4.0;
	   vplAttenuation= (vplAttenuation-0.5)*maxDistance*4.0;
       
	   vec3 normalized_vplNormal = normalize(vplNormal);


	   // Calculating The Vector From The Vertex Position To The Light Position and vice versa
	   vec3 vertex_to_light_vector = vec3(vplPosition - vec3(vertex_in_modelview_space));
	   vec3 light_to_vertex_vector = vec3(vec3(vertex_in_modelview_space) - vplPosition);	
   	
	   // Normalizing Vectors
	   vec3 normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);
	   vec3 normalized_light_to_vertex_vector = normalize(light_to_vertex_vector);
	   
	   //Distance Vector
	   float distance_vertex_to_light = length(vertex_to_light_vector);
     	
  	   // Reflection term of object
	   float DiffuseTermObj = clamp(dot(normalized_normal, normalized_vertex_to_light_vector), 0.0, 1.0);
   	
	   //Reflection term of directional light
	   float DiffuseTermLight = dot(normalized_vplNormal, normalized_light_to_vertex_vector);
	   
	   //Clamping between -0.5 and 1 allows for an angle of of 240 degeres (120*2) instead of 180 degrees (90*2)
	   //Normalize larger angle allowances
	   DiffuseTermLight = (DiffuseTermLight + 0.5)/1.5;
	   DiffuseTermLight = clamp(DiffuseTermLight, 0.0, 1.0);

	   // Calculating The VPL Contribution
	   indirect_color += gl_Color*DiffuseTermObj*DiffuseTermLight*(1-vplAttenuation);///distance_vertex_to_light;
   }	

   indirect_color = vec4(indirect_color.r/numLights,indirect_color.g/numLights,indirect_color.b/numLights,1);
	

	ShadowCoord= gl_TextureMatrix[7] * gl_Vertex;
	
}