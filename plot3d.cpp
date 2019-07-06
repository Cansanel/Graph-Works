#include<cmath>
#include<cstdio>
#include<graphics.h>

const double pi = 3.1415926535;

const int ScrW = 640;
const int ScrH = 480;

const int MAXN = 101;

double min(double x,double y) { return x>y?y:x; }

double max(double x,double y) { return x>y?x:y; }

double f(double x,double y) { return sin(sqrt(x*x+y*y)); }

void Line(double X0,double Y0,double X1,double Y1){
	line(round(X0),round(Y0),round(X1),round(Y1));
}

void proginit(){
	setinitmode(0);
	initgraph(ScrW,ScrH);
	setcaption("Plot3D Demo Graphics Output [By Cansanel]");
	setrendermode(RENDER_MANUAL);
	system("title Plot3D Demo Main [By Cansanel]");
	setcolor(WHITE);
	setbkcolor(BLACK);
	clearviewport();
	printf("Plot3D Demo Program [By Cansanel] 2019/05/30\n");
	printf("Plot:f(x,y)=Sin(Sqrt(x^2+y^2)\n");
	printf("Press ESC to exit.\n");
	printf("Roate:W A S D\n");
	printf("Scale:+/-\n");
	printf("WARNING:YOU SHOULD PRESS ESC IN THE GRAPHICS WINDOW TO HALT THE PROGRAM!");
}
void plot3d(double minx,double miny,double maxx,double maxy,int stp){
	double xct,yst,stsp,xstsp,ctsp,yctsp,acpb;
	double delta,theta,alpha,beta,phi,zoom;
	double cost,cosp,sint,sinp;
	double px,py,py0,dx,dy,x,y;
	double minz,maxz;
	double dat[MAXN][MAXN];
	char key;
	int l=8;
	
	delta = pi/12;
	theta = phi = pi/6;
	beta  = 1;
	alpha = min(ScrW*0.618/(maxx-minx),ScrH*0.618/(maxy-miny));
	dx    = (maxx-minx)/stp*1.0;
	dy    = (maxy-miny)/stp*1.0;
	maxz  = minz = f(minx,miny);
	
	//参数初始化 
	
	for(register int i=0;i<=stp;i++)
		for(register int j=0;j<=stp;j++){
			double X=minx+i*dx;
			double Y=miny+j*dy;
			dat[i][j]=f(X,Y);
			maxz=max(maxz,dat[i][j]);
			minz=min(minz,dat[i][j]);
		}
	
	//生成函数值表 
	
	zoom = 0.6*minz-0.4*maxz;
	beta = min(1.0,(ScrW/2)/(alpha*maxz-minz));
	while(true){
		dx    = alpha*(maxx-minx)/stp*1.0;
		dy    = alpha*(maxy-miny)/stp*1.0;
		cost  = cos(theta);
		cosp  = cos(phi);
		sint  = sin(theta);
		sinp  = sin(phi);
		xct   = dx*cost;
		yst   = dy*sint;
		stsp  = sint*sinp;
		xstsp = dx*stsp;
		ctsp  = cost*sinp;
		yctsp = dy*ctsp;
		acpb  = alpha*cosp*beta;
		
		l=16; x=16; y=16;
		Line(x,y,x-cost*l,y+l*stsp);
		Line(x,y,x+sint*l,y+l*ctsp);
		Line(x,y,x,y-l*cosp);
		//坐标系标识 
		
		for(register int j=0;j<stp;j++){
			x   = alpha*(minx-maxx)/2;
			y   = alpha*(miny-maxy)/2+j*dy;
			px  = -x*cost+y*sint+ScrW/2;
			py0 = -x*stsp-y*ctsp-ScrH/2;
			for(register int i=0;i<stp;i++){
				py = py0+acpb*(zoom+dat[i][j]);
				Line(px,-py,px-xct,-py+xstsp+acpb*(dat[i][j]-dat[i+1][j]));
				Line(px,-py,px+yst,-py+yctsp+acpb*(dat[i][j]-dat[i][j+1])); 
				px  -= xct;
				py0 -= xstsp;
			}
		}
		y = alpha*(maxy-miny)/2;
		for(register int i=0;i<stp;i++){
			x  = alpha*(minx-maxx)/2+i*dx;
			px = -x*cost+y*sint;
			py = -x*stsp-y*ctsp+acpb*(zoom+dat[i][stp]);
			Line(ScrW/2+px,ScrH/2-py,ScrW/2+px-xct,ScrH/2-py+xstsp+acpb*(dat[i][stp]-dat[i+1][stp]));
		}
		x=alpha*(maxx-minx)/2;
		for(register int j=0;j<stp;j++){
			y  = alpha*(miny-maxy)/2+j*dy;
			px = -x*cost+y*sint;
			py = -x*stsp-y*ctsp+acpb*(zoom+dat[stp][j]);
			Line(ScrW/2+px,ScrH/2-py,ScrW/2+px+yst,ScrH/2-py+yctsp+acpb*(dat[stp][j]-dat[stp][j+1])); 
		}
		key=getch();
		if (key==27) break;
		if (key==45) alpha*=0.8;
		if (key==43) alpha/=0.8;
		if (key=='w') phi-=pi/24;
		if (key=='s') phi+=pi/24;
		if (key=='d') theta+=pi/24;
		if (key=='a') theta-=pi/24;
		clearviewport();
	}
}
int main(){
	proginit();
	plot3d(-10,-10,10,10,32);
	//getch();
	closegraph();
	return 0;
}
