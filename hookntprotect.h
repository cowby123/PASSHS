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
				//�p�G�OHS�եΪ�
				popad
				popfd
				jmp nHookNtProtectVirtualMemoryHSJmp
				}
			}
		__asm{
			popad
			popfd
			call NTProtectCall//�I�s�ե�_SEH_prolog�a�}
			jmp nHookNtProtectVirtualMemoryJmp//����եت��u�U�@��
		}

			
	}




void HookNtProtectVirtualMemory(){
	//��OP  OT�a�}
	nNtProtectVirtualMemoryAddr=GetSSDTFunctionAddr(215);
	//DbgPrint("nNtProtectVirtualMemoryAddr=%x\n",nNtProtectVirtualMemoryAddr);
	nHookNtProtectVirtualMemoryAddr=nNtProtectVirtualMemoryAddr;
	//DbgPrint("nHookNtProtectVirtualMemoryAddr=%x\n",nHookNtProtectVirtualMemoryAddr);
	//==================================�O�s�e2�r�`UNHOOK
	GetProtectByte(nHookNtProtectVirtualMemoryAddr,hookntProtect,5);
	
	//==================================
	NTProtectpush = nNtProtectVirtualMemoryAddr+0x2;
	int *push= reinterpret_cast<int*>(NTProtectpush+1);
	NTProtectpush = *push;
	nHookNtProtectVirtualMemoryJmp=nNtProtectVirtualMemoryAddr+0xc;//�DHS�եθ����I
	nHookNtProtectVirtualMemoryHSJmp=nNtProtectVirtualMemoryAddr+0x7;//HS�եθ����I*/
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
	InLineHookEngine(nHookNtProtectVirtualMemoryAddr,(int)MyNtProtectVirtualMemory);//���Ĥl�o
}

void UnHookNtProtectVirtualMemory(){

	UnInLineHookEngine(nHookNtProtectVirtualMemoryAddr,hookntProtect,5);
}


#endif