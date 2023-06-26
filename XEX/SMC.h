#pragma once
#include "stdafx.h"

#define POWER_LED_BLINK     0x10 
#define POWER_LED_DEFAULT   0x02 
#define POWER_LED_ON        0x01 
#define POWER_LED_OFF		0X03 

#define REQUEST_TEMP            0x07    //SMC message to request temperature 
#define REQUEST_TILT            0x17    //SMC message to request tilt 
#define REQUEST_SMC_VERSION     0x12    //SMC message to request the smc version 
#define REQUEST_TRAY_STATE      0x0A    //SMC message to request the tray state 
#define REQUEST_AV_PACK         0x0F    //SMC message to request the AV-Pack type 


typedef enum {
	OFF = 0x00,
	RED = 0x08,
	GREEN = 0x80,
	ORANGE = 0x88
} LEDState;

typedef enum {
	CPU,
	GPU,
	MEM,
	BRD
} TEMP_INDEX;

typedef enum {
	VERTICAL,
	HORIZONTAL
} TILT_STATE;

typedef enum {
	OPEN,
	UNKNOWN,
	CLOSED,
	OPENING,
	CLOSING
} TRAY_STATE;

typedef enum {
	AV_HDMI = 0x1F,
	AV_COMPONENT = 0x0F,
	AV_VGA = 0x5B,
	AV_COMPOSITETV = 0x43,
	AV_COMPOSITEHD = 0x4F,
	AV_HDMIAUDIO = 0x13
} AV_PACK;

class SMC {
private:
	static VOID PrepareBuffers();
public:
	static VOID SetPowerLED(BYTE Command, BOOL Animate);
	static VOID SetLEDS(LEDState sTopLeft, LEDState sTopRight, LEDState sBottomLeft, LEDState sBottomRight);
	static VOID SetLED(LEDState Color, BYTE Index);
	static VOID SetFanSpeed(INT Fan, INT Speed);
	static FLOAT GetTemperature(TEMP_INDEX sTempIndex, BOOL Celsius);
	static PCHAR GetSMCVersion();
	static TILT_STATE GetTiltState();
	static BYTE GetAVPack();
	static TRAY_STATE GetTrayState();
	static VOID OpenTray();
	static VOID CloseTray();
	static VOID HalSendSMCMessageHook(LPVOID pCommandBuffer, LPVOID pRecvBuffer);
	static DWORD HardwareInfoSpoof();
};