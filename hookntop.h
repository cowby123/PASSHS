#ifndef HOOKNTOpenPorcess
#define HOOKNTOpenPorcess
int nNtOpenPorcessAddr;
int nHookNtOpenPorcessAddr;
int nHookNtOpenPorcessJmp;
int nHookNtOpenPorcessHSJmp;
int PsOpenProcessAdr;
char hookntop[7];
int adr = 0;
int hsadr = 0;
int opcalladr = 0;
__declspec(naked)void MyNTOpenPorcess(){
	_asm{
		pushfd
		pushad
	}


	if(PanDuanProcessName("KartRider.exe")||PanDuanProcessName("MapleStory.exe")||PanDuanProcessName("CA.exe")){
		_asm{
			popad
			popfd
			call hsadr
			jmp nHookNtOpenPorcessJmp
		}
	}
	else{
		_asm{
			popad
			popfd
			call opcalladr
			jmp nHookNtOpenPorcessJmp
		}
	}


	
	/*_asm{
		push    dword ptr [ebp-4]
		push    dword ptr [ebp-4]
        push    dword ptr [ebp+0Ch]
        push    dword ptr [ebp+8]
		pushfd
		pushad
		}
		if(PanDuanProcessName("KartRider.exe")||PanDuanProcessName("MapleStory.exe")||PanDuanProcessName("CA.exe")){
			__asm
			{
				//�p�G�OHS�եΪ�
				popad
				popfd
				jmp nHookNtOpenPorcessHSJmp
				}
			}
		__asm{
			popad
			popfd
			call PsOpenProcessAdr//�I�s�ե�_SEH_prolog�a�}
			jmp nHookNtOpenPorcessJmp//����եت��u�U�@��
		}
		*/
			
	}




void HookNtOpenPorcess(){
	//��OP  OT�a�}
	/*nNtOpenPorcessAddr=GetSSDTFunctionAddr(190);
	//DbgPrint("nNtOpenPorcessAddr=%x\n",nNtOpenPorcessAddr);
	nHookNtOpenPorcessAddr=nNtOpenPorcessAddr+0x1c;
	//DbgPrint("nHookNtOpenPorcessAddr=%x\n",nHookNtOpenPorcessAddr);
	//==================================�O�s�e2�r�`UNHOOK
	GetProtectByte(nHookNtOpenPorcessAddr,hookntop,5);
	
	//==================================
	nHookNtOpenPorcessJmp=nNtOpenPorcessAddr+0x2d;//�DHS�եθ����I
	nHookNtOpenPorcessHSJmp=nNtOpenPorcessAddr+0x28;//HS�եθ����I
	PsOpenProcessAdr = GetCallAddr(nHookNtOpenPorcessHSJmp+1);
	char *op1= reinterpret_cast<char*>(PsOpenProcessAdr);
	int bytecode=*op1;
	if (bytecode == 0xffffffe9){
		PsOpenProcessAdr = PsOpenProcessAdr+0x8;
		//DbgPrint("PsOpenProcessAdr=%x\n",PsOpenProcessAdr);
		PsOpenProcessAdr = GetJmpAddr(PsOpenProcessAdr+1);
		//DbgPrint("PsOpenProcessAdr=%x\n",PsOpenProcessAdr);
		//DbgPrint("hs is runnint\n");
	}
	//DbgPrint("nHookNtOpenPorcessJmp=%x\n",nHookNtOpenPorcessJmp);
	//DbgPrint("nHookNtOpenPorcessHSJmp=%x\n",nHookNtOpenPorcessHSJmp);
	//DbgPrint("PsOpenProcessAdr=%x\n",PsOpenProcessAdr);
	//DbgPrint("bytecode=%x\n",bytecode);
*/

	nNtOpenPorcessAddr=GetSSDTFunctionAddr(190);
	//DbgPrint("nNtOpenPorcessAddr=%x\n",nNtOpenPorcessAddr);
	nHookNtOpenPorcessAddr=nNtOpenPorcessAddr+0x28;
	//DbgPrint("nHookNtOpenPorcessAddr=%x\n",nHookNtOpenPorcessAddr);
	adr = GetCallAddr(nHookNtOpenPorcessAddr+1);
	hsadr = adr;
	opcalladr = adr+0x8;
	//DbgPrint("adr=%x\n",adr);
	nHookNtOpenPorcessJmp=nNtOpenPorcessAddr+0x2d;//�DHS�եθ����I
	InLineHookEngine(nHookNtOpenPorcessAddr,(int)MyNTOpenPorcess);//���Ĥl�o
}

void UnHookNtOpenPorcess(){

	//UnInLineHookEngine(nHookNtOpenPorcessAddr,hookntop,5);
}


#endif