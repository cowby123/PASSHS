#ifndef HOOKNtReadVirtualMemory
#define HOOKNtReadVirtualMemory
int nNtReadVirtualMemoryAddr;
int nHookNtReadVirtualMemoryAddr;
int nHookNtReadVirtualMemoryJmp;
int nHookNtReadVirtualMemoryHSJmp;
int NTReadCall;
int NTReadPush;

char hookntread[7];

__declspec(naked)void MyNtReadVirtualMemory(){
	_asm{
		push    18h
		push    NTReadPush
		pushfd
		pushad
		}
		if(PanDuanProcessName("KartRider.exe")||PanDuanProcessName("MapleStory.exe")||PanDuanProcessName("CA.exe")){
			__asm
			{
				//如果是HS調用的
				popad
				popfd
				jmp nHookNtReadVirtualMemoryHSJmp
				}
			}
		__asm{
			popad
			popfd
			call NTReadCall//呼叫白目_SEH_prolog地址
			jmp nHookNtReadVirtualMemoryJmp//跳到白目的只下一行
		}

			
	}




void HookNtReadVirtualMemory(){
	//取OP  OT地址
	nNtReadVirtualMemoryAddr=GetSSDTFunctionAddr(277);
	//DbgPrint("nNtReadVirtualMemoryAddr=%x\n",nNtReadVirtualMemoryAddr);
	nHookNtReadVirtualMemoryAddr=nNtReadVirtualMemoryAddr;
	//DbgPrint("nHookNtReadVirtualMemoryAddr=%x\n",nHookNtReadVirtualMemoryAddr);
	//==================================保存前2字節UNHOOK
	GetProtectByte(nHookNtReadVirtualMemoryAddr,hookntread,5);
	
	//==================================
	NTReadPush = nNtReadVirtualMemoryAddr+0x2;
	int *push= reinterpret_cast<int*>(NTReadPush+1);
	NTReadPush = *push;
	nHookNtReadVirtualMemoryJmp=nNtReadVirtualMemoryAddr+0xc;//非HS調用跳轉點
	nHookNtReadVirtualMemoryHSJmp=nNtReadVirtualMemoryAddr+0x7;//HS調用跳轉點*/
	NTReadCall = GetCallAddr(nHookNtReadVirtualMemoryHSJmp+1);
	char *op1= reinterpret_cast<char*>(NTReadCall);
	int bytecode=*op1;
	if (bytecode == 0xffffffe9){
		NTReadCall = NTReadCall+0x8;
		//DbgPrint("NTReadCall=%x\n",NTReadCall);
		NTReadCall = GetJmpAddr(NTReadCall+1);
		//DbgPrint("NTReadCall=%x\n",NTReadCall);
		//DbgPrint("hs is runnint\n");
	}
	//DbgPrint("nHookNtReadVirtualMemoryJmp=%x\n",nHookNtReadVirtualMemoryJmp);
	//DbgPrint("nHookNtReadVirtualMemoryHSJmp=%x\n",nHookNtReadVirtualMemoryHSJmp);
	//DbgPrint("NTReadPush=%x\n",NTReadPush);
	//DbgPrint("NTReadCall=%x\n",NTReadCall);
	//DbgPrint("bytecode=%x\n",bytecode);
	InLineHookEngine(nHookNtReadVirtualMemoryAddr,(int)MyNtReadVirtualMemory);//掛勾子囉
}

void UnHookNtReadVirtualMemory(){

	UnInLineHookEngine(nHookNtReadVirtualMemoryAddr,hookntread,5);
}


#endif