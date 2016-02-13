#ifndef HOOKNtProtectVirtualMemory
#define HOOKNtProtectVirtualMemory
int nNtProtectVirtualMemoryAddr;
int nHookNtProtectVirtualMemoryAddr;
int nHookNtProtectVirtualMemoryJmp;
int nHookNtProtectVirtualMemoryHSJmp;
int NTProtectCall;
int NTProtectpush;

char hookntProtect[7];

__declspec(naked)void MyNtProtectVirtualMemory(){
	_asm{
		push    38h
		push    NTProtectpush
		pushfd
		pushad
		}
		if(PanDuanProcessName("KartRider.exe")||PanDuanProcessName("MapleStory.exe")||PanDuanProcessName("CA.exe")){
			__asm
			{
				//如果是HS調用的
				popad
				popfd
				jmp nHookNtProtectVirtualMemoryHSJmp
				}
			}
		__asm{
			popad
			popfd
			call NTProtectCall//呼叫白目_SEH_prolog地址
			jmp nHookNtProtectVirtualMemoryJmp//跳到白目的只下一行
		}

			
	}




void HookNtProtectVirtualMemory(){
	//取OP  OT地址
	nNtProtectVirtualMemoryAddr=GetSSDTFunctionAddr(215);
	//DbgPrint("nNtProtectVirtualMemoryAddr=%x\n",nNtProtectVirtualMemoryAddr);
	nHookNtProtectVirtualMemoryAddr=nNtProtectVirtualMemoryAddr;
	//DbgPrint("nHookNtProtectVirtualMemoryAddr=%x\n",nHookNtProtectVirtualMemoryAddr);
	//==================================保存前2字節UNHOOK
	GetProtectByte(nHookNtProtectVirtualMemoryAddr,hookntProtect,5);
	
	//==================================
	NTProtectpush = nNtProtectVirtualMemoryAddr+0x2;
	int *push= reinterpret_cast<int*>(NTProtectpush+1);
	NTProtectpush = *push;
	nHookNtProtectVirtualMemoryJmp=nNtProtectVirtualMemoryAddr+0xc;//非HS調用跳轉點
	nHookNtProtectVirtualMemoryHSJmp=nNtProtectVirtualMemoryAddr+0x7;//HS調用跳轉點*/
	NTProtectCall = GetCallAddr(nHookNtProtectVirtualMemoryHSJmp+1);
	//DbgPrint("NTProtectCall=%x\n",NTProtectCall);
	char *op1= reinterpret_cast<char*>(NTProtectCall);
	int bytecode=*op1;
	if (bytecode == 0xffffffe9){
		NTProtectCall = NTProtectCall+0x8;
		//DbgPrint("NTProtectCall=%x\n",NTProtectCall);
		NTProtectCall = GetJmpAddr(NTProtectCall+1);
		//DbgPrint("NTProtectCall=%x\n",NTProtectCall);
		//DbgPrint("hs is runnint\n");
	}
	//DbgPrint("nHookNtProtectVirtualMemoryJmp=%x\n",nHookNtProtectVirtualMemoryJmp);
	//DbgPrint("nHookNtProtectVirtualMemoryHSJmp=%x\n",nHookNtProtectVirtualMemoryHSJmp);
	//DbgPrint("NTProtectpush=%x\n",NTProtectpush);
	//DbgPrint("NTProtectCall=%x\n",NTProtectCall);
	//DbgPrint("bytecode=%x\n",bytecode);
	InLineHookEngine(nHookNtProtectVirtualMemoryAddr,(int)MyNtProtectVirtualMemory);//掛勾子囉
}

void UnHookNtProtectVirtualMemory(){

	UnInLineHookEngine(nHookNtProtectVirtualMemoryAddr,hookntProtect,5);
}


#endif