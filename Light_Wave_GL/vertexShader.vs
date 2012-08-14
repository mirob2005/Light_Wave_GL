varying vec3 normal;
varying vec3 vertex_to_light_vector;
varying vec3 light_to_vertex_vector;
varying vec3 lightDir;

varying vec4 ShadowCoord;

varying vec4 debugOutput;

void main( ){
	gl_FrontColor = gl_Color;
	gl_BackColor = gl_Color;
	// Transforming The Vertex
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	// Transforming The Normal To ModelView-Space
	normal = gl_NormalMatrix * gl_Normal; 
	
	//Light Direction
	lightDir = normalize(vec3(gl_LightSource[0].spotDirection));

	// Transforming The Vertex Position To ModelView-Space
	vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;

	// Calculating The Vector From The Vertex Position To The Light Position
	vertex_to_light_vector = vec3(gl_LightSource[0].position - vertex_in_modelview_space);
	
	light_to_vertex_vector = vec3(vertex_in_modelview_space - gl_LightSource[0].position);
	
	ShadowCoord= gl_TextureMatrix[7] * gl_Vertex;
	
	//NEW CODE
	
	//Use this to extract rows from the matrix (or column as it appears in .cpp):
	//(1.0,0.0,0.0,0.0) = first row
	//(0.0,1.0,0.0,0.0) = 2nd row
	//(0.0,0.0,1.0,0.0) = 3rd row
	//(0.0,0.0,0.0,1.0) = last row
	vec4 extractRow1 = vec4(1.0,0.0,0.0,0.0);
	vec4 extractRow2 = vec4(0.0,1.0,0.0,0.0);
	vec4 extractRow3 = vec4(0.0,0.0,1.0,0.0);
	vec4 extractRow4 = vec4(0.0,0.0,0.0,1.0);
	
	double[6485] light_Array;
	//light_Array[6480] = 0;
	
	//lightCoord = vec4(light_Array[6480],light_Array[6481],light_Array[6482],light_Array[6483]);
	//Texture 5 is light Matrix
	//Texture 6 is color tests
	vec4 masterLightPosition = extractRow1*gl_TextureMatrix[5];
	vec4 masterLightLookAt = extractRow2*gl_TextureMatrix[5];
	vec4 lightProperties = extractRow3*gl_TextureMatrix[5];
	
	int lightsAngle = lightProperties.x;
	int lightsPerRay = lightProperties.y;
	int numLights = lightProperties.z;
	
	debugOutput = vec4(lightsAngle, lightsPerRay, numLights,1);
}
