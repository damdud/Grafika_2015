#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
GLuint shaderProgramID;
GLuint vao = 0;
GLuint vbo;
GLuint positionID, colorID;

#pragma region Funkcje_Shader
static char* readFile(const char* filename)
{
	//otwórz plik
	FILE* fp = fopen (filename, "r");

	fseek(fp, 0, SEEK_END);
	long file_length = ftell(fp);
	fseek (fp, 0, SEEK_SET);
	char* contents = new char [file_length+1];

	for (int i = 0; i < file_length+1; i++)
	{
		contents[i] = 0;
	}

	fread (contents, 1, file_length, fp);

	contents[file_length+1] = '\0';
	fclose(fp);
	return contents;
}

bool compiledStatus(GLint shaderID)
{
	GLint compiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled)
	{
		return true;
	}
	else
	{
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* msgBuffer = new char[logLength];
		glGetShaderInfoLog(shaderID, logLength, NULL, msgBuffer);
		printf ("%s\n", msgBuffer);
		delete (msgBuffer);
		return false;
	}
}


GLuint makeVertexShader(const char* shaderSource)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource (vertexShaderID, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader (vertexShaderID);
	bool compiledCorrectly = compiledStatus(vertexShaderID);
	if (compiledCorrectly)
	{
		return vertexShaderID;
	}
	return -1;
}

GLuint makeFragmentShader (const char* shaderSource) 
{
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(fragmentShaderID);
	bool compiledCorrectly = compiledStatus(fragmentShaderID);
	if (compiledCorrectly)
	{
		return fragmentShaderID;
	}
	return -1;
}

GLuint makeShaderProgram (GLuint vertexShaderID, GLuint fragmentShaderID)
{
	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShaderID);
	glAttachShader(shaderID, fragmentShaderID);
	glLinkProgram(shaderID);
	return shaderID;
}
#pragma endregion Funkcja_Shader

void changeViewport(int w, int h)
{
	glViewport(0, 0, w, h);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
}

int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Shaders");
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glewInit();

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	char* vertexShaderSourceCode = readFile("vertexShader.vsh");
	char* fragmentShaderSourceCode = readFile("fragmentShader.fsh");
	GLuint vertShaderID = makeVertexShader(vertexShaderSourceCode);
	GLuint fragShaderID = makeFragmentShader(fragmentShaderSourceCode);
	GLuint shaderProgramID = makeShaderProgram(vertShaderID, fragShaderID);
	
	printf ("vertShaderID jest %d\n", vertShaderID);
	printf ("fragShaderID jest %d\n", fragShaderID);
	printf ("shaderProgramID jest %d\n", shaderProgramID);
	printf ("s_vPosition's ID jest %d\n", positionID);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//tworzy pusty bufor
	glBufferData(GL_ARRAY_BUFFER, 7*3*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//³adowanie punktów
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3*3*sizeof(GLfloat), vertices);
	//³adowanie kolorów
	glBufferSubData(GL_ARRAY_BUFFER, 3*3*sizeof(GLfloat), 3*4*sizeof(GLfloat), colors);

	positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
	colorID = glGetAttribLocation(shaderProgramID, "s_vColor");

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(3*3*sizeof(GLfloat)));
	glUseProgram(shaderProgramID);
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);

	glutMainLoop();

	return 0;
}
