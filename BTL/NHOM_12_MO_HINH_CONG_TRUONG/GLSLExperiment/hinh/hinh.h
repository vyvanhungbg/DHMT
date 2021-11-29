#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/
#include <vector>
#include "hinh_lap_phuong.h"

typedef vec4 point4;
typedef vec4 color4;
using namespace std;


#define doToRadian 0.0174532925199432


void creat_points_array(vec4 points[], vec4 colors[], vec3 normals[]);
void noi(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, vector<vec4> diem1, vector<vec4> diem2, int n);
void hinh_ong(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, int n, float a);
void hinh_non(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, int n, float a);

void creat_points_array(vec4 points[], vec4 colors[], vec3 normals[])
{
	vector<vec4> x, y;
	vector<vec3> z;
	// x lưu danh sách tập điểm
	// y lưu danh sách tập màu
	//  z lưu danh sách tập màu

	//định nghĩa cac dạng hình
	int  n = 40;
	hinh_lap_phuong(x, y, z); // hình lập phương
	hinh_ong(x, y, z, n, 0.5); // hình ống
	hinh_non(x, y, z, n, 0.5); // hình nón
	hinh_thang(x, y, z, 0.2); // hình thang
	hinh_ong(x, y, z, n, 0.001); // hình ống đặc
	hinh_thang(x, y, z, 0.7); // 792 - 36
	hinh_thang(x, y, z, 0.3); // 792 - 36
	hinh_thang(x, y, z, 0.5); // 792 - 36
	hinh_chop_thang(x, y, z);
	hinh_thang_duoi(x, y, z, 0.3);
	hinh_canh(x, y, z, 0.1);
	hinh_canh_trong(x, y, z, 0.1);
	hinh_thang_duoi_tren(x, y, z, 0.9);

	// sao chép mảng x, y, z sang points, colors, normals
	for (int i = 0; i < x.size(); i++)
	{
		points[200+i] = x[i];
		colors[200 + i] = y[i];
		normals[200 + i] = z[i];
	}
}







// nối giữa hai mảng điểm
void noi(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, vector<vec4> diem1, vector<vec4> diem2, int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		vec4 u = diem1[i + 1] - diem1[i];
		vec4 v = diem2[i + 1] - diem1[i + 1];
		vec3 normal = normalize(cross(u, v));
		for (int i = 0; i < 6; i++)
			z.push_back(normal);
		x.push_back(diem1[i]); y.push_back(vec4(1.0, 0.0, 0.0, 1.0));
		x.push_back(diem1[i + 1]); y.push_back(y[0]);
		x.push_back(diem2[i]); y.push_back(y[0]);
		x.push_back(diem2[i]); y.push_back(y[0]);
		x.push_back(diem1[i + 1]); y.push_back(y[0]);
		x.push_back(diem2[i + 1]); y.push_back(y[0]);
	}
	vec4 u = diem1[0] - diem1[n - 1];
	vec4 v = diem2[0] - diem1[0];
	vec3 normal = normalize(cross(u, v));
	for (int i = 0; i < 6; i++)
		z.push_back(normal);
	x.push_back(diem1[n - 1]); y.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	x.push_back(diem1[0]); y.push_back(y[0]);
	x.push_back(diem2[n - 1]); y.push_back(y[0]);
	x.push_back(diem2[n - 1]); y.push_back(y[0]);
	x.push_back(diem1[0]); y.push_back(y[0]);
	x.push_back(diem2[0]); y.push_back(y[0]);
}


// tập điểm hình ống
// nhập n thì số lượng phần tử là : 24*n
void hinh_ong(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, int n, float a)
// n là số đỉnh của đa giác điều
// a là tỉ lệ giữa vòng trong và vòng ngoài
{
	vector<vec4> diem1;
	vector<vec4> diem2;
	vector<vec4> diem3;
	vector<vec4> diem4;
	for (int i = 0; i < n; i++)
	{
		float goc = float(i) * 360.0 / float(n) * doToRadian;
		diem1.push_back(vec4(0.5*cos(goc), 0.5, 0.5*sin(goc), 1.0));
		diem2.push_back(vec4(0.5*cos(goc), -0.5, 0.5*sin(goc), 1.0));
		diem3.push_back(vec4(0.5*a*cos(goc), 0.5, 0.5*a*sin(goc), 1.0));
		diem4.push_back(a*vec4(0.5*a*cos(goc), -0.5, 0.5*a*sin(goc), 1.0));
	}
	/*
	noi(x, y, z, diem2, diem1, n);
	noi(x, y, z, diem3, diem4, n);
	noi(x, y, z, diem1, diem3, n);
	noi(x, y, z, diem4, diem2, n);
	*/

	noi(x, y, z, diem1, diem2, n);
	noi(x, y, z, diem2, diem4, n);
	noi(x, y, z, diem3, diem1, n);
	noi(x, y, z, diem4, diem3, n);
}





// tập điểm hình nón
// nhập n thì số lượng phần tử là : 24*n
void hinh_non(vector<vec4> &x, vector<vec4> &y, vector<vec3> &z, int n, float a)
// n là số đỉnh của đa giác điều
// a là tỉ lệ giữa vòng trong và vòng ngoài
{
	vector<vec4> diem1;
	vector<vec4> diem2;
	vector<vec4> diem3;
	vector<vec4> diem4;
	float k = 0.01;
	for (int i = 0; i < n; i++)
	{
		float goc = float(i) * 360.0 / float(n) * doToRadian;
		diem1.push_back(vec4(0.5*k*cos(goc), 0.5, 0.5*k*sin(goc), 1.0));
		diem2.push_back(vec4(0.5*cos(goc), -0.5, 0.5*sin(goc), 1.0));
		diem3.push_back(vec4(0.5*k*a*cos(goc), 0.5, 0.5*k*a*sin(goc), 1.0));
		diem4.push_back(a*vec4(0.5*k*a*cos(goc), -0.5, 0.5*k*a*sin(goc), 1.0));
	}
	/*
	noi(x, y, z, diem2, diem1, n);
	noi(x, y, z, diem3, diem4, n);
	noi(x, y, z, diem1, diem3, n);
	noi(x, y, z, diem4, diem2, n);
	*/

	noi(x, y, z, diem1, diem2, n);
	noi(x, y, z, diem2, diem4, n);
	noi(x, y, z, diem3, diem1, n);
	noi(x, y, z, diem4, diem3, n);
}
