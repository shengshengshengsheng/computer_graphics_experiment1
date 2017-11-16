//改编自书上源码5.3
//  Display a rotating cube with lighting

#include "objLoader.h"
#include<gl\glaux.h>

typedef Angel::vec4  color4;
typedef Angel::vec3  point3;
typedef Angel::vec4  point4;
// Array of rotation angles (in degrees) for each coordinate axis
objLoader objfile;
int vSize, vnSize, fSize;
// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

// OpenGL initialization
void
init()
{
	if (objfile.loadobj("cube.obj") == false)
		exit(0);
	vSize = objfile.vArr.size();
	vnSize = objfile.vnArr.size();
	fSize = objfile.f.size();
	point3* points;
	point3* normals;
	vec3 temp;
	points = new point3[fSize * 3];
	normals = new point3[fSize * 3];
	for (int i = 0, j = 0; i < fSize; i++)
	{
		temp = objfile.f[i].V;
		points[j] = objfile.vArr[temp.x - 1]; normals[j] = objfile.vnArr[temp.x - 1];
		points[j].x *= 0.5; points[j].y *= 0.5; points[j].z *= 0.5; j++;

		points[j] = objfile.vArr[temp.y - 1]; normals[j] = objfile.vnArr[temp.x - 1];
		points[j].x *= 0.5; points[j].y *= 0.5; points[j].z *= 0.5; j++;

		points[j] = objfile.vArr[temp.z - 1]; normals[j] = objfile.vnArr[temp.x - 1];
		points[j].x *= 0.5; points[j].y *= 0.5; points[j].z *= 0.5; j++;
	}
	int size = sizeof(point3) * fSize * 3;

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, size * 2, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, size, &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, size, size, &normals[0]);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader53.glsl", "fshader53.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vposition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vposition);
	glVertexAttribPointer(vposition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(size));

	// Initialize shader lighting parameters
	point4 light_position(0.0, 0.0, -1.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(1.0, 0.0, 1.0, 1.0);
	color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
	color4 material_specular(1.0, 0.8, 0.0, 1.0);
	float  material_shininess = 100.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product);

	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
		1, light_position);

	glUniform1f(glGetUniformLocation(program, "Shininess"),
		material_shininess);

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
	glShadeModel(GL_FLAT);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//  Generate tha model-view matrixn

	const vec3 viewer_pos(0.0, 0.0, 2.0);
	mat4  model_view = (Translate(-viewer_pos) *
		RotateX(Theta[Xaxis]) *
		RotateY(Theta[Yaxis]) *
		RotateZ(Theta[Zaxis]));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
	//	glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(4, GL_FLOAT, 0, points);
	glDrawArrays(GL_TRIANGLES, 0, fSize * 3);
	//glDisableClientState(GL_VERTEX_ARRAY);
	glutSwapBuffers();
}
//----------------------------------------------------------------------------

void
mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
		case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
		case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		}
	}
}
//----------------------------------------------------------------------------

void
idle(void)
{
	Theta[Axis] += 0.05;

	if (Theta[Axis] > 360.0) {
		Theta[Axis] -= 360.0;
	}

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;
	case 'w': case 'W':
		Theta[Xaxis] += 1;
		if (Theta[Xaxis] > 360.0) {
			Theta[Xaxis] -= 360.0;
		}
		glutPostRedisplay();
		break;
	case 's':case'S':
		Theta[Xaxis] -= 1;
		if (Theta[Xaxis] <0) {
			Theta[Xaxis] += 360.0;
		}
		glutPostRedisplay();
		break;
	case 'a':case'A':
		Theta[Yaxis] -= 1;
		if (Theta[Yaxis] <0) {
			Theta[Yaxis] += 360.0;
		}
		glutPostRedisplay();
		break;
	case 'd':case'D':
		Theta[Yaxis] += 1;
		if (Theta[Yaxis] >360.0) {
			Theta[Yaxis] -= 360.0;
		}
		glutPostRedisplay();
		break;
	case 'q':case'Q':
		Theta[Zaxis] -= 1;
		if (Theta[Zaxis] <0) {
			Theta[Zaxis] += 360.0;
		}
		glutPostRedisplay();
		break;
	case 'e':case'E':
		Theta[Zaxis] += 1;
		if (Theta[Zaxis] >360.0) {
			Theta[Zaxis] -= 360.0;
		}
		glutPostRedisplay();
		break;
	}
}
//----------------------------------------------------------------------------

void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat aspect = GLfloat(width) / height;
	mat4  projection = Perspective(45.0, aspect, 0.05, 3.0);

	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("cube");
	glewExperimental = true;
	glewInit();
	init();
	glutDisplayFunc(display);
	//键盘控制
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	//鼠标控制
	//glutMouseFunc(mouse);
	//glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
