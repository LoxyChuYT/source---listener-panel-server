#pragma once
#include "stdafx.h"
#include "INIReader.h"
#include "Utilities.h"
#include "Config.h"

BOOL mspspoof = FALSE;
BOOL offhost = FALSE;
BOOL gtabypass = FALSE;
BOOL awbypass = FALSE;
BOOL bo3bypass = FALSE;
BOOL bo2bypass = FALSE;
BOOL ghostbypass = FALSE;
BOOL customui = FALSE;

VOID IniSettings() {
start:
	if (!FileExists(PATH_INI_HDD)) {
		DbgPrint("OutHereLive INI Not Found Or Corrupt Creating New");
		CWriteFile(PATH_INI_HDD, mData.iniBytes, sizeof mData.iniBytes);
	}
	else {
		DbgPrint("OutHereLive INI Found Using Existing");
	}
	INIReader reader(PATH_INI_HDD);
	std::string temp = reader.Get("OutHereLive_Settings", "COD_Offhost", "UNKNOWN");
	if (temp == "TRUE" || temp == "true" || temp == "True") {
		DbgPrint("Cod OffHost - Enabled");
		offhost = TRUE;
	}
	else if (temp == "UNKNOWN") {
		remove(PATH_INI_HDD);
		goto start;
	}
	else {
		DbgPrint("Cod OffHost - Disabled");
	}
	std::string temp1 = reader.Get("OutHereLive_Settings", "BO2_Bypass", "UNKNOWN");
	if (temp1 == "TRUE" || temp1 == "true" || temp1 == "True") {
		DbgPrint("Bo2 Bypass - Enabled");
		bo2bypass = TRUE;
	}
	else if (temp1 == "UNKNOWN") {
		remove(PATH_INI_HDD);
		goto start;
	}
	else {
		DbgPrint("Bo2 Bypass - Disabled");
	}
	std::string temp2 = reader.Get("OutHereLive_Settings", "AW_Bypass", "UNKNOWN");
	if (temp2 == "TRUE" || temp2 == "true" || temp2 == "True") {
		DbgPrint("AW Bypass - Enabled");
		awbypass = TRUE;
	}
	else if (temp2 == "UNKNOWN") {
		remove(PATH_INI_HDD);
		goto start;
	}
	else {
		DbgPrint("AW Bypass - Disabled");
	}
	std::string temp3 = reader.Get("OutHereLive_Settings", "BO3_Bypass", "UNKNOWN");
	if (temp3 == "TRUE" || temp3 == "true" || temp3 == "True") {
		DbgPrint("Bo3 Bypass - Enabled");
		bo3bypass = TRUE;
	}
	else if (temp3 == "UNKNOWN") {
		remove(PATH_INI_HDD);
		goto start;
	}
	else {
		DbgPrint("Bo3 Bypass - Disabled");
	}
	std::string temp4 = reader.Get("OutHereLive_Settings", "GHOST_Bypass", "UNKNOWN");
	if (temp4 == "TRUE" || temp4 == "true" || temp4 == "True") {
		DbgPrint("Ghost Bypass - Enabled");
		ghostbypass = TRUE;
	}
	else if (temp4 == "UNKNOWN") {
		remove(PATH_INI_HDD);
		goto start;
	}
	else {
		DbgPrint("Ghost Bypass - Disabled");
	}
	std::string temp6 = reader.Get("OutHereLive_Settings", "GTAV_Bypass", "UNKNOWN");
	if (temp6 == "TRUE" || temp6 == "true" || temp6 == "True") {
		DbgPrint("GtaV Bypass - Enabled");
		gtabypass = TRUE;
	}
	else if (temp6 == "UNKNOWN") {
		remove(PATH_INI_HDD);
		goto start;
	}
	else {
		DbgPrint("GtaV Bypass - Disabled");
	}
	std::string temp7 = reader.Get("OutHereLive_Settings", "MSP_Spoof", "UNKNOWN");
	if (temp7 == "TRUE" || temp7 == "true" || temp7 == "True") {
		DbgPrint("Msp Spoof - Enabled");
		mspspoof = TRUE;
	}
	else if (temp7 == "UNKNOWN") {
		remove(PATH_INI_HDD);
		goto start;
	}
	else {
		DbgPrint("Msp Spoof - Disabled");
	}
	std::string temp8 = reader.Get("OutHereLive_Settings", "CUSTOM_UI", "UNKNOWN");
	if (temp8 == "TRUE" || temp8 == "true" || temp8 == "True") {
		DbgPrint("Custom UI - Enabled");
		customui = TRUE;
	}
	else if (temp8 == "UNKNOWN") {
		remove(PATH_INI_HDD);
		goto start;
	}
	else {
		DbgPrint("Custom UI - Disabled");
	}
}
