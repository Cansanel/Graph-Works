#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<cmath>
#include<algorithm>
#include<stack>
#include<graphics.h>

//-lgraphics64 -lgdi32 -limm32 -lmsimg32 -lole32 -loleaut32 -lwinmm -luuid

using namespace std;

const string BuildInOperators [] = {"mod","div","==","!=","*-","/-","^-","*+","/+","^+","<=",">=","<",">","+","-","*","/","%","^","(",")",","};

const string BuildInFunctions [] = {"NTHRoot","Floor","Trunc","Round","Sqrt","Ceil","Asin","Atan","Acos","Log","Abs","Sin","Cos","Tan","Sgn","Min","Max","Ln","Lg"};

const double pi = 3.1415926535;

struct DataStream{
	vector <string> dat;
	vector <int>    typ;
	vector <int>	tag;
	//typ 0 BuildInOperators
	//typ 1 Number
	//typ 2 User
	//typ 3 Function
};

vector <long long> ErrMsg;

// 1 Syntax Error
// 2 Div by 0
// 3 Argu Number does not match
// 4 Real Number "." too much
// 5 "()" does not match
// 6 Sqrt(-...)

map <string,long double> Values;
map <string,long double/*string*/> UserFunctions;

string InputString;

int PlotScrX1=2,PlotScrY1=2,PlotScrX2=800-2,PlotScrY2=600-22;

int ScrW=800,ScrH=600;

long double stp = 0.01;
long double rx1=-10,rx2=10,ry1=-10,ry2=10;
//long double ValueX;

string DelSpace(string st){
	string res;
	for(register int i=0;i<st.size();i++)
		if (st[i]!=' ') res.push_back(st[i]);
	return res;
}

long double String2Number(string st){
	bool test=false;
	for(register int i=0;i<st.size();i++){
		if (st[i]=='.'){
			test=true; continue;
		}
		if (test && st[i]=='.'){
			ErrMsg.push_back(4);
			return 0;
		}
	}
	long double res=0;
	bool flg=false;
	int tims=1;
	if (st[0]=='-') tims=-1,st.erase(st.begin(),st.begin()+1);
	for(register int i=0;i<st.size();i++){
		if (st[i]=='.'){
			flg=true;
			continue;
		}
		if (flg) tims*=10;
		res=res*10+st[i]-'0';
	}
	return res/tims*1.0;
}

string Number2String(long double x,int floatpoint){
	string res="";
	bool flg=x<0; x*=pow(10,floatpoint); int inp=abs(round(x));
	while(inp){
		res.push_back(inp%10+'0');
		inp/=10; floatpoint--;
		if (!floatpoint) res.push_back('.');
	}
	reverse(res.begin(),res.end());
	if (flg) res="-"+res;
	return res;
}

bool StringFirstPart(string text,string matcher){
	if (text.size()<matcher.size()) return false;
	for(register int i=0;i<matcher.size();i++)
		if (text[i]!=matcher[i])
			return false;
	return true;
}

void Line(double X0,double Y0,double X1,double Y1){
	line(round(X0),round(Y0),round(X1),round(Y1));
}

DataStream Separator(string InputString){
	DataStream res;
	string tmp;
	int flg,addr=0;
	bool ign;
	while(!InputString.empty()){
		flg=InputString.size();
		ign=false;
		tmp.clear();
		for(register int i=0;i<sizeof(BuildInFunctions)/sizeof(BuildInFunctions[0]) && !InputString.empty();i++)
			if (StringFirstPart(InputString,BuildInFunctions[i])){
				res.dat.push_back(BuildInFunctions[i]);
				res.typ.push_back(3);
				res.tag.push_back(addr);
				InputString.erase(InputString.begin(),InputString.begin()+BuildInFunctions[i].size());
				addr+=BuildInFunctions[i].size();
				ign=true;
			}
		if (ign) continue;
		for(register int i=0;i<sizeof(BuildInOperators)/sizeof(BuildInOperators[0]) && !InputString.empty();i++)
			if (StringFirstPart(InputString,BuildInOperators[i])){
				res.dat.push_back(BuildInOperators[i]);
				res.typ.push_back(0);
				res.tag.push_back(addr);
				InputString.erase(InputString.begin(),InputString.begin()+BuildInOperators[i].size());
				addr+=BuildInOperators[i].size();
				ign=true;
			}
		if (ign) continue;
		for(register int i=0;i<InputString.size() && ((InputString[i]>='0' && InputString[i]<='9') || InputString[i]=='.');i++)
			tmp.push_back(InputString[i]);
		if (!tmp.empty()){
			res.dat.push_back(tmp);
			res.typ.push_back(1);
			res.tag.push_back(addr);
			InputString.erase(InputString.begin(),InputString.begin()+tmp.size());
			addr+=tmp.size();
			tmp.clear();
			ign=true;
		}
		if (ign) continue;
		for(register int i=0;i<InputString.size() && ((InputString[i]>='a' && InputString[i]<='z') || (InputString[i]>='A' && InputString[i]<='Z') || InputString[i]=='_');i++)
			tmp.push_back(InputString[i]);
		if (!tmp.empty()){
			res.dat.push_back(tmp);
			res.typ.push_back(2);
			res.tag.push_back(addr);
			InputString.erase(InputString.begin(),InputString.begin()+tmp.size());
			addr+=tmp.size();
			tmp.clear();
		}
		if (flg==InputString.size()){
			ErrMsg.push_back(1);
			return res;
		}
	}
	return res;
}

long long FastPow(long long a,long long n){
	long long res=1;
	while(n){
		if (n&1) res*=a;
		a*=a; 
		n>>=1;
	}
	return res;
}

int CalcPriority(string cmd){
	if (cmd=="(" || cmd==")" || cmd=="[" || cmd=="]") return 1;
	if (cmd=="^" || cmd=="^-") return 2;
	if (cmd=="*" || cmd=="/" || cmd=="*-" || cmd=="/-" || cmd=="%") return 3;
	if (cmd=="+" || cmd=="-") return 4;
	if (cmd=="<" || cmd=="<=" || cmd==">" || cmd==">=") return 6;
	if (cmd=="==" || cmd=="!=") return 7;
	if (cmd==":=" || cmd=="->" || cmd=="<-") return 8;
	if (cmd==",") return 9;
	return 0;
}

DataStream Infix2Postfix(DataStream InputStream){
	DataStream res;
	stack <pair<string,int> > Operators;
	for(register int i=0;i<InputStream.dat.size();i++){
		if (InputStream.typ[i]==1 || InputStream.typ[i]==2){
			res.dat.push_back(InputStream.dat[i]);
			res.typ.push_back(InputStream.typ[i]);
			continue;
		}
		if (InputStream.dat[i]=="("){
			Operators.push(make_pair("(",0));
			continue;
		}
		if (InputStream.dat[i]==")"){
			while(!Operators.empty() && Operators.top().first!="("){
				res.dat.push_back(Operators.top().first);
				res.typ.push_back(Operators.top().second);
				Operators.pop();
			}
			if (!Operators.empty()) Operators.pop();
			continue;
		}
		if (InputStream.typ[i]==0 || InputStream.typ[i]==3){
			while(!Operators.empty() && Operators.top().first!="(" && CalcPriority(Operators.top().first)<=CalcPriority(InputStream.dat[i])){
				res.dat.push_back(Operators.top().first);
				res.typ.push_back(Operators.top().second);
				Operators.pop();
			}
			Operators.push(make_pair(InputStream.dat[i],InputStream.typ[i]));
		}
	}
	while(!Operators.empty()){
		res.dat.push_back(Operators.top().first);
		res.typ.push_back(Operators.top().second);
		Operators.pop();
	}
	return res;
}

long double DoFunction(vector <long double> Argus,string cmd){
	if (cmd=="Sin" || cmd=="Cos" || cmd=="Tan" || cmd=="Sqrt" || cmd=="Ceil"
		|| cmd=="Round" || cmd=="Trunc" || cmd=="Floor" || cmd=="Sgn" || cmd=="Abs"){
			if (Argus.size()!=1){
				ErrMsg.push_back(3);
				return 0;
			}
		}
	if (cmd=="Log"){
		if (Argus.size()>2){
			ErrMsg.push_back(3);
			return 0;
		}
	}
	if (cmd=="NTHRoot"){
		if (Argus.size()!=2){
			ErrMsg.push_back(3);
			return 0;
		}
	}
	if (cmd=="Sin") return sin(Argus[0]);
	if (cmd=="Cos") return cos(Argus[0]);
	if (cmd=="Tan") return tan(Argus[0]);
	if (cmd=="Asin") return asin(Argus[0]);
	if (cmd=="Acos") return acos(Argus[0]);
	if (cmd=="Atan") return atan(Argus[0]);
	if (cmd=="Abs") return abs(Argus[0]);
	if (cmd=="Log"){
		long double base=10,num;
		if (Argus.size()>=2) base=Argus[0],num=Argus[1];
			else num=Argus[0];
		return log10(num)/log10(base);
	}
	if (cmd=="Sqrt"){
		if (Argus[0]<0){
			ErrMsg.push_back(6);
			return 0;
		}
		return sqrt(Argus[0]);
	}
	if (cmd=="Ceil") return ceil(Argus[0]);
	if (cmd=="Round") return round(Argus[0]);
	if (cmd=="Trunc") return trunc(Argus[0]);
	if (cmd=="Floor") return floor(Argus[0]);
	if (cmd=="Sgn") return Argus[0]<0?-1:(Argus[0]==0?0:1);
	if (cmd=="NTHRoot"){
		if (Argus[1]==0){
			ErrMsg.push_back(2);
			return 0;
		}
		return pow(Argus[0],1/Argus[1]);
	}
}

long double DoOperator(long double x,long double y,string cmd,bool onlyone){
	if (onlyone){
		if (cmd=="+") return x;
		if (cmd=="-") return -x;
	}
	if (cmd=="+") return x+y;
	if (cmd=="-") return x-y;
	if (cmd=="*") return x*y;
	if (cmd=="/"){
		if (y==0){
			ErrMsg.push_back(2);
			return 0;
		}
		return x/y;
	}
	if (cmd=="*-") return x*(-y);
	if (cmd=="/-"){
		if (y==0){
			ErrMsg.push_back(2);
			return 0;
		}
		return x/(-y);
	}
	if (cmd=="^") return pow(x,y);
	if (cmd=="^-") return pow(x,-y);
	if (cmd=="div"){
		if (y==0){
			ErrMsg.push_back(2);
			return 0;
		}
		return trunc(x/y);
	}
	if (cmd=="mod"){
		if (y==0){
			ErrMsg.push_back(2);
			return 0;
		}
		return ((int) x)%((int) y);
	}
}

long double Calc(DataStream InputStream){
	stack <long double> NumStack;
	vector <long double> Argus;
	Argus.clear();
	for(register int i=0;i<InputStream.dat.size();i++){
		if (InputStream.typ[i]==1) NumStack.push(String2Number(InputStream.dat[i]));
		if (InputStream.typ[i]==2)
			NumStack.push(Values[InputStream.dat[i]]);
		if (InputStream.dat[i]==","){
			long double tmp=NumStack.top(); NumStack.pop();
			Argus.push_back(NumStack.top()); NumStack.pop();
			Argus.push_back(tmp);
			continue;
		}
		if (InputStream.typ[i]==0){
			long double x=NumStack.top(); NumStack.pop();
			long double res;
			if (NumStack.empty()) res=DoOperator(x,0,InputStream.dat[i],true);
				else{
					res=DoOperator(NumStack.top(),x,InputStream.dat[i],false);
					NumStack.pop();
				}
			if (!ErrMsg.empty()) return 0;
			NumStack.push(res);
		}
		if (InputStream.typ[i]==3){
			if (Argus.empty()){
				if (NumStack.empty()){
					ErrMsg.push_back(3);
					return 0;
				}
				Argus.push_back(NumStack.top());
				NumStack.pop();
			}
			NumStack.push(DoFunction(Argus,InputStream.dat[i]));
			if (!ErrMsg.empty()) return 0;
			Argus.clear();
		}
	}
	return NumStack.top();
}

vector <pair<long double,long double> > FunctionNumList(DataStream Expr,long double step,long double minx,long double maxx){
	vector <pair<long double,long double> > res;
	ErrMsg.clear();
	for(register long double x=minx;x<=maxx;x+=step){
		Values["x"]=x;
		long double tmp=Calc(Expr);
		if (!ErrMsg.empty()){
			ErrMsg.clear();
			continue;
		}
		res.push_back(make_pair(x,tmp));
	}
	return res;
}

void DrawPlotScreen(long double minx,long double maxx,long double miny,long double maxy){
	setcolor(WHITE); setbkcolor(BLACK);
	clearviewport();
	rectangle(PlotScrX1,PlotScrY1,PlotScrX2,PlotScrY2);
	long double scalex=(maxx-minx)/(PlotScrX2-PlotScrX1);
	long double scaley=(maxy-miny)/(PlotScrY2-PlotScrY1);
	if (minx<=0 && 0<=maxx){
		line(PlotScrX1+(0-minx)/scalex,PlotScrY1,PlotScrX1+(0-minx)/scalex,PlotScrY2);
	}
	if (miny<=0 && 0<=maxy){
		line(PlotScrX1,PlotScrY2-(0-miny)/scaley,PlotScrX2,PlotScrY2-(0-miny)/scaley);
	}
}

void FunctionPlot(string Expr,long double minx,long double maxx,long double miny,long double maxy){
	DataStream expr=Infix2Postfix(Separator(Expr));
	DrawPlotScreen(minx,maxx,miny,maxy);
	vector <pair<long double,long double> > FunctionData=FunctionNumList(expr,stp,minx,maxx);
	long double scalex=(maxx-minx)/(PlotScrX2-PlotScrX1);
	long double scaley=(maxy-miny)/(PlotScrY2-PlotScrY1);
	for(register int i=1;i<FunctionData.size();i++){
		long double x1=FunctionData[i-1].first;
		long double x2=FunctionData[ i ].first;
		long double y1=FunctionData[i-1].second;
		long double y2=FunctionData[ i ].second;
		x1=(long double) PlotScrX1+(x1-minx)/scalex;
		x2=(long double) PlotScrX1+(x2-minx)/scalex;
		y1=(long double) PlotScrY2-(y1-miny)/scaley-1;
		y2=(long double) PlotScrY2-(y2-miny)/scaley-1;
		Line(max(PlotScrX1,min((int)x1,PlotScrX2)),max(PlotScrY1,min((int)y1,PlotScrY2-1)),max(PlotScrX1,min((int)x2,PlotScrX2)),max(PlotScrY1,min((int)y2,PlotScrY2-1)));
	}
}

void PolarPlot(string Expr,long double mintheta,long double maxtheta,long double minx,long double maxx,long double miny,long double maxy){
	DataStream expr=Infix2Postfix(Separator(Expr));
	DrawPlotScreen(minx,maxx,miny,maxy);
	vector <pair<long double,long double> > FunctionData=FunctionNumList(expr,stp,mintheta,maxtheta);
	long double scalex=(maxx-minx)/(PlotScrX2-PlotScrX1);
	long double scaley=(maxy-miny)/(PlotScrY2-PlotScrY1);
	for(register int i=0;i<FunctionData.size();i++){
		long double tx=FunctionData[i].first;
		long double ty=FunctionData[i].second;
		FunctionData[i].first=ty*cos(tx);
		FunctionData[i].second=ty*sin(tx);
	}
	for(register int i=1;i<FunctionData.size();i++){
		long double x1=FunctionData[i-1].first;
		long double x2=FunctionData[ i ].first;
		long double y1=FunctionData[i-1].second;
		long double y2=FunctionData[ i ].second;
		x1=(long double) PlotScrX1+(x1-minx)/scalex;
		x2=(long double) PlotScrX1+(x2-minx)/scalex;
		y1=(long double) PlotScrY2-(y1-miny)/scaley-1;
		y2=(long double) PlotScrY2-(y2-miny)/scaley-1;
		Line(max(PlotScrX1,min((int)x1,PlotScrX2)),max(PlotScrY1,min((int)y1,PlotScrY2-1)),max(PlotScrX1,min((int)x2,PlotScrX2)),max(PlotScrY1,min((int)y2,PlotScrY2-1)));
	}
}

void plot3d(string Expr,double minx,double miny,double maxx,double maxy,int stp){
	double xct,yst,stsp,xstsp,ctsp,yctsp,acpb;
	double delta,theta,alpha,beta,phi,zoom;
	double cost,cosp,sint,sinp;
	double px,py,py0,dx,dy,x,y;
	double minz,maxz;
	double dat[101][101];
	char key;
	int l=8;
	
	DataStream expr=Infix2Postfix(Separator(Expr));
	
	delta = pi/12;
	theta = phi = pi/6;
	beta  = 1;
	alpha = min(ScrW*0.618/(maxx-minx),ScrH*0.618/(maxy-miny));
	dx    = (maxx-minx)/stp*1.0;
	dy    = (maxy-miny)/stp*1.0;
	Values["x"]=minx; Values["y"]=miny;
	maxz  = minz = Calc(expr);
	
	//������ʼ�� 
	
	clearviewport();
	
	for(register int i=0;i<=stp;i++)
		for(register int j=0;j<=stp;j++){
			Values["x"]=minx+i*dx;
			Values["y"]=miny+j*dy;
			dat[i][j]=Calc(expr);
			maxz=max(maxz,dat[i][j]);
			minz=min(minz,dat[i][j]);
		}
	
	//maxz=min(10.0,maxz); minz=max(-10.0,minz);
	
	//���ɺ���ֵ�� 
	
	zoom = 0.6*minz-0.4*maxz;
	beta = min(1.0,(ScrW/2)/(alpha*maxz-minz));
	while(true){
		outtextxy(1,ScrH-32,"Press ESC to exit Plot3D Mode.");
		outtextxy(1,ScrH-16,"Roate:W A S D ; Scale:+/-");
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
		//����ϵ��ʶ 
		
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

void ContourPlot(string Expr,long double minx,long double maxx,long double miny,long double maxy){
	DrawPlotScreen(minx,maxx,miny,maxy);
}

void SyntaxCheck(string InputString){
	int prs=0;
	for(register int i=0;i<InputString.size();i++){
		if (InputString[i]=='(') prs++;
		if (InputString[i]==')') prs--;
	}
	if (prs!=0){
		ErrMsg.push_back(5); return;
	}
}

void OutputDataStream(DataStream ds){
	for(register int i=0;i<ds.dat.size();i++)
		cout<<i<<'	'<<ds.typ[i]<<'	'<<ds.dat[i]<<endl;
}

void DrawProgUI(){
	
}


void ProgInit(){
	setinitmode(0);
	initgraph(ScrW,ScrH);
	setrendermode(RENDER_MANUAL);
	system("title GRAPH WORKS");
	setcaption("GRAPH WORKS");
	system("cls");
	cout<<"GRAPH WORKS"<<endl;
	cout<<"Copyright 2017-2019 COMFORE Studio"<<endl<<endl;
	cout<<"Type \"Help\" for some information."<<endl<<endl;
}

int main(){
	ProgInit();
	//string st; cin>>st; cout<<Calc(Separator(st));
	string st,mode="Function";
	while(cin>>st){
		if (st=="Exit") break;
		if (st=="SetPlotRange"){
			cout<<"Input 4 numbers (x1,x2,y1,y2):";
			cin>>rx1>>rx2>>ry1>>ry2;
			continue;
		}
		if (st=="SetPlotStep"){
			cout<<"Input Step (10e-5 ~ 10e100):";
			cin>>stp;
			continue;
		}
		if (st=="SetWindowSize"){
			cout<<"Input screen size (x,y):";
			cin>>ScrW>>ScrH;
			closegraph();
			ProgInit();
		}
		if (st=="SetPlotScreenAddress"){
			cout<<"Input 4 numbers (x1,y1,x2,y2):";
			cin>>PlotScrX1>>PlotScrY1>>PlotScrX2>>PlotScrY2;
		}
		if (st=="Polar" || st=="Function" || st=="Plot3D") mode=st;
		//FunctionPlot(st,rx1,rx2,ry1,ry2);
		if (mode=="Polar") PolarPlot(st,0,10,rx1,rx2,ry1,ry2);
		if (mode=="Function") FunctionPlot(st,rx1,rx2,ry1,ry2);
		outtextxy(680,580,mode=="Polar"?"Polar Plot":(mode=="Function"?"Function Plot":"Plot3D"));
		if (mode=="Plot3D") plot3d(st,rx1,ry1,rx2,ry2,32);
		delay_fps(10);
	}
	closegraph();
	return 0;
}

