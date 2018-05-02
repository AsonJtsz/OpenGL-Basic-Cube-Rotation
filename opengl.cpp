//Header Files
#include <iostream>
using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Global Variables
GLFWwindow* window;
const char* Title="OpenGL Window";
int width= 500,height = 500;

GLuint vbo,vao;
GLuint vs,fs,program;
const char* vertexShader   = 	"#version 330 core\n"
				"layout(location=0) in vec3 pos;\n"
				"layout(location=1) in vec2 texCoord;\n"
				"out vec2 TexCoord;\n"
				"uniform mat4 model,view,projection;\n"
				"void main(){\n"
				"TexCoord = texCoord;"
				"gl_Position = projection*view*model*vec4(pos,1.0f);\n"
				"}";

const char* fragmentShader = 	"#version 330 core\n"
				"in vec2 TexCoord;\n"
				"out vec4 fragcolor;\n"
				"uniform sampler2D myTex;\n"
				"void main(){\n"
				"fragcolor = texture(myTex,TexCoord);\n"
				"}";
GLfloat vertices[] = 	{
				//front
				-1.0f, 1.0f,-1.0f, 0.0f,1.0f,	//top-left
				 1.0f, 1.0f,-1.0f, 1.0f,1.0f,	//top-right
				 1.0f,-1.0f,-1.0f, 1.0f,0.0f,	//bottom-right
				-1.0f, 1.0f,-1.0f, 0.0f,1.0f,	//top-left
				 1.0f,-1.0f,-1.0f, 1.0f,0.0f,	//bottom-right
				-1.0f,-1.0f,-1.0f, 0.0f,0.0f,	//bottom-left

				//back
				-1.0f, 1.0f, 1.0f, 0.0f,1.0f,	//top-left
				 1.0f, 1.0f, 1.0f, 1.0f,1.0f,	//top-right
				 1.0f,-1.0f, 1.0f, 1.0f,0.0f,	//bottom-right
				-1.0f, 1.0f, 1.0f, 0.0f,1.0f,	//top-left
				 1.0f,-1.0f, 1.0f, 1.0f,0.0f,	//bottom-right
				-1.0f,-1.0f, 1.0f, 0.0f,0.0f,	//bottom-left

				//left
				-1.0f, 1.0f,-1.0f, 0.0f,1.0f,	//top-left
				-1.0f, 1.0f, 1.0f, 1.0f,1.0f,	//top-right
				-1.0f,-1.0f, 1.0f, 1.0f,0.0f,	//bottom-right
				-1.0f, 1.0f,-1.0f, 0.0f,1.0f,	//top-left
				-1.0f,-1.0f, 1.0f, 1.0f,0.0f,	//bottom-right
				-1.0f,-1.0f,-1.0f, 0.0f,0.0f,	//bottom-left

				//right

				 1.0f, 1.0f,-1.0f, 0.0f,1.0f,	//top-left
				 1.0f, 1.0f, 1.0f, 1.0f,1.0f,	//top-right
				 1.0f,-1.0f, 1.0f, 1.0f,0.0f,	//bottom-right
				 1.0f, 1.0f,-1.0f, 0.0f,1.0f,	//top-left
				 1.0f,-1.0f, 1.0f, 1.0f,0.0f,	//bottom-right
				 1.0f,-1.0f,-1.0f, 0.0f,0.0f,	//bottom-left

				//top
				-1.0f, 1.0f, 1.0f, 0.0f,1.0f,	//back top-left
				 1.0f, 1.0f, 1.0f, 1.0f,1.0f,	//back top-right
				 1.0f, 1.0f,-1.0f, 1.0f,0.0f,	//front top-right
				-1.0f, 1.0f, 1.0f, 0.0f,1.0f,	//back top-left
				 1.0f, 1.0f,-1.0f, 1.0f,0.0f,	//front top-right
				-1.0f, 1.0f,-1.0f, 0.0f,0.0f,	//front top-left				 
				
				

				//bottom
				-1.0f,-1.0f, 1.0f, 0.0f,1.0f,	//back bottom-left
				 1.0f,-1.0f, 1.0f, 1.0f,1.0f,	//back bottom-right
				 1.0f,-1.0f,-1.0f, 1.0f,0.0f,	//front bottom-right
				-1.0f,-1.0f, 1.0f, 0.0f,1.0f,	//back bottom-left
				 1.0f,-1.0f,-1.0f, 1.0f,0.0f,	//front bottom-right
				-1.0f,-1.0f,-1.0f, 0.0f,0.0f,	//front bottom-left

			};

//Main Program
int main()
{
	//Constructor Code
	if(!glfwInit())
	{
		cerr<<"Error!!GLFW";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	if(!(window = glfwCreateWindow(width,height,Title,NULL,NULL)))
	{
		cerr<<"Error!!GLFW window";
		glfwTerminate();
		return -1;		
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if(glewInit()!=GLEW_OK)
	{
		cerr<<"Error!!GLEW";
		glfwTerminate();
		return -1;
	}

	//Mesh Creation
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),NULL);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Shader Creation
	vs = glCreateShader(GL_VERTEX_SHADER);	
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs,1,&vertexShader,NULL);
	glShaderSource(fs,1,&fragmentShader,NULL);
	glCompileShader(vs);
	glCompileShader(fs);
	int result;
	GLchar infoLog[1024];
	glGetShaderiv(vs,GL_COMPILE_STATUS,&result);
	if(!result)
	{
		glGetShaderInfoLog(vs,sizeof(infoLog),NULL,infoLog);
		cerr<<"Vertex Shader : Error !! "<<infoLog;
	}
	glGetShaderiv(fs,GL_COMPILE_STATUS,&result);
	if(!result)
	{
		glGetShaderInfoLog(fs,sizeof(infoLog),NULL,infoLog);
		cerr<<"Fragment Shader : Error !! "<<infoLog;
	}
	program = glCreateProgram();
	glAttachShader(program,vs);
	glAttachShader(program,fs);
	glLinkProgram(program);
	glGetProgramiv(fs,GL_LINK_STATUS,&result);
	if(!result)
	{
		glGetProgramInfoLog(fs,sizeof(infoLog),NULL,infoLog);
		cerr<<"Shader Program : Error !! "<<infoLog;
	}

	//Textures Uploading and Creation
	GLint imageWidth,imageHeight,components;
	unsigned char* data;
	data = stbi_load("crate.jpeg",&imageWidth,&imageHeight,&components,STBI_rgb_alpha);
	if(data==NULL)
	{
		cout<<"Texture Error!!"<<endl;
		return -1;
	}
	GLuint texId;
	glGenTextures(1,&texId);
	glBindTexture(GL_TEXTURE_2D,texId);
	glTextureParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imageWidth,imageHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);


	//MVP matrix and ( Cube Position and Rotation ) 
	mat4 model,view,projection;
	view 	   = lookAt(vec3(0.0f,0.0f,0.0f),vec3(0.0f,0.0f,-1.0f),vec3(1.0f,1.0f,0.0f));
	projection = perspective(radians(45.0f),(float)width/(float)height,0.1f,100.0f);
	float cubeAngle = 0.0f;
	vec3 cubePos(0.0f,0.0f,-5.0f);
	//Loop Events
	while(!glfwWindowShouldClose(window))
	{
		//Calculation
		cubeAngle+=1.0f;
		if(cubeAngle>=360) cubeAngle=0;
		model = mat4(1.0f);
		model = translate(model,cubePos)*rotate(model,radians(cubeAngle),vec3(1.0f,0.0f,0.0f));
		//Rendering
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f,0.3f,0.6f,1.0f);
		glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D,texId);
		glUseProgram(program);
			//Assign Shader Uniforms
		glUniformMatrix4fv(glGetUniformLocation(program,"model"),1,GL_FALSE,&model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program,"view"),1,GL_FALSE,&view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program,"projection"),1,GL_FALSE,&projection[0][0]);
		glDrawArrays(GL_TRIANGLES,0,36);	//Draw Number of vertices at a time of loop
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
	}

	//Destructor Code
	glDetachShader(program,vs);	
	glDetachShader(program,fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(program);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDeleteVertexArrays(1,&vao);
	glDeleteBuffers(1,&vbo);
	glfwTerminate();
	return 0;

}//End of Main Program

