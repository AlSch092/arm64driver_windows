# arm64driver_windows
Simple ARM64 KM Driver for Windows with CMake

## Basic compilation steps:  
1. `mkdir build`  
2. `cd build`  
3. `cmake -G "Visual Studio 17 2022" -A ARM64 ..`  

...then after our project files are generated:  

4. `cmake --build . --config Release`   

## Optional LLVM passes:    
1. Generate .ll from driver.c (you will of course need to update paths to your correct WDK folders):  
`clang -c -S -emit-llvm -target arm64-pc-windows-msvc -D_ARM64_ -D"ARM64_PREVENT_REGISTER_WRITEBACK"=1  -DNT_KERNEL_MODE -D__ARM64__ -D__aarch64__ -DCLANG_DISABLE_MACRO_WARNINGS   -I"C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/km" -I"C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/shared" -I"C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/km/crt" -I"C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/um/"   -I"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\km\arm64" driver.c -o driver.ll`  
2. If you get errors related to macro expanding in files like wdm.h, you need to patch over this manually in wdm.h:

`//#define ARM64_PREVENT_REGISTER_WRITEBACK(_type, _variable) \
//    _variable = (volatile _type *)ReadPointerNoFence((PVOID const volatile *)&##_variable);

#define ARM64_PREVENT_REGISTER_WRITEBACK(_type, _variable) \
    _variable = (volatile _type *)ReadPointerNoFence((PVOID const volatile *)&(_variable));`

3. Once you have your .ll, run an LLVM pass on it with: `opt -load-pass-plugin="MyPass.dll" -passes='opaque' driver.ll -S -o driver_out.ll` (you may need to take away the 'optnone' attribute in the .ll file for any functions holding attribute tags (like #0))  

Since my DriverEntry has a "#0" attribute tag when generating my driver.ll, I made another attribute list that has the same attributes as #0, but without the "optnone" one:  
`attributes #3 = { noinline nounwind uwtable "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fp-armv8,+neon,+v8a,-fmv" }`  

then our DriverEntry in the .ll becomes:  
`define dso_local i32 @DriverEntry(ptr noundef %0, ptr noundef %1) #3 {`  

..and our LLVM pass can now be applied to the routines in driver.ll.
  
