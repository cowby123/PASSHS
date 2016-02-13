#ifndef HOOKNTSetContextThread
#define HOOKNTSetContextThread
int nNtSetContextThreadAddr;
int nHookNtSetContextThreadAddr;
int nHookNtSetContextThreadJmp;
int SCTjmp = 0;
int SCTjmpHS = 0;
int SCTAdr = 0;
__declspec(naked)void MyNTSetContextThread(){
	_asm{
		pushfd
		pushad
	}


	if(PanDuanProcessName("KartRider.exe")||PanDuanProcessName("MapleStory.exe")||PanDuanProcessName("CA.exe")){
		_asm{
			popad
			popfd
			call SCTjmpHS
			jmp nHookNtSetContextThreadJmp 
		}
	}
	else{
		_asm{
			popad
			popfd
			call SCTjmp
			jmp nHookNtSetContextThreadJmp
		}
	}


	
			
	}




void HookNtSetContextThread(){
	

	nNtSetContextThreadAddr=GetSSDTFunctionAddr(316);
	DbgPrint("nNtSetContextThreadAddr=%x\n",nNtSetContextThreadAddr);
	nHookNtSetContextThreadAddr=nNtSetContextThreadAddr+0x2b;
	DbgPrint("nHookNtSetContextThreadAddr=%x\n",nHookNtSetContextThreadAddr);
	SCTAdr = GetCallAddr(nHookNtSetContextThreadAddr+1);
	SCTjmpHS = SCTAdr;
	SCTjmp = SCTAdr+0x8;
	//DbgPrint("adr=%x\n",adr);
	DbgPrint("SCTjmpHS=%x\n",SCTjmpHS);
	DbgPrint("SCTjmp=%x\n",SCTjmp);
	nHookNtSetContextThreadJmp=nNtSetContextThreadAddr+0x30;//非HS調用跳轉點
	DbgPrint("nHookNtSetContextThreadJmp=%x\n",nHookNtSetContextThreadJmp);
	InLineHookEngine(nHookNtSetContextThreadAddr,(int)MyNTSetContextThread);//掛勾子囉
}

void UnHookNtSetContextThread(){

	//UnInLineHookEngine(nHookNtSetContextThreadAddr,hookntop,5);
}


#endif