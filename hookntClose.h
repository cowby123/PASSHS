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
				//�p�G�OHS�եΪ�
				popad
				popfd
				jmp jmpclosehs
				}
			}
		__asm{
			popad
			popfd
			jmp jmpclose//����եت��u�U�@��
		}
		
			
	}




void HookNtClose(){
	//��OP  OT�a�}
	nNtCloseAddr=GetSSDTFunctionAddr(50);
	//DbgPrint("nNtCloseAddr=%x\n",nNtCloseAddr);
	nHookNtCloseAddr=nNtCloseAddr;
	//DbgPrint("nHookNtCloseAddr=%x\n",nHookNtCloseAddr);
	//==================================�O�s�e2�r�`UNHOOK
	GetProtectByte(nHookNtCloseAddr,hookntClose,5);
	jmpclosehs = GetJmpAddr(nNtCloseAddr+1);
	jmpclose = jmpclosehs+8;
	InLineHookEngine(nHookNtCloseAddr,(int)MyNtClose);//���Ĥl�o
}

void UnHookNtClose(){

	//UnInLineHookEngine(nHookNtCloseAddr,hookntClose,5);
}


#endif