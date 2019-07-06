#include<graphics.h>
#include<vector>
#include<string>
#include<cstdio>

using namespace std;

const int MAXN = 65536;

struct CmdRecord{
	unsigned int CommandID;
	unsigned int ArguAddrs[32];
};

CmdRecord GraData[MAXN];

vector <string> Data;

vector <string> ErrMsg;

PIMAGE Buffers[256];

int ScrW,ScrH;

void CommandExecute(unsigned int CmdID){
	
}

void FileReader(){
	
}

int main(int arugcnt,char ** arugs){
	return 0;	
}
