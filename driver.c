#include <ntddk.h>

void DriverUnload(PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("HelloWorldDriver: Unload called\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    DriverObject->DriverUnload = DriverUnload;
    
    DbgPrint("HelloWorldDriver: Loaded on ARM64!\n");

    //some useless instructions for the sake of creating larger IR, since we want to run LLVM passes on this routine
    int x = 20;
    int y = 10;
    int z = 35;

    if (x > y)
        z = x;
    else if (z == y)
        z = x;
    else if (z < y)
        x = 0;

    DbgPrint("We did a thing\n");

    return STATUS_SUCCESS;
}
