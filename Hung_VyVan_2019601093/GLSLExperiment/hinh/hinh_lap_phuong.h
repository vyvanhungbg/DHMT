#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/
#include <vector>
#include "noi.h"

typedef vec4 point4;
typedef vec4 color4;
using namespace std;


void quad(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, vector<vec4> diem, vector<vec4> mau, int a, int b, int c, int d);

void hinh_lap_phuong(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z)
{
	vector<vec4> diem, mau;
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	diem.push_back(point4(-0.5, -0.5, 0.5, 1.0));
	diem.push_back(point4(-0.5, 0.5, 0.5, 1.0));
	diem.push_back(point4(0.5, 0.5, 0.5, 1.0));
	diem.push_back(point4(0.5, -0.5, 0.5, 1.0));
	diem.push_back(point4(-0.5, -0.5, -0.5, 1.0));
	diem.push_back(point4(-0.5, 0.5, -0.5, 1.0));
	diem.push_back(point4(0.5, 0.5, -0.5, 1.0));
	diem.push_back(point4(0.5, -0.5, -0.5, 1.0));

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	mau.push_back(color4(0.0, 0.0, 0.0, 1.0)); // black
	mau.push_back(color4(1.0, 0.0, 0.0, 1.0)); // red
	mau.push_back(color4(1.0, 1.0, 0.0, 1.0)); // yellow
	mau.push_back(color4(0.0, 1.0, 0.0, 1.0)); // green
	mau.push_back(color4(0.0, 0.0, 1.0, 1.0)); // blue
	mau.push_back(color4(1.0, 0.0, 1.0, 1.0)); // magenta
	mau.push_back(color4(1.0, 0.5, 0.0, 1.0)); // orange
	mau.push_back(color4(0.0, 1.0, 1.0, 1.0)); // cyan

	quad(x, y, z, diem, mau, 1, 0, 3, 2);
	quad(x, y, z, diem, mau, 2, 3, 7, 6);
	quad(x, y, z, diem, mau, 3, 0, 4, 7);
	quad(x, y, z, diem, mau, 6, 5, 1, 2);
	quad(x, y, z, diem, mau, 4, 5, 6, 7);
	quad(x, y, z, diem, mau, 5, 4, 0, 1);
}

// hình thang
void hinh_thang(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, float d)
// d là cạnh nhỏ < 1
{
	vector<vec4> diem, mau;
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	diem.push_back(point4(-0.5, -0.5, 0.5, 1.0));
	diem.push_back(point4(-0.5, -0.5 + d, 0.5, 1.0));
	diem.push_back(point4(0.5, 0.5, 0.5, 1.0));
	diem.push_back(point4(0.5, -0.5, 0.5, 1.0));
	diem.push_back(point4(-0.5, -0.5, -0.5, 1.0));
	diem.push_back(point4(-0.5, -0.5 + d, -0.5, 1.0));
	diem.push_back(point4(0.5, 0.5, -0.5, 1.0));
	diem.push_back(point4(0.5, -0.5, -0.5, 1.0));

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	mau.push_back(color4(0.0, 0.0, 0.0, 1.0)); // black
	mau.push_back(color4(1.0, 0.0, 0.0, 1.0)); // red
	mau.push_back(color4(1.0, 1.0, 0.0, 1.0)); // yellow
	mau.push_back(color4(0.0, 1.0, 0.0, 1.0)); // green
	mau.push_back(color4(0.0, 0.0, 1.0, 1.0)); // blue
	mau.push_back(color4(1.0, 0.0, 1.0, 1.0)); // magenta
	mau.push_back(color4(1.0, 0.5, 0.0, 1.0)); // orange
	mau.push_back(color4(0.0, 1.0, 1.0, 1.0)); // cyan

	quad(x, y, z, diem, mau, 1, 0, 3, 2);
	quad(x, y, z, diem, mau, 2, 3, 7, 6);
	quad(x, y, z, diem, mau, 3, 0, 4, 7);
	quad(x, y, z, diem, mau, 6, 5, 1, 2);
	quad(x, y, z, diem, mau, 4, 5, 6, 7);
	quad(x, y, z, diem, mau, 5, 4, 0, 1);
}


void quad(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, vector<vec4> diem, vector<vec4> mau, int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = diem[b] - diem[a];
	vec4 v = diem[c] - diem[b];
	vec3 normal = normalize(cross(u, v));

	z.push_back(normal); y.push_back(mau[a]); x.push_back(diem[a]);
	z.push_back(normal); y.push_back(mau[a]); x.push_back(diem[b]); 
	z.push_back(normal); y.push_back(mau[a]); x.push_back(diem[c]);
	z.push_back(normal); y.push_back(mau[a]); x.push_back(diem[a]);
	z.push_back(normal); y.push_back(mau[a]); x.push_back(diem[c]); 
	z.push_back(normal); y.push_back(mau[a]); x.push_back(diem[d]);
}


void hinh_chop_thang(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z)
// d là cạnh nhỏ < 1
{
	vector<vec4> diem, mau;
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	diem.push_back(point4(-0.5, -0.5, 0.5, 1.0));
	diem.push_back(point4(-0.5, 0, 0, 1.0));
	diem.push_back(point4(-0.5, 0, 0, 1.0));
	diem.push_back(point4(0.5, -0.5, 0.5, 1.0));
	diem.push_back(point4(-0.5, -0.5, -0.5, 1.0));
	diem.push_back(point4(-0.5, 0, 0, 1.0));
	diem.push_back(point4(-0.5, 0, 0, 1.0));
	diem.push_back(point4(0.5, -0.5, -0.5, 1.0));

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	mau.push_back(color4(0.0, 0.0, 0.0, 1.0)); // black
	mau.push_back(color4(1.0, 0.0, 0.0, 1.0)); // red
	mau.push_back(color4(1.0, 1.0, 0.0, 1.0)); // yellow
	mau.push_back(color4(0.0, 1.0, 0.0, 1.0)); // green
	mau.push_back(color4(0.0, 0.0, 1.0, 1.0)); // blue
	mau.push_back(color4(1.0, 0.0, 1.0, 1.0)); // magenta
	mau.push_back(color4(1.0, 0.5, 0.0, 1.0)); // orange
	mau.push_back(color4(0.0, 1.0, 1.0, 1.0)); // cyan

	quad(x, y, z, diem, mau, 1, 0, 3, 2);
	quad(x, y, z, diem, mau, 2, 3, 7, 6);
	quad(x, y, z, diem, mau, 3, 0, 4, 7);
	quad(x, y, z, diem, mau, 6, 5, 1, 2);
	quad(x, y, z, diem, mau, 4, 5, 6, 7);
	quad(x, y, z, diem, mau, 5, 4, 0, 1);
}

void hinh_thang_duoi(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, float d)
// d là cạnh nhỏ < 1
{

	vector<vec4> diem, mau;
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	diem.push_back(point4(-0.5, -0.5, 0.2, 1.0));
	diem.push_back(point4(-0.5, -0.5 + d, 0.2, 1.0));
	diem.push_back(point4(0.5, 0.5, 0.5, 1.0));
	diem.push_back(point4(0.5, -0.5, 0.5, 1.0));
	diem.push_back(point4(-0.5, -0.5, -0.2, 1.0));
	diem.push_back(point4(-0.5, -0.5 + d, -0.2, 1.0));
	diem.push_back(point4(0.5, 0.5, -0.5, 1.0));
	diem.push_back(point4(0.5, -0.5, -0.5, 1.0));

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	mau.push_back(color4(0.0, 0.0, 0.0, 1.0)); // black
	mau.push_back(color4(1.0, 0.0, 0.0, 1.0)); // red
	mau.push_back(color4(1.0, 1.0, 0.0, 1.0)); // yellow
	mau.push_back(color4(0.0, 1.0, 0.0, 1.0)); // green
	mau.push_back(color4(0.0, 0.0, 1.0, 1.0)); // blue
	mau.push_back(color4(1.0, 0.0, 1.0, 1.0)); // magenta
	mau.push_back(color4(1.0, 0.5, 0.0, 1.0)); // orange
	mau.push_back(color4(0.0, 1.0, 1.0, 1.0)); // cyan

	quad(x, y, z, diem, mau, 1, 0, 3, 2);
	quad(x, y, z, diem, mau, 2, 3, 7, 6);
	quad(x, y, z, diem, mau, 3, 0, 4, 7);
	quad(x, y, z, diem, mau, 6, 5, 1, 2);
	quad(x, y, z, diem, mau, 4, 5, 6, 7);
	quad(x, y, z, diem, mau, 5, 4, 0, 1);


}
void hinh_canh(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, float d)
// d là cạnh nhỏ < 1
{
	vector<vec4> diem, mau;
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	diem.push_back(point4(-0.5, -0.5, -0.3, 1.0));
	diem.push_back(point4(-0.5, -0.5 + d, -0.3, 1.0));
	diem.push_back(point4(0.5, -0.1, 0.5, 1.0));
	diem.push_back(point4(0.5, -0.5, 0.5, 1.0));
	diem.push_back(point4(-0.5, -0.5, -0.5, 1.0));
	diem.push_back(point4(-0.5, -0.5 + d, -0.5, 1.0));
	diem.push_back(point4(0.5, -0.1, -0.5, 1.0));
	diem.push_back(point4(0.5, -0.5, -0.5, 1.0));

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	mau.push_back(color4(0.0, 0.0, 0.0, 1.0)); // black
	mau.push_back(color4(1.0, 0.0, 0.0, 1.0)); // red
	mau.push_back(color4(1.0, 1.0, 0.0, 1.0)); // yellow
	mau.push_back(color4(0.0, 1.0, 0.0, 1.0)); // green
	mau.push_back(color4(0.0, 0.0, 1.0, 1.0)); // blue
	mau.push_back(color4(1.0, 0.0, 1.0, 1.0)); // magenta
	mau.push_back(color4(1.0, 0.5, 0.0, 1.0)); // orange
	mau.push_back(color4(0.0, 1.0, 1.0, 1.0)); // cyan

	quad(x, y, z, diem, mau, 1, 0, 3, 2);
	quad(x, y, z, diem, mau, 2, 3, 7, 6);
	quad(x, y, z, diem, mau, 3, 0, 4, 7);
	quad(x, y, z, diem, mau, 6, 5, 1, 2);
	quad(x, y, z, diem, mau, 4, 5, 6, 7);
	quad(x, y, z, diem, mau, 5, 4, 0, 1);
}
void hinh_canh_trong(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, float d)
// d là cạnh nhỏ < 1
{
	vector<vec4> diem, mau;
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	diem.push_back(point4(-0.5, -0.5, 0.5, 1.0));
	diem.push_back(point4(-0.5, -0.5 + d, 0.5, 1.0));
	diem.push_back(point4(0.5, -0.1, 0.5, 1.0));
	diem.push_back(point4(0.5, -0.5, 0.5, 1.0));
	diem.push_back(point4(-0.5, -0.5, 0.3, 1.0));
	diem.push_back(point4(-0.5, -0.5 + d, 0.3, 1.0));
	diem.push_back(point4(0.5, -0.1, -0.5, 1.0));
	diem.push_back(point4(0.5, -0.5, -0.5, 1.0));

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	mau.push_back(color4(0.0, 0.0, 0.0, 1.0)); // black
	mau.push_back(color4(1.0, 0.0, 0.0, 1.0)); // red
	mau.push_back(color4(1.0, 1.0, 0.0, 1.0)); // yellow
	mau.push_back(color4(0.0, 1.0, 0.0, 1.0)); // green
	mau.push_back(color4(0.0, 0.0, 1.0, 1.0)); // blue
	mau.push_back(color4(1.0, 0.0, 1.0, 1.0)); // magenta
	mau.push_back(color4(1.0, 0.5, 0.0, 1.0)); // orange
	mau.push_back(color4(0.0, 1.0, 1.0, 1.0)); // cyan

	quad(x, y, z, diem, mau, 1, 0, 3, 2);
	quad(x, y, z, diem, mau, 2, 3, 7, 6);
	quad(x, y, z, diem, mau, 3, 0, 4, 7);
	quad(x, y, z, diem, mau, 6, 5, 1, 2);
	quad(x, y, z, diem, mau, 4, 5, 6, 7);
	quad(x, y, z, diem, mau, 5, 4, 0, 1);
}
void hinh_thang_duoi_tren(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, float d)
// d là cạnh nhỏ < 1
{
	vector<vec4> diem, mau;
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	diem.push_back(point4(-0.5, -0.5, 0.1, 1.0));
	diem.push_back(point4(-0.5, 0.5, 0.1, 1.0));
	diem.push_back(point4(0.5 - d, 0.5, 0.1, 1.0));
	diem.push_back(point4(0.5, -0.5, 0.1, 1.0));
	diem.push_back(point4(-0.5, -0.5, -0.1, 1.0));
	diem.push_back(point4(-0.5, 0.5, -0.1, 1.0));
	diem.push_back(point4(0.5 - d, 0.5, -0.1, 1.0));
	diem.push_back(point4(0.5, -0.5, -0.1, 1.0));

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	mau.push_back(color4(0.0, 0.0, 0.0, 1.0)); // black
	mau.push_back(color4(1.0, 0.0, 0.0, 1.0)); // red
	mau.push_back(color4(1.0, 1.0, 0.0, 1.0)); // yellow
	mau.push_back(color4(0.0, 1.0, 0.0, 1.0)); // green
	mau.push_back(color4(0.0, 0.0, 1.0, 1.0)); // blue
	mau.push_back(color4(1.0, 0.0, 1.0, 1.0)); // magenta
	mau.push_back(color4(1.0, 0.5, 0.0, 1.0)); // orange
	mau.push_back(color4(0.0, 1.0, 1.0, 1.0)); // cyan

	quad(x, y, z, diem, mau, 1, 0, 3, 2);
	quad(x, y, z, diem, mau, 2, 3, 7, 6);
	quad(x, y, z, diem, mau, 3, 0, 4, 7);
	quad(x, y, z, diem, mau, 6, 5, 1, 2);
	quad(x, y, z, diem, mau, 4, 5, 6, 7);
	quad(x, y, z, diem, mau, 5, 4, 0, 1);
}
