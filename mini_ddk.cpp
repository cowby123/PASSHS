extern "C"
{
#include "NTDDK.h"
}

#include "gg.h"
#include "hookntop.h"
#include "hookntread.h"
#include "hookntwrite.h"
#include "hookntprotect.h"
#include "hookntClose.h"
#include "hookNtDeviceIoControlFile.h"
#include "hookNtGetContextThread.h"
#include "hookNtSetContextThread.h"


VOID OnUnload(IN PDRIVER_OBJECT DriverObject);

ULONG GetFunctionAddr( IN PCWSTR FunctionName);



void Unload(PDRIVER_OBJECT pDriverObject)
{
	//UnHookNtOpenPorcess();
	UnHookNtReadVirtualMemory();
	UnHookNtWriteVirtualMemory();
	UnHookNtProtectVirtualMemory();
	//UnHookNtClose();
	UnHookNtDeviceIoControlFile();
	UnHookNtGetContextThread();
	UnHookNtSetContextThread();
	DbgPrint("驅動成功被卸載\n");
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,//I\O管理器傳進來的內核對象
								PUNICODE_STRING pRegistryPath)//驅動程序在注冊表中的路徑
{ 
// 	PLIST_ENTRY pLE = (PLIST_ENTRY)pDriverObject->DriverSection;      
// 	
// 	//摘鏈表隱藏驅動      
// 	pLE->Flink->Blink = pLE->Blink;    

	HookNtOpenPorcess();
	HookNtReadVirtualMemory();
	HookNtWriteVirtualMemory();
	HookNtProtectVirtualMemory();
	HookNtClose();
	HookNtDeviceIoControlFile();
	HookNtGetContextThread();
	HookNtSetContextThread();
	DbgPrint("驅動加載成功\n");

	pDriverObject->DriverUnload=Unload;
	
	return 1;
}

