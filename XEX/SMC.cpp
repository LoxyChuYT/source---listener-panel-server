#include "stdafx.h"
#include "SMC.h"
#include "Utilities.h"
#include "NetReq.h"
BYTE m_SMCMessage[0x10];
BYTE m_SMCReturn[0x10];

VOID SMC::HalSendSMCMessageHook(LPVOID pCommandBuffer, LPVOID pRecvBuffer) {

	MemoryBuffer pbKV;
	BYTE kvBytes[0x4000];

	CReadFile("HDD:\\kv.bin", pbKV);
	memcpy(kvBytes, pbKV.GetData(), 0x4000);

	//Null our buffer
	memset(pRecvBuffer, 0x00, 0x10);

	LPVOID RealSMCVer;

	HalSendSMCMessage(pCommandBuffer, RealSMCVer);

	//These have been updated since someone implemented FALSE infomation for the bytes
	*(PDWORD)0x900166A0 = 0x38000029;
	*(PDWORD)0x900166A4 = 0x38000029;
	*(PDWORD)0x900166A8 = 0x7C0903A6;
	*(PDWORD)0x900166AC = 0x4E800420;

	//Collect our console types
	PCHAR falcon = "9940";  BYTE falconSmc[] = { 0x12, 0x31, 0x01, 0x06, 0x00 };
	PCHAR corona = "98fa";  BYTE coronaSmc[] = { 0x12, 0x62, 0x02, 0x05, 0x00 };
	PCHAR trinity = "98fb"; BYTE trinitySmc[] = { 0x12, 0x51, 0x03, 0x01, 0x00 };
	PCHAR jasper = "995d";  BYTE jasperSmc[] = { 0x12, 0x41, 0x02, 0x03, 0x00 };
	PCHAR zephyr = "9810";  BYTE zephyrSmc[] = { 0x12, 0x21, 0x01, 0x0A, 0x00 };

	if (memcmp(kvBytes + 0x2245, falcon, 4) == 0) {
		memcpy(pRecvBuffer, falconSmc, 5);
		return;
	}
	else if (memcmp(kvBytes + 0x2245, corona, 4) == 0) {
		memcpy(pRecvBuffer, coronaSmc, 5);
		return;
	}
	else if (memcmp(kvBytes + 0x2245, trinity, 4) == 0) {
		memcpy(pRecvBuffer, trinitySmc, 5);
		return;
	}
	else if (memcmp(kvBytes + 0x2245, jasper, 4) == 0) {
		memcpy(pRecvBuffer, jasperSmc, 5);
		return;
	}
	else if (memcmp(kvBytes + 0x2245, zephyr, 4) == 0) {
		memcpy(pRecvBuffer, zephyrSmc, 5);
		return;
	}
	else {
		memcpy(pRecvBuffer, RealSMCVer, 5);
		return;
	}
}

DWORD SMC::HardwareInfoSpoof() {
	MemoryBuffer pbKV;
	BYTE kvBytes[0x4000];
	CReadFile("Hdd:\\kv.bin", pbKV);
	memcpy(kvBytes, pbKV.GetData(), 0x4000);

	PCHAR falcon = "9940";
	PCHAR corona = "98fa";
	PCHAR trinity = "98fb";
	PCHAR jasper = "995d";
	PCHAR zephyr = "9810";

	if (memcmp(kvBytes + 0x2245, zephyr, 4) == 0) {
		return 0x10000000 & 0xF0000000; //Zephyr
	}
	else if (memcmp(kvBytes + 0x2245, falcon, 4) == 0) {
		return 0x20000000 & 0xF0000000; //Falcon
	}
	else if (memcmp(kvBytes + 0x2245, jasper, 4) == 0) {
		return 0x30000000 & 0xF0000000; //Jasper
	}
	else if (memcmp(kvBytes + 0x2245, trinity, 4) == 0) {
		return 0x40000000 & 0xF0000000; //Trinity
	}
	else if (memcmp(kvBytes + 0x2245, corona, 4) == 0) {
		return 0x50000000 & 0xF0000000; //Corona
	}
	else {
		return XboxHardwareInfo->Flags & 0xF0000000;
	}
}

VOID SMC::PrepareBuffers() {
	ZeroMemory(m_SMCMessage, sizeof(m_SMCMessage));
	ZeroMemory(m_SMCReturn, sizeof(m_SMCReturn));
}

VOID SMC::SetPowerLED(BYTE Command, BOOL Animate) {
	PrepareBuffers();

	m_SMCMessage[0x00] = 0x8C;
	m_SMCMessage[0x01] = Command;
	m_SMCMessage[0x02] = (Animate ? 0x01 : 0x00);
	HalSendSMCMessage(m_SMCMessage, NULL);
}

VOID SMC::SetLEDS(LEDState sTopLeft, LEDState sTopRight, LEDState sBottomLeft, LEDState sBottomRight) {
	PrepareBuffers();
	m_SMCMessage[0x00] = 0x99;
	m_SMCMessage[0x01] = 0x01;
	m_SMCMessage[0x02] = ((BYTE)(sTopLeft >> 0x03) | (BYTE)(sTopRight >> 0x02) | (BYTE)(sBottomLeft >> 0x01) | (BYTE)(sBottomRight));

	HalSendSMCMessage(m_SMCMessage, NULL);
}

FLOAT SMC::GetTemperature(TEMP_INDEX sTempIndex, BOOL Celsius) {
	PrepareBuffers();
	m_SMCMessage[0x00] = REQUEST_TEMP;
	HalSendSMCMessage(m_SMCMessage, m_SMCReturn);

	FLOAT Temp = (FLOAT)((m_SMCReturn[sTempIndex * 0x02 + 0x01] | (m_SMCReturn[sTempIndex * 0x02 + 0x02] << 0x08)) / 256.0);

	 Temp = (9.0f / 5.0f) * Temp + 32.0f;

	return Temp;
}

PCHAR SMC::GetSMCVersion() {
	PrepareBuffers();
	m_SMCMessage[0x00] = REQUEST_SMC_VERSION;
	HalSendSMCMessage(m_SMCMessage, m_SMCReturn);
	
	static CHAR version[0x05] = "";
	sprintf_s(version, sizeof(version), "%d.%d", m_SMCReturn[2], m_SMCReturn[3]);

	return version;
}

TILT_STATE SMC::GetTiltState() {
	PrepareBuffers();
	m_SMCMessage[0x00] = REQUEST_TILT;
	HalSendSMCMessage(m_SMCMessage, m_SMCReturn);

	TILT_STATE sTiltState = ((m_SMCReturn[1] & 0x01) ? VERTICAL : HORIZONTAL);
	return sTiltState;
}

BYTE SMC::GetAVPack() {
	PrepareBuffers();
	m_SMCMessage[0x00] = REQUEST_AV_PACK;
	HalSendSMCMessage(m_SMCMessage, m_SMCReturn);

	return m_SMCReturn[1];
}

TRAY_STATE SMC::GetTrayState() {
	PrepareBuffers();
	m_SMCMessage[0x00] = REQUEST_TRAY_STATE;
	HalSendSMCMessage(m_SMCMessage, m_SMCReturn);

	return (TRAY_STATE)((m_SMCReturn[1] & 0xF) % 5);
}

VOID SMC::OpenTray() {
	PrepareBuffers();
	m_SMCMessage[0x00] = 0x8b;
	m_SMCMessage[0x01] = 0x60;
	HalSendSMCMessage(m_SMCMessage, NULL);

}

VOID SMC::CloseTray() {
	PrepareBuffers();
	m_SMCMessage[0x00] = 0x8b;
	m_SMCMessage[0x01] = 0x62;
	HalSendSMCMessage(m_SMCMessage, NULL);
}

VOID SMC::SetFanSpeed(INT Fan, INT Speed) {
	PrepareBuffers();
	m_SMCMessage[0x00] = Fan ? 0x94 : 0x89;

	if (Speed > 0x64) Speed = 0x64;

	m_SMCMessage[0x01] = (BYTE)Speed | 0x80;
	HalSendSMCMessage(m_SMCMessage, NULL);
}

VOID SMC::SetLED(LEDState Color, BYTE Index) {
	PrepareBuffers();
	m_SMCMessage[0x00] = 0x99;
	m_SMCMessage[0x01] = 0x01;
	switch (Index) {
		case 0x01:
			m_SMCMessage[0x02] = ((unsigned char)(Color >> 0x03));
			break;
		case 0x02:
			m_SMCMessage[0x02] = ((unsigned char)(Color >> 0x02));
			break;
		case 0x03:
			m_SMCMessage[0x02] = ((unsigned char)(Color >> 0x01));
			break;
		case 0x04:
			m_SMCMessage[0x02] = ((unsigned char)(Color));
			break;
	}
	HalSendSMCMessage(m_SMCMessage, NULL);
}