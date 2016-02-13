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
				//�p�G�OHS�եΪ�
				popad
				popfd
				jmp nHookNtReadVirtualMemoryHSJmp
				}
			}
		__asm{
			popad
			popfd
			call NTReadCall//�I�s�ե�_SEH_prolog�a�}
			jmp nHookNtReadVirtualMemoryJmp//����եت��u�U�@��
		}

			
	}




void HookNtReadVirtualMemory(){
	//��OP  OT�a�}
	nNtReadVirtualMemoryAddr=GetSSDTFunctionAddr(277);
	//DbgPrint("nNtReadVirtualMemoryAddr=%x\n",nNtReadVirtualMemoryAddr);
	nHookNtReadVirtualMemoryAddr=nNtReadVirtualMemoryAddr;
	//DbgPrint("nHookNtReadVirtualMemoryAddr=%x\n",nHookNtReadVirtualMemoryAddr);
	//==================================�O�s�e2�r�`UNHOOK
	GetProtectByte(nHookNtReadVirtualMemoryAddr,hookntread,5);
	
	//==================================
	NTReadPush = nNtReadVirtualMemoryAddr+0x2;
	int *push= reinterpret_cast<int*>(NTReadPush+1);
	NTReadPush = *push;
	nHookNtReadVirtualMemoryJmp=nNtReadVirtualMemoryAddr+0xc;//�DHS�եθ����I
	nHookNtReadVirtualMemoryHSJmp=nNtReadVirtualMemoryAddr+0x7;//HS�եθ����I*/
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
	InLineHookEngine(nHookNtReadVirtualMemoryAddr,(int)MyNtReadVirtualMemory);//���Ĥl�o
}

void UnHookNtReadVirtualMemory(){

	UnInLineHookEngine(nHookNtReadVirtualMemoryAddr,hookntread,5);
}


#endif