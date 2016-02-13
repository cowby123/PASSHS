#ifndef HOOKNtClose
#define HOOKNtClose
int nNtCloseAddr;
int nHookNtCloseAddr;
int nHookNtCloseJmp;
int nHookNtCloseHSJmp;
int NTCloseCall;
int NTClosepush;

char hookntClose[7];
int jmpclosehs = 0;
int jmpclose = 0;
__declspec(naked)void MyNtClose(){

	_asm{
		pushfd
		pushad
		}
		if(PanDuanProcessName("KartRider.exe")||PanDuanProcessName("MapleStory.exe")||PanDuanProcessName("CA.exe")){
			__asm
			{
				//如果是HS調用的
				popad
				popfd
				jmp jmpclosehs
				}
			}
		__asm{
			popad
			popfd
			jmp jmpclose//跳到白目的只下一行
		}
		
			
	}




void HookNtClose(){
	//取OP  OT地址
	nNtCloseAddr=GetSSDTFunctionAddr(50);
	//DbgPrint("nNtCloseAddr=%x\n",nNtCloseAddr);
	nHookNtCloseAddr=nNtCloseAddr;
	//DbgPrint("nHookNtCloseAddr=%x\n",nHookNtCloseAddr);
	//==================================保存前2字節UNHOOK
	GetProtectByte(nHookNtCloseAddr,hookntClose,5);
	jmpclosehs = GetJmpAddr(nNtCloseAddr+1);
	jmpclose = jmpclosehs+8;
	InLineHookEngine(nHookNtCloseAddr,(int)MyNtClose);//掛勾子囉
}

void UnHookNtClose(){

	//UnInLineHookEngine(nHookNtCloseAddr,hookntClose,5);
}


#endif