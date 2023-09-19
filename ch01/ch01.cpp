#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define BUFFER_OFFSET(a) ((void*)(a))



enum VAO_IDs {Triangles,NumVAOs};
enum Buffer_IDs {ArrayBuffer,NumBuffers};
enum Attrib_IDs {vPosition = 0};

typedef unsigned int uint; // == GLuint
			   //
typedef struct {
    GLenum       type;
    const char*  filename;
    GLuint       shader;
} ShaderInfo;

void init();
uint LoadShaders(ShaderInfo *shaders);
static const char* ReadShader(const char *filename);



uint VAOs[NumVAOs];
uint Buffers[NumBuffers];

const uint NumVertices = 6;



int main(void){

glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


GLFWwindow *window = glfwCreateWindow(800,600,"ch01",NULL,NULL);
glfwMakeContextCurrent(window);

gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);



while(!glfwWindowShouldClose(window)){
glClearColor(0.3f,.2f,.3f,1.0f);
glClear(GL_COLOR_BUFFER_BIT);

init();
glBindVertexArray(VAOs[Triangles]);
glDrawArrays(GL_TRIANGLES,0,NumVertices);


glfwSwapBuffers(window);
glfwPollEvents();
}







}









void init(){
glGenVertexArrays(NumVAOs,VAOs);
glBindVertexArray(VAOs[Triangles]);

float vertices[NumVertices] [2] = {
	{ -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
        {  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f }   // Triangle 2
};

glGenBuffers(NumBuffers,Buffers);
glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);



ShaderInfo shaders[] = {
	{GL_VERTEX_SHADER,"./triangles.vert"},
	{GL_FRAGMENT_SHADER,"./triangles.frag"},
	{GL_NONE,NULL}
};

uint program = LoadShaders(shaders);

glUseProgram(program);

glVertexAttribPointer(vPosition,2,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
glEnableVertexAttribArray(vPosition);
}




























uint LoadShaders(ShaderInfo *shaders){

uint program = glCreateProgram();
ShaderInfo *entry = shaders;

while(entry->type != GL_NONE){
	uint shader = glCreateShader(entry->type);
	entry->shader = shader;


	const char *source = ReadShader(entry->filename);

	if (source == NULL){
		for(entry = shaders;entry->type!=GL_NONE;++entry){
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}
		return 0;
	}
glShaderSource(shader,1,&source,NULL);
delete []source;
glCompileShader(shader);

glAttachShader(program,shader);
++entry;
}

glLinkProgram(program);




return program;
}


static const char* ReadShader(const char* filename){
FILE *infile = fopen(filename,"rb");
fseek(infile,0,SEEK_END);
int len = ftell(infile);
fseek(infile,0,SEEK_SET);

char *source = new char[len+1];
fread(source,1,len,infile);
fclose(infile);

source[len] = 0;

return const_cast<const char*>(source);
}







