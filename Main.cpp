//Include all the Header files
#include <GL/glew.h> 
#include <freeGLUT/glut.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdio.h>
#include <assimp/cimport.h> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;
using namespace glm;
GLfloat vertices1[208998*3];
GLfloat vertices2[208998 * 3];
int verticesNum1;
int verticesNum2;


//Specify the object files to upload
#define MESH_FILE_Bunny "bunny.obj"
#define MESH_FILE_Cactus "cactus.obj"



//Define the size of the windows 
int width = 1024;
int height = 768;
float Clipping = 0.005f;


//Declare all the global variables used for displaying the windows
GLFWwindow* window1;
GLFWwindow* window2;
GLuint model_vao_bunny;
int model_point_count_bunny = 0;
GLuint model_vao_cactus;
int model_point_count_cactus = 0;
GLuint model_vao_displayed;
int model_point_count_displayed;
GLuint VertexShader, FragmentShader, p;
bool cam_moved = false;
int Bunny_model = 0;
int Cactus_model = 0; 
const double PI = 3.14159265;
float lightState = 0.0;



//Introduce the default color values as global
//float red_color = 1.0f;
//float green_color = 0.0f;
//float blue_color = 0.0f;
//float alpha_color = 1.0f;

//Introduce the default color values of the global ambient light as global variables 
float global_red_color = 1.0f;
float global_green_color = 0.0f;
float global_blue_color = 1.0f;
float global_alpha_color = 1.0f;

//Introduce the default color values of the ambient light as global variables 
float ambient_red_color = 0.2f;
float ambient_green_color = 0.0f;
float ambient_blue_color = 0.2f;
float ambient_alpha_color = 1.0f;

//Introduce the default color values of the diffuse light as global variables 
float diffuse_red_color = 0.7f;
float diffuse_green_color = 0.7f;
float diffuse_blue_color = 0.7f;
float diffuse_alpha_color = 1.0f;

//Introduce the default color values of the specular light as global variables 
float specular_red_color = 1.0f;
float specular_green_color = 1.0f;
float specular_blue_color = 1.0f;
float specular_alpha_color = 1.0f;


//Function to load the Bunny file with Assimp 
bool load_mesh1(const char* file_name, GLuint* vao, int* point_count) {

	const aiScene* scene = aiImportFile(file_name, aiProcess_Triangulate);
	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return false;
	}

	const aiMesh* mesh = scene->mMeshes[0];
	printf(" %i vertices in mesh[0]\n", mesh->mNumVertices);
	*point_count = mesh->mNumVertices;
	verticesNum1 = mesh->mNumVertices;

	//Generate a vertex attribute object 
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	//Copy out all the data from AssImp's data structures into arrays and thene copy it into data buffers
	GLfloat* points = NULL; // array of vertex points
	if (mesh->HasPositions()) {
		points = (GLfloat*)malloc(*point_count * 3 * sizeof(GLfloat));
		for (int i = 0; i < *point_count; i++) {
			const aiVector3D* vp = &(mesh->mVertices[i]);
			points[i * 3] = (GLfloat)vp->x;
			points[i * 3 + 1] = (GLfloat)vp->y;
			points[i * 3 + 2] = (GLfloat)vp->z;
						
		}
	}

		
	//Copy the mesh data into VBOs 
	GLuint vbo;
	glGenBuffers(1, &vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		3 * *point_count * sizeof(GLfloat),
		points,
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	int vert;
	for (vert = 0; vert < verticesNum1 * 3; vert++)
	{
		vertices1[vert] = points[vert];
	}

	
	free(points);
	
	//Free assimp's copy of memory 
	aiReleaseImport(scene);
	printf("mesh loaded\n");
	
	return true;
}

//Function to load the Cactus file with Assimp 
bool load_mesh2(const char* file_name, GLuint* vao, int* point_count) {

	const aiScene* scene = aiImportFile(file_name, aiProcess_Triangulate);
	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return false;
	}

	const aiMesh* mesh = scene->mMeshes[0];
	printf(" %i vertices in mesh[0]\n", mesh->mNumVertices);
	*point_count = mesh->mNumVertices;
	verticesNum2 = mesh->mNumVertices;

	//Generate a vertex attribute object 
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	//Copy out all the data from AssImp's data structures into arrays and thene copy it into data buffers
	GLfloat* points = NULL; // array of vertex points
	if (mesh->HasPositions()) {
		points = (GLfloat*)malloc(*point_count * 3 * sizeof(GLfloat));
		for (int i = 0; i < *point_count; i++) {
			const aiVector3D* vp = &(mesh->mVertices[i]);
			points[i * 3] = (GLfloat)vp->x;
			points[i * 3 + 1] = (GLfloat)vp->y;
			points[i * 3 + 2] = (GLfloat)vp->z;

		}
	}


	//Copy the mesh data into VBOs 
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		3 * *point_count * sizeof(GLfloat),
		points,
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	int vert;
	for (vert = 0; vert < verticesNum2 * 3; vert++)
	{
		vertices2[vert] = points[vert];
	}


	free(points);

	//Free assimp's copy of memory 
	aiReleaseImport(scene);
	printf("mesh loaded\n");

	return true;
}

//Load and read the shader files
char *textFileRead(char *fn) {
	FILE *fp;
	char *content = NULL;
	int count = 0;
	if (fn != NULL) {
		fp = fopen(fn, "rt");
		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

void setShaders()
{
	char *VertShader = NULL, *FragShader = NULL;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	VertShader = textFileRead("VertexShader.txt");
	FragShader = textFileRead("FragmentShader.txt"); 
	const char * vv = VertShader;
	const char * ff = FragShader;
	glShaderSource(VertexShader, 1, &vv, NULL);
	glShaderSource(FragmentShader, 1, &ff, NULL);
	glCompileShader(VertexShader);
	glCompileShader(FragmentShader);
	p = glCreateProgram();
	glAttachShader(p, VertexShader);
	glAttachShader(p, FragmentShader);
	glLinkProgram(p);
	glUseProgram(p);
}

void drawBunny(void)
{

	glBegin(GL_TRIANGLES);
	int i;
	for (i = 0; i < (verticesNum1); i+=3)
	{
		glNormal3f(((vertices1[i * 3 + 1]- vertices1[i * 3 + 4])*(vertices1[i * 3 + 2]- vertices1[i * 3 + 8]))- ((vertices1[i * 3 + 2] - vertices1[i * 3 + 5])*(vertices1[i * 3 + 1] - vertices1[i * 3 + 7])), 
			((vertices1[i * 3 + 2] - vertices1[i * 3 + 5])*(vertices1[i * 3] - vertices1[i * 3 + 6])) - ((vertices1[i * 3] - vertices1[i * 3 + 3])*(vertices1[i * 3 + 2] - vertices1[i * 3 + 8])),
			((vertices1[i * 3 ] - vertices1[i * 3 + 3])*(vertices1[i * 3 + 1] - vertices1[i * 3 + 7])) - ((vertices1[i * 3 + 1] - vertices1[i * 3 + 4])*(vertices1[i * 3] - vertices1[i * 3 + 6])));
		glVertex3f(vertices1[i * 3], vertices1[i * 3 + 1], vertices1[i * 3 + 2]);
		glVertex3f(vertices1[i * 3 + 3], vertices1[i * 3 + 4], vertices1[i * 3 + 5]);
		glVertex3f(vertices1[i * 3 + 6], vertices1[i * 3 + 7], vertices1[i * 3 + 8]);

		


						
	}
	glEnd();
}

void drawCactus(void)
{

	glBegin(GL_TRIANGLES);
	int i;
	for (i = 0; i < (verticesNum2); i += 3)
	{
		glNormal3f(((vertices2[i * 3 + 1] - vertices2[i * 3 + 4])*(vertices2[i * 3 + 2] - vertices2[i * 3 + 8])) - ((vertices2[i * 3 + 2] - vertices2[i * 3 + 5])*(vertices2[i * 3 + 1] - vertices2[i * 3 + 7])),
			((vertices2[i * 3 + 2] - vertices2[i * 3 + 5])*(vertices2[i * 3] - vertices2[i * 3 + 6])) - ((vertices2[i * 3] - vertices2[i * 3 + 3])*(vertices2[i * 3 + 2] - vertices2[i * 3 + 8])),
			((vertices2[i * 3] - vertices2[i * 3 + 3])*(vertices2[i * 3 + 1] - vertices2[i * 3 + 7])) - ((vertices2[i * 3 + 1] - vertices2[i * 3 + 4])*(vertices2[i * 3] - vertices2[i * 3 + 6])));
		glVertex3f(vertices2[i * 3], vertices2[i * 3 + 1], vertices2[i * 3 + 2]);
		glVertex3f(vertices2[i * 3 + 3], vertices2[i * 3 + 4], vertices2[i * 3 + 5]);
		glVertex3f(vertices2[i * 3 + 6], vertices2[i * 3 + 7], vertices2[i * 3 + 8]);
		}
	glEnd();
}




/*

void drawCactus(void)
{
	glNormal3f((rand() %10), (rand() % 10), (rand() % 10));
	glBegin(GL_TRIANGLES);
	int i;
	for (i = 0; i < verticesNum2; i++)
	{

		glNormal3f((rand() % 10), (rand() % 10), (rand() % 10));
		glVertex3f(vertices2[i * 3], vertices2[i * 3 + 1], vertices2[i * 3 + 2]);
	}
	glEnd();
}

*/
int main(void) {
	


	//Define the projection matrix and its parameters
	float near_clipping = 2.0f; // clipping plane
	float far_clipping = 5.0f; // clipping plane
	float fov = 70.0f;
	float aspect = (float)width / (float)height; // aspect ratio				 
	float range = tan(fov * 0.5f) * near_clipping;
	float Sx = (2.0f * near_clipping) / (range * aspect + range * aspect);
	float Sy = near_clipping / range;
	float Sz = -(far_clipping + near_clipping) / (far_clipping - near_clipping);
	float Pz = -(2.0f * far_clipping * near_clipping) / (far_clipping - near_clipping);
	
	float projection_matrix[] = {
		Sx, 0.0f, 0.0f, 0.0f,
		0.0f, Sy, 0.0f, 0.0f,
		0.0f, 0.0f, Sz, -1.0f,
		0.0f, 0.0f, Pz, 0.0f
	};
	
	//Define the view matrix and its parametes
	//1//Define the translation matrix
	float eye_x = 0.0f;
	float eye_y = 0.0f;
	float eye_z = 4.0f;
	float camera_translation_speed = 0.01f;
	float lookAt_position_x = 0.0f;
	float lookAt_position_y = 0.0f;
	float lookAt_position_z = 0.0f;
	float translation_matrix[] = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		-eye_x,-eye_y,-eye_z,1.0f
	};
	/*//2//Define the rotation matrix 
	float angle = 0*(PI/180);
	float c = cos(angle);
	float s = sin(angle);
	float t = 1 - c;
	float x = 0;
	float y = 0;
	float z = 0;
	float camera_rotation_speed = 0.1f;

	float rotation_matrix[] = {
		t*x*x + c,t*x*y + z*s,t*x*z - y*s,0.0f,
		t*x*y - z*s,t*y*y + c,t*y*z + x*s,0.0f,
		t*x*z + y*s,t*y*z - x*s,t*z*z + c,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};*/
	
	
	/////////////////////////////////////////////////////////////////
	/////////////////%%% Shader Rendering %%%///////////////////////
	///////////////////////////////////////////////////////////////

	//Initialize GLFW 
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(1);
	}

	//Open the first window 
	GLFWwindow* window1 = glfwCreateWindow(width, height, "Shader-Based Rendering", NULL, NULL);
	if (!window1) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window1);

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	
	//Load the model The bunny model
	assert(load_mesh1(MESH_FILE_Bunny, &model_vao_bunny, &model_point_count_bunny));

	//Load the model The cactus model
	assert(load_mesh2(MESH_FILE_Cactus, &model_vao_cactus, &model_point_count_cactus));
	
	

	//Set the shaders
	setShaders();

	//Get the location of the light state into the shaders
	GLint lightStateValue_location = glGetUniformLocation(p, "lightStateValue");
	glUseProgram(p);
	glUniform4f(lightStateValue_location, lightState , 0.0, 0.0, 0.0);

	//Get the location of ambient, diffuse and specular lights into the shaders
	GLint La_location = glGetUniformLocation(p, "La");
	glUseProgram(p);
	glUniform3f(La_location, ambient_red_color, ambient_green_color, ambient_blue_color);
	GLint Ld_location = glGetUniformLocation(p, "Ld");
	glUseProgram(p);
	glUniform3f(Ld_location, diffuse_red_color, diffuse_green_color, diffuse_blue_color);
	GLint Ls_location = glGetUniformLocation(p, "Ls");
	glUseProgram(p);
	glUniform3f(Ls_location, specular_red_color, specular_green_color, specular_blue_color);


	//Get the location of the matrices into the shaderss
	//color vector
	GLint VC_location = glGetUniformLocation(p, "VC");
	glUseProgram(p);
	glUniform4f(VC_location, 1.0, 0.0, 0.0, 1.0);//glUniform4f(VC_location, red_color, green_color, blue_color, alpha_color);
	

	//view matrix 
	//1//Translation
	int translation_matrix_location = glGetUniformLocation(p, "translation_matrix");
	glUseProgram(p);
	glUniformMatrix4fv(translation_matrix_location, 1, GL_FALSE, translation_matrix);
	//2//Rotation
	//int rotation_matrix_location = glGetUniformLocation(p, "rotation_matrix");
	//glUseProgram(p);
	//glUniformMatrix4fv(rotation_matrix_location, 1, GL_FALSE, rotation_matrix);
	//projection matrix
	int projection_matrix_location = glGetUniformLocation(p, "projection_matrix");
	glUseProgram(p);
	glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, projection_matrix);

	/////////////////////////////////////////////////////////////////
	///////////////%%% Fixed Pipeline Rendering %%%/////////////////
	///////////////////////////////////////////////////////////////


	//Open the second window
	/*GLFWwindow* window2 = glfwCreateWindow(width, height, "Fixed Pipeline Rendering", NULL, NULL);
	if (!window2) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window2);
	
	//Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	*/
	//Set the shaders
	
	//Define the rotation parameters 
	float x_rotate = 0.0f;
	float y_rotate = 0.0f;
	float z_rotate = 0.0f;
	
	//Open the first window 
	GLFWwindow* window2 = glfwCreateWindow(width, height, "Fixed Pipeline", NULL, NULL);
	if (!window2) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window2);

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//Load the model The bunny model
	assert(load_mesh1(MESH_FILE_Bunny, &model_vao_bunny, &model_point_count_bunny));

	//Load the model The cactus model
	assert(load_mesh2(MESH_FILE_Cactus, &model_vao_cactus, &model_point_count_cactus));



	//Set the shaders
	setShaders();

	//Get the location of the light state into the shaders
    lightStateValue_location = glGetUniformLocation(p, "lightStateValue");
	glUseProgram(p);
	glUniform4f(lightStateValue_location, lightState, 0.0, 0.0, 0.0);

	//Get the location of ambient, diffuse and specular lights into the shaders
 La_location = glGetUniformLocation(p, "La");
	glUseProgram(p);
	glUniform3f(La_location, ambient_red_color, ambient_green_color, ambient_blue_color);
 Ld_location = glGetUniformLocation(p, "Ld");
	glUseProgram(p);
	glUniform3f(Ld_location, diffuse_red_color, diffuse_green_color, diffuse_blue_color);
 Ls_location = glGetUniformLocation(p, "Ls");
	glUseProgram(p);
	glUniform3f(Ls_location, specular_red_color, specular_green_color, specular_blue_color);


	//Get the location of the matrices into the shaderss
	//color vector
 VC_location = glGetUniformLocation(p, "VC");
	glUseProgram(p);
	glUniform4f(VC_location, 1.0, 0.0, 0.0, 1.0);//glUniform4f(VC_location, red_color, green_color, blue_color, alpha_color);


												 //view matrix 
												 //1//Translation
 translation_matrix_location = glGetUniformLocation(p, "translation_matrix");
	glUseProgram(p);
	glUniformMatrix4fv(translation_matrix_location, 1, GL_FALSE, translation_matrix);
	//2//Rotation
	//int rotation_matrix_location = glGetUniformLocation(p, "rotation_matrix");
	//glUseProgram(p);
	//glUniformMatrix4fv(rotation_matrix_location, 1, GL_FALSE, rotation_matrix);
	//projection matrix
 projection_matrix_location = glGetUniformLocation(p, "projection_matrix");
	glUseProgram(p);
	glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, projection_matrix);

	//Open the first GL context (Shader-Based Rendering) 
	while (!glfwWindowShouldClose(window1) && !glfwWindowShouldClose(window2))
	{

		/////////////////////////////////////////////////////////////////
		/////////////////%%% Shader Rendering %%%///////////////////////
		///////////////////////////////////////////////////////////////

		glfwMakeContextCurrent(window1);
		
		//Clear the Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Center the object in the center of the window
		glfwGetFramebufferSize(window1, &width, &height);
		glViewport(0, 0, width, height);
		glEnable(GL_DEPTH_TEST);
		
		//Draw the model
		glBindVertexArray(model_vao_displayed);
		glDrawArrays(GL_TRIANGLES, 0, model_point_count_displayed);

		if (glfwGetKey(window1, GLFW_KEY_B)|| glfwGetKey(window2, GLFW_KEY_B)) {
			model_vao_displayed = model_vao_bunny;
			model_point_count_displayed = model_point_count_bunny;

		}
		if (glfwGetKey(window1, GLFW_KEY_M)|| glfwGetKey(window2, GLFW_KEY_M)) {
			model_vao_displayed = model_vao_cactus;
			model_point_count_displayed = model_point_count_cactus;
		}
		
		//Points, wireframe and solid model rendering
		if (glfwGetKey(window1, GLFW_KEY_L) || glfwGetKey(window2, GLFW_KEY_L)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (glfwGetKey(window1, GLFW_KEY_K) || glfwGetKey(window2, GLFW_KEY_K)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
		if (glfwGetKey(window1, GLFW_KEY_J) || glfwGetKey(window2, GLFW_KEY_J)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (glfwGetKey(window1, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window2, GLFW_KEY_LEFT_CONTROL)) {
			//Enable counter-clockwise culling			
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CCW);
		}
		if (glfwGetKey(window1, GLFW_KEY_LEFT_ALT) || glfwGetKey(window2, GLFW_KEY_LEFT_ALT)) {
			//Enable clockwise culling			
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CW);
		}
		if (glfwGetKey(window1, GLFW_KEY_SPACE) || glfwGetKey(window2, GLFW_KEY_SPACE)) {
			//Disable culling			
			glDisable(GL_CULL_FACE);
		}

		//Change the value of the near clippling plane
		if (glfwGetKey(window1, GLFW_KEY_F1)||glfwGetKey(window2, GLFW_KEY_F1)) {
			near_clipping += Clipping;
			range = tan(fov * 0.5f) * near_clipping;
			Sx = (2.0f * near_clipping) / (range * aspect + range * aspect);
			Sy = near_clipping / range;
			Sz = -(far_clipping + near_clipping) / (far_clipping - near_clipping);
			Pz = -(2.0f * far_clipping * near_clipping) / (far_clipping - near_clipping);
			cam_moved = true;
		}

		if (glfwGetKey(window1, GLFW_KEY_F2)|| glfwGetKey(window2, GLFW_KEY_F2)) {
			near_clipping -= Clipping;
			range = tan(fov * 0.5f) * near_clipping;
			Sx = (2.0f * near_clipping) / (range * aspect + range * aspect);
			Sy = near_clipping / range;
			Sz = -(far_clipping + near_clipping) / (far_clipping - near_clipping);
			Pz = -(2.0f * far_clipping * near_clipping) / (far_clipping - near_clipping);
			cam_moved = true;
			}

		//Change the value of the far clippling plane
		if (glfwGetKey(window1, GLFW_KEY_F5)|| glfwGetKey(window2, GLFW_KEY_F5)) {
			far_clipping += Clipping;
			range = tan(fov * 0.5f) * near_clipping;
			Sx = (2.0f * near_clipping) / (range * aspect + range * aspect);
			Sy = near_clipping / range;
			Sz = -(far_clipping + near_clipping) / (far_clipping - near_clipping);
			Pz = -(2.0f * far_clipping * near_clipping) / (far_clipping - near_clipping);
			cam_moved = true;
		}

		if (glfwGetKey(window1, GLFW_KEY_F6)|| glfwGetKey(window2, GLFW_KEY_F6)) {
			far_clipping -= Clipping;
			range = tan(fov * 0.5f) * near_clipping;
			Sx = (2.0f * near_clipping) / (range * aspect + range * aspect);
			Sy = near_clipping / range;
			Sz = -(far_clipping + near_clipping) / (far_clipping - near_clipping);
			Pz = -(2.0f * far_clipping * near_clipping) / (far_clipping - near_clipping);
			cam_moved = true;
		}

		//Apply the camera translation
		if (glfwGetKey(window1, GLFW_KEY_D) || glfwGetKey(window2, GLFW_KEY_D)) {
			eye_x += camera_translation_speed;
			lookAt_position_x += camera_translation_speed;
			cam_moved = true;
		}
		if (glfwGetKey(window1, GLFW_KEY_A) || glfwGetKey(window2, GLFW_KEY_A)) {
			eye_x -= camera_translation_speed;
			lookAt_position_x -= camera_translation_speed;
			cam_moved = true;
		}
		if (glfwGetKey(window1, GLFW_KEY_W) || glfwGetKey(window2, GLFW_KEY_W)) {
			eye_y += camera_translation_speed;
			lookAt_position_y += camera_translation_speed;
			cam_moved = true;
		}
		if (glfwGetKey(window1, GLFW_KEY_S) || glfwGetKey(window2, GLFW_KEY_S)) {
			eye_y -= camera_translation_speed;
			lookAt_position_y -= camera_translation_speed;
			cam_moved = true;
		}
		if (glfwGetKey(window1, GLFW_KEY_Z) || glfwGetKey(window2, GLFW_KEY_Z)) {
			eye_z += camera_translation_speed;
			lookAt_position_z += camera_translation_speed;
			cam_moved = true;
		}
		if (glfwGetKey(window1, GLFW_KEY_E) || glfwGetKey(window2, GLFW_KEY_E)) {
			eye_z -= camera_translation_speed;
			lookAt_position_z -= camera_translation_speed;
			cam_moved = true;
		}
		
		//reset the camera to its original position
		if (glfwGetKey(window1, GLFW_KEY_KP_5) || glfwGetKey(window2, GLFW_KEY_KP_5)) {
			eye_x = 0;
			eye_y = 0;
			eye_z = 4;
			lookAt_position_x = 0.0f;
			lookAt_position_y = 0.0f;
			lookAt_position_z = 0.0f;
			near_clipping = 2.0f; 
			far_clipping = 5.0f;
			fov = 70.0f;
			aspect = (float)width / (float)height; // aspect ratio				 
			range = tan(fov * 0.5f) * near_clipping;
			Sx = (2.0f * near_clipping) / (range * aspect + range * aspect);
			Sy = near_clipping / range;
			Sz = -(far_clipping + near_clipping) / (far_clipping - near_clipping);
			Pz = -(2.0f * far_clipping * near_clipping) / (far_clipping - near_clipping);
			cam_moved = true;
		}

		//Define the RGBA values of Phong ambient light
		if (glfwGetKey(window1, GLFW_KEY_F7) || glfwGetKey(window2, GLFW_KEY_F7)) {

			float ambient_red_entered, ambient_green_entered, ambient_blue_entered, ambient_alpha_entered;
			cout << "Please enter the value of the red color:" << endl;
			cin >> ambient_red_entered;
			cout << "Please enter the value of the green color:" << endl;
			cin >> ambient_green_entered;
			cout << "Please enter the value of the blue color:" << endl;
			cin >> ambient_blue_entered;
			cout << "Please enter the value of the alpha color:" << endl;
			cin >> ambient_alpha_entered;
			if ((ambient_red_entered > 1 || ambient_red_entered < 0) || (ambient_green_entered > 1 || ambient_green_entered < 0) || (ambient_blue_entered > 1 || ambient_blue_entered < 0) || (ambient_alpha_entered > 1 || ambient_alpha_entered < 0))
			{
				cout << "The colors must be floating points whose values are between 0 and 1!" << endl;
				cam_moved = false;
			}
			else
			{
				ambient_red_color = ambient_red_entered;
				ambient_green_color = ambient_green_entered;
				ambient_blue_color = ambient_blue_entered;
				ambient_alpha_color = ambient_alpha_entered;
				glUniform3f(La_location, ambient_red_color, ambient_green_color, ambient_blue_color);
				cam_moved = true;
			}
		}

		//Define the RGBA values of Phong diffuse light
		if (glfwGetKey(window1, GLFW_KEY_F8) || glfwGetKey(window2, GLFW_KEY_F8)) {

			float diffuse_red_entered, diffuse_green_entered, diffuse_blue_entered, diffuse_alpha_entered;
			cout << "Please enter the value of the red color:" << endl;
			cin >> diffuse_red_entered;
			cout << "Please enter the value of the green color:" << endl;
			cin >> diffuse_green_entered;
			cout << "Please enter the value of the blue color:" << endl;
			cin >> diffuse_blue_entered;
			cout << "Please enter the value of the alpha color:" << endl;
			cin >> diffuse_alpha_entered;
			if ((diffuse_red_entered > 1 || diffuse_red_entered < 0) || (diffuse_green_entered > 1 || diffuse_green_entered < 0) || (diffuse_blue_entered > 1 || diffuse_blue_entered < 0) || (diffuse_alpha_entered > 1 || diffuse_alpha_entered < 0))
			{
				cout << "The colors must be floating points whose values are between 0 and 1!" << endl;
				cam_moved = false;
			}
			else
			{
				diffuse_red_color = diffuse_red_entered;
				diffuse_green_color = diffuse_green_entered;
				diffuse_blue_color = diffuse_blue_entered;
				diffuse_alpha_color = diffuse_alpha_entered;
				cam_moved = true;
			}
		}

		//Define the RGBA values of Phong specular light
		if (glfwGetKey(window1, GLFW_KEY_F9) || glfwGetKey(window2, GLFW_KEY_F9)) {

			float specular_red_entered, specular_green_entered, specular_blue_entered, specular_alpha_entered;
			cout << "Please enter the value of the red color:" << endl;
			cin >> specular_red_entered;
			cout << "Please enter the value of the green color:" << endl;
			cin >> specular_green_entered;
			cout << "Please enter the value of the blue color:" << endl;
			cin >> specular_blue_entered;
			cout << "Please enter the value of the alpha color:" << endl;
			cin >> specular_alpha_entered;
			if ((specular_red_entered > 1 || specular_red_entered < 0) || (specular_green_entered > 1 || specular_green_entered < 0) || (specular_blue_entered > 1 || specular_blue_entered < 0) || (specular_alpha_entered > 1 || specular_alpha_entered < 0))
			{
				cout << "The colors must be floating points whose values are between 0 and 1!" << endl;
				cam_moved = false;
			}
			else
			{
				specular_red_color = specular_red_entered;
				specular_green_color = specular_green_entered;
				specular_blue_color = specular_blue_entered;
				specular_alpha_color = specular_alpha_entered;
				cam_moved = true;
			}
		}

		//Define all the operations when cam_moved is set to true
		if (cam_moved=true)
		{
			//Update the color vector 
			//glUniform4f(VC_location, red_color, green_color, blue_color, alpha_color);
			//glUniform4f(VC_location, 1.0,0.0,0.0,1.0);

			//update the projection matrix
			float projection_matrix[] = {
				Sx, 0.0f, 0.0f, 0.0f,
				0.0f, Sy, 0.0f, 0.0f,
				0.0f, 0.0f, Sz, -1.0f,
				0.0f, 0.0f, Pz, 0.0f
			};						
			glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, projection_matrix);

			//update the translation matrix
			float translation_matrix[] = {
				1.0f,0.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,0.0f,
				0.0f,0.0f,1.0f,0.0f,
				-eye_x,-eye_y,-eye_z,1.0f
			};
			glUniformMatrix4fv(translation_matrix_location, 1, GL_FALSE, translation_matrix);

			//update the light parameters
			glUniform3f(La_location, ambient_red_color, ambient_green_color, ambient_blue_color);
			glUniform3f(Ld_location, diffuse_red_color, diffuse_green_color, diffuse_blue_color);
			glUniform3f(Ls_location, specular_red_color, specular_green_color, specular_blue_color);
			}

		//Enable and Disable the light
		if (glfwGetKey(window1, GLFW_KEY_F3) || glfwGetKey(window2, GLFW_KEY_F3)) {
			lightState = 1.0;
			glUniform4f(lightStateValue_location, lightState, 0.0, 0.0, 0.0);
		}
		if (glfwGetKey(window1, GLFW_KEY_F4) || glfwGetKey(window2, GLFW_KEY_F4)) {
			lightState = 0.0;
			glUniform4f(lightStateValue_location, lightState, 0.0, 0.0, 0.0);
		}

		//Swap Back and forth buffers
		//glfwSwapBuffers(window1);
		//glfwPollEvents();

		/////////////////////////////////////////////////////////////////
		///////////////%%% Fixed Pipeline Rendering %%%/////////////////
		///////////////////////////////////////////////////////////////


		//Clear the Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Center the object in the center of the window
		glfwGetFramebufferSize(window2, &width, &height);
		glViewport(0, 0, width, height);
		glEnable(GL_DEPTH_TEST);
		
		//Implement the model view and projection matrices
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluPerspective(fov, width / height, near_clipping, far_clipping);
		gluLookAt(eye_x, eye_y, eye_z, lookAt_position_x, lookAt_position_y, lookAt_position_z, 0, 1, 0);
		
		//Draw the model
		if (glfwGetKey(window1, GLFW_KEY_B) || glfwGetKey(window2, GLFW_KEY_B)) {
			Cactus_model = 0;
			Bunny_model = 1;
		}
		if (glfwGetKey(window1, GLFW_KEY_M) || glfwGetKey(window2, GLFW_KEY_M)) {
			Bunny_model = 0;
			Cactus_model = 1;
			
		}	
		if (Bunny_model == 1)
		{
			drawBunny();
		}
		else if (Cactus_model == 1)
		{
			drawCactus();
		}
		
		//////Homework3//////
		////Lighting Part/// 
		////////////////////

		//Point light source
		GLfloat light_position[] = { 0.0, 0.0, 2.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glEnable(GL_LIGHT0);

		if (glfwGetKey(window1, GLFW_KEY_F3) || glfwGetKey(window2, GLFW_KEY_F3)) {
			glEnable(GL_LIGHTING);
			}
		if (glfwGetKey(window1, GLFW_KEY_F4) || glfwGetKey(window2, GLFW_KEY_F4)) {
			glDisable(GL_LIGHTING);
			
		}

		
		GLfloat global_light_ambient[] = { global_red_color, global_green_color, global_blue_color, global_alpha_color };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_light_ambient);

		
		GLfloat light_ambient[] = { ambient_red_color, ambient_green_color, ambient_blue_color, ambient_alpha_color };
		GLfloat light_diffuse[] = { diffuse_red_color, diffuse_green_color, diffuse_blue_color, diffuse_alpha_color };
		GLfloat light_specular[] ={ specular_red_color, specular_green_color, specular_blue_color, specular_alpha_color };
		glEnable(GL_NORMALIZE);
		glEnable(GL_COLOR_MATERIAL);
		GLfloat mat_a[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_d[] = { 1.0, 0.0, 1.0, 1.0 };
		GLfloat mat_s[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat low_sh[] = { 10.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_sh);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		

		//Define the RGBA values of the global ambient light
		if (glfwGetKey(window1, GLFW_KEY_G) || glfwGetKey(window2, GLFW_KEY_G)) {

			float global_red_entered, global_green_entered, global_blue_entered, global_alpha_entered;
			cout << "Please enter the value of the red color:" << endl;
			cin >> global_red_entered;
			cout << "Please enter the value of the green color:" << endl;
			cin >> global_green_entered;
			cout << "Please enter the value of the blue color:" << endl;
			cin >> global_blue_entered;
			cout << "Please enter the value of the alpha color:" << endl;
			cin >> global_alpha_entered;
			if ((global_red_entered > 1 || global_red_entered < 0) || (global_green_entered > 1 || global_green_entered < 0) || (global_blue_entered > 1 || global_blue_entered < 0) || (global_alpha_entered > 1 || global_alpha_entered < 0) )
			{
				cout << "The colors must be floating points whose values are between 0 and 1!" << endl;
				cam_moved = false;
			}
			else
			{
				global_red_color = global_red_entered;
				global_green_color = global_green_entered;
				global_blue_color = global_blue_entered;
				global_alpha_color = global_alpha_entered;
				cam_moved = true;
			}
		}
		
		//Define the RGBA values of Phong ambient light
		if (glfwGetKey(window1, GLFW_KEY_F7) || glfwGetKey(window2, GLFW_KEY_F7)) {

			float ambient_red_entered, ambient_green_entered, ambient_blue_entered, ambient_alpha_entered;
			cout << "Please enter the value of the red color:" << endl;
			cin >> ambient_red_entered;
			cout << "Please enter the value of the green color:" << endl;
			cin >> ambient_green_entered;
			cout << "Please enter the value of the blue color:" << endl;
			cin >> ambient_blue_entered;
			cout << "Please enter the value of the alpha color:" << endl;
			cin >> ambient_alpha_entered;
			if ((ambient_red_entered > 1 || ambient_red_entered < 0) || (ambient_green_entered > 1 || ambient_green_entered < 0) || (ambient_blue_entered > 1 || ambient_blue_entered < 0) || (ambient_alpha_entered > 1 || ambient_alpha_entered < 0))
			{
				cout << "The colors must be floating points whose values are between 0 and 1!" << endl;
				cam_moved = false;
			}
			else
			{
				ambient_red_color = ambient_red_entered;
				ambient_green_color = ambient_green_entered;
				ambient_blue_color = ambient_blue_entered;
				ambient_alpha_color = ambient_alpha_entered;
				cam_moved = true;
			}
		}

		//Define the RGBA values of Phong diffuse light
		if (glfwGetKey(window1, GLFW_KEY_F8) || glfwGetKey(window2, GLFW_KEY_F8)) {

			float diffuse_red_entered, diffuse_green_entered, diffuse_blue_entered, diffuse_alpha_entered;
			cout << "Please enter the value of the red color:" << endl;
			cin >> diffuse_red_entered;
			cout << "Please enter the value of the green color:" << endl;
			cin >> diffuse_green_entered;
			cout << "Please enter the value of the blue color:" << endl;
			cin >> diffuse_blue_entered;
			cout << "Please enter the value of the alpha color:" << endl;
			cin >> diffuse_alpha_entered;
			if ((diffuse_red_entered > 1 || diffuse_red_entered < 0) || (diffuse_green_entered > 1 || diffuse_green_entered < 0) || (diffuse_blue_entered > 1 || diffuse_blue_entered < 0) || (diffuse_alpha_entered > 1 || diffuse_alpha_entered < 0))
			{
				cout << "The colors must be floating points whose values are between 0 and 1!" << endl;
				cam_moved = false;
			}
			else
			{
				diffuse_red_color = diffuse_red_entered;
				diffuse_green_color = diffuse_green_entered;
				diffuse_blue_color = diffuse_blue_entered;
				diffuse_alpha_color = diffuse_alpha_entered;
				cam_moved = true;
			}
		}

		//Define the RGBA values of Phong specular light
		if (glfwGetKey(window1, GLFW_KEY_F9) || glfwGetKey(window2, GLFW_KEY_F9)) {

			float specular_red_entered, specular_green_entered, specular_blue_entered, specular_alpha_entered;
			cout << "Please enter the value of the red color:" << endl;
			cin >> specular_red_entered;
			cout << "Please enter the value of the green color:" << endl;
			cin >> specular_green_entered;
			cout << "Please enter the value of the blue color:" << endl;
			cin >> specular_blue_entered;
			cout << "Please enter the value of the alpha color:" << endl;
			cin >> specular_alpha_entered;
			if ((specular_red_entered > 1 || specular_red_entered < 0) || (specular_green_entered > 1 || specular_green_entered < 0) || (specular_blue_entered > 1 || specular_blue_entered < 0) || (specular_alpha_entered > 1 || specular_alpha_entered < 0))
			{
				cout << "The colors must be floating points whose values are between 0 and 1!" << endl;
				cam_moved = false;
			}
			else
			{
				specular_red_color = specular_red_entered;
				specular_green_color = specular_green_entered;
				specular_blue_color = specular_blue_entered;
				specular_alpha_color = specular_alpha_entered;
				cam_moved = true;
			}
		}


		/////////////////////////
		////////////////////////
		///////////////////////
		

		//Points, wireframe and solid model rendering
		if (glfwGetKey(window1, GLFW_KEY_L) || glfwGetKey(window2, GLFW_KEY_L)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (glfwGetKey(window1, GLFW_KEY_K) || glfwGetKey(window2, GLFW_KEY_K)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
		if (glfwGetKey(window1, GLFW_KEY_J) || glfwGetKey(window2, GLFW_KEY_J)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (glfwGetKey(window1, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window2, GLFW_KEY_LEFT_CONTROL)) {
			//Enable counter-clockwise culling			
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CCW);
		}
		if (glfwGetKey(window1, GLFW_KEY_LEFT_ALT) || glfwGetKey(window2, GLFW_KEY_LEFT_ALT)) {
			//Enable clockwise culling			
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CW);
		}
		if (glfwGetKey(window1, GLFW_KEY_SPACE) || glfwGetKey(window2, GLFW_KEY_SPACE)) {
			//Disable culling			
			//glDisable(GL_CULL_FACE);
		}

		
		
		/*
		//Define the color vector
		if (glfwGetKey(window1, GLFW_KEY_C) || glfwGetKey(window2, GLFW_KEY_C)) {

			float red_entered, green_entered, blue_entered;
			cout << "Please enter the value of the red color:" << endl;
			cin >> red_entered;
			cout << "Please enter the value of the green color:" << endl;
			cin >> green_entered;
			cout << "Please enter the value of the blue color:" << endl;
			cin >> blue_entered;
			if ((red_entered > 1 || red_entered < 0) || (green_entered > 1 || green_entered < 0) || (blue_entered > 1 || blue_entered < 0))
			{
				cout << "The colors must be floating points whose values are between 0 and 1!" << endl;
				cam_moved = false;
			}
			else
			{
				red_color = red_entered;
				green_color = green_entered;
				blue_color = blue_entered;
				cam_moved = true;
			}
		}
		*/


		//Define all the operations when cam_moved is set to true  
		if (cam_moved = true)
		{
			//update the color vector 
			//glColor3f(0.5, 0.0, 0.5);//(red_color, green_color, blue_color);
			
						
		}
		
		//Swap Back and forth buffers
		glfwSwapBuffers(window1);
		glfwPollEvents();

		glfwMakeContextCurrent(window2);

		//Clear the Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Center the object in the center of the window
		glfwGetFramebufferSize(window1, &width, &height);
		glViewport(0, 0, width, height);
		glEnable(GL_DEPTH_TEST);

		//Draw the model
		glBindVertexArray(model_vao_displayed);
		glDrawArrays(GL_TRIANGLES, 0, model_point_count_displayed);

		if (glfwGetKey(window1, GLFW_KEY_B) || glfwGetKey(window2, GLFW_KEY_B)) {
			model_vao_displayed = model_vao_bunny;
			model_point_count_displayed = model_point_count_bunny;

		}
		if (glfwGetKey(window1, GLFW_KEY_M) || glfwGetKey(window2, GLFW_KEY_M)) {
			model_vao_displayed = model_vao_cactus;
			model_point_count_displayed = model_point_count_cactus;
		}

		//Points, wireframe and solid model rendering
		if (glfwGetKey(window1, GLFW_KEY_L) || glfwGetKey(window2, GLFW_KEY_L)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (glfwGetKey(window1, GLFW_KEY_K) || glfwGetKey(window2, GLFW_KEY_K)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
		if (glfwGetKey(window1, GLFW_KEY_J) || glfwGetKey(window2, GLFW_KEY_J)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (glfwGetKey(window1, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window2, GLFW_KEY_LEFT_CONTROL)) {
			//Enable counter-clockwise culling			
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CCW);
		}
		if (glfwGetKey(window1, GLFW_KEY_LEFT_ALT) || glfwGetKey(window2, GLFW_KEY_LEFT_ALT)) {
			//Enable clockwise culling			
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CW);
		}
		if (glfwGetKey(window1, GLFW_KEY_SPACE) || glfwGetKey(window2, GLFW_KEY_SPACE)) {
			//Disable culling			
			glDisable(GL_CULL_FACE);
		}

		//Change the value of the near clippling plane
		if (glfwGetKey(window1, GLFW_KEY_F1) || glfwGetKey(window2, GLFW_KEY_F1)) {
			near_clipping += Clipping;
			range = tan(fov * 0.5f) * near_clipping;
			Sx = (2.0f * near_clipping) / (range * aspect + range * aspect);
			Sy = near_clipping / range;
			Sz = -(far_clipping + near_clipping) / (far_clipping - near_clipping);
			Pz = -(2.0f * far_clipping * near_clipping) / (far_clipping - near_clipping);
			cam_moved = true;
		}

		if (glfwGetKey(window1, GLFW_KEY_F2) || glfwGetKey(window2, GLFW_KEY_F2)) {
			near_clipping -= Clipping;
			range = tan(fov * 0.5f) * near_clipping;
			Sx = (2.0f * near_clipping) / (range * aspect + range * aspect);
			Sy = near_clipping / range;
			Sz = -(far_clipping + near_clipping) / (far_clipping - near_clipping);
			Pz = -(2.0f * far_clipping * near_clipping) / (far_clipping - near_clipping);
			cam_moved = true;
		}

		//Change the value of the far clippling plane
		if (glfwGetKey(window1, GLFW_KEY_F5) || glfwGetKey(window2, GLFW_KEY_F5)) {
			far_clipping += Clipping;
			range = tan(fov * 0.5f) * near_clipping;
			Sx = (2.0f * near_clipping) / (range * aspect + range * aspect);
			Sy = near_clipping / range;
			Sz = -(far_clipping + near_clipping) / (far_clipping - near_clipping);
			Pz = -(2.0f * far_clipping * near_clipping) / (far_clipping - near_clipping);
			cam_moved = true;
		}

		if (glfwGetKey(window1, GLFW_KEY_F6) || glfwGetKey(window2, GLFW_KEY_F6)) {
			far_clipping -= Clipping;
			range = tan(fov * 0.5f) * near_clipping;
			Sx = (2.0f * near_clipping) / (range * aspect + range * aspect);
			Sy = near_clipping / range;
			Sz = -(far_clipping + near_clipping) / (far_clipping - near_clipping);
			Pz = -(2.0f * far_clipping * near_clipping) / (far_clipping - near_clipping);
			cam_moved = true;
		}

		//Apply the camera translation
		if (glfwGetKey(window1, GLFW_KEY_D) || glfwGetKey(window2, GLFW_KEY_D)) {
			eye_x += camera_translation_speed;
			lookAt_position_x += camera_translation_speed;
			cam_moved = true;
		}
		if (glfwGetKey(window1, GLFW_KEY_A) || glfwGetKey(window2, GLFW_KEY_A)) {
			eye_x -= camera_translation_speed;
			lookAt_position_x -= camera_translation_speed;
			cam_moved = true;
		}
		if (glfwGetKey(window1, GLFW_KEY_W) || glfwGetKey(window2, GLFW_KEY_W)) {
			eye_y += camera_translation_speed;
			lookAt_position_y += camera_translation_speed;
			cam_moved = true;
		}
		if (glfwGetKey(window1, GLFW_KEY_S) || glfwGetKey(window2, GLFW_KEY_S)) {
			eye_y -= camera_translation_speed;
			lookAt_position_y -= camera_translation_speed;
			cam_moved = true;
		}
		if (glfwGetKey(window1, GLFW_KEY_Z) || glfwGetKey(window2, GLFW_KEY_Z)) {
			eye_z += camera_translation_speed;
			lookAt_position_z += camera_translation_speed;
			cam_moved = true;
		}
		if (glfwGetKey(window1, GLFW_KEY_E) || glfwGetKey(window2, GLFW_KEY_E)) {
			eye_z -= camera_translation_speed;
			lookAt_position_z -= camera_translation_speed;
			cam_moved = true;
		}

		//reset the camera to its original position
		if (glfwGetKey(window1, GLFW_KEY_KP_5) || glfwGetKey(window2, GLFW_KEY_KP_5)) {
			eye_x = 0;
			eye_y = 0;
			eye_z = 4;
			lookAt_position_x = 0.0f;
			lookAt_position_y = 0.0f;
			lookAt_position_z = 0.0f;
			near_clipping = 2.0f;
			far_clipping = 5.0f;
			fov = 70.0f;
			aspect = (float)width / (float)height; // aspect ratio				 
			range = tan(fov * 0.5f) * near_clipping;
			Sx = (2.0f * near_clipping) / (range * aspect + range * aspect);
			Sy = near_clipping / range;
			Sz = -(far_clipping + near_clipping) / (far_clipping - near_clipping);
			Pz = -(2.0f * far_clipping * near_clipping) / (far_clipping - near_clipping);
			cam_moved = true;
		}

		//Define the RGBA values of Phong ambient light
		if (glfwGetKey(window1, GLFW_KEY_F7) || glfwGetKey(window2, GLFW_KEY_F7)) {

			float ambient_red_entered, ambient_green_entered, ambient_blue_entered, ambient_alpha_entered;
			cout << "Please enter the value of the red color:" << endl;
			cin >> ambient_red_entered;
			cout << "Please enter the value of the green color:" << endl;
			cin >> ambient_green_entered;
			cout << "Please enter the value of the blue color:" << endl;
			cin >> ambient_blue_entered;
			cout << "Please enter the value of the alpha color:" << endl;
			cin >> ambient_alpha_entered;
			if ((ambient_red_entered > 1 || ambient_red_entered < 0) || (ambient_green_entered > 1 || ambient_green_entered < 0) || (ambient_blue_entered > 1 || ambient_blue_entered < 0) || (ambient_alpha_entered > 1 || ambient_alpha_entered < 0))
			{
				cout << "The colors must be floating points whose values are between 0 and 1!" << endl;
				cam_moved = false;
			}
			else
			{
				ambient_red_color = ambient_red_entered;
				ambient_green_color = ambient_green_entered;
				ambient_blue_color = ambient_blue_entered;
				ambient_alpha_color = ambient_alpha_entered;
				glUniform3f(La_location, ambient_red_color, ambient_green_color, ambient_blue_color);
				cam_moved = true;
			}
		}

		//Define the RGBA values of Phong diffuse light
		if (glfwGetKey(window1, GLFW_KEY_F8) || glfwGetKey(window2, GLFW_KEY_F8)) {

			float diffuse_red_entered, diffuse_green_entered, diffuse_blue_entered, diffuse_alpha_entered;
			cout << "Please enter the value of the red color:" << endl;
			cin >> diffuse_red_entered;
			cout << "Please enter the value of the green color:" << endl;
			cin >> diffuse_green_entered;
			cout << "Please enter the value of the blue color:" << endl;
			cin >> diffuse_blue_entered;
			cout << "Please enter the value of the alpha color:" << endl;
			cin >> diffuse_alpha_entered;
			if ((diffuse_red_entered > 1 || diffuse_red_entered < 0) || (diffuse_green_entered > 1 || diffuse_green_entered < 0) || (diffuse_blue_entered > 1 || diffuse_blue_entered < 0) || (diffuse_alpha_entered > 1 || diffuse_alpha_entered < 0))
			{
				cout << "The colors must be floating points whose values are between 0 and 1!" << endl;
				cam_moved = false;
			}
			else
			{
				diffuse_red_color = diffuse_red_entered;
				diffuse_green_color = diffuse_green_entered;
				diffuse_blue_color = diffuse_blue_entered;
				diffuse_alpha_color = diffuse_alpha_entered;
				cam_moved = true;
			}
		}

		//Define the RGBA values of Phong specular light
		if (glfwGetKey(window1, GLFW_KEY_F9) || glfwGetKey(window2, GLFW_KEY_F9)) {

			float specular_red_entered, specular_green_entered, specular_blue_entered, specular_alpha_entered;
			cout << "Please enter the value of the red color:" << endl;
			cin >> specular_red_entered;
			cout << "Please enter the value of the green color:" << endl;
			cin >> specular_green_entered;
			cout << "Please enter the value of the blue color:" << endl;
			cin >> specular_blue_entered;
			cout << "Please enter the value of the alpha color:" << endl;
			cin >> specular_alpha_entered;
			if ((specular_red_entered > 1 || specular_red_entered < 0) || (specular_green_entered > 1 || specular_green_entered < 0) || (specular_blue_entered > 1 || specular_blue_entered < 0) || (specular_alpha_entered > 1 || specular_alpha_entered < 0))
			{
				cout << "The colors must be floating points whose values are between 0 and 1!" << endl;
				cam_moved = false;
			}
			else
			{
				specular_red_color = specular_red_entered;
				specular_green_color = specular_green_entered;
				specular_blue_color = specular_blue_entered;
				specular_alpha_color = specular_alpha_entered;
				cam_moved = true;
			}
		}

		//Define all the operations when cam_moved is set to true
		if (cam_moved = true)
		{
			//Update the color vector 
			//glUniform4f(VC_location, red_color, green_color, blue_color, alpha_color);
			//glUniform4f(VC_location, 1.0,0.0,0.0,1.0);

			//update the projection matrix
			float projection_matrix[] = {
				Sx, 0.0f, 0.0f, 0.0f,
				0.0f, Sy, 0.0f, 0.0f,
				0.0f, 0.0f, Sz, -1.0f,
				0.0f, 0.0f, Pz, 0.0f
			};
			glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, projection_matrix);

			//update the translation matrix
			float translation_matrix[] = {
				1.0f,0.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,0.0f,
				0.0f,0.0f,1.0f,0.0f,
				-eye_x,-eye_y,-eye_z,1.0f
			};
			glUniformMatrix4fv(translation_matrix_location, 1, GL_FALSE, translation_matrix);

			//update the light parameters
			glUniform3f(La_location, ambient_red_color, ambient_green_color, ambient_blue_color);
			glUniform3f(Ld_location, diffuse_red_color, diffuse_green_color, diffuse_blue_color);
			glUniform3f(Ls_location, specular_red_color, specular_green_color, specular_blue_color);
		}

		//Enable and Disable the light
		if (glfwGetKey(window1, GLFW_KEY_F3) || glfwGetKey(window2, GLFW_KEY_F3)) {
			lightState = 1.0;
			glUniform4f(lightStateValue_location, lightState, 0.0, 0.0, 0.0);
		}
		if (glfwGetKey(window1, GLFW_KEY_F4) || glfwGetKey(window2, GLFW_KEY_F4)) {
			lightState = 0.0;
			glUniform4f(lightStateValue_location, lightState, 0.0, 0.0, 0.0);
		}

		//Swap Back and forth buffers
		glfwSwapBuffers(window2);
		glfwPollEvents();


	}


	



	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}