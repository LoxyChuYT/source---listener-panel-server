#include "stdafx.h"
#include "liveblock.h"
#include "utilities.h"

 typedef BOOL(*DLAUNCHSETOPTVALBYNAME)(char* optName, PDWORD val);

 VOID Setliveblock(BOOL Value) {
     DLAUNCHSETOPTVALBYNAME dlaunchSetOptValByName = (DLAUNCHSETOPTVALBYNAME)ResolveFunction(MODULE_LAUNCH, 10);
	 DbgPrint("Live Block Set %s", Value ? "True" : "False");
     if (dlaunchSetOptValByName != NULL) {
         if (dlaunchSetOptValByName("liveblock", (PDWORD)&Value) == FALSE) {
             XNotifyUI(L"warning: could not set liveblock!");
         }
     }
	 else {
		 DbgPrint("warning: could not resolve dashlaunch export!");
	 }
 }

 VOID Setlivestrong(BOOL Value) {
	 DLAUNCHSETOPTVALBYNAME dlaunchSetOptValByName = (DLAUNCHSETOPTVALBYNAME)ResolveFunction(MODULE_LAUNCH, 10);
	 DbgPrint("Live Strong Set %s", Value ? "True" : "False");
	 if (dlaunchSetOptValByName != NULL) {
		 if (dlaunchSetOptValByName("livestrong", (PDWORD)&Value) == FALSE) {
			 XNotifyUI(L"warning: could not set liveblock!");
		 }
	 }
	 else {
		 DbgPrint("warning: could not resolve dashlaunch export!");
	 }
 }