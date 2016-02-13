#ifndef HOOKNtWriteVirtualMemory
#define HOOKNtWriteVirtualMemory
int nNtWriteVirtualMemoryAddr;
int nHookNtWriteVirtualMemoryAddr;
int nHookNtWriteVirtualMemoryJmp;
int nHookNtWriteVirtualMemoryHSJmp;
int NTWriteCall;
int NTWritePush;

char hookntwrite[7];

__declspec(naked)void MyNtWriteVirtualMemory(){
	_asm{
		push    18h
		push    NTWritePush
		pushfd
		pushad
		}
		if(PanDuanProcessName("KartRider.exe")||PanDuanProcessName("MapleStory.exe")||PanDuanProcessName("CA.exe")){
			__asm
			{
				//如果是HS調用的
				popad
				popfd
				jmp nHookNtWriteVirtualMemoryHSJmp
				}
			}
		__asm{
			popad
			popfd
			call NTWriteCall//呼叫白目_SEH_prolog地址
			jmp nHookNtWriteVirtualMemoryJmp//跳到白目的只下一行
		}

			
	}




void HookNtWriteVirtualMemory(){
	//取OP  OT地址
	nNtWriteVirtualMemoryAddr=GetSSDTFunctionAddr(399);
	//DbgPrint("nNtWriteVirtualMemoryAddr=%x\n",nNtWriteVirtualMemoryAddr);
	nHookNtWriteVirtualMemoryAddr=nNtWriteVirtualMemoryAddr;
	//DbgPrint("nHookNtWriteVirtualMemoryAddr=%x\n",nHookNtWriteVirtualMemoryAddr);
	//==================================保存前2字節UNHOOK
	GetProtectByte(nHookNtWriteVirtualMemoryAddr,hookntwrite,5);
	
	//==================================
	NTWritePush = nNtWriteVirtualMemoryAddr+0x2;
	int *push= reinterpret_cast<int*>(NTWritePush+1);
	NTWritePush = *push;
	nHookNtWriteVirtualMemoryJmp=nNtWriteVirtualMemoryAddr+0xc;//非HS調用跳轉點
	nHookNtWriteVirtualMemoryHSJmp=nNtWriteVirtualMemoryAddr+0x7;//HS調用跳轉點*/
	NTWriteCall = GetCallAddr(nHookNtWriteVirtualMemoryHSJmp+1);
	//DbgPrint("NTWriteCall=%x\n",NTWriteCall);
	char *op1= reinterpret_cast<char*>(NTWriteCall);
	int bytecode=*op1;
	if (bytecode == 0xffffffe9){
		NTWriteCall = NTWriteCall+0x8;
		//DbgPrint("NTWriteCall=%x\n",NTWriteCall);
		NTWriteCall = GetJmpAddr(NTWriteCall+1);
		//DbgPrint("NTWriteCall=%x\n",NTWriteCall);
		//DbgPrint("hs is runnint\n");
	}
	//DbgPrint("nHookNtWriteVirtualMemoryJmp=%x\n",nHookNtWriteVirtualMemoryJmp);
	//DbgPrint("nHookNtWriteVirtualMemoryHSJmp=%x\n",nHookNtWriteVirtualMemoryHSJmp);
	//DbgPrint("NTWritePush=%x\n",NTWritePush);
	//DbgPrint("NTWriteCall=%x\n",NTWriteCall);
	//DbgPrint("bytecode=%x\n",bytecode);
	InLineHookEngine(nHookNtWriteVirtualMemoryAddr,(int)MyNtWriteVirtualMemory);//掛勾子囉
}

void UnHookNtWriteVirtualMemory(){

	UnInLineHookEngine(nHookNtWriteVirtualMemoryAddr,hookntwrite,5);
}


#endif