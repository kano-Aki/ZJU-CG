#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<math.h>
using namespace std;
#define PI 3.14159265358979323846
void ppmRead(char* filename, unsigned char* data, int* w, int* h) {
	char header[1024];
	FILE* fp = NULL;
	int line = 0;

	fp = fopen(filename, "rb");
	while (line < 2) {
		fgets(header, 1024, fp);
		if (header[0] != '#') {
			++line;
		}
	}
	sscanf(header, "%d %d\n", w, h);//第二行为宽和高，第一行为P6标识
	fgets(header, 20, fp);//读最大像素
	fread(data, (*w)*(*h) * 3, 1, fp);//w*h个像素点，每个像素点rpg值

	fclose(fp);
}
void ppmWrite(const char* filename, unsigned char* data, int w, int h) {
	FILE* fp;
	fp = fopen(filename, "wb");

	fprintf(fp, "P6\n%d %d\n255\n", w, h);
	fwrite(data, w*h * 3, 1, fp);

	fclose(fp);
}
void ellipse_scan(int xc,int yc,int ra,int rb,unsigned char* data,int w,int h);
void ellipse_scan_middle_point(int xc,int yc,int ra,int rb,unsigned char* data,int w,int h);

int main() {
	int w, h,x,y,ra,rb;
	cin>>x>>y>>ra>>rb;
	w=400;
	h=300;
	unsigned char *data=new unsigned char[w*h*3];
	for(int i=0;i<w*h*3;i++) data[i]=255;//初始化为白色
	ellipse_scan_middle_point(x,y,ra,rb,data,w,h);
	//ellipse_scan(x,y,ra,rb,data,w,h);
	ppmWrite("test.ppm", data, w, h);
	delete [] data;
	return 0;
}

int my_round(double num)
{
	return (int)(num+0.5);
}

void ellipse_scan(int xc,int yc,int ra,int rb,unsigned char* data,int w,int h)
{
	double delta=0.1;//弧度差值,单位为度
	int xi,yi;
	double tmp_xi,tmp_yi; 
	double delta_cos=cos(delta/180.0*PI),delta_sin=sin(delta/180.0*PI);
	double last_cos=1,last_sin=0;//上一个像素点的cos和sin值
	for(int i=0;i<=900;i++)//只求1/4椭圆弧
	{
		tmp_xi=xc+ra*last_cos*delta_cos-ra*last_sin*delta_sin;
		tmp_yi=yc+rb*last_cos*delta_sin+rb*last_sin*delta_cos;
		last_cos=(tmp_xi-xc)/ra;
		last_sin=(tmp_yi-yc)/rb;
		xi=my_round(tmp_xi);
		yi=my_round(tmp_yi);
		if(abs(xi)<w/2 && abs(yi)<h/2)//在屏幕内
		{
			data[(h/2-yi)*w*3+(xi+w/2)*3]=0;
			data[(h/2-yi)*w*3+(xi+w/2)*3+1]=0;
			data[(h/2-yi)*w*3+(xi+w/2)*3+2]=0;
		}
		if(abs(2*xc-xi)<w/2 && abs(yi)<h/2)//左上圆弧
		{
			data[(h/2-yi)*w*3+(2*xc-xi+w/2)*3]=0;
			data[(h/2-yi)*w*3+(2*xc-xi+w/2)*3+1]=0;
			data[(h/2-yi)*w*3+(2*xc-xi+w/2)*3+2]=0;
		}
		if(abs(2*xc-xi)<w/2 && abs(2*yc-yi)<h/2)//左下圆弧
		{
			data[(h/2-(2*yc-yi))*w*3+(2*xc-xi+w/2)*3]=0;
			data[(h/2-(2*yc-yi))*w*3+(2*xc-xi+w/2)*3+1]=0;
			data[(h/2-(2*yc-yi))*w*3+(2*xc-xi+w/2)*3+2]=0;
		}
		if(abs(xi)<w/2 && abs(2*yc-yi)<h/2)//在屏幕内
		{
			data[(h/2-(2*yc-yi))*w*3+(xi+w/2)*3]=0;
			data[(h/2-(2*yc-yi))*w*3+(xi+w/2)*3+1]=0;
			data[(h/2-(2*yc-yi))*w*3+(xi+w/2)*3+2]=0;
		}
	}
}

void ellipse_scan_middle_point(int xc,int yc,int ra,int rb,unsigned char* data,int w,int h)//Bresenham中点画法
//参考https://blog.csdn.net/m0_62187515/article/details/127352287
{
	int xi,yi,i,last_x,last_y,last_d;
	// double x_45degree,y_45degree;//法向量恰为正向45度的点
	// x_45degree=xc+ra*ra/sqrt(ra*ra+rb*rb);
	// y_45degree=yc+rb*rb/sqrt(ra*ra+rb*rb);
	last_x=0;
	last_y=rb;
	last_d=rb*rb*(last_x+1)*(last_x+1)+ra*ra*(last_y-0.5)*(last_y-0.5)-ra*ra*rb*rb;//判断(x+1,y-1/2)在椭圆的内还是外.//初始值
	while(rb*rb*(last_x-xc)<ra*ra*(last_y-yc))//y变化更平缓的部分
	{
		xi=last_x+1;//以x为基准+1，下一个点要么为(x+1,y)，要么为(x+1,y-1)
		if(last_d<0) //中点在内部，取(x+1,y)
		{
			yi=last_y;
			last_d+=2*rb*rb*last_x+3*rb*rb;//中点位置的递推
		}
		else //外部或曲线上，取(x+1,y-1)
		{
			yi=last_y-1;
			last_d+=2*rb*rb*last_x+3*rb*rb-2*ra*ra*last_y+2*ra*ra;//中点位置的递推
		}

		last_x=xi;
		last_y=yi;
		if(abs(xi)<w/2 && abs(yi)<h/2)//在屏幕内
		{
			data[(h/2-yi)*w*3+(xi+w/2)*3]=0;
			data[(h/2-yi)*w*3+(xi+w/2)*3+1]=0;
			data[(h/2-yi)*w*3+(xi+w/2)*3+2]=0;
		}
		if(abs(2*xc-xi)<w/2 && abs(yi)<h/2)//左上圆弧
		{
			data[(h/2-yi)*w*3+(2*xc-xi+w/2)*3]=0;
			data[(h/2-yi)*w*3+(2*xc-xi+w/2)*3+1]=0;
			data[(h/2-yi)*w*3+(2*xc-xi+w/2)*3+2]=0;
		}
		if(abs(2*xc-xi)<w/2 && abs(2*yc-yi)<h/2)//左下圆弧
		{
			data[(h/2-(2*yc-yi))*w*3+(2*xc-xi+w/2)*3]=0;
			data[(h/2-(2*yc-yi))*w*3+(2*xc-xi+w/2)*3+1]=0;
			data[(h/2-(2*yc-yi))*w*3+(2*xc-xi+w/2)*3+2]=0;
		}
		if(abs(xi)<w/2 && abs(2*yc-yi)<h/2)//在屏幕内
		{
			data[(h/2-(2*yc-yi))*w*3+(xi+w/2)*3]=0;
			data[(h/2-(2*yc-yi))*w*3+(xi+w/2)*3+1]=0;
			data[(h/2-(2*yc-yi))*w*3+(xi+w/2)*3+2]=0;
		}
	}
	while(rb*rb*(last_x-xc)>ra*ra*(last_y-yc) && last_y>=0)//x部分变化更平缓的部分
	{
		yi=last_y-1;//以y为基准-1，下一个点要么为(x,y-1)，要么为(x+1,y-1)
		int middle_point_value=rb*rb*(last_x+0.5)*(last_x+0.5)+ra*ra*(last_y-1)*(last_y-1)-ra*ra*rb*rb;
		if(middle_point_value<0) xi=last_x+1;//中点在内部，取(x,y-1)
		else xi=last_x;//外部或曲线上，取(x+1,y-1)

		last_x=xi;
		last_y=yi;
		if(abs(xi)<w/2 && abs(yi)<h/2)//在屏幕内
		{
			data[(h/2-yi)*w*3+(xi+w/2)*3]=0;
			data[(h/2-yi)*w*3+(xi+w/2)*3+1]=0;
			data[(h/2-yi)*w*3+(xi+w/2)*3+2]=0;
		}
		if(abs(2*xc-xi)<w/2 && abs(yi)<h/2)//左上圆弧
		{
			data[(h/2-yi)*w*3+(2*xc-xi+w/2)*3]=0;
			data[(h/2-yi)*w*3+(2*xc-xi+w/2)*3+1]=0;
			data[(h/2-yi)*w*3+(2*xc-xi+w/2)*3+2]=0;
		}
		if(abs(2*xc-xi)<w/2 && abs(2*yc-yi)<h/2)//左下圆弧
		{
			data[(h/2-(2*yc-yi))*w*3+(2*xc-xi+w/2)*3]=0;
			data[(h/2-(2*yc-yi))*w*3+(2*xc-xi+w/2)*3+1]=0;
			data[(h/2-(2*yc-yi))*w*3+(2*xc-xi+w/2)*3+2]=0;
		}
		if(abs(xi)<w/2 && abs(2*yc-yi)<h/2)//在屏幕内
		{
			data[(h/2-(2*yc-yi))*w*3+(xi+w/2)*3]=0;
			data[(h/2-(2*yc-yi))*w*3+(xi+w/2)*3+1]=0;
			data[(h/2-(2*yc-yi))*w*3+(xi+w/2)*3+2]=0;
		}
	}
	//这只是a>b的情况，a<b的情况只需要交换x和y的位置即可
	//求出了middle_point_value的递推式
	//并未感受到两个算法在作图效果的差异
}

//如何画非轴对齐椭圆，一种简单想法，在对齐状态下求出各点，再求出旋转之后的对应各点坐标