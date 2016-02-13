#ifndef HANSHU
#define HANSHU


extern "C" long KeServiceDescriptorTable;

int  GetSSDTFunctionAddr(int nSSDTIndex);
bool PanDuanProcessName(char *szName);
void MemoryWritable();
void MemoryNotWritable();
int SSDTHookEngine(int nSSDTIndex,int nFunctionAddr);
void SSDTUnHookEngine(int nSSDTIndex,int nFunctionAddr);
int GetCallAddr(int nCallAddr);

void CallHook(int nCallAddr,int nFunctionAddr)
{
	int nRCallAddr=(nFunctionAddr-nCallAddr-4);
	MemoryWritable();
	__asm
	{
		mov eax,nCallAddr
			mov ebx,nRCallAddr
			mov dword ptr ds:[eax],ebx
	}
	MemoryNotWritable();
}

int GetCallAddr(int nCallAddr)
{
	return (*((int*)nCallAddr)+nCallAddr+4);
}

int GetJmpAddr(int nCallAddr)
{
	return (*((int*)nCallAddr)+nCallAddr+4);
}

bool PanDuanProcessName(char *szName)
{
	int nEProcess;
	
	nEProcess=(int)PsGetCurrentProcess();
	
	char szProessaName[16];
		
	strcpy(szProessaName,(char*)(nEProcess+0x16c));//0x16c是win7

	//DbgPrint("------%s------\n",szProessaName);
	
	if(strcmp(szProessaName,szName)==0)
	{
		//DbgPrint("楓之谷調用了此函數\n");
		return true;
	}
	
	return false;
}

int SearchFeature(int nAddr,char* pFeature,int nLeng)
{
	char szStatus[256]="";
	int i=5000;
	
	while(i--)
	{
		RtlMoveMemory(szStatus,(char*)nAddr,nLeng);
		
		if (RtlCompareMemory(pFeature,szStatus,nLeng)==nLeng)
		{
			return nAddr+nLeng;
		}
		nAddr++;
	}
	
	return 0;
}

int GetSSDTFunctionAddr(int nSSDTIndex)
{
	int Addr;
	
	__asm
	{
		mov ebx,nSSDTIndex
		shl ebx,2
		mov eax,KeServiceDescriptorTable
		mov eax,[eax]
		add eax,ebx
		mov ecx,[eax]
		mov Addr,ecx
	}
	
	return Addr;
}
int GetSSDTFunctionAddrr(int nSSDTIndex)
{
	int Addr;
	
	__asm
	{
		mov ebx,nSSDTIndex
		shl ebx,2
		mov eax,KeServiceDescriptorTable
		mov eax,[eax]
		mov ecx,[eax]
		mov Addr,ecx
	}
	
	return Addr;
}


int SSDTHookEngine(int nSSDTIndex,int nFunctionAddr)
{
	MemoryWritable();
	
	int nOldAddr;
	
	__asm
	{
		mov ebx,nSSDTIndex
		shl ebx,2
		mov eax,KeServiceDescriptorTable
		mov eax,[eax]
		add eax,ebx
		mov ecx,[eax]
		mov nOldAddr,ecx
		mov ecx,nFunctionAddr
		mov [eax],ecx
	}
	
	MemoryNotWritable();
	
	return nOldAddr;
}

void InLineHookEngine(int nRHookAddr,int nMyFunctionAddr)
{
	MemoryWritable();
	
	int nJmpAddr=nMyFunctionAddr-nRHookAddr-5;
	
	__asm
	{
		mov eax,nRHookAddr
		mov byte ptr ds:[eax],0xe9
		mov ebx,nJmpAddr
		mov dword ptr ds:[eax+1],ebx
	}
	
	MemoryNotWritable();
}
void GetProtectByte(int nRProtectAddr,char*szMacCode,int nLeng){

	for(int i=0;i<nLeng;i++){
		int *op1= reinterpret_cast<int*>(nRProtectAddr+i);
		szMacCode[i]=*op1;
	}

}


void UnInLineHookEngine(int nRHookAddr,char *szMacCode,int nLeng)
{
	MemoryWritable();
	
	RtlMoveMemory((char*)nRHookAddr,szMacCode,nLeng);
	
	MemoryNotWritable();
}

void SSDTUnHookEngine(int nSSDTIndex,int nOldFunctionAddr)
{
	MemoryWritable();
	
	__asm
	{
		mov ebx,nSSDTIndex
		shl ebx,2
		mov eax,KeServiceDescriptorTable
		mov eax,[eax]
		add eax,ebx
		mov ecx,nOldFunctionAddr
		mov [eax],ecx
	}
	
	MemoryNotWritable();
}

void MemoryWritable()
{
	__asm 
	{
		cli
		mov eax,cr0
		and eax,not 10000h 
		mov cr0,eax	
	}
}

void MemoryNotWritable()
{
	__asm 
	{  
		mov     eax, cr0 
		or     eax, 10000h 
		mov     cr0, eax 
		sti 
	} 
}

int GetFunCtionAddr(WCHAR* szFunCtionAName)
{
	UNICODE_STRING FsRtlLegalAnsiCharacterArray_String;
	RtlInitUnicodeString(&FsRtlLegalAnsiCharacterArray_String,szFunCtionAName);
	return (int)MmGetSystemRoutineAddress(&FsRtlLegalAnsiCharacterArray_String);
}

int GetKiAttachProcessAddr()
{
	char s=(char)0xe8;
	int	nCallAddr=SearchFeature(GetFunCtionAddr(L"KeAttachProcess"),&s,1);
	
	if (nCallAddr==0)
	{
		return 0;
	}
	int nKiAttachProcessAddr=*((int*)nCallAddr)+nCallAddr+4;
	
	return nKiAttachProcessAddr;
}

#endif