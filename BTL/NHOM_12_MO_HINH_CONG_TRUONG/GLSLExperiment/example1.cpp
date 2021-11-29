//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/
// Hop nhat

// remember to prototype
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);


typedef vec4 point4;
typedef vec4 color4;
using namespace std;

// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/


point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/
vec3 normals[5000];
GLuint program, model_loc, projection_loc, view_loc;

GLfloat l = -1.0, r = 1.0;
GLfloat bottom = -1.0, top = 1.0;
GLfloat zNear = 1, zFar = 4;


//GLfloat zNear = 3.5, zFar = 15;
// Max mau
vec4 black = color4(0.0, 0.0, 0.0, 1.0); // black
vec4 red = color4(1.0, 0.0, 0.0, 1.0); // red
vec4 yellow = color4(1.0, 1.0, 0.0, 1.0); // yellow
vec4 green = color4(0.0, 1.0, 0.0, 1.0); // green
vec4 blue = color4(0.0, 0.0, 1.0, 1.0); // blue
vec4 magenta = color4(1.0, 0.0, 1.0, 1.0); // magenta
vec4 white = color4(1.0, 1.0, 1.0, 1.0); // white
vec4 cyan = color4(0.0, 1.0, 1.0, 1.0); // cyan



/* Khởi tạo các tham số chiếu sáng - tô bóng*/
point4 light_position(0.0, 0.0, 1.0, 0.0);
color4 light_ambient(0.2, 0.2, 0.2, 1.0);
color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
color4 light_specular(1.0, 1.0, 1.0, 1.0);

color4 material_ambient(1.0, 1.0, 1.0, 1.0);
color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
color4 material_specular(1.0, 0.8, 0.0, 1.0);
float material_shininess = 100.0;

color4 ambient_product = light_ambient * material_ambient;
color4 diffuse_product = light_diffuse * material_diffuse;
color4 specular_product = light_specular * material_specular;



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
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);
}

void shaderSetup(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);


	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}


/// VY VĂN HÙNG -2019601093 - Mô hình máy cẩu

/// Bat dau Vex May cau

mat4 model_may_cau;
mat4 mv;

mat4 instance_may_cau;
mat4 model_khuan_may_cau; // phep bien doi co ban trên bộ phận máy cẩu
GLfloat goc_quay_x_may_cau = 20, goc_quay_y_may_cau = 0, goc_quay_z_may_cau = 0, goc_quay_banh_xe_may_cau = 0, goc_quay_truc_cau = -45;
GLfloat dich_x_may_cau = -1, dich_y_may_cau = -2, dich_z_may_cau = 0; // dịch xyz
GLfloat goc_quay_canh_cua_may_cau = 0;

GLfloat ti_le_may_cau = 0.4;   // chiều dài tượng trưng cho sàn máy cẩu
GLfloat cr_may_cau = ti_le_may_cau * 1.2;  // chiều rộng  tượng trưng cho sàn máy cẩu
GLfloat cr_san_may_cau = ti_le_may_cau * 1.8;
GLfloat cd_may_cau = ti_le_may_cau * 4;   // chiều dài = 3 rộng  tượng trưng cho sàn máy cẩu
GLfloat do_day_vat_lieu_may_cau = ti_le_may_cau * 0.1;  // độ dày các miếng ván

GLfloat cr_dau_may_cau = cr_san_may_cau; // cho đầu rộng hơn sàn
GLfloat cd_dau_may_cau = cr_san_may_cau;
GLfloat cc_dau_may_cau = cr_may_cau * 0.8; // chiều cao đầu máy cẩu

GLfloat cr_zip_gam_xe_truoc = cr_may_cau * 0.15;
GLfloat cd_zip_gam_xe_truoc = cd_may_cau / 4;

GLfloat cr_zip_gam_xe_sau = cr_may_cau * 0.15;
GLfloat cd_zip_gam_xe_sau = cd_may_cau / 4 * 2;
GLfloat cc_zip_gam_xe = do_day_vat_lieu_may_cau * 0.5;

GLfloat dg_kinh_banh_xe_may_cau = cr_may_cau / 3;
GLfloat cc__banh_xe_may_cau = cr_may_cau / 3;


GLfloat cc_thung_hang_may_cau = cr_may_cau / 3; // chiều cao  gia do hang sau thung
// truc cau
GLfloat cc_truc_cau_may_cau = cd_may_cau * 1.2;
GLfloat cr_truc_cau_may_cau = cr_dau_may_cau / 4;
void khuan_may_cau(GLfloat w, GLfloat h, GLfloat l, vec4 color)
{
	//instance = Translate(0,h/2,0)*Scale(w, h, l);
	//color4 material_diffuse_t(0, 1, 0.0, 1);  // mau vat
	material_diffuse = color;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

	instance_may_cau = Scale(w, h, l);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, mv * model_may_cau * model_khuan_may_cau * instance_may_cau);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}


void khuan_zip_xe(GLfloat w, GLfloat h, GLfloat l, GLfloat cr_zip_gam_xe, GLfloat cd_zip_gam_xe)
// khuan tạo thanh zip trên xe
{
	mat4 vitri = Translate(w, h, l);

	model_khuan_may_cau = vitri * Translate(0, -do_day_vat_lieu_may_cau, 0);
	khuan_may_cau(cd_zip_gam_xe, cc_zip_gam_xe, cr_zip_gam_xe, cyan); // thanh zip dai nhat

	model_khuan_may_cau = vitri * Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe), 0);    // giảm chiều dài *0.8, dịch lên trên thanh dai nhat
	khuan_may_cau(cd_zip_gam_xe * 0.8, cc_zip_gam_xe, cr_zip_gam_xe, cyan); // thanh zip t2

	model_khuan_may_cau = vitri * Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe * 2), 0);
	khuan_may_cau(cd_zip_gam_xe * 0.7, cc_zip_gam_xe, cr_zip_gam_xe, cyan); //

	model_khuan_may_cau = vitri * Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe * 3), 0);
	khuan_may_cau(cd_zip_gam_xe * 0.6, cc_zip_gam_xe, cr_zip_gam_xe, cyan); // thanh zip ngan nhat

	model_khuan_may_cau = vitri * Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe * 3), 0);
	khuan_may_cau(cr_zip_gam_xe + 0.01, cc_zip_gam_xe, cr_zip_gam_xe, cyan); // thanh zip ngan nhat
}
void khuan_truc_banh_xe(GLfloat w, GLfloat h, GLfloat l) /// 3 truc gan 6 banh xe
{
	mat4 vitri = Translate(w, h, l);
	model_khuan_may_cau = vitri * Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe * 5), 0) * RotateY(90); // dịch truc ve 2 zip 
	khuan_may_cau(cr_may_cau + 0.05, cc_zip_gam_xe, cc_zip_gam_xe, black);
}

void khuan_banh_xe(GLfloat w, GLfloat h, GLfloat l, int n, bool isLeft) // n la so hinh
{
	mat4 vitri = Translate(w, h, l);
	if (isLeft)
	{
		model_khuan_may_cau = vitri * Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe * 5), dg_kinh_banh_xe_may_cau / 2) * RotateZ(goc_quay_banh_xe_may_cau);  // nan xe ben trái
		khuan_may_cau(dg_kinh_banh_xe_may_cau / 10, dg_kinh_banh_xe_may_cau / 2, 0.005, white);

		model_khuan_may_cau = vitri * Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe * 5), dg_kinh_banh_xe_may_cau / 2) * RotateZ(goc_quay_banh_xe_may_cau + 90);  // nan xe ben trái +90 tao hinh chu thap
		khuan_may_cau(dg_kinh_banh_xe_may_cau / 10, dg_kinh_banh_xe_may_cau / 2, 0.005, white);
	}
	else
	{
		model_khuan_may_cau = vitri * Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe * 5), -dg_kinh_banh_xe_may_cau / 2) * RotateZ(goc_quay_banh_xe_may_cau);  //nan xe ben phai
		khuan_may_cau(dg_kinh_banh_xe_may_cau / 10, dg_kinh_banh_xe_may_cau / 2, 0.005, white);

		model_khuan_may_cau = vitri * Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe * 5), -dg_kinh_banh_xe_may_cau / 2) * RotateZ(goc_quay_banh_xe_may_cau + 90);  //nan xe ben phai+90 tao hinh chu thap
		khuan_may_cau(dg_kinh_banh_xe_may_cau / 10, dg_kinh_banh_xe_may_cau / 2, 0.005, white);
		khuan_may_cau(dg_kinh_banh_xe_may_cau / 10, dg_kinh_banh_xe_may_cau / 2, 0.005, white);

	}

	for (int i = 0; i < n; i++)
	{
		int rotate = 360 / n * i;
		model_khuan_may_cau = vitri * Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe * 5), 0) * RotateZ(rotate + goc_quay_banh_xe_may_cau);  // dich truc x 1 doan bang voi zip be nhat //
		khuan_may_cau(dg_kinh_banh_xe_may_cau, dg_kinh_banh_xe_may_cau, cc__banh_xe_may_cau, black); // thanh zip dai nhat
	}
}
void khuan_gam_xe()
{
	GLfloat dichXvesau = -cd_may_cau / 2 - cd_zip_gam_xe_truoc / 2;   /// tac dung dich toan bo khung banh xe truoc
	GLfloat dichZvetruoc = -cr_may_cau / 2 + cr_zip_gam_xe_truoc / 2;
	// zip xe sau sau
	khuan_zip_xe(-dichXvesau, 0, dichZvetruoc, cr_zip_gam_xe_truoc, cd_zip_gam_xe_truoc); // zip xe sau truoc
	khuan_zip_xe(-dichXvesau, 0, -dichZvetruoc, cr_zip_gam_xe_truoc, cd_zip_gam_xe_truoc); // zip xe sau truoc

	khuan_truc_banh_xe(-dichXvesau, 0, 0);
	khuan_banh_xe(-dichXvesau, 0, (dichZvetruoc - 0.055), 100, false);
	khuan_banh_xe(-dichXvesau, 0, -(dichZvetruoc - 0.055), 100, true);

	//// BỘ TRUC XE SAU
	// tang kich thuoc cua zip de banh sau cho 2 truc   ?? VI Dang sau co 2 truc nen phải gap doi cac thong so nen

	GLfloat dichXvesau_banh_sau = -cd_may_cau / 2 + cd_zip_gam_xe_sau / 2; /// tac dung dich toan bo khung banh xe sau
	GLfloat dichZvetruoc_banh_sau = -cr_may_cau / 2 + cr_zip_gam_xe_sau / 2;
	khuan_zip_xe(dichXvesau_banh_sau, 0, dichZvetruoc_banh_sau, cr_zip_gam_xe_sau, cd_zip_gam_xe_sau); // zip xe sau sau
	khuan_zip_xe(dichXvesau_banh_sau, 0, -dichZvetruoc_banh_sau, cr_zip_gam_xe_sau, cd_zip_gam_xe_sau);

	// trucbanh xe cuoi
	khuan_truc_banh_xe(dichXvesau_banh_sau - cd_zip_gam_xe_sau / 4, 0, 0);   // chia đôi zip để tao 2 truc banh xe

	khuan_banh_xe(dichXvesau_banh_sau - cd_zip_gam_xe_sau / 4, 0, (dichZvetruoc_banh_sau - 0.055), 100, false);   // banh xe cuoi   them 0.055 vi de banh xe lui ra ngoai ko cham vao truc
	khuan_banh_xe(dichXvesau_banh_sau - cd_zip_gam_xe_sau / 4, 0, -(dichZvetruoc_banh_sau - 0.055), 100, true);  // banh xe cuoi 

	// truc banh xe giua
	khuan_truc_banh_xe(dichXvesau_banh_sau + cd_zip_gam_xe_sau / 5, 0, 0);
	khuan_banh_xe(dichXvesau_banh_sau + cd_zip_gam_xe_sau / 5, 0, (dichZvetruoc_banh_sau - 0.055), 100, false);   // banh xe giua
	khuan_banh_xe(dichXvesau_banh_sau + cd_zip_gam_xe_sau / 5, 0, -(dichZvetruoc_banh_sau - 0.055), 100, true);

	// ĐỘNG CƠ   // thong so đong co cd = cr = (cd_dau_may_cau/3 , cc = dg_kinh_banh_xe_may_cau/1.5
	model_khuan_may_cau = Translate(cd_dau_may_cau / 2, -dg_kinh_banh_xe_may_cau / 1.5, 0); // Dong co 1 hinh vuong
	khuan_may_cau(cd_dau_may_cau / 3, dg_kinh_banh_xe_may_cau / 1.5 - 0.01, cd_dau_may_cau / 3, green);     // tru 0.01 cho thap xuong so voi dong co hình chữ nhập
	model_khuan_may_cau = Translate(cd_dau_may_cau / 2 + cd_dau_may_cau / 3 / 2, -dg_kinh_banh_xe_may_cau / 1.5, 0) * RotateY(90); // Dong co 1 hinh chu nhat // cd_dau_may_cau / 3/2 la dich len 1 nua dong co hinhg vuong // xoay 90 cho thanh hcn
	khuan_may_cau(cd_dau_may_cau / 2, dg_kinh_banh_xe_may_cau / 1.5, cd_dau_may_cau / 3, green);

	// Khuân nối 2 truc bánh xe sau 

	model_khuan_may_cau = Translate(-cd_dau_may_cau / 2, -dg_kinh_banh_xe_may_cau, 0); // 
	khuan_may_cau(cd_dau_may_cau / 1.2, dg_kinh_banh_xe_may_cau / 2, cd_dau_may_cau / 4, green);//

	// Truc truyền chuyển động
	model_khuan_may_cau = Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe * 5), 0); // truc doc 
	khuan_may_cau(cr_may_cau + 0.05, cc_zip_gam_xe, cc_zip_gam_xe, black);

	//khoa truc  nam giũa truc xoay để thể hiện khớp nối quay
	model_khuan_may_cau = Translate(0, -(do_day_vat_lieu_may_cau + cc_zip_gam_xe * 5), 0) * RotateX(goc_quay_banh_xe_may_cau); // năm giữa trục doc
	khuan_may_cau(cc_zip_gam_xe * 2, cc_zip_gam_xe * 4, cc_zip_gam_xe * 2, red);

}
vec4 mau_dau_may_cau = blue;
bool den_xe_sang = true;
void DauMayCau()
{
	//model_khuan_may_cau = identity();
	mat4 dich_dau_ra_dau = Translate(cd_may_cau / 2 + cd_dau_may_cau / 2, 0, 0);

	model_khuan_may_cau = dich_dau_ra_dau * Translate(0, do_day_vat_lieu_may_cau, 0);// Dịch đầu máy cẩu trên sàn 
	khuan_may_cau(cr_dau_may_cau, do_day_vat_lieu_may_cau, cr_dau_may_cau, mau_dau_may_cau);

	model_khuan_may_cau = dich_dau_ra_dau * Translate(-cd_dau_may_cau / 2, cr_dau_may_cau / 2, 0); // mặt sau
	khuan_may_cau(do_day_vat_lieu_may_cau, cr_dau_may_cau, cr_dau_may_cau, mau_dau_may_cau);

	model_khuan_may_cau = dich_dau_ra_dau * Translate(+cd_dau_may_cau / 2, cr_dau_may_cau / 4, 0); // mặt truoc  // chieu cao là cr_dau_may_cau/2 thi translate xuong 1 doan /4
	khuan_may_cau(do_day_vat_lieu_may_cau, cr_dau_may_cau / 2, cr_dau_may_cau, mau_dau_may_cau);


	model_khuan_may_cau = Translate(cd_may_cau / 2 + cd_dau_may_cau, cr_dau_may_cau / 2, 0) * RotateZ(40) * Translate(0, cr_dau_may_cau / 1.5 / 2, 0); // mặt kinh  // dịch len cao  roi moi dịch ra dau
	khuan_may_cau(do_day_vat_lieu_may_cau, cr_dau_may_cau / 1.5, cr_dau_may_cau, mau_dau_may_cau);                                                    // doan dich nayf dich bang 1 nua chieu cao khuan

	model_khuan_may_cau = dich_dau_ra_dau * Translate(-cd_dau_may_cau / 2 + cr_dau_may_cau * 0.3, cr_dau_may_cau, 0); // mặt trên
	khuan_may_cau(cr_dau_may_cau * 0.6, do_day_vat_lieu_may_cau, cr_dau_may_cau, mau_dau_may_cau);    // * 0.5 ép bé đi so voi mat duoi


	// Măt phải cánh cửa ô to
	model_khuan_may_cau = Translate(cd_may_cau / 2 + cd_dau_may_cau, 0, 0) * Translate(0, cr_dau_may_cau / 4, cr_dau_may_cau / 2) * RotateY(goc_quay_canh_cua_may_cau) * Translate(-cd_dau_may_cau / 2, 0, 0);// mawt phải canh o to , dich canh cua ra đầu rồi tiến hành dịch  nửa đoạn cảnh cử để cảnh cửa quay ở mép k phải ở giua
	khuan_may_cau(cr_dau_may_cau, cr_dau_may_cau / 2, do_day_vat_lieu_may_cau, cyan);
	// Mặt trái cánh cửa ô tô
	model_khuan_may_cau = Translate(cd_may_cau / 2 + cd_dau_may_cau, 0, 0) * Translate(0, cr_dau_may_cau / 4, -cr_dau_may_cau / 2) * RotateY(-goc_quay_canh_cua_may_cau) * Translate(-cd_dau_may_cau / 2, 0, 0); //măt trai
	khuan_may_cau(cr_dau_may_cau, cr_dau_may_cau / 2, do_day_vat_lieu_may_cau, cyan);

	//Den xe
	vec4 mau_den_xe = red;
	if (den_xe_sang == false)
		mau_den_xe = white;
	else
		mau_den_xe = red;
	model_khuan_may_cau = dich_dau_ra_dau * Translate(+cd_dau_may_cau / 2 + do_day_vat_lieu_may_cau, cr_dau_may_cau / 2 / 6 / 2, cr_dau_may_cau / 2 - cr_dau_may_cau / 6 / 2); // den xe phai //  cr_dau_may_cau / 2 / 6/2 laf 1 nua kich thuoc den // dich ra giua nhuw mat truoc roi sacele -> transl
	khuan_may_cau(do_day_vat_lieu_may_cau / 6, cr_dau_may_cau / 2 / 6, cr_dau_may_cau / 6, mau_den_xe);

	model_khuan_may_cau = dich_dau_ra_dau * Translate(+cd_dau_may_cau / 2 + do_day_vat_lieu_may_cau, cr_dau_may_cau / 2 / 6 / 2, -(cr_dau_may_cau / 2 - cr_dau_may_cau / 6 / 2)); // den xe trai
	khuan_may_cau(do_day_vat_lieu_may_cau / 6, cr_dau_may_cau / 2 / 6, cr_dau_may_cau / 6, mau_den_xe);


	/// Thung hang sau dau xe 1
	model_khuan_may_cau = Translate(cd_dau_may_cau - 0.05, cc_thung_hang_may_cau / 2, 0); // lui thung hang ve cuoi xe
	khuan_may_cau(cd_dau_may_cau / 2, cc_thung_hang_may_cau, cr_dau_may_cau, red);
	// thung hang sau dau xe 2
	model_khuan_may_cau = Translate(cd_dau_may_cau - 0.05, cc_thung_hang_may_cau + cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4); // lui thung hang ve cuoi xe
	khuan_may_cau(cd_dau_may_cau / 2, cc_thung_hang_may_cau, cr_dau_may_cau / 2, green);
	// Hai thanh rao chắn


}
GLfloat truot_bac_2_may_cau = 0;
GLfloat truot_bac_3_may_cau = 0; // thanh truot 3
GLfloat quay_be_truc_may_cau = 0;
GLfloat tha_day_may_cau = 0.15;
GLfloat goc_quay_rong_roc = 0;
int cau=0 ;// hanh đọng câu

void TrucCau()
{
	// thung hang cố định trục cẩu
	// thugn hang 1
	mat4 dieu_khien = RotateY(quay_be_truc_may_cau);
	model_khuan_may_cau = Translate(-cd_dau_may_cau + 0.05, cc_thung_hang_may_cau / 2, 0) * dieu_khien; // lui thung hang ve cuoi xe
	khuan_may_cau(cd_dau_may_cau / 2, cc_thung_hang_may_cau, cr_dau_may_cau, blue);
	// thùng hang 2
	model_khuan_may_cau *= Translate(cd_dau_may_cau / 2, 0, cr_dau_may_cau / 4); // thung hang 2 // lui ve truoc 1 đoạn = cr thung hàng 1 // nhan cung model_khuan_may_cau de cho thung hang 2 gán vơi thùng hàng 1 -> cùng xoay
	khuan_may_cau(cd_dau_may_cau / 2, cc_thung_hang_may_cau, cr_dau_may_cau / 2, blue);// 

	//Cần cẩu 
	//truc bac 1
	//phan cap
	dieu_khien *= RotateZ(goc_quay_truc_cau) * Translate(0, cc_truc_cau_may_cau / 2 - cc_thung_hang_may_cau / 2, 0); // điều kiển nâng hạ cần cẩu + dich len 1 doan 1 nua can cau cap 1
	model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien; // lui thung hang ve cuoi xe
	khuan_may_cau(cr_truc_cau_may_cau, cc_truc_cau_may_cau, cr_truc_cau_may_cau, green);

	// truc bac 2
	dieu_khien *= Translate(0, truot_bac_2_may_cau + 0.1 * cc_truc_cau_may_cau, 0);  // dieu khien ra vao truc bac 2 , khoogn cần dịch y 1 đoạn cc_truc_cau_may_cau / 2 vì đã dịch truc 1 rồi => phân cấp ,  0.1* cc_truc_cau_may_cau là phần dịch thêm do tăng độ dài truc2 1.2 làn
	model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien; // lui thung hang ve cuoi xe
	khuan_may_cau(cr_truc_cau_may_cau * 0.8, cc_truc_cau_may_cau * 1.2, cr_truc_cau_may_cau * 0.8, yellow);  // *0.8 giam kich thuoc cho vừa trong truc bac 1

	// truc bac 3
	dieu_khien *= Translate(0, truot_bac_3_may_cau + 0.1 * cc_truc_cau_may_cau, 0);  //0.2 * cc_truc_cau_may_cau là phần dịch thêm do tăng độ dài truc 3 =  1.3 làn - 1.2 lan so voi truc 2
	model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien; // lui thung hang ve cuoi xe
	khuan_may_cau(cr_truc_cau_may_cau * 0.6, cc_truc_cau_may_cau * 1.3, cr_truc_cau_may_cau * 0.6, magenta);  // *0.8 giam kich thuoc cho vừa trong truc bac 2
	//
	//truc bac 4 //  bac nay khong rut ra 
	dieu_khien *= Translate(0, cc_truc_cau_may_cau * 1.3 * 0.5 + cc_truc_cau_may_cau * 0.1 / 2, 0);  // translate y moot đoạn bằng chiếu dài khuân trên /2
	model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien; // lui thung hang ve cuoi xe
	khuan_may_cau(cr_truc_cau_may_cau * 0.5, cc_truc_cau_may_cau * 0.1, cr_truc_cau_may_cau * 0.5, red);  // 

	// truc bac5 gấp khúc
	dieu_khien *= Translate(0, cc_truc_cau_may_cau * 0.1 * 0.5, 0) * RotateZ(-80) * Translate(0, cc_truc_cau_may_cau * 0.07 / 2, 0);  //0.2 * cc_truc_cau_may_cau là phần dịch thêm do tăng độ dài truc 3 =  1.4 làn - 1.2 lan so voi truc 2
	model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien; // lui thung hang ve cuoi xe
	khuan_may_cau(cr_truc_cau_may_cau * 0.3, cc_truc_cau_may_cau * 0.07, cr_truc_cau_may_cau * 0.3, blue);  // *0.8 giam kich thuoc cho vừa trong truc bac 2

	mat4 tmp = dieu_khien; // nhảy bước3 dòng này giữ lại phân cấp  cho 2 sơ dây trách ko bị trục 6 ròng rọc quay 2 sợ dây

	// truc 6 làm vòng bánh xe ròng roc

	// hoa tiet cua banh xe rong roc the hien quay
	model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien * Translate(0, cc_truc_cau_may_cau * 0.07 / 2, cr_truc_cau_may_cau / 2) * RotateZ(goc_quay_rong_roc);  // nan xe ben trái
	khuan_may_cau(cr_truc_cau_may_cau * 0.3 / 5, cc_truc_cau_may_cau * 0.07 / 1.5, 0.005, red);

	// Banh xe rong roc
	dieu_khien *= Translate(0, cc_truc_cau_may_cau * 0.07 / 2, 0) * RotateZ(goc_quay_rong_roc);
	int n = 30;
	for (int i = 0; i < 30; i++)
	{
		GLfloat quay = 360 / n;
		model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien * RotateZ(quay * i);
		khuan_may_cau(cr_truc_cau_may_cau * 0.3, cc_truc_cau_may_cau * 0.07, cr_truc_cau_may_cau * 0.3, black);
	}

	//  sợi day cáp
	dieu_khien = tmp * Translate(0, cc_truc_cau_may_cau * 0.07 / 2 + cc_truc_cau_may_cau * 0.06, 0) * RotateZ(80) * RotateZ(-goc_quay_truc_cau);  // rotatez 80 là để khử góc quay của trục 5 , tiếp tục khử góc quay của trục cẩu đảm bảo sợi dây luôn thẳng đứng
	model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2 - tha_day_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien; // dịch y = tha_day_may_cau /2 giúp dây dài về 1 bên
	khuan_may_cau(cr_truc_cau_may_cau * 0.1, cc_truc_cau_may_cau * 0.08 + tha_day_may_cau, cr_truc_cau_may_cau * 0.1, green);

	// cục khóa cố định
	dieu_khien *= Translate(0, -cc_truc_cau_may_cau * 0.08 - tha_day_may_cau, 0);
	model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien; // dịch y = tha_day_may_cau /2 giúp dây dài về 1 bên
	khuan_may_cau(cr_truc_cau_may_cau * 0.3, cc_truc_cau_may_cau * 0.08, cr_truc_cau_may_cau * 0.3, green);

	// cuc nam châm hút
	dieu_khien *= Translate(0, -cc_truc_cau_may_cau * 0.08, 0);
	for (int i = 0; i < 30; i++)
	{
		GLfloat quay = 360 / n;
		model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien * RotateY(quay * i); // dịch y = tha_day /2 giúp dây dài về 1 bên
		khuan_may_cau(cr_truc_cau_may_cau * 1.2, cc_truc_cau_may_cau * 0.06, cr_truc_cau_may_cau * 1.2, cyan);
	}

	// Thùng hàng tự do 
	if (cau == 1)
	{
		dieu_khien = Translate(cd_may_cau * 1.55, -cc_truc_cau_may_cau * 0.08, 0);// trả lại vịt rí cũ của thùng hàng ( trước đầu xe)
		model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien; // dịch y = tha_day_may_cau /2 giúp dây dài về 1 bên
		khuan_may_cau(cr_truc_cau_may_cau * 1.5, cc_truc_cau_may_cau * 0.08, cr_truc_cau_may_cau * 1.5, red);
	}
	else if(cau == 2)
	{
		dieu_khien *= Translate(0, -cc_truc_cau_may_cau * 0.08, 0);
		model_khuan_may_cau = Translate(-cd_dau_may_cau + cr_truc_cau_may_cau / 2, cc_thung_hang_may_cau / 2, cr_dau_may_cau / 4) * dieu_khien; // dịch y = tha_day_may_cau /2 giúp dây dài về 1 bên
		khuan_may_cau(cr_truc_cau_may_cau * 1.5, cc_truc_cau_may_cau * 0.08, cr_truc_cau_may_cau * 1.5, red);
	}
	

}

void MayCau()
{
	model_may_cau = Translate(dich_x_may_cau, dich_y_may_cau, dich_z_may_cau) * RotateX(goc_quay_x_may_cau) * RotateY(goc_quay_y_may_cau) * RotateZ(goc_quay_z_may_cau);
	model_khuan_may_cau = identity();
	khuan_may_cau(cd_may_cau + 0.08, do_day_vat_lieu_may_cau, ti_le_may_cau * 1.8, blue); // mặt sàn

	khuan_gam_xe();
	DauMayCau();
	TrucCau();
}





/// ++++++++++++End Ve HUNG+++++++++++++++++


/// TRẦN QUANG VINH -2019600230 - Mô hình máy ủi

//+++++++++++++++++ Bat dau Vẽ máy ủi

mat4 instance_may_ui;
mat4 model_mu;
GLfloat dich_may_ui = 3;
GLfloat dich_y_may_ui = -2;
GLfloat goc_quayx_may_ui = 30, goc_quayy_may_ui = -90, goc_quayz_may_ui = 0, goc_quay_luoi_may_ui = 0;
mat4 model_mayui;

void mayui(GLfloat w, GLfloat h, GLfloat l)
{
	instance_may_ui = Scale(w, h, l);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, mv * model_mu * model_mayui * instance_may_ui);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}

GLfloat may_ui = 0.1;

//thong so tong quat
GLfloat cr_may_ui = may_ui * 1.2;
GLfloat cd_may_ui = may_ui * 4;
GLfloat do_day_vat_lieu = may_ui * 0.1;

//san
GLfloat cr_san_may_ui = may_ui * 2.8;
GLfloat cd_san_may_ui = cr_san_may_ui * 1.5;

//than
GLfloat cr_than_may_ui = may_ui * 2;
GLfloat cd_than_may_ui = cr_than_may_ui * 2.5;
GLfloat cc_than_may_ui = cr_than_may_ui * 1.5;

//dau
GLfloat cc_dau_may_ui = cr_san_may_ui * 0.8;

GLfloat cua_may_ui;

//luoi ui
GLfloat cr_luoi_ui = may_ui * 7;
GLfloat cd_luoi_ui = may_ui * 4;

//banh
GLfloat cr_banh_may_ui = may_ui * 1.5;
GLfloat cd_banh_may_ui = may_ui * 5;
GLfloat cc_banh_may_ui = may_ui * 1.5;

void DauMayUi()
{

	material_diffuse = yellow;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

	//model_khuan = identity();
	mat4 dau;

	model_mayui = dau * Translate(0, cc_than_may_ui / 2, cd_than_may_ui / 2); // dau
	mayui(cr_than_may_ui, cc_than_may_ui, cd_than_may_ui);

	model_mayui = dau * Translate(0, cc_than_may_ui * 0.5 / 2, -cd_than_may_ui / 2); // than
	mayui(cr_than_may_ui, cc_than_may_ui * 0.5, cd_than_may_ui);

	model_mayui = dau * Translate(0, cc_than_may_ui * 0.5, -cd_san_may_ui / 2); //san
	mayui(cr_san_may_ui, do_day_vat_lieu, cd_san_may_ui);

	model_mayui = dau * Translate(-cr_san_may_ui / 2, cc_dau_may_ui / 2 + cc_than_may_ui * 0.5, -cd_san_may_ui / 2); //phai
	mayui(do_day_vat_lieu, cc_dau_may_ui, cd_san_may_ui);

	model_mayui = dau * Translate(0, cc_dau_may_ui / 2 + cc_than_may_ui * 0.5, 0); // truoc
	mayui(cr_san_may_ui, cc_dau_may_ui, do_day_vat_lieu);

	model_mayui = dau * Translate(0, cc_dau_may_ui * 1.8 / 2 + cc_than_may_ui * 0.5, -cd_san_may_ui); //sau
	mayui(cr_san_may_ui, cc_dau_may_ui * 1.8, do_day_vat_lieu);

	model_mayui = dau * Translate(0, cc_than_may_ui * 0.5 + cc_dau_may_ui * 1.8, -cd_san_may_ui * 0.65); // mui
	mayui(cr_san_may_ui, do_day_vat_lieu, cd_san_may_ui * 0.7);

	model_mayui = dau * Translate(0, cc_than_may_ui * 0.5 + cc_dau_may_ui / 2 + cc_dau_may_ui * 0.86, -cd_san_may_ui * 0.3 / 2) * RotateX(-38);
	mayui(cr_san_may_ui, cc_dau_may_ui, do_day_vat_lieu);

	material_diffuse = cyan;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

	model_mayui = dau * Translate(cr_san_may_ui / 2, cc_dau_may_ui / 2 + cc_than_may_ui * 0.5, -cd_san_may_ui + cd_san_may_ui * 0.5 / 2); //cua trai
	mayui(do_day_vat_lieu, cc_dau_may_ui, cd_san_may_ui * 0.5);

	model_mayui = dau * Translate(cr_san_may_ui / 2, cc_dau_may_ui / 2 + cc_than_may_ui * 0.5, 0) * RotateY(cua_may_ui) * Translate(0, 0, -cd_san_may_ui * 0.5 / 2); //cua phai
	mayui(do_day_vat_lieu, cc_dau_may_ui, cd_san_may_ui * 0.5);
}

bool ui = true;

void LuoiUi() {

	material_diffuse = yellow;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

	mat4 luoi;

	model_mayui = luoi * Translate(0, cd_luoi_ui / 3, cd_than_may_ui + may_ui * 3.5) * RotateX(90); // 
	mayui(cr_luoi_ui, do_day_vat_lieu * 2, cd_luoi_ui);

	model_mayui = luoi * Translate(0, cd_luoi_ui / 4 - cd_luoi_ui / 2, cd_than_may_ui + may_ui * 3.5 + cd_luoi_ui / 8) * RotateX(45); // luoi duoi
	mayui(cr_luoi_ui, do_day_vat_lieu * 2, cd_luoi_ui / 3);



	model_mayui = luoi * Translate(0, -cd_luoi_ui / 9 + cd_luoi_ui, cd_than_may_ui + may_ui * 3.5 + cd_luoi_ui / 8) * RotateX(135); // luoi tren
	mayui(cr_luoi_ui, do_day_vat_lieu * 2, cd_luoi_ui / 3);

	model_mayui = luoi * Translate(cr_luoi_ui / 2, cd_luoi_ui * 1.3 / 4, cd_than_may_ui + may_ui * 3.5 + cd_luoi_ui / 10); // luoi trai
	mayui(do_day_vat_lieu, cd_luoi_ui * 1.2, cd_luoi_ui / 6);

	model_mayui = luoi * Translate(-cr_luoi_ui / 2, cd_luoi_ui * 1.3 / 4, cd_than_may_ui + may_ui * 3.5 + cd_luoi_ui / 10); // luoi phai
	mayui(do_day_vat_lieu, cd_luoi_ui * 1.2, cd_luoi_ui / 6);

	model_mayui = luoi * Translate(cr_than_may_ui / 2 + cr_banh_may_ui, 0, cd_banh_may_ui / 1.2); // thanh noi giua banh va luoi
	mayui(cr_banh_may_ui * 0.7, cc_banh_may_ui * 0.7, cd_banh_may_ui * 1.7);

	model_mayui = luoi * Translate(cr_than_may_ui / 2 + cr_banh_may_ui, cc_banh_may_ui * 0.3 + cc_banh_may_ui * 0.7 / 2, cd_banh_may_ui * 1.35) * RotateX(150); // thanh nho noi giua banh va luoi
	mayui(cr_banh_may_ui * 0.3, cc_banh_may_ui * 0.3, cd_banh_may_ui * 0.8);

	model_mayui = luoi * Translate(-cr_than_may_ui / 2 - cr_banh_may_ui, 0, cd_banh_may_ui / 1.2); // thanh noi giua banh va luoi
	mayui(cr_banh_may_ui * 0.7, cc_banh_may_ui * 0.7, cd_banh_may_ui * 1.7);

	model_mayui = luoi * Translate(-cr_than_may_ui / 2 - cr_banh_may_ui, cc_banh_may_ui * 0.3 + cc_banh_may_ui * 0.7 / 2, cd_banh_may_ui * 1.35) * RotateX(150); // thanh nho noi giua banh va luoi
	mayui(cr_banh_may_ui * 0.3, cc_banh_may_ui * 0.3, cd_banh_may_ui * 0.8);

	if (ui == true)
	{
		material_diffuse = red;  // mau vat
		diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		model_mayui = luoi * Translate(0, -0.07, 1.1); // vat the
		mayui(0.2, 0.2, 0.2);
	}
}


void BanhXeTrai() {

	material_diffuse = black;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

	mat4 banh;
	model_mayui = banh * Translate(0, 0, 0);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cd_banh_may_ui * 2);

	model_mayui = banh * Translate(0, cc_banh_may_ui / 2, 0);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cd_banh_may_ui * 1.5);

	model_mayui = banh * Translate(0, -cc_banh_may_ui / 2, 0);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cd_banh_may_ui * 1.5);

	model_mayui = banh * Translate(0, cr_banh_may_ui / 2 - cc_banh_may_ui / 2 - cc_banh_may_ui / 4, cd_banh_may_ui - cd_may_ui * 0.25) * RotateX(55);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cr_banh_may_ui);

	model_mayui = banh * Translate(0, -cr_banh_may_ui / 2 + cc_banh_may_ui / 2 + cc_banh_may_ui / 4, cd_banh_may_ui - cd_may_ui * 0.25) * RotateX(125);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cr_banh_may_ui);

	model_mayui = banh * Translate(0, -cr_banh_may_ui / 2 + cc_banh_may_ui / 2 + cc_banh_may_ui / 4, -cd_banh_may_ui + cd_may_ui * 0.25) * RotateX(55);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cr_banh_may_ui);

	model_mayui = banh * Translate(0, cr_banh_may_ui / 2 - cc_banh_may_ui / 2 - cc_banh_may_ui / 4, -cd_banh_may_ui + cd_may_ui * 0.25) * RotateX(125);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cr_banh_may_ui);

	model_mayui = banh * Translate(cr_than_may_ui / 2 + cr_banh_may_ui / 2, 0, 0);
}

void BanhXePhai() {

	material_diffuse = black;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

	mat4 banh;

	model_mayui = banh * Translate(0, 0, 0);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cd_banh_may_ui * 2);

	model_mayui = banh * Translate(0, cc_banh_may_ui / 2, 0);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cd_banh_may_ui * 1.5);

	model_mayui = banh * Translate(0, -cc_banh_may_ui / 2, 0);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cd_banh_may_ui * 1.5);

	model_mayui = banh * Translate(0, cr_banh_may_ui / 2 - cc_banh_may_ui / 2 - cc_banh_may_ui / 4, cd_banh_may_ui - cd_may_ui * 0.25) * RotateX(55);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cr_banh_may_ui);

	model_mayui = banh * Translate(0, -cr_banh_may_ui / 2 + cc_banh_may_ui / 2 + cc_banh_may_ui / 4, cd_banh_may_ui - cd_may_ui * 0.25) * RotateX(125);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cr_banh_may_ui);

	model_mayui = banh * Translate(0, -cr_banh_may_ui / 2 + cc_banh_may_ui / 2 + cc_banh_may_ui / 4, -cd_banh_may_ui + cd_may_ui * 0.25) * RotateX(55);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cr_banh_may_ui);

	model_mayui = banh * Translate(0, cr_banh_may_ui / 2 - cc_banh_may_ui / 2 - cc_banh_may_ui / 4, -cd_banh_may_ui + cd_may_ui * 0.25) * RotateX(125);
	mayui(cr_banh_may_ui, cc_banh_may_ui, cr_banh_may_ui);

	model_mayui = banh * Translate(-cr_than_may_ui / 2 - cr_banh_may_ui / 2, 0, 0);
}


void MayUi()
{

	
	
	model_mayui = identity();
	model_mu = Translate(dich_may_ui, dich_y_may_ui, 0) * RotateX(goc_quayx_may_ui) * RotateY(goc_quayy_may_ui) * RotateZ(goc_quayz_may_ui);

	DauMayUi();

	model_mu *= Translate(-cr_than_may_ui / 2 - cr_banh_may_ui / 2, 0, 0);
	BanhXeTrai();
	model_mu *= Translate(+cr_than_may_ui + cr_banh_may_ui, 0, 0);
	BanhXePhai();
	model_mu *= Translate(-cr_than_may_ui / 2 - cr_banh_may_ui / 2, 0, 0) * RotateX(goc_quay_luoi_may_ui);
	LuoiUi();

}
//++++++++++++++ Hết Vinnh+++++++++++

	

/// NGUYỄN VĂN KHIÊM -2019601289 - Mô hình ô tô tải

//+++++++++++++++++ Bat dau Vẽ máy ủi

mat4 model_oto_1;
GLfloat cua_oto;
mat4 instance_oto;
mat4 model_oto;

void ve_o_to(GLfloat w, GLfloat h, GLfloat l)
{
	//instance = Translate(0,h/2,0)*Scale(w, h, l);
	instance_oto = Scale(w, h, l);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, mv * model_oto_1 * model_oto * instance_oto);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void ve_goi_hang(GLfloat w, GLfloat h, GLfloat l)
{
	material_diffuse = red;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	instance_oto = Scale(w, h, l);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, mv *  model_oto * instance_oto);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
GLfloat i = 0;
GLfloat o_to = 0.35;
//x: chieu rong; y: chieu dai; z:do day; h: chieu cao; d: duong kinh
GLfloat x_o_to = o_to * 1.3;
GLfloat y_o_to = o_to * 4.1;
GLfloat z_o_to = o_to * 0.1;
GLfloat xoay_o_to[] = { 50,0,0 };
GLfloat tinhtien = 0.0;
GLfloat goc = 0;


GLfloat x_dau_o_to = o_to * 1.8;
GLfloat y_dau_o_to = o_to * 1.8;
GLfloat h_dau_o_to = x_o_to * 0.8;

GLfloat x_khung = x_o_to * 0.15;
GLfloat y_khung = y_o_to / 4;
GLfloat h_khung = z_o_to * 0.5;

GLfloat d_banh_xe = x_o_to / 3;
GLfloat h_banh_xe = x_o_to / 9;

GLfloat x_thung = x_o_to / 2;
GLfloat y_thung = y_o_to / 4;
GLfloat z_thung = z_o_to / 2.5;
GLfloat lat_thung = 0.0;

GLfloat dichxgh;
GLfloat dichygh;
GLfloat dichzgh;

void ve_banh_xe(GLfloat w, GLfloat h, GLfloat l, int n)
{
	mat4 banhxe = Translate(w, h, l);

	for (int i = 0; i < n; i++)
	{
		int rotate = 360 / n * i;
		model_oto = banhxe * Translate(0, -(z_o_to + h_khung * 5) - 0.07, 0) * RotateZ(rotate);
		ve_o_to(d_banh_xe, d_banh_xe, h_banh_xe);
	}
}
void banh_xe()
{
	material_diffuse = black;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

	ve_banh_xe(y_o_to / 2 + y_khung / 2, 0.1, -x_o_to / 2 + x_khung / 2 - 0.055, 100);
	ve_banh_xe(y_o_to / 2 + y_khung / 2, 0.1, x_o_to / 2 - x_khung / 2 + 0.055, 100);

	ve_banh_xe(-y_o_to / 2 + y_khung / 2 - y_khung / 4, 0.1, -x_o_to / 2 + x_khung / 2 - 0.055, 100);
	ve_banh_xe(-y_o_to / 2 + y_khung / 2 - y_khung / 4, 0.1, x_o_to / 2 - x_khung / 2 + 0.055, 100);
}
void dau_o_to()
{
	material_diffuse = red;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mat4 dauxe = Translate(y_o_to / 2 + y_dau_o_to / 2, 0, 0);

	model_oto = dauxe * Translate(0, z_o_to, 0);							// day 
	ve_o_to(x_dau_o_to, z_o_to, x_dau_o_to);

	model_oto = dauxe * Translate(-y_dau_o_to / 2, x_dau_o_to / 2, 0); //  sau
	ve_o_to(z_o_to, x_dau_o_to, x_dau_o_to);

	model_oto = dauxe * Translate(y_dau_o_to / 2, x_dau_o_to / 4, 0); // truoc  
	ve_o_to(z_o_to, x_dau_o_to / 2, x_dau_o_to);


	model_oto = Translate(y_o_to / 2 + y_dau_o_to, x_dau_o_to / 2, 0) * RotateZ(40) * Translate(0, x_dau_o_to / 1.5 / 2, 0); //cua so
	ve_o_to(z_o_to, x_dau_o_to / 1.5, x_dau_o_to);

	model_oto = dauxe * Translate(-y_dau_o_to / 2 + x_dau_o_to * 0.3, x_dau_o_to, 0); // nap
	ve_o_to(x_dau_o_to * 0.6, z_o_to, x_dau_o_to);

	mat4 mocua = Translate(-x_dau_o_to / 2, 0, 0);

	model_oto = dauxe * Translate(x_dau_o_to / 2, x_dau_o_to / 4, x_dau_o_to / 2) * RotateY(cua_oto) * mocua;  // phai
	ve_o_to(x_dau_o_to, x_dau_o_to / 2, z_o_to);


	model_oto = dauxe * Translate(0, x_dau_o_to / 4, -x_dau_o_to / 2); // trai
	ve_o_to(x_dau_o_to, x_dau_o_to / 2, z_o_to);


}

void goihang()
{
	material_diffuse = red;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mat4 hang;
	hang = Translate(-0.09, 0.02, 0) * Translate(-0.03, -z_o_to + 0.04, 0);
	model_oto = Translate(-0.03, -z_o_to + 0.04, 0) * Translate(-0.09, 0.09, 0) * hang;
	ve_o_to(0.2, 0.2, 0.2);
}
void thung()
{
	material_diffuse = blue;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

	mat4 thungxe = Translate(-0.09, 0.02, 0);
	model_oto = thungxe * Translate(-0.03, -z_o_to + 0.04, 0);						//day
	ve_o_to(y_o_to + 0.15, z_o_to - 0.1, o_to * 1.72);

	model_oto = thungxe * Translate(-0.05, x_dau_o_to / 4, x_dau_o_to / 2);// phai
	ve_o_to(y_o_to / 0.92, x_dau_o_to / 2, z_o_to / 2);

	model_oto = thungxe * Translate(-0.05, x_dau_o_to / 4, -x_dau_o_to / 2);//  trai
	ve_o_to(y_o_to / 0.92, x_dau_o_to / 2, z_o_to / 2);

	model_oto = thungxe * Translate(y_dau_o_to / 0.847, z_o_to * 2 + 0.08, 0); // truoc
	ve_o_to(z_thung, y_dau_o_to / 1.8, x_dau_o_to);

	model_oto = thungxe * Translate(-y_dau_o_to / 0.75, z_o_to * 2 + 0.08, 0); // sau
	ve_o_to(z_thung, y_dau_o_to / 1.8, x_dau_o_to);

	
	
	
}

mat4 thanhgo;
void ve_thanhgo(GLfloat w, GLfloat h, GLfloat l)
{
	thanhgo = Scale(w, h, l);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, mv * thanhgo);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void go(GLfloat a, GLfloat b, GLfloat c, int x)
{
	material_diffuse = green;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mat4 da;
	da = Translate(a, b, c - 0.2) * Scale(0.015, 0.07, 1.1) * RotateZ(goc) * RotateY(60) * RotateX(30);
	thanhgo = Scale(-1.55, 0.8, -2) * da;
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, mv * thanhgo);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void donggo()
{
	for (GLfloat j = 1.36; j <= 1.94; j = j + 0.04) {
		for (int i = 1; i <= 360; i++) {
			go(j, -0.077, 0.015, i);
		}
	}
	for (GLfloat j = 1.40; j <= 1.90; j = j + 0.04) {
		for (int i = 1; i <= 360; i++) {
			go(j, -0.018, 0.015, i);
		}
	}
	for (GLfloat j = 1.44; j <= 1.86; j = j + 0.04) {
		for (int i = 1; i <= 360; i++) {
			go(j, 0.041, 0.015, i);
		}
	}
	for (GLfloat j = 1.47; j <= 1.82; j = j + 0.04) {
		for (int i = 1; i <= 360; i++) {
			go(j, 0.1, 0.015, i);
		}
	}
	for (GLfloat j = 1.51; j <= 1.78; j = j + 0.04) {
		for (int i = 1; i <= 360; i++) {
			go(j, 0.159, 0.015, i);
		}
	}
	for (GLfloat j = 1.55; j <= 1.74; j = j + 0.04) {
		for (int i = 1; i <= 360; i++) {
			go(j, 0.218, 0.015, i);
		}
	}
	for (GLfloat j = 1.59; j <= 1.70; j = j + 0.04) {
		for (int i = 1; i <= 360; i++) {
			go(j, 0.277, 0.015, i);
		}
	}
	for (GLfloat j = 1.63; j <= 1.66; j = j + 0.04) {
		for (int i = 1; i <= 360; i++) {
			go(j, 0.336, 0.015, i);
		}
	}

}


void O_To()
{
	model_oto = identity();
	model_oto_1 = Translate(tinhtien, 0.5, 0) * RotateX(xoay_o_to[0]) * RotateY(xoay_o_to[1]) * RotateZ(xoay_o_to[2]);

	ve_o_to(y_o_to + 0.1, z_o_to, o_to * 1.8);
	banh_xe();
	dau_o_to();
	mat4 lat = Translate((y_o_to + 0.15) / 2, 0, 0);
	model_oto_1 = model_oto_1 * Translate(-(y_o_to + 0.15) / 2, 0, 0) * (RotateZ(lat_thung)) * lat;
	thung();
	
}


//// END ++++++++++++++++++++KHIÊM++++++++++++++++++++++
int status =1;
void VatPham()
{
	mat4 vitri;
	if (status == 1)
	{
		cau = 3;
		ui = true;
		glutPostRedisplay();
	}else if (status == 2)
	{
		cau = 1;
		ui = false;
		glutPostRedisplay();
	}
	else if (status == 3)
	{
		cau = 2;
	}
	else if (status == 4)
	{
		cau = 3;
		model_oto_1 = model_oto_1 * Translate(dichxgh * 2, dichygh, dichzgh * 2);
		goihang();
		
	}
	else if (status == 5) {
		cau = 3;
		
		/*model_oto_1 = model_oto_1 * Translate(dichxgh * 2, dichygh, dichzgh * 2);
		goihang();
		glutPostRedisplay();*/

	
		ve_goi_hang(0.2, 0.2, 0.2);
		
	}
	
}

void chieu(int width, int height) {
	/*point4 eye(1, 1, 1.5, 1);
	point4 at(0.0, 0.0, 0.0, 1.0);
	vec4 up(0.0, 1.0, 0.0, 1.0);*/

	vec4 eye(0, 0, 2, 1);
	vec4 at(0, 0, 0, 1);
	vec4 up(0, 1, 0, 1);
	mv = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, mv);

	mat4 projection = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);

	glViewport(0, 0, width, height);
}

/// Ve Hien thi
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//donggo();

	MayUi();
	VatPham();
	MayCau();
	
	O_To();
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
	glutSwapBuffers();

}


void keyboard(unsigned char key, int x, int y)
{


	switch (key) {
	case 033: // Escape Key
		exit(0);
		break;
	/// HÙNG KEYBOARD

	case 'A':
		dich_x_may_cau -= cd_may_cau / 30;
		// banh xe lui
		goc_quay_banh_xe_may_cau += 15;
		if (goc_quay_banh_xe_may_cau > 360)
			goc_quay_banh_xe_may_cau -= 360;
		cout << "xe dang chay lui ve phia sau"<< dich_y_may_cau << endl;
		glutPostRedisplay();
		break;
	case 'a':
		dich_x_may_cau += cd_may_cau / 30;
		// banh xe quay theo khi dich
		goc_quay_banh_xe_may_cau -= 15;
		if (goc_quay_banh_xe_may_cau < -360)
			goc_quay_banh_xe_may_cau += 360;
		cout << "xe dang chay ve phia truoc" << endl;
		glutPostRedisplay();
		break;
	case 's':
		goc_quay_x_may_cau += 30;
		if (goc_quay_x_may_cau > 360)
			goc_quay_x_may_cau -= 360;
		glutPostRedisplay();
		break;
	case 'S':
		goc_quay_x_may_cau -= 30;
		if (goc_quay_x_may_cau < -360)
			goc_quay_x_may_cau += 360;
		glutPostRedisplay();
		break;
	case 'd':
		goc_quay_y_may_cau += 30;
		if (goc_quay_y_may_cau > 360)
			goc_quay_y_may_cau -= 360;
		glutPostRedisplay();
		break;
	case 'D':
		goc_quay_y_may_cau -= 30;
		if (goc_quay_y_may_cau < -360)
			goc_quay_y_may_cau += 360;
		glutPostRedisplay();
		break;
	case 'f':
		goc_quay_z_may_cau -= 30;
		if (goc_quay_z_may_cau < -360)
			goc_quay_z_may_cau += 360;
		glutPostRedisplay();
		break;
	case 'F':
		goc_quay_z_may_cau += 30;
		if (goc_quay_z_may_cau > 360)
			goc_quay_z_may_cau -= 360;
		glutPostRedisplay();
		break;
	case 'b':
		goc_quay_banh_xe_may_cau -= 15;
		if (goc_quay_banh_xe_may_cau < -360)
			goc_quay_banh_xe_may_cau += 360;
		glutPostRedisplay();
		cout << "Banh xe dang chay ve phia truoc" << endl;
		break;
	case 'B':
		goc_quay_banh_xe_may_cau += 15;
		if (goc_quay_banh_xe_may_cau > 360)
			goc_quay_banh_xe_may_cau -= 360;
		glutPostRedisplay();
		break;
	case 'h':
		if (goc_quay_truc_cau - 15 < -70 || goc_quay_truc_cau + 15 > 75)
			break;
		goc_quay_truc_cau -= 15;
		if (goc_quay_truc_cau < -360)
			goc_quay_truc_cau += 360;
		glutPostRedisplay();
		cout << "Ha can cau " << endl;
		break;
	case 'H':
		if (goc_quay_truc_cau + 15 > 0)
			break;
		goc_quay_truc_cau += 15;
		if (goc_quay_truc_cau > 360)
			goc_quay_truc_cau -= 360;
		cout << "Nang can cau " << endl;
		glutPostRedisplay();
		break;
	case 'j':
		quay_be_truc_may_cau += 15;
		if (quay_be_truc_may_cau + 15 > 360)
			quay_be_truc_may_cau -= 360;
		cout << "Xoay  can cau " << endl;
		glutPostRedisplay();
		break;
	case 'J':
		quay_be_truc_may_cau -= 15;
		if (quay_be_truc_may_cau - 15 < -360)
			quay_be_truc_may_cau += 360;
		cout << "Xoay  can cau " << endl;
		glutPostRedisplay();
		break;
	case 'k':
		if (truot_bac_2_may_cau + 0.03 > cc_truc_cau_may_cau * 0.5)
			break;
		truot_bac_2_may_cau += 0.03;
		glutPostRedisplay();
		break;
	case 'K':
		if (truot_bac_2_may_cau - 0.03 < 0)
			break;
		truot_bac_2_may_cau -= 0.03;
		glutPostRedisplay();
		break;
	case 'l':
		if (truot_bac_3_may_cau + 0.03 > cc_truc_cau_may_cau * 0.5)
			break;
		truot_bac_3_may_cau += 0.03;
		glutPostRedisplay();
		break;
	case 'L':
		if (truot_bac_3_may_cau - 0.03 < 0)
			break;
		truot_bac_3_may_cau -= 0.03;
		glutPostRedisplay();
		break;

	case ';':
		
		tha_day_may_cau += 0.01;
		goc_quay_rong_roc -= 10;
		if (goc_quay_rong_roc < -360)
			goc_quay_rong_roc += 360;
		glutPostRedisplay();
		cout << "Tha day cau" << endl;
		break;
	case ':':
		if (tha_day_may_cau < 0)
			break;
		tha_day_may_cau -= 0.01;
		goc_quay_rong_roc += 10; // mo ta rong roc quay
		if (goc_quay_rong_roc > 360)
			goc_quay_rong_roc -= 360;
		glutPostRedisplay();
		cout << "Keo day cau" << endl;
		break;
	case '?':
		if (status > 5)
			break;
		status +=1;
		cout << "trang thai cau " << status << endl;
		glutPostRedisplay();
		break;
	case '/':
		if (status < 1)
			break;
		status -= 1;
		cout << "trang thai cau " << status << endl;
		glutPostRedisplay();
		break;
	case 'n':
		cout << "Den xe1 " << den_xe_sang << endl;
		den_xe_sang = !den_xe_sang;
		glutPostRedisplay();
		cout << "Den xe2 " << den_xe_sang << endl;
		break;
	case 'g':
		if (goc_quay_canh_cua_may_cau + 10 > 70)
			break;
		cout << " Mo cua xe " << endl;
		goc_quay_canh_cua_may_cau += 10;
		if (goc_quay_canh_cua_may_cau > 360)
			goc_quay_canh_cua_may_cau -= 360;
		glutPostRedisplay();
		break;
	case 'G':
		if (goc_quay_canh_cua_may_cau - 10 < 0)
			break;
		cout << " Dong cua xe " << endl;
		goc_quay_canh_cua_may_cau -= 10;
		if (goc_quay_canh_cua_may_cau < -360)
			goc_quay_canh_cua_may_cau += 360;
		glutPostRedisplay();
		break;

	/// VINH KEYBOARD
	case '5':
		cua_may_ui -= 5;
		if (cua_may_ui == -90) cua_may_ui += 5;
		glutPostRedisplay();
		break;

	case '%':
		cua_may_ui += 5;
		if (cua_may_ui == 5) cua_may_ui -= 5;
		glutPostRedisplay();
		break;

	case '6':
		goc_quay_luoi_may_ui -= 5;
		if (goc_quay_luoi_may_ui == -30) goc_quay_luoi_may_ui += 5;
		glutPostRedisplay();
		break;

	case '^':
		goc_quay_luoi_may_ui += 5;
		if (goc_quay_luoi_may_ui == 5) goc_quay_luoi_may_ui -= 5;
		glutPostRedisplay();
		break;

	case '2':
		goc_quayx_may_ui += 30;
		if (goc_quayx_may_ui > 360)
			goc_quayx_may_ui -= 360;
		glutPostRedisplay();
		break;

	case '@':
		goc_quayx_may_ui -= 30;
		if (goc_quayx_may_ui < 30)
			goc_quayx_may_ui += 360;
		glutPostRedisplay();
		break;

	case '3':
		goc_quayy_may_ui += 30;
		if (goc_quayy_may_ui > 360)
			goc_quayy_may_ui -= 360;
		glutPostRedisplay();
		break;

	case '#':
		goc_quayy_may_ui -= 30;
		if (goc_quayy_may_ui > 30)
			goc_quayy_may_ui += 360;
		glutPostRedisplay();
		break;

	case '4':
		goc_quayz_may_ui += 30;
		if (goc_quayz_may_ui > 360)
			goc_quayz_may_ui -= 360;
		glutPostRedisplay();
		break;

	case '$':
		goc_quayz_may_ui -= 30;
		if (goc_quayz_may_ui < 30)
			goc_quayz_may_ui += 360;
		glutPostRedisplay();
		break;

	case '!':
		dich_may_ui += 0.01;
		glutPostRedisplay();
		break;

	case '1':
		dich_may_ui -= 0.01;
		glutPostRedisplay();
		break;

	
	
	/// KHIÊM 
	case 'e': // xoay xe theo chiều y
		xoay_o_to[1] += 5;
		if (xoay_o_to[1] > 360) xoay_o_to[1] -= 360;
		glutPostRedisplay();
		break;
	case 'E':
		// một số lệnh 
		xoay_o_to[1] -= 5;
		if (xoay_o_to[1] > 360) xoay_o_to[1] -= 360;
		glutPostRedisplay();
		break;
	case 'w': // xoay xe theo chiều x
		xoay_o_to[0] += 5;
		if (xoay_o_to[0] > 360) xoay_o_to[0] -= 360;
		glutPostRedisplay();
		break;
	case 'W':
		// một số lệnh 
		xoay_o_to[0] -= 5;
		if (xoay_o_to[0] > 360) xoay_o_to[0] -= 360;
		glutPostRedisplay();
		break;
	case 'r': // xoay xe theo chiều z
		xoay_o_to[2] += 5;
		if (xoay_o_to[2] > 360) xoay_o_to[2] -= 360;
		glutPostRedisplay();
		break;
	case 'R':
		// một số lệnh 
		xoay_o_to[2] -= 5;
		if (xoay_o_to[2] > 360) xoay_o_to[2] -= 360;
		glutPostRedisplay();
		break;

	case 'q': // tịnh tiến xe
		tinhtien += 0.05;
		glutPostRedisplay();
		break;
	case 'Q':
		// một số lệnh 
		tinhtien -= 0.05;

		glutPostRedisplay();
		break;

	case 'y': // lật thùng xe
		// một số lệnh 
		if (lat_thung < 30)
		{
			lat_thung += 5;
			i = i + 0.01;
		}
		glutPostRedisplay();
		break;
	case 'Y':
		// một số lệnh 

		if (lat_thung > 0)
		{
			lat_thung -= 5;
			i = i - 0.01;
		}
		glutPostRedisplay();
		break;
	case 't':
		cua_oto += 5;
		glutPostRedisplay();
		break;
	case 'T':
		cua_oto -= 5;
		glutPostRedisplay();
		break;

	
	/// Lenh chung'
	case 'z':
		xoay_o_to[1] += 30;
		if (xoay_o_to[1] > 360) xoay_o_to[1] -= 360;
		goc_quayy_may_ui += 30;
		if (goc_quayy_may_ui > 360)
			goc_quayy_may_ui -= 360;
		goc_quay_y_may_cau += 30;
		if (goc_quay_y_may_cau > 360)
			goc_quay_y_may_cau -= 360;
		glutPostRedisplay();
		break;
	case 'x':
		xoay_o_to[0] += 30;
		if (xoay_o_to[0] > 360) xoay_o_to[0] -= 360;
		goc_quay_x_may_cau += 30;
		if (goc_quay_x_may_cau > 360)
			goc_quay_x_may_cau -= 360;
		goc_quayx_may_ui -= 30;
		if (goc_quayx_may_ui < 30)
			goc_quayx_may_ui += 360;
		glutPostRedisplay();
		break;
	case 'c':
		xoay_o_to[2] += 30;
		if (xoay_o_to[2] > 360) xoay_o_to[2] -= 360;
		goc_quayz_may_ui -= 30;
		if (goc_quayz_may_ui < 30)
			goc_quayz_may_ui += 360;
		goc_quay_z_may_cau += 30;
		if (goc_quay_z_may_cau > 360)
			goc_quay_z_may_cau -= 360;
		glutPostRedisplay();
		break;

	
	}

	glutPostRedisplay();
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
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(chieu);
	glutMainLoop();
	return 0;
}
