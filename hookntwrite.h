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
				//�p�G�OHS�եΪ�
				popad
				popfd
				jmp nHookNtWriteVirtualMemoryHSJmp
				}
			}
		__asm{
			popad
			popfd
			call NTWriteCall//�I�s�ե�_SEH_prolog�a�}
			jmp nHookNtWriteVirtualMemoryJmp//����եت��u�U�@��
		}

			
	}




void HookNtWriteVirtualMemory(){
	//��OP  OT�a�}
	nNtWriteVirtualMemoryAddr=GetSSDTFunctionAddr(399);
	//DbgPrint("nNtWriteVirtualMemoryAddr=%x\n",nNtWriteVirtualMemoryAddr);
	nHookNtWriteVirtualMemoryAddr=nNtWriteVirtualMemoryAddr;
	//DbgPrint("nHookNtWriteVirtualMemoryAddr=%x\n",nHookNtWriteVirtualMemoryAddr);
	//==================================�O�s�e2�r�`UNHOOK
	GetProtectByte(nHookNtWriteVirtualMemoryAddr,hookntwrite,5);
	
	//==================================
	NTWritePush = nNtWriteVirtualMemoryAddr+0x2;
	int *push= reinterpret_cast<int*>(NTWritePush+1);
	NTWritePush = *push;
	nHookNtWriteVirtualMemoryJmp=nNtWriteVirtualMemoryAddr+0xc;//�DHS�եθ����I
	nHookNtWriteVirtualMemoryHSJmp=nNtWriteVirtualMemoryAddr+0x7;//HS�եθ����I*/
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
	InLineHookEngine(nHookNtWriteVirtualMemoryAddr,(int)MyNtWriteVirtualMemory);//���Ĥl�o
}

void UnHookNtWriteVirtualMemory(){

	UnInLineHookEngine(nHookNtWriteVirtualMemoryAddr,hookntwrite,5);
}


#endif