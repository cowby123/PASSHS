#ifndef HOOKNTDeviceIoControlFile
#define HOOKNTDeviceIoControlFile
int nNtDeviceIoControlFileAddr;
int nHookNtDeviceIoControlFileAddr;
int nHookNtDeviceIoControlFileJmp;
int DCFjmp = 0;
int DCFjmpHS = 0;
int DCFAdr = 0;
__declspec(naked)void MyNTDeviceIoControlFile(){
	_asm{
		pushfd
		pushad
	}


	if(PanDuanProcessName("KartRider.exe")||PanDuanProcessName("MapleStory.exe")||PanDuanProcessName("CA.exe")){
		_asm{
			popad
			popfd
			call DCFjmpHS
			jmp nHookNtDeviceIoControlFileJmp 
		}
	}
	else{
		_asm{
			popad
			popfd
			call DCFjmp
			jmp nHookNtDeviceIoControlFileJmp
		}
	}


	
			
	}




void HookNtDeviceIoControlFile(){
	

	nNtDeviceIoControlFileAddr=GetSSDTFunctionAddr(107);
	//DbgPrint("nNtDeviceIoControlFileAddr=%x\n",nNtDeviceIoControlFileAddr);
	nHookNtDeviceIoControlFileAddr=nNtDeviceIoControlFileAddr+0x25;
	//DbgPrint("nHookNtDeviceIoControlFileAddr=%x\n",nHookNtDeviceIoControlFileAddr);
	DCFAdr = GetCallAddr(nHookNtDeviceIoControlFileAddr+1);
	DCFjmpHS = DCFAdr;
	DCFjmp = DCFAdr+0x8;
	//DbgPrint("adr=%x\n",adr);
	//DbgPrint("DCFjmpHS=%x\n",DCFjmpHS);
	//DbgPrint("DCFjmp=%x\n",DCFjmp);
	nHookNtDeviceIoControlFileJmp=nNtDeviceIoControlFileAddr+0x2a;//非HS調用跳轉點
	//DbgPrint("nHookNtDeviceIoControlFileJmp=%x\n",nHookNtDeviceIoControlFileJmp);
	InLineHookEngine(nHookNtDeviceIoControlFileAddr,(int)MyNTDeviceIoControlFile);//掛勾子囉
}

void UnHookNtDeviceIoControlFile(){

	//UnInLineHookEngine(nHookNtDeviceIoControlFileAddr,hookntop,5);
}


#endif