//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/


// remember to prototype
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );


typedef vec4 point4;
typedef vec4 color4;
using namespace std;

// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/


point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;
GLuint model_loc, projection_loc;
mat4 ctm;
mat4 model;

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
	vertex_colors[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int Index = 0;
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void generateGeometry( void )
{
	initCube();
	makeColorCube();
}


void initGPUBuffers( void )
{
	// Tạo một VAO - vertex array object
	GLuint vao;
    glGenVertexArrays( 1, &vao );     
    glBindVertexArray( vao );

    // Tạo và khởi tạo một buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW );

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors); 
	
}


void shaderSetup( void )
{
	// Nạp các shader và sử dụng chương trình shader
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );   // hàm InitShader khai báo trong Angel.h
    glUseProgram( program );

    // Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
    GLuint loc_vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( loc_vPosition );
    glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
	
	model_loc = glGetUniformLocation(program, "model");
	projection_loc = glGetUniformLocation(program, "projection");
	glEnable(GL_DEPTH_TEST);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );        /* Thiết lập màu trắng là màu xóa màn hình*/
}

GLfloat l_ban = 1, t_ban = 0.05, w_ban = 0.4;
GLfloat w_cb = 0.05, h_cb = 0.6;
void ban() 
{
	mat4 m;
	//Chan ban
	m = Translate(-0.5 * (l_ban - w_cb), -0.5 *(t_ban + h_cb), 0.5 * (w_ban - w_cb)) * Scale(w_cb, h_cb, w_cb);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(-0.5 * (l_ban - w_cb), -0.5 *(t_ban + h_cb), -0.5 * (w_ban - w_cb)) * Scale(w_cb, h_cb, w_cb);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(0.5 * (l_ban - w_cb), -0.5 *(t_ban + h_cb), 0.5 * (w_ban - w_cb)) * Scale(w_cb, h_cb, w_cb);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(0.5 * (l_ban - w_cb), -0.5 *(t_ban + h_cb), -0.5 * (w_ban - w_cb)) * Scale(w_cb, h_cb, w_cb);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// Mat ban
	m = Scale(l_ban, t_ban, w_ban);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

GLfloat l_nk = 0.5, t_nk = 0.04, w_nk = 0.4;
void ngankeo() 
{
	mat4 m;

	// Mat day
	m = /*Translate(0, -0.1 + -0.5*(t_ban + t_nk), 0) */ Scale(l_nk, t_nk, w_nk);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// Hai ben
	m = Translate(-0.5*(l_nk - 0.02), 0.5*(t_nk + 0.1), 0) * Scale(0.02, 0.1, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(0.5*(l_nk - 0.02), 0.5*(t_nk + 0.1), 0) * Scale(0.02, 0.1, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// Hai day
	m = Translate(0,0.5*(0.1 + t_nk),0.5*(w_nk - 0.02))*Scale(0.46, 0.1, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(0, 0.5*(0.1 + t_nk), -0.5*(w_nk - 0.02))*Scale(0.46, 0.1, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// Núm
	m = Translate(0, 0.5*(0.04 + 0.1), 0.5*(0.4 + 0.02)) * Scale(0.2, 0.02, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

GLfloat xx = 0, yy = 0, zz = 0, a = 0, b = 0, c = 0, z_nk = 0;

mat4 mv;

void banthaydoi()
{
	ctm = mv * Translate(xx, yy, zz) * RotateX(a) * RotateY(b) * RotateZ(c);
	ban();
	ctm = ctm * Translate(0, 0, z_nk)*Translate(0, -0.1 + -0.5*(t_ban + t_nk), 0);
	ngankeo();
}

GLfloat l = -0.5, r = 0.5;
GLfloat bottom = -0.5, top = 0.5;
GLfloat zNear = 0.5, zFar = 3.0;

void display( void )
{
	point4 eye(1, 1, 1.5, 1.0);
	point4 at(0.0, 0.0, 0.0, 1.0);
	vec4 up(0.0, 1.0, 0.0, 1.0);

	mv = LookAt(eye, at, up);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, mv);

	mat4 p = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	banthaydoi();
    //glDrawArrays( GL_TRIANGLES, 0, NumPoints );    /*Vẽ các tam giác*/
	glutSwapBuffers();									   
}


void keyboard( unsigned char key, int x, int y )
{
	// keyboard handler

    switch ( key ) {
    case 033:			// 033 is Escape key octal value
        exit(1);		// quit program
        break;
	// Di chuyen ban
	case 'x':
		xx = xx + 0.1;
		glutPostRedisplay();
		break;
	case 'X':
		xx = xx - 0.1;
		glutPostRedisplay();
		break;
	case 'y':
		yy = yy + 0.1;
		glutPostRedisplay();
		break;
	case 'Y':
		yy = yy - 0.1;
		glutPostRedisplay();
		break;
	case 'z':
		zz = zz + 0.1;
		glutPostRedisplay();
		break;
	case 'Z':
		zz = zz - 0.1;
		glutPostRedisplay();
		break;
	// Xoay ban
	case 'a': a += 5; // dieu khien ban quay
		if (a > 360) a = a - 360;
		glutPostRedisplay();
		break;
	case 'A': a -= 5; // dieu khien ban quay
		if (a < -360) a = a + 360;
		glutPostRedisplay();
		break;
	case 'b': b += 5; // dieu khien ban quay
		if (b >= 360) b = b - 360;
		glutPostRedisplay();
		break;
	case 'B': b -= 5; // dieu khien ban quay
		if (b < -360) b = b + 360;
		glutPostRedisplay();
		break;
	case 'c': c += 5; // dieu khien ban quay
		if (c >= 360) c = c - 360;
		glutPostRedisplay();
		break;
	case 'C': c -= 5; // dieu khien ban quay
		if (c < -360) c = c + 360;
		glutPostRedisplay();
		break;
	// Dieu khien ngan keo
	case 'n': z_nk += 0.1; // dieu khien ngan keo
		if (z_nk >= (w_ban - 0.02)) z_nk = (w_ban - 0.02);
		glutPostRedisplay();
		break;
	case 'N': z_nk -= 0.1; // dieu khien ngan keo
		if (z_nk <= 0) z_nk = 0;
		glutPostRedisplay();
		break;
    }
}


int main( int argc, char **argv )
{
	// main function: program starts here

    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize( 640, 640 );                 
	glutInitWindowPosition(100,150);               
    glutCreateWindow( "Drawing a Cube" );            

   
	glewInit();										

    generateGeometry( );                          
    initGPUBuffers( );							   
    shaderSetup( );                               

    glutDisplayFunc( display );                   
    glutKeyboardFunc( keyboard );                  

	glutMainLoop();
    return 0;
}
