#include "Angel.h"
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
//void keyboard( unsigned char key, int x, int y );
typedef vec4 point4;
typedef vec4 color4;
using namespace std;
// Số các đỉnh của các tam giác
const int NumPoints = 36;
point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập
phương*/
GLuint program;
// Các tham số cho viewing
GLfloat radius = 1, theta = 0, phi = 0;
const GLfloat dr = 5.0 * DegreesToRadians;
GLuint model_view_loc;
// Các tham số cho projection

GLuint projection_loc;
void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices[7] = point4(0.5, -0.5, -0.5, 1.0);
	// Gán giá trị màu sắc cho các đỉnh của hình lập phương
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 0.7, 0, 1.0); // orange
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int Index = 0;
void quad(int a, int b, int c, int d) /*Tạo một mặt hình lập phương = 2 tam giác,
gán màu cho mỗi đỉnh tương ứng trong mảng colors*/

{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
}
void makeColorCube(void) /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void generateGeometry(void)
{
	initCube();
	makeColorCube();
}
void initGPUBuffers(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL,
		GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
}
void shaderSetup(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl"); // hàm InitShader
	//khai báo trong Angel.h
		glUseProgram(program);
	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	model_view_loc = glGetUniformLocation(program, "model");
	projection_loc = glGetUniformLocation(program, "projection");
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0); /* Thiết lập màu trắng là màu xóa
	màn hình*/
}


mat4 ctm;
mat4 instance_ban;
float dai = 1.2, rong = 0.8, cao = 0.05;
float cb = 0.1 * dai;
GLfloat td[] = { -30,30,0 };
float truotx = 0, truoty = 0, truotz = 0;
void toado(void)
{
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0, 0.0f);

	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(0.0f, 0, 0.0f);


	glVertex3f(0.0f, 0.0f, 2.0f);
	glVertex3f(0.0f, 0, 0.0f);

	glEnd();
	glFlush();
}
void chanban(float x, float y, float z)
{
	mat4 instance;
	// truoc phải
	instance = Translate(x, y, z) * Scale(cb, 1, cb);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctm * instance_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


}
void ban(float d, float r, float c)
{
	mat4 instance;
	// ban
	instance = Scale(d, c, r);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctm * instance_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//chanban(truotx,truoty,truotz);
	chanban(0.5 * d - cb * 0.5, -(0.5 * 1 - 0.5 * c), 0.5 * r - 0.5 * cb);
	chanban(0.5 * d - cb * 0.5, -(0.5 * 1 - 0.5 * c), -(0.5 * r - 0.5 * cb));
	chanban(-(0.5 * d - cb * 0.5), -(0.5 * 1 - 0.5 * c), -(0.5 * r - 0.5 * cb));
	chanban(-(0.5 * d - cb * 0.5), -(0.5 * 1 - 0.5 * c), 0.5 * r - 0.5 * cb);
}
mat4 viewing;
mat4 projection;
GLfloat l = -3, r = 3;
GLfloat bottom = -4, top = 4;
GLfloat zNear = 1, zFar = 5;
point4 P(2,3,4,1);
point4 P1;
point4 P_phay;
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*Thiết lập phép chiếu*/
	/*point4 eye(radius*sin(theta)*cos(phi),
	radius*sin(theta)*sin(phi),
	radius*cos(theta),
	1.0);*/
	point4 eye(0, 0, 6, 1);
	point4 at(0.0, 0.0, 0.0, 1.0);
	vec4 up(0.0, 1.0, 0.0, 1.0);
	 ctm = LookAt(eye, at, up);
	 viewing = ctm;
	 

	instance_ban = identity();
	toado();
	ban(dai, rong, cao);

	
	mat4 chieu = Frustum(l, r, bottom, top, zNear, zFar);

	projection = chieu;
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, chieu);


	P1 = projection * viewing *P;
	P_phay = P1 / P1.w;




	/*Vẽ hình*/
	
	/*Vẽ các tam giác*/
	
	glDrawArrays(GL_TRIANGLES, 0, NumPoints); /*Vẽ các tam giác*/
	glutSwapBuffers();
}

void viewMatrix(mat4 c)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << c[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}
void viewPoint(point4 p)
{
	cout << "P_phay " << " = " << p.x << ", " << p.y << ", " << p.z << ", " << p.w << endl;;
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	

	case '1':
		if (td[0] > 360)
			td[0] = td[0] - 360;
		td[0] += 5;
		glutPostRedisplay();
		cout << "X = " << td[0] << " Y = " << td[1] << " Z = " << td[2] << endl;
		cout << "a" << endl;;
		viewMatrix(viewing);
		viewMatrix(projection);
		viewPoint(P1);
		viewPoint(P_phay);
		break;
	case '2':
		if (td[1] > 360)
			td[1] = td[1] - 360;
		td[1] += 5;
		glutPostRedisplay();
		cout << "X = " << td[0] << " Y = " << td[1] << " Z = " << td[2] << endl;
		break;
}
glutPostRedisplay();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}
int main(int argc, char** argv)
{
	// main function: program starts here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Drawing a Cube");
	glewInit();
	
	generateGeometry();
	initGPUBuffers();
	shaderSetup();
	glutDisplayFunc(display);
	glutKeyboardFunc( keyboard );
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
