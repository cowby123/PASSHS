#ifndef HOOKNTGetContextThread
#define HOOKNTGetContextThread
int nNtGetContextThreadAddr;
int nHookNtGetContextThreadAddr;
int nHookNtGetContextThreadJmp;
int GCTjmp = 0;
int GCTjmpHS = 0;
int GCTAdr = 0;
__declspec(naked)void MyNTGetContextThread(){
	_asm{
		pushfd
		pushad
	}


	if(PanDuanProcessName("KartRider.exe")||PanDuanProcessName("MapleStory.exe")||PanDuanProcessName("CA.exe")){
		_asm{
			popad
			popfd
			call GCTjmpHS
			jmp nHookNtGetContextThreadJmp 
		}
	}
	else{
		_asm{
			popad
			popfd
			call GCTjmp
			jmp nHookNtGetContextThreadJmp
		}
	}


	
			
	}




void HookNtGetContextThread(){
	

	nNtGetContextThreadAddr=GetSSDTFunctionAddr(135);
	//DbgPrint("nNtGetContextThreadAddr=%x\n",nNtGetContextThreadAddr);
	nHookNtGetContextThreadAddr=nNtGetContextThreadAddr+0x2b;
	//DbgPrint("nHookNtGetContextThreadAddr=%x\n",nHookNtGetContextThreadAddr);
	GCTAdr = GetCallAddr(nHookNtGetContextThreadAddr+1);
	GCTjmpHS = GCTAdr;
	GCTjmp = GCTAdr+0x8;
	//DbgPrint("adr=%x\n",adr);
	//DbgPrint("GCTjmpHS=%x\n",GCTjmpHS);
	//DbgPrint("GCTjmp=%x\n",GCTjmp);
	nHookNtGetContextThreadJmp=nNtGetContextThreadAddr+0x30;//非HS調用跳轉點
	//DbgPrint("nHookNtGetContextThreadJmp=%x\n",nHookNtGetContextThreadJmp);
	InLineHookEngine(nHookNtGetContextThreadAddr,(int)MyNTGetContextThread);//掛勾子囉
}

void UnHookNtGetContextThread(){

	//UnInLineHookEngine(nHookNtGetContextThreadAddr,hookntop,5);
}


#endif