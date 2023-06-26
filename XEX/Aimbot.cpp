#include "stdafx.h"
#include "Utilities.h"
#include "BO3 Defines.h"
#include "Aimbot.h"


BYTE Aim[] = { 0x46, 0x61, 0x74, 0x68, 0x65, 0x72, 0x4c, 0x65, 0x77, 0x69, 0x73, 0x20, 0x63, 0x61, 0x6e, 0x20, 0x73, 0x75, 0x63, 0x6b, 0x20, 0x69, 0x74, 0x21 };

#define M_PI       3.14159265358979323846
#define GET_ORIGIN(VecOut,pMatrix) {VecOut.x = pMatrix._41;VecOut.y = pMatrix._42;VecOut.z = pMatrix._43;};
#define GET_X(VecOut,pMatrix) {D3DXVec3Normalize(&VecOut,&D3DXVECTOR3(pMatrix._11,pMatrix._12,pMatrix._13));};
#define GET_Y(VecOut,pMatrix) {D3DXVec3Normalize(&VecOut,&D3DXVECTOR3(pMatrix._21,pMatrix._22,pMatrix._23));};
#define GET_Z(VecOut,pMatrix) {D3DXVec3Normalize(&VecOut,&D3DXVECTOR3(pMatrix._31,pMatrix._32,pMatrix._33));};

#define DotProduct(x,y)			((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define VectorSubtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define VectorCopy4(a,b)		((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])
#define	VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define VectorCmp(a,b)			(((a)[0]==(b)[0])&&((a)[1]==(b)[1])&&((a)[2]==(b)[2]))

#define VALID( x ) ( x != NULL && HIWORD( x ) )

typedef FLOAT vec_t;

class Vector
{
public:
	// Construction/destruction
	inline Vector(VOID) { }
	inline Vector(FLOAT X, FLOAT Y, FLOAT Z) { x = X; y = Y; z = Z; }
	inline Vector(double X, double Y, double Z) { x = (FLOAT)X; y = (FLOAT)Y; z = (FLOAT)Z; }
	inline Vector(int X, int Y, int Z) { x = (FLOAT)X; y = (FLOAT)Y; z = (FLOAT)Z; }
	inline Vector(const Vector& v) { x = v.x; y = v.y; z = v.z; }
	inline Vector(FLOAT rgfl[3]) { x = rgfl[0]; y = rgfl[1]; z = rgfl[2]; }

	// Operators
	inline Vector operator-(VOID) const { return Vector(-x, -y, -z); }
	inline int operator==(const Vector& v) const { return x == v.x && y == v.y && z == v.z; }
	inline int operator!=(const Vector& v) const { return !(*this == v); }
	inline Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
	inline Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
	inline Vector operator*(FLOAT fl) const { return Vector(x*fl, y*fl, z*fl); }
	inline Vector operator/(FLOAT fl) const { return Vector(x / fl, y / fl, z / fl); }

	// Methods
	inline VOID CopyToArray(FLOAT* rgfl) const { rgfl[0] = x, rgfl[1] = y, rgfl[2] = z; }
	inline FLOAT Length(VOID) const { return (FLOAT)sqrt(x*x + y*y + z*z); }
	operator FLOAT *() { return &x; } // Vectors will now automatically convert to FLOAT * when needed
	operator const FLOAT *() const { return &x; } // Vectors will now automatically convert to FLOAT * when needed
	inline Vector Normalize(VOID) const
	{
		FLOAT flLen = Length();
		if (flLen == 0) return Vector(0, 0, 1); // ????
		flLen = 1 / flLen;
		return Vector(x * flLen, y * flLen, z * flLen);
	}

	inline FLOAT Length2D(VOID) const { return (FLOAT)sqrt(x*x + y*y); }

	// Members
	vec_t x, y, z;
};
inline Vector operator*(FLOAT fl, const Vector& v) { return v * fl; }

class vec2
{
public:
	union {
		struct { FLOAT x, y; };
		struct { FLOAT s, t; };
		struct { FLOAT r, g; };
	};
	vec2() : x(0.0f), y(0.0f) {}
	~vec2() {}
	vec2(FLOAT num) : x(num), y(num) {}
	vec2(FLOAT x, FLOAT y) : x(x), y(y) {}
	vec2(const vec2 &u) : x(u.x), y(u.y) {}
	vec2& operator = (const vec2 &u) { x = u.x; y = u.y; return *this; }
	vec2 operator - () { return vec2(-x, -y); }
	FLOAT* operator & () { return (FLOAT*)this; };
	vec2& operator += (FLOAT num) { x += num; y += num; return *this; }
	vec2& operator += (const vec2 &u) { x += u.x; y += u.y; return *this; }
	vec2& operator -= (FLOAT num) { x -= num; y -= num; return *this; }
	vec2& operator -= (const vec2 &u) { x -= u.x; y -= u.y; return *this; }
	vec2& operator *= (FLOAT num) { x *= num; y *= num; return *this; }
	vec2& operator *= (const vec2 &u) { x *= u.x; y *= u.y; return *this; }
	vec2& operator /= (FLOAT num) { x /= num; y /= num; return *this; }
	vec2& operator /= (const vec2 &u) { x /= u.x; y /= u.y; return *this; }
	friend vec2 operator + (const vec2 &u, FLOAT num) { return vec2(u.x + num, u.y + num); }
	friend vec2 operator + (FLOAT num, const vec2 &u) { return vec2(num + u.x, num + u.y); }
	friend vec2 operator + (const vec2 &u, const vec2 &v) { return vec2(u.x + v.x, u.y + v.y); }
	friend vec2 operator - (const vec2 &u, FLOAT num) { return vec2(u.x - num, u.y - num); }
	friend vec2 operator - (FLOAT num, const vec2 &u) { return vec2(num - u.x, num - u.y); }
	friend vec2 operator - (const vec2 &u, const vec2 &v) { return vec2(u.x - v.x, u.y - v.y); }
	friend vec2 operator * (const vec2 &u, FLOAT num) { return vec2(u.x * num, u.y * num); }
	friend vec2 operator * (FLOAT num, const vec2 &u) { return vec2(num * u.x, num * u.y); }
	friend vec2 operator * (const vec2 &u, const vec2 &v) { return vec2(u.x * v.x, u.y * v.y); }
	friend vec2 operator / (const vec2 &u, FLOAT num) { return vec2(u.x / num, u.y / num); }
	friend vec2 operator / (FLOAT num, const vec2 &u) { return vec2(num / u.x, num / u.y); }
	friend vec2 operator / (const vec2 &u, const vec2 &v) { return vec2(u.x / v.x, u.y / v.y); }
};

#define ANGLE2SHORT(x) ((int)((x)*65536/360) & 65535)

short angle2short(FLOAT x) {
	return ((int)((x) * 65536 / 360) & 65535);
}

int short2angle(short x) {
	return ((x)*(360.0 / 65536));
}

///////////////////////////AW Structs//////////////////////

D3DXVECTOR3 SilentAngles;
BOOL GotVisible = FALSE;

class cRefdef
{
public:
	char _0x0000[4];
	WORD iWidth; //0x0004 
	WORD iHeight; //0x0006 
	char _0x0008[8];
	FLOAT fov_X; //0x0010 
	FLOAT fov_Y; //0x0014 
	D3DXVECTOR3 EyePos; //0x0018 
	D3DXVECTOR3 ViewAxis[3]; //0x0024 
};//Size=0x0440

class cEntity
{
public:
	char _0x0000[2]; //0x0000 
	short Valid; //0x0002 
	char _0x0004[16];
	D3DXVECTOR3 mPos; //0x0014 
	char _0x0020[76];
	BYTE unknown; //0x006C 
	BYTE isZomming; //0x006D 
	BYTE unknown1; //0x006E 
	BYTE mStand; //0x006F 
	char _0x0070[12];
	Vector mPos2; //0x007C 
	char _0x0088[24];
	D3DXVECTOR3 ViewAngle; //0x00A0 
	char _0x00AC[56];
	__int32 ClientNum; //0x00E4 
	char _0x00E8[60];
	BYTE Unknown; //0x0124 
	char _0x0125[2];
	BYTE WeaponID; //0x0127 
	char _0x0129[24];
	D3DXVECTOR3 mPos3; //0x0140 
	char _0x014C[24];
	D3DXVECTOR3 ViewAngle2; //0x0164 
	char _0x1CF[107];//0x170
	BYTE State;//0x1DB
	char _0x0170[156];
};//Size=0x020C

class CGS//chui d'accord, mais ce probleme la on devra le fixé tot ou tard, ya des gens tv 32cm devil ? jcrois pour l'instant on sais meme pas si sa marche donc autant test un truck sur ! ou on verra le text
{
public:
	char _0x0000[8];
	__int32 screenX; //0x0008 
	__int32 screenY; //0x000C 
	char _0x0010[20];
	char gametype[4]; //0x0024 
	char _0x0028[24];
};

class clientInfo_t
{
public:
	__int32 mNum; //0x0000 
	__int32 Valid; //0x0004 
	char _0x0008[4];
	__int32 mTeam; //0x000C 
	char _0x0010[48];
};

class PlayerNames
{
public:
	char _0x0000[4];
	__int16 mNum; //0x0004 
	char _0x0006[6];
	BYTE Unk; //0x000C 
	char _Unkz[2]; //0x000D 
	BYTE mTeam; //0x000F 
	char _0x0010[72];
	char mName[0x10]; //0x0058 
	char _0x0068[196];
};//Size=0x012C

class CG_t
{
public:
	BYTE cNum; //0x0000 
	char _0x0001[75];
	__int32 ServerTime; //0x004C 
	char _0x0050[356];
	D3DXVECTOR3 wAngles; //0x01B4 
	char _0x01C0[1664];

};

class UserCmd_t
{
public:
public:
	int ServerTime; //0x0000 
	int Button; //0x0004 
	int ViewAngle[3]; //0x0008 
	int WeaponID; //0x0014 
	int moveTonpere;//0x0018
	int MoveTaMere; //0x001C 
	char _0x0020[36];
};

class clientActive
{
public:

	char _0x0000[272];
	D3DXVECTOR3 RefAngles; //0x0110 
	char _0x011C[15436];
	D3DXVECTOR3 ViewAngles; //0x3D68 
	UserCmd_t cmds[128]; //0x03D74
	int CmdNum; //0x5F74 

	UserCmd_t* GetCmd(int index)
	{
		return &cmds[(index & 0x7F)];
	}
};
/////////////////////////////////////////////////

///////////////////GO Structs//////////////////////////
class GO_cRefdef
{
public:
	char _0x0000[8];
	__int32 iWidth; //0x0008 
	__int32 iHeight; //0x000C 
	FLOAT fov_X; //0x0010 
	FLOAT fov_Y; //0x0014 
	D3DXVECTOR3 EyePos; //0x0018 
	D3DXVECTOR3 ViewAxis[3]; //0x0024 
	char _0x0030[19276];
	D3DXVECTOR3 vAngleidk; //0x4B7C 
	D3DXVECTOR3 refView; //0x4B88 
	D3DXVECTOR3 vOrig; //0x4B94 
	D3DXVECTOR3 vOrig2; //0x4BA0 
	D3DXVECTOR3 refView1; //0x4BAC 
	char _0x4BB8[12];
	D3DXVECTOR3 refView2; //0x4BC4 
	char _0x4BD0[11376];
};

class GO_cEntity
{
public:
	char _0x0000[2];
	short Valid; //0x0002 
	char _0x0004[16];
	D3DXVECTOR3 mPos; //0x0014 
	D3DXVECTOR3 ViewAngles; //0x0020 
	char _0x002C[49];
	BYTE Zooming; //0x005D 
	BYTE iMenu; //0x005E 
	BYTE Pose; //0x005F 
	char _0x0060[12];
	D3DXVECTOR3 mPos2; //0x006C 
	char _0x0078[24];
	D3DXVECTOR3 ViewAngles1; //0x0090 
	char _0x009C[64];
	D3DXVECTOR3 mPos3; //0x00DC 
	char _0x00E8[24];
	D3DXVECTOR3 ViewAngles2; //0x0100 
	char _0x010C[127];
	BYTE WeaponID; //0x018B 
	char _0x018C[59];
	BYTE State; //0x01C7 
	char _0x01C8[632];
};

class GO_CGS
{
public:
	char _0x0000[8];
	__int32 screenX; //0x0008 
	__int32 screenY; //0x000C 
	char _0x0010[20];
	char gametype[4]; //0x0024 
	char _0x0028[304];
	char mapName[20]; //0x0158 
};//Size=0x016C

class GO_PlayerNames
{
public:
	char _0x0000[22];
	__int16 mNum; //0x0016 
	char _0x0018[2];
	BYTE mTeam; //0x001A 
	char _0x001B[56];
	char mName[16]; //0x0053 
	char _0x0063[989];
};

class GO_clientInfo_t
{
public:
	__int32 mNum; //0x0000 
	__int32 Valid; //0x0004 
	char _0x0008[4];
	__int32 mTeam; //0x000C 
	char _0x0010[1520];

};

class GO_CG_t
{
public:
	__int32 ServerTime; //0x0000 
	char _0x0004[376];
	__int32 cNum; //0x017C 
	char _0x0180[72];
	__int32 Health1; //0x01C8 
	char _0x01CC[4];
	__int32 Health2; //0x01D0 
	char _0x01D4[620];

};//Size=0x0440

class GO_UserCmd_t
{
public:
	int ServerTime; //0x0000 
	int Button; //0x0004 
	int ViewAngle[3]; //0x0008 
	int WeaponID; //0x0014 
	char _0x0018[4];
	int moveTonpere; //0x001C 
	char _0x0020[28];
}; //size=0x003C

class GO_clientActive
{
public:
	char _0x0000[156];
	D3DXVECTOR3 vOrigin; //0x009C 
	char _0x00A8[60];
	D3DXVECTOR3 RefAngles; //0x00E4 
	char _0x00F0[12768];
	D3DXVECTOR3 ViewAngles; //0x32D0 
	GO_UserCmd_t cmds[128]; // 0x32DC
	int CmdNum; // 0x50DC

	GO_UserCmd_t* GetCmd(int index)
	{
		return &cmds[(index & 0x7F)];
	}
};

/////////////////////////////////////////////////


///////////////////MW3 Structs//////////////////////////
class MW3_cRefdef//to check
{
public:
	char _0x0000[8];
	__int32 iWidth; //0x0008 
	__int32 iHeight; //0x000C 
	FLOAT fov_X; //0x0010 
	FLOAT fov_Y; //0x0014 
	D3DXVECTOR3 EyePos; //0x0018 
	D3DXVECTOR3 ViewAxis[3]; //0x0024 
	char _0x0030[19276];
	D3DXVECTOR3 vAngleidk; //0x4B7C 
	D3DXVECTOR3 refView; //0x4B88 
	D3DXVECTOR3 vOrig; //0x4B94 
	D3DXVECTOR3 vOrig2; //0x4BA0 
	D3DXVECTOR3 refView1; //0x4BAC 
	char _0x4BB8[12];
	D3DXVECTOR3 refView2; //0x4BC4 
	char _0x4BD0[11376];
};

class MW3_cEntity
{
public:
	char _0x0000[2];
	short Valid; //0x0002 
	char _0x0004[16];
	D3DXVECTOR3 mPos; //0x0014 
	D3DXVECTOR3 ViewAngles; //0x0020 
	char _0x002C[49];
	BYTE Zooming; //0x005D 
	BYTE iMenu; //0x005E 
	BYTE Pose; //0x005F 
	char _0x0060[12];
	D3DXVECTOR3 mPos2; //0x006C 
	char _0x0078[24];
	D3DXVECTOR3 ViewAngles1; //0x0090 
	char _0x009C[64];
	D3DXVECTOR3 mPos3; //0x00DC 
	char _0x00E8[24];
	D3DXVECTOR3 ViewAngles2; //0x0100 
	char _0x010C[131];
	BYTE WeaponID; //0x018F 
	char _0x0190[55];
	BYTE State; //0x01C7 
	char _0x01C8[632];
};

class MW3_CGS
{
public:
	char _0x0000[8];
	__int32 screenX; //0x0008 
	__int32 screenY; //0x000C 
	char _0x0010[20];
	char gametype[4]; //0x0024 
	char _0x0028[304];
	char mapName[20]; //0x0158 
};//Size=0x016C

class MW3_ClientInfo
{
public:
	__int32 Valid; //0x0000 
	char _0x0004[4];
	__int32 mNum; //0x0008 
	char mName[20]; //0x000C 
	char _0x0020[12];
	__int32 mTeam; //0x002C 
	char _0x0030[1040];
};

//clientInfo refdef- 0x1099

class MW3_CG_t
{
public:
	char _0x0000[336];
	__int32 cNum; //0x0150 
	char _0x0154[72];
	__int32 Health1; //0x019C 
	char _0x01A0[4];
	__int32 Health2; //0x01A4 
	char _0x01A8[664];
};

class MW3_UserCmd_t
{
public:
	int ServerTime; //0x0000 
	int Button; //0x0004 
	int ViewAngle[3]; //0x0008 
	int WeaponID; //0x0014 
	char _0x0018[4];
	int moveTonpere; //0x001C 
	char _0x0020[12];
};// Size;0x2C

class MW3_clientActive
{
public:
	char _0x0000[128];
	int ServerTime; //0x0080 
	char _0x0084[24];
	D3DXVECTOR3 vOrigin; //0x009C 
	char _0x00A8[56];
	D3DXVECTOR3 RefAngles; //0x00E0 
	char _0x00EC[13508]; //0x00EC 
	D3DXVECTOR3 ViewAngles; //0x35B0 
	MW3_UserCmd_t cmds[128]; //0x35BC
	int CmdNum; //0x4BBC 

	MW3_UserCmd_t * GetCmd(int Index)
	{
		return &cmds[(Index & 0x7F)];
	}

};

/////////////////////////////////////////////////


///////////////////BO2 Structs//////////////////////////
class BO2_cRefdef
{
public:
	char _0x0000[8];
	__int32 screenX; //0x0008 
	__int32 screenY; //0x000C 
	char _0x0010[20];
	FLOAT fov_X; //0x0024 
	FLOAT fov_Y; //0x0028 
	FLOAT fov_Z; //0x002C 
	char _0x0030[4];
	FLOAT TotalFov; //0x0034 
	D3DXVECTOR3 EyePos; //0x0038 
	char _0x0044[4];
	D3DXVECTOR3 ViewAxis[3]; //0x0048 
	char _0x0054[94284];
	D3DXVECTOR3 vAngleidk; //0x170A0 
	D3DXVECTOR3 vOrig; //0x170AC 
	D3DXVECTOR3 refView1; //0x170B8 
	char _0x170C4[280];
	D3DXVECTOR3 vAngleidk2; //0x171DC 
	D3DXVECTOR3 refView2; //0x171E8 
	char _0x171F4[16972];
};

/*
class BO2_cEntity
{
public:
char _0x0000[2];
short Valid; //0x0002
char _0x0004[40];
D3DXVECTOR3 mPos; //0x002C
char _0x0038[285];
BYTE Zooming; //0x0155
char _0x0156[1];
BYTE Pose; //0x0157
char _0x0158[16];
D3DXVECTOR3 mPos2; //0x0168
char _0x0174[116];
D3DXVECTOR3 mPos3; //0x01E8
char _0x01F4[195];
BYTE WeaponID; //0x02B7
char _0x02B8[180];
__int8 State; //0x036C //64
char _0x036D[7];
};//Size=0x0374
*/

class BO2_cEntity
{
public:
	char _0x0000[2];
	short Valid; //0x0002 
	char _0x0004[40];
	D3DXVECTOR3 mPos; //0x002C 
	char _0x0038[168];
	__int8 AliveZombie; //0x00E0 
	char _0x00E1[116];
	BYTE Zooming; //0x0155 
	char _0x0156[1];
	BYTE Pose; //0x0157 
	char _0x0158[16];
	D3DXVECTOR3 mPos2; //0x0168 
	char _0x0174[116];
	D3DXVECTOR3 mPos3; //0x01E8 
	char _0x01F4[195];
	BYTE WeaponID; //0x02B7 
	char _0x02B8[180];
	__int8 State; //0x036C //64
	char _0x036D[7];
};//Size=0x0374

class BO2_CGS
{
public:
	char _0x0000[8];
	__int32 screenX; //0x0008 
	__int32 screenY; //0x000C 
	char _0x0010[32];
	char gametype[20]; //0x0030 
};//Size=0x0500


class BO2_ClientInfo
{
public:
	__int32 Valid; //0x0000 
	char _0x0004[4];
	__int32 mNum; //0x0008 
	char mName[20]; //0x000C 
	char _0x0020[12];
	__int32 mTeam; //0x002C 
	char _0x0030[92];
	__int32 State; //0x008C 
	char _0x0090[64];
	DWORD State2; //0x00D0 
	char _0x00D4[876];
};

class BO2_CG_t
{
public:
	__int32 cNum; //0x0000 
	char _0x0001[711];
	__int32 Health; //0x02C8 
	char _0x02CC[4];
	__int32 Health2; //0x02D0 
	char _0x02D4[364];
};//Size=0x0440

class BO2_UserCMD
{
public:
	int ServerTime; //0x0000 
	int Button; //0x0004 
	int ViewAngle[3]; //0x0008 
	int WeaponID; //0x0014 
	char _0x0018[4];
	int moveTonpere; //0x001C 
	char _0x0020[38];
};//Size=0x0046

class BO2_clientActive
{
public:
	char _0x0000[152];
	D3DXVECTOR3 vOrigin; //0x0098 
	char _0x00A4[100];
	D3DXVECTOR3 RefAngles; //0x0108 
	char _0x0114[11128];
	D3DXVECTOR3 ViewAngles; //0x2C8C
	char _0x02C98[16];
	BO2_UserCMD cmds[128];//0x2CA8
	int CmdNum; // 0x4AA8

	BO2_UserCMD * GetCmd(int Index)
	{
		return &cmds[(Index & 0x7F)];
	}
};
/////////////////////////////////////////////////

///////////////////BO1 Structs//////////////////////////
class BO1_cRefdef
{
public:
	char _0x0000[8];
	__int32 iWidth; //0x0008 
	__int32 iHeight; //0x000C 
	char _0x0010[20];
	FLOAT fov_X; //0x0024 
	FLOAT fov_Y; //0x0028 
	FLOAT TotalFov; //0x002C 
	D3DXVECTOR3 EyePos; //0x0030 
	char _0x003C[4];
	D3DXVECTOR3 ViewAxis[3]; //0x0040 
	char _0x004C[1012];
};

class BO1_cEntity
{
public:
	char _0x0000[2];
	__int8 Valid; //0x0002 
	char _0x0003[45];
	D3DXVECTOR3 mPos; //0x0030 
	char _0x003C[320];
	D3DXVECTOR3 mPos2; //0x017C 
	char _0x0188[108];
	D3DXVECTOR3 mPos3; //0x01F4 
	char _0x0200[166];
	short WeaponID; //0x02A6 
	char _0x02A8[112];
	__int8 State; //0x0318 
	char _0x0319[295];
};

class BO1_CGS
{
public:
	char _0x0000[8];
	__int32 screenX; //0x0008 
	__int32 screenY; //0x000C 
	char _0x0010[32];
	char gametype[4]; //0x0030 
	char _0x0034[12];
};

class BO1_CG_t
{
public:
	__int32 cNum; //0x0000 
	char _0x0001[595];
	__int32 Health1; //0x0254 
	char _0x0258[4];
	__int32 Health2; //0x025C 
	char _0x0260[32];
};

class BO1_ClientInfo
{
public:
	__int32 Valid; //0x0000 
	char _0x0004[4];
	__int32 mNum; //0x0008 
	char mName[20]; //0x000C 
	char _0x0020[12];
	__int32 mTeam; //0x002C 
	char _0x0030[52];
	__int32 State; //0x0064 
	char _0x0068[40];
	DWORD State2; //0x0090 
	char _0x0094[1644];
};

class BO1_UserCmd
{
public:
	int servertime; // 0x0
	int buttons; // 0x4
	char Unknown001[0x4]; // 0x8
	int viewangles[3]; // 0xC
	char Unknown002[0x1C]; // 0x18
	int moveTonpere;//0x0018
};//size 0x34


class BO1_clientActive
{
public:
	char _0x0000[76];
	D3DXVECTOR3 vOrigin; //0x004C 
	char _0x0058[84];
	D3DXVECTOR3 RefAngles; //0x00AC 
	char _0x00B8[10120];
	D3DXVECTOR3 ViewAngles; //0x2840 
	char _0x284C[16]; //284C
	BO1_UserCmd cmds[128]; //0x285C
	int CurrentCmd; //0x425C


	BO1_UserCmd *GetUserCmd(int cmdNum)
	{
		int id = cmdNum & 0x7F;
		return &cmds[id];
	}
};
/////////////////////////////////////////////////

//MW2 STRUCTS

class playerState_sMW2
{
public:
	BYTE _0x0000[4];
	int cmdTime; // 0x0004
	BYTE _0x0008[88];
	Vector3 origin; // 0x0060
	Vector3 velocity; // 0x006C
	BYTE _0x0078[592];
	FLOAT weaponPosFraction; //0x02C8
}; // size: custom: 0x02CC

class MW2_CGS
{
public:
	char _0x0000[8];
	__int32 screenX; //0x0008 
	__int32 screenY; //0x000C 
	char _0x0010[16];
	char gametype[4]; //0x0020 
	char _0x0024[180572];
	playerState_sMW2 ps; // 0x2C180
	char _0x2C44C[242004];
	FLOAT aimSpreadScale; // 0x675A0
};

class MW2_CG_t
{
public:
	char _0x0000[260];
	__int32 cNum; //0x0104 
	char _0x0108[72];
	__int32 Health; //0x0150 
	char _0x0154[4];
	__int32 Health2; //0x0158 
	char _0x015C[24292];
};

class MW2_cEntity //SIZE 0x1F8
{
public:
	char _0x0000[2];
	__int8 Valid; //0x0002 
	char _0x0003[21];
	D3DXVECTOR3 mPos; //0x0018 
	char _0x0024[84];
	D3DXVECTOR3 mPos2; //0x0078 
	char _0x0084[112];
	D3DXVECTOR3 mPos3; //0x00F4 
	char _0x0100[156];
	WORD WeaponID; //0x019C 
	char _0x019E[50];
	__int32 State; //0x01D0 
	char _0x01D4[620];
};

class MW2_cRefdef
{
public:
	char _0x0000[8];
	__int32 iWidth; //0x0008 
	__int32 iHeight; //0x000C 
	FLOAT fov_X; //0x0010 
	FLOAT fov_Y; //0x0014 
	D3DXVECTOR3 EyePos; //0x0018 
	D3DXVECTOR3 ViewAxis[3]; //0x0024 
	char _0x0030[1040];
};

class MW2_UserCmd_t
{
public:
	int ServerTime; //0x0000 
	int Button; //0x0004 
	int ViewAngles[3]; //0x0008  
	int Weapon; //0x0014 
	int moveTonpere; //0x001C
	char _0x0018[12];
};//Size = 0x24

class MW2_clientActive
{
public:
	char _0x0000[128];
	__int32 ServerTime; //0x0080 
	char _0x0084[24];
	D3DXVECTOR3 vOrigin; //0x009C 
	char _0x00A8[56];
	D3DXVECTOR3 RefAngles; //0x00E0 
	char _0x00EC[12908];
	D3DXVECTOR3 ViewAngles; //0x3358 
	MW2_UserCmd_t cmds[128]; //0x3360 //1404 size total
	int CmdNum; //0x4764 

	MW2_UserCmd_t * GetCmd(int Index)
	{
		return &cmds[(Index & 0x7F)];
	}

};

class MW2_ClientInfo
{
public:
	__int32 Valid; //0x0000 
	char _0x0004[4];
	__int32 mNum; //0x0008 
	char mName[20]; //0x000C 
	char _0x0020[12];
	__int32 mTeam; //0x002C 
	char _0x0030[1040];
};

//////////////////////////////////


BOOL updateOffset = FALSE;

BOOL LastFrameAimKey = FALSE;
cEntity * pSavedTarget;
GO_cEntity * pSavedTarget1;
MW3_cEntity * pSavedTarget2;
BO1_cEntity * pSavedTarget3;
BO2_cEntity * pSavedTarget4;
MW2_cEntity * pSavedTarget5;

enum entity_type
{
	ET_GENERAL_MP = 0,
	ET_PLAYER_MP = 1,
	ET_PLAYER_CORPSE_MP = 2,
	ET_ITEM_MP = 3,
	ET_MISSILE_MP = 4,
	ET_INVISIBLE_MP = 5,
	ET_SCRIPTMOVER_MP = 6,
	ET_SOUND_BLEND_MP = 7,
	ET_FX_MP = 8,
	ET_LOOP_FX_MP = 9,
	ET_PRIMARY_LIGHT_MP = 10,
	ET_TURRET_MP = 11,
	ET_HELICOPTER_MP = 12,
	ET_PLANE_MP = 13,
	ET_VEHICLE_MP = 14,
	ET_VEHICLE_COLLMAP_MP = 15,
	ET_ZOMBIE_MP = 16,
	ET_VEHICLE_SPAWNER_MP = 17
};

#define __isync() __emit(0x4C00012C)
#define SYS_STRING "\\System??\\%s"
#define USR_STRING "\\??\\%s"

//HANDLE hThread; 
//DWORD hThreadId;
#define ROTY 0
#define ROTX 1
#define ROTZ 2

#define INI_NAME "\\XeCheats.ini"
DWORD BO2 = 0x415608C3;//BO2
DWORD AW = 0x41560914;//AW
DWORD GO = 0x415608FC;//GO
DWORD MW3 = 0x415608CB;//MW3
DWORD BO1 = 0x41560855;//BO1
DWORD MW2 = 0x41560817;//MW2

					   //--OFFSETS AW----------------------------------
DWORD Cl_ReadyToSendPacket = 0x826489F0;//TU13
DWORD Cl_DrawText = 0x823CB3C0; //TU13
DWORD clientActive_t = 0x82BCF6A8;//TU13
DWORD centity_t = 0x82B921D0; //TU13
DWORD refdef_s = 0x82B801C8; //TU13
DWORD cg_t = 0x82B92638; //TU13
DWORD cgs_t = 0x82B921D4; //TU13
DWORD AddCmdDrawText_t = 0x822AF250;//TU13
DWORD AddCmdDrawStretchPic_t = 0x822AE388;//TU13
DWORD DrawRotatedPicPhysical_t = 0x826CB478;//TU13
DWORD DrawRotated_R3 = 0x82C56790;//TU13
DWORD DvarGetBOOL = 0x823A31E0;//TU13
DWORD CG_CanSeePlayer = 0x82669AC0;//TU13 
DWORD KeyIsDown = 0x826475B0;//TU13
DWORD GetPos = 0x828394C0;//TU13
DWORD j_head = 0x83A2BDBE;//TU13
DWORD j_neck = 0x83A2BDC0;//TU13
DWORD NoRecoil = NULL;//0x82640B68 AW TU4 //82654C30 TU5 //82654C48 --NEEDS UPDATING
DWORD NoSpread1 = NULL;//0x8263E788 AW TU4 //82652850 TU5 //82652868
DWORD NoSpread2 = NULL;//0x824D26D0 AW TU4 //824E44D8 TU5 //824E44B8
DWORD WeaponSway = NULL;//0x826B58D0 AW TU4 //826C9908 TU5 //826C9920
DWORD ZommingSway = NULL;//0x826B5800 AW TU4 //826C9838 TU5 //826C9850
DWORD Radar = NULL;//0x826A9E1C AW TU4 //826BDE74 TU5 //826BDE8C
DWORD AW_RegTag = 0x82489A20; //8247E7B0 TU5 //8247E790
							  // ----------------------------------------

							  //--OFFSETS GO----------------------------------
DWORD GO_Cl_ReadyToSendPacket = 0x822D4918;// 0x822D4918//GO TU17
DWORD GO_clientActive_t = 0x82BAC404;// 0x82BAC404//GO TU17
DWORD GO_centity_t = 0x82AD11AC;// 0x82AD11AC//GO TU17
DWORD GO_refdef_s = 0x82ACCCE8;// 0x82ACCCE8//GO TU17
DWORD GO_cg_t = 0x82AD56FC;// 0x82AD56FC//GO TU17
DWORD GO_cgs_t = 0x82AD56F8;// 0x82AD56F8//GO TU17
DWORD GO_Cl_DrawText = 0x8253E140;// 0x8253E140//GO TU17
DWORD GO_AddCmdDrawText_t = 0x8266CEB8;// 0x8266CEB8//GO TU17
DWORD GO_AddCmdDrawStretchPic_t = 0x8266AF28;// 0x8266AF28//GO TU17
DWORD GO_DrawRotatedPicPhysical_t = 0x82270138;// 0x82270138//GO TU17
DWORD GO_DrawRotated_R3 = 0x82BCDD10;// 0x82BCDD10//GO TU17
DWORD GO_DvarGetBOOL = 0x825639B0;// 0x825639B0//GO TU17
DWORD GO_CG_CanSeePlayer = 0x822C6048;// 0x822C6048//GO TU17
DWORD GO_KeyIsDown = 0x822D6E90;// 0x822D6E90//GO TU17
DWORD GO_GetPos = 0x821379E0;// 0x821379E0//GO TU17
DWORD GO_j_head = 0x834DC504;// 0x834DC504//GO TU17
DWORD GO_j_neck = 0x834DC506;// 0x834DC506//GO TU17
DWORD GO_NoRecoil = 0x822C9344;// 0x822C9344//GO TU17
DWORD GO_NoSpread1 = 0x8226E388;// 0x8226E388//GO TU17
DWORD GO_NoSpread2 = 0x82255C90;// 0x82255C90//GO TU17
DWORD GO_WeaponSway = 0x822583D0;// 0x822583D0//GO TU17
DWORD GO_ZommingSway = 0x82258BEC;// 0x82258BEC//GO TU17
DWORD GO_Radar = 0x822654C8;// 0x822654C8//GO TU17 
DWORD GO_RegTag = 0x824053F0;
// ----------------------------------------

//--OFFSETS MW3----------------------------------
DWORD MW3_Cl_ReadyToSendPacket = 0x8216D998;// 0x8216D998;//MW3 TU23
DWORD MW3_clientActive_t = 0x82713DC4;//0x82713DC4;//MW3 TU23
DWORD MW3_centity_t = 0x826436B8;//0x826436B8;//MW3 TU23
DWORD MW3_refdef_s = 0x82643E5C;// 0x82643E5C;//MW3 TU23
DWORD MW3_cg_t = 0x8263EE60;// 0x8263EE60;//MW3 TU23
DWORD MW3_cgs_t = 0x8263EE44;//= 0x8263EE44;//MW3 TU23
DWORD MW3_Cl_DrawText = 0x82174C78; //0x82309108;//MW3 TU23 //updated to TU24
DWORD MW3_AddCmdDrawText_t = 0x8241F9E0; //0x8241F918;//MW3 TU23 //updated to TU24
DWORD MW3_AddCmdDrawStretchPic_t = 0x8241F288; //0x8241F038;//MW3 TU23 //updated to TU24
DWORD MW3_DrawRotatedPicPhysical_t = 0x82117668; //0x82117668;//MW3 TU23
DWORD MW3_DrawRotated_R3 = 0x82732100; //0x82732100;//MW3 TU23
DWORD MW3_DvarGetBool = 0x8232E2C8; //0x8232E200;//MW3 TU23 //updated to TU24
DWORD MW3_CG_CanSeePlayer = 0x8215D848; //0x8215D848;//MW3 TU23
DWORD MW3_KeyIsDown = 0x8216ED28; //0x8216ED28;//MW3 TU23//updated to TU24
DWORD MW3_GetPos = 0x820D3860; // 0x820D3860;//MW3 TU23
DWORD MW3_j_head = 0x82FFA106;//= 0x82FFA106;//MW3 TU23
DWORD MW3_j_neck = 0x82FFA108;//= 0x82FFA108;//MW3 TU23
DWORD MW3_NoRecoil = 0x821614D4;//= 0x821614D4;//MW3 TU23
DWORD MW3_NoSpread1 = 0x82116CDC;//= 0x82116CDC;//MW3 TU23
DWORD MW3_NoSpread2 = 0x82160380; //0x82160380;//MW3 TU23
DWORD MW3_WeaponSway = 0x82103CD8; //0x82103CD8;//MW3 TU23
DWORD MW3_ZommingSway = 0x8210468C; //0x8210468C;//MW3 TU23
DWORD MW3_Radar = 0x8210E58C; //0x8210E58C;//MW3 TU23
DWORD MW3_RegTag = 0x82251320;
// ----------------------------------------


//--OFFSETS BO2----------------------------------
#define BO2_Cl_ReadyToSendPacket 0x8225E930//BO2 TU18
#define	BO2_clientActive_t 0x82C70F4C//BO2 TU18
#define BO2_CL_RenderScene 0x82262978//BO2 TU18
#define BO2_R_RenderScene 0x828AF5A0//BO2 TU18
#define BO2_centity_t 0x82BBC554//BO2 TU18
#define BO2_cg_t 0x82BBAE68//BO2 TU18
#define BO2_cgs_t 0x82BBAE44//BO2 TU18
#define BO2_AddCmdDrawText_t 0x828B8BA0//BO2 TU18
#define BO2_AddCmdDrawStretchPic_t 0x828B86C0//BO2 TU18
#define BO2_DrawRotatedPicPhysical_t 0x821C7F58//BO2 TU18
#define BO2_DrawRotated_R3 0x82CBC168//BO2 TU18
#define BO2_DvarGetBOOL 0x82461610//BO2 TU18
#define BO2_CanSeeFriend 0x821C47B8//BO2 TU18
#define BO2_KeyIsDown 0x8226DB40//BO2 TU18
#define BO2_GetPos 0x821A1A40//BO2 TU18 0x8219F298
#define BO2_j_head 0x836C5C92//BO2 TU18
#define BO2_j_neck 0x836C5C94//BO2 TU18
#define BO2_j_spineupper 0x836C5C96//BO2 TU18
#define BO2_j_spinelower 0x836C5C98//BO2 TU18
#define BO2_NoRecoil 0x82259BC8//BO2 TU18
#define BO2_NoSpread1 0x82254810//BO2 TU18
#define BO2_NoSpread2 0x821C7234//BO2 TU18
#define BO2_WeaponSway 0x826C6E6C//BO2 TU18
#define BO2_ZommingSway 0x826C7A7C//BO2 TU18
#define BO2_Radar 0x821B8FD0//BO2 TU18
#define BO2_SL_GetStringOfSize 0x82532BE8//BO2 TU18
#define BO2_Red_Boxes 0x821f5b7c//BO2 TU18
#define BO2_Laser 0x82255E1C//BO2 TU18
#define BO2_Cbuf_AddText 0x824015E0//BO2 TU18
DWORD BO2_RefDefCalc = NULL;
DWORD BO2_RegTag = 0x823599E0;
// ----------------------------------------

//--OFFSETS BO1----------------------------------
#define BO1_Cl_ReadyToSendPacket 0x82239508//BO1 TU11
#define	BO1_clientActive_t 0x829D9728//BO1 TU11
#define BO1_CL_RenderScene 0x8222B5D8//BO1 TU11
#define BO1_R_RenderScene 0x82634A30//BO1 TU11
#define BO1_centity_t 0x8296808C//BO1 TU11
#define BO1_cg_t 0x82964EA0//BO1 TU11
#define BO1_cgs_t 0x82964E64//BO1 TU11
#define BO1_AddCmdDrawText_t 0x8263EF00//BO1 TU11
#define BO1_AddCmdDrawStretchPic_t 0x8263EA68//BO1 TU11
#define BO1_DrawRotatedPicPhysical_t 0x821B86A8//BO1 TU11
#define BO1_DrawRotated_R3 0x82A06718//BO1 TU11
#define BO1_DvarGetBOOL 0x823E2890//BO1 TU11
#define BO1_CanSeeFriend 0x821B51B0//BO1 TU11
#define BO1_KeyIsDown 0x8223CCA8//BO1 TU11
#define BO1_GetPos 0x82156260//BO1 TU11
#define BO1_j_head 0x8311E48A//BO1 TU11
#define BO1_j_neck 0x8311E48C//BO1 TU11
#define BO1_NoRecoil 0x82227624//BO1 TU11
#define BO1_NoSpread1 0x8232786C//BO1 TU11
#define BO1_NoSpread2 0x821B7C04//BO1 TU11
#define BO1_WeaponSway 0x82195200//BO1 TU11
#define BO1_ZommingSway 0x82195D1C//BO1 TU11
#define BO1_Radar 0x821A819C//BO1 TU11
DWORD BO1_RefDefCalc = NULL;
DWORD BO1_RegTag = 0x822E1B58;
// ----------------------------------------

//--OFFSETS MW2----------------------------------
DWORD MW2_Cl_ReadyToSendPacket = 0x821401E0; //0x82140220 Tu8
DWORD MW2_clientActive_t = 0x82682354;
DWORD MW2_centity_t = 0x825BE150;
DWORD MW2_cg_t = 0x825B8638;
DWORD MW2_cgs_t = 0X825B861C;/*-4*/
DWORD MW2_Cl_DrawText = 0x821461E8; //Tu8 82146178
DWORD MW2_AddCmdDrawText_t = 0x82350278; //Tu8 0x82350258
DWORD MW2_AddCmdDrawStretchPic_t = 0x821384D8;//Tu8 0x8234F998 or 0x8234F9B8 0x821384D8
DWORD MW2_DrawRotatedPicPhysical_t = 0x820F7900;
DWORD MW2_DrawRotated_R3 = 0x82690F40; //Tu8 0x82690F48 
DWORD MW2_DvarGetBool = 0x8229EEE8;//Tu8 0x8229EF58 
DWORD MW2_CG_CanSeePlayer = 0x820F2C80; //820F2C80 
DWORD MW2_KeyIsDown = 0x82141308;
DWORD MW2_GetPos = 0x820C4458;
DWORD MW2_j_head = 0x83129306;
DWORD MW2_j_neck = 0x83129308;
DWORD MW2_NoRecoil = 0x82135BFC;
DWORD MW2_NoSpread1 = 0x820F584C;
DWORD MW2_NoSpread2 = 0x82134E88;
DWORD MW2_WeaponSway = 0x820E5B38;
DWORD MW2_ZommingSway = 0x820E657C;
DWORD MW2_Radar = 0x820EF12C;
DWORD BG_SetNewAnimation = 0x820C8908; //R4 ClientInfo
DWORD R_SetLodOrigin = 0x823575C0; //R3 Reddef Tu8 0x823575A0
#define MW2_refdef_s 0x82553764
DWORD MW2_ClientInfoCalc = NULL;
// ----------------------------------------

/*
//--OFFSETS WAW----------------------------------
//DWORD Cl_ReadyToSendPacket;//// 821B0DF8
//DWORD Cl_DrawText// 821BF5B0
DWORD clientActive_t = NULL;// 826E8BD8 or 826E8BDC
DWORD centity_t = NULL; // 82510478
DWORD refdef_s= NULL; //
DWORD cg_t= NULL; // 8268CD98
DWORD cgs_t= NULL; // 8268CD78
DWORD AddCmdDrawText_t = NULL;// 82401C30
DWORD AddCmdDrawStretchPic_t =NULL;// 82401410
DWORD DrawRotatedPicPhysical_t = NULL;// 8215CEC0
DWORD DrawRotated_R3 = NULL;// 82C143F8
DWORD DvarGetBOOL = NULL;// 822BE0E8
DWORD CG_CanSeePlayer =NULL;//
DWORD KeyIsDown = NULL;// 821B2908
DWORD GetPos = NULL;// 82123C08
DWORD j_head = NULL;// 82DD4746
DWORD j_neck  = NULL;// 82DD4748
DWORD NoRecoil = NULL;// 821A0BFC
DWORD NoSpread1 = NULL;// 8215C7E4
DWORD NoSpread2 = NULL;// 821A00C0
DWORD WeaponSway = NULL;// 8214789C
DWORD ZommingSway = NULL;// 8214804C
DWORD Radar = NULL;// 821548A4
*/

char* BoneNames[20] = {
	/*Head*/          "j_helmet"     , "j_head"            , "j_neck"
	/*Arms*/        , "j_shoulder_le", "j_shoulder_ri"  , "j_elbow_le"     , "j_elbow_ri", "j_wrist_le", "j_wrist_ri", "j_gun"
	/*Back*/        , "j_mainroot"   , "j_spineupper"   , "j_spinelower" , "j_spine4"
	/*Legs*/        , "j_hip_ri"     , "j_hip_le"       , "j_knee_le"    , "j_knee_ri" , "j_ankle_ri", "j_ankle_le" };

#define FONT_SMALL_DEV    "fonts/smallDevFont"
#define FONT_BIG_DEV      "fonts/bigDevFont"
#define FONT_CONSOLE      "fonts/consoleFont"
#define FONT_BIG          "fonts/bigFont"
#define FONT_SMALL        "fonts/smallFont"
#define FONT_BOLD         "fonts/boldFont"
#define FONT_NORMAL       "fonts/normalFont"
#define FONT_EXTRA_BIG    "fonts/extraBigFont"
#define FONT_OBJECTIVE    "fonts/objectiveFont"

//--FONTS AW----------------------------------
DWORD titleFont = NULL;//AW TU4 //83095E6C TU5 //83095E6C
					   //-----------------------------------------

					   //--FONTS GO----------------------------------
DWORD bigFont = 0x82E8E224;//GO TU17
						   //-----------------------------------------

						   //--FONTS MW3----------------------------------
DWORD MW3_bigFont = 0x82AD816C;//MW3 TU23
							   //-----------------------------------------

							   //--FONTS MW2----------------------------------
DWORD MW2_bigFont = 0x82997634;
//-----------------------------------------

//--FONTS BO2----------------------------------
DWORD BO2_bigFont = 0x82FC3378;//BO2 TU18
							   //-----------------------------------------

							   //--FONTS BO1----------------------------------
DWORD BO1_bigFont = 0x82C1CF24;//BO1 TU11
							   //-----------------------------------------

							   //--SHADERS AW----------------------------------
DWORD White = NULL;//AW TU4// 83269700 TU5 //83269700
				   //-----------------------------------------

				   //--SHADERS GO----------------------------------
DWORD GO_White = 0x82FE4630;//GO TU17
							//-----------------------------------------

							//--SHADERS MW3----------------------------------
DWORD MW3_White = 0x82B518D8;//MW3 TU23
							 //-----------------------------------------

							 //--SHADERS BO2----------------------------------
DWORD BO2_White = 0x83170788;//BO2 TU18
							 //-----------------------------------------

							 //--SHADERS BO1----------------------------------
DWORD BO1_White = 0x82D30AF8;//BO1 TU11
							 //-----------------------------------------

							 //--SHADERS MW2----------------------------------
DWORD MW2_White = 0x829FDC60;
//-----------------------------------------

//------TYPEDEF AW-------------------------------------------------------------------
typedef VOID(__cdecl *R_AddCmdDrawText_t)(const char *text, int maxChars, DWORD font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, const FLOAT *color, int style);
R_AddCmdDrawText_t R_AddCmdDrawText = (R_AddCmdDrawText_t)AddCmdDrawText_t;

typedef VOID(__cdecl *R_AddCmdDrawStretchPic_t)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, const FLOAT *color, DWORD material);
R_AddCmdDrawStretchPic_t R_AddCmdDrawStretchPic = (R_AddCmdDrawStretchPic_t)AddCmdDrawStretchPic_t;

typedef VOID(__cdecl *CG_DrawRotatedPicPhysical)(DWORD ScreenPlacement, FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT angle, const FLOAT *color, int Material);
CG_DrawRotatedPicPhysical DrawRotated = (CG_DrawRotatedPicPhysical)DrawRotatedPicPhysical_t;

typedef BOOL(*Dvar_GetBOOL_t)(const char *dvarName);
Dvar_GetBOOL_t Dvar_GetBOOL = (Dvar_GetBOOL_t)DvarGetBOOL;

typedef BOOL(*CanSeePlayer_)(int unknown, cEntity * pEnt, DWORD mask);
CanSeePlayer_ CanSeePlayer = (CanSeePlayer_)CG_CanSeePlayer;

typedef int(*Key_IsDown)(int ClientNum, int ButtonEnum);
Key_IsDown Key_Down = (Key_IsDown)KeyIsDown;

typedef int(*GetTagPos_t)(int unknown, cEntity * TheEnt, short tag, FLOAT* out);
static GetTagPos_t GetTagPos = (GetTagPos_t)GetPos;

typedef int(*AW_RTGBN) (char* szName);
AW_RTGBN AW_GetTagByName = (AW_RTGBN)AW_RegTag;

//-------------------------------------------------------------------------------

//------TYPEDEF GO-------------------------------------------------------------------
typedef VOID(__cdecl *R_AddCmdDrawText_t)(const char *text, int maxChars, DWORD font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, const FLOAT *color, int style);
R_AddCmdDrawText_t GO_R_AddCmdDrawText = (R_AddCmdDrawText_t)GO_AddCmdDrawText_t;

typedef VOID(__cdecl *R_AddCmdDrawStretchPic_t)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, const FLOAT *color, DWORD material);
R_AddCmdDrawStretchPic_t GO_R_AddCmdDrawStretchPic = (R_AddCmdDrawStretchPic_t)GO_AddCmdDrawStretchPic_t;

typedef VOID(__cdecl *CG_DrawRotatedPicPhysical)(DWORD ScreenPlacement, FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT angle, const FLOAT *color, int Material);
CG_DrawRotatedPicPhysical GO_DrawRotated = (CG_DrawRotatedPicPhysical)GO_DrawRotatedPicPhysical_t;

typedef BOOL(*Dvar_GetBOOL_t)(const char *dvarName);
Dvar_GetBOOL_t GO_Dvar_GetBOOL = (Dvar_GetBOOL_t)GO_DvarGetBOOL;

typedef BOOL(*GO_CanSeePlayer_)(int unknown, GO_cEntity * pEnt, DWORD mask);
GO_CanSeePlayer_ GO_CanSeePlayer = (GO_CanSeePlayer_)GO_CG_CanSeePlayer;

typedef int(*Key_IsDown)(int ClientNum, int ButtonEnum);
Key_IsDown GO_Key_Down = (Key_IsDown)GO_KeyIsDown;

typedef int(*GO_GetTagPos_t)(int unknown, GO_cEntity * TheEnt, short tag, FLOAT* out);
static GO_GetTagPos_t GO_GetTagPos = (GO_GetTagPos_t)GO_GetPos;

typedef VOID *(__cdecl * DB_FindXAssetHeader_t)(int assetType, const char *szName);
DB_FindXAssetHeader_t DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x00552490;

typedef int(*Go_RTGBN) (char* szName);
Go_RTGBN GetTagByName = (Go_RTGBN)GO_RegTag;

//-----------------------------------------

//------TYPEDEF MW3-------------------------------------------------------------------
typedef VOID(__cdecl *R_AddCmdDrawText_t)(const char *text, int maxChars, DWORD font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, const FLOAT *color, int style);
R_AddCmdDrawText_t MW3_R_AddCmdDrawText = (R_AddCmdDrawText_t)MW3_AddCmdDrawText_t;

typedef VOID(__cdecl *R_AddCmdDrawStretchPic_t)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, const FLOAT *color, DWORD material);
R_AddCmdDrawStretchPic_t MW3_R_AddCmdDrawStretchPic = (R_AddCmdDrawStretchPic_t)MW3_AddCmdDrawStretchPic_t;

typedef VOID(__cdecl *CG_DrawRotatedPicPhysical)(DWORD ScreenPlacement, FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT angle, const FLOAT *color, int Material);
CG_DrawRotatedPicPhysical MW3_DrawRotated = (CG_DrawRotatedPicPhysical)MW3_DrawRotatedPicPhysical_t;

typedef BOOL(*Dvar_GetBOOL_t)(const char *dvarName);
Dvar_GetBOOL_t MW3_Dvar_GetBOOL = (Dvar_GetBOOL_t)MW3_DvarGetBool;

typedef BOOL(*MW3_CanSeePlayer_)(int unknown, MW3_cEntity * pEnt, DWORD mask);
MW3_CanSeePlayer_ MW3_CanSeePlayer = (MW3_CanSeePlayer_)MW3_CG_CanSeePlayer;

typedef int(*Key_IsDown)(int ClientNum, int ButtonEnum);
Key_IsDown MW3_Key_Down = (Key_IsDown)MW3_KeyIsDown;

typedef int(*MW3_GetTagPos_t)(int unknown, MW3_cEntity * TheEnt, short tag, FLOAT* out);
static MW3_GetTagPos_t MW3_GetTagPos = (MW3_GetTagPos_t)MW3_GetPos;

typedef int(*MW3_RTGBN) (char* szName);
MW3_RTGBN MW3_GetTagByName = (MW3_RTGBN)MW3_RegTag;

//-----------------------------------------

//------TYPEDEF BO2-------------------------------------------------------------------
typedef VOID(__cdecl *R_AddCmdDrawText_t)(const char *text, int maxChars, DWORD font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, const FLOAT *color, int style);
R_AddCmdDrawText_t BO2_R_AddCmdDrawText = (R_AddCmdDrawText_t)BO2_AddCmdDrawText_t;

typedef VOID(__cdecl *R_AddCmdDrawStretchPic_t)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, const FLOAT *color, DWORD material);
R_AddCmdDrawStretchPic_t BO2_R_AddCmdDrawStretchPic = (R_AddCmdDrawStretchPic_t)BO2_AddCmdDrawStretchPic_t;

typedef VOID(__cdecl *CG_DrawRotatedPicPhysical)(DWORD ScreenPlacement, FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT angle, const FLOAT *color, int Material);
CG_DrawRotatedPicPhysical BO2_DrawRotated = (CG_DrawRotatedPicPhysical)BO2_DrawRotatedPicPhysical_t;

typedef BOOL(*Dvar_GetBOOL_t)(const char *dvarName);
Dvar_GetBOOL_t BO2_Dvar_GetBOOL = (Dvar_GetBOOL_t)BO2_DvarGetBOOL;

typedef int(*BO2_CanSeeFriendlyHead_)(int unknown, BO2_cEntity * pEnt);
BO2_CanSeeFriendlyHead_ BO2_CanSeeFriendlyHead = (BO2_CanSeeFriendlyHead_)BO2_CanSeeFriend;

typedef VOID(__cdecl *R_RenderScene_t)(DWORD refdef);
R_RenderScene_t BO2_R_RenderScene_ = (R_RenderScene_t)BO2_R_RenderScene;

typedef int(*Key_IsDown)(int ClientNum, int ButtonEnum);
Key_IsDown BO2_Key_Down = (Key_IsDown)BO2_KeyIsDown;

typedef int(*BO2_GetTagPos_t)(BO2_cEntity * TheEnt, short tag, FLOAT* out);
static BO2_GetTagPos_t BO2_GetTagPos = (BO2_GetTagPos_t)BO2_GetPos;

typedef int(*BO2_RTGBN) (char* szName);
BO2_RTGBN BO2_GetTagByName = (BO2_RTGBN)BO2_RegTag;

typedef int(__cdecl *SL_GetStringOfSizeBO2_)(const char *str, unsigned int user, unsigned int len);
SL_GetStringOfSizeBO2_ SL_GetStringOfSizeBO2 = (SL_GetStringOfSizeBO2_)BO2_SL_GetStringOfSize;

short registerTagBO2(const char *tagname) {
	return SL_GetStringOfSizeBO2(tagname, 1, strlen(tagname) + 1);
}

typedef VOID(__cdecl *Cbuf_AddTextBO2_t)(int localClientNum, const char *text);
Cbuf_AddTextBO2_t Cbuf_AddTextBO2 = (Cbuf_AddTextBO2_t)BO2_Cbuf_AddText;
//-----------------------------------------

//------TYPEDEF BO1-------------------------------------------------------------------
typedef VOID(__cdecl *R_AddCmdDrawText_t)(const char *text, int maxChars, DWORD font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, const FLOAT *color, int style);
R_AddCmdDrawText_t BO1_R_AddCmdDrawText = (R_AddCmdDrawText_t)BO1_AddCmdDrawText_t;

typedef VOID(__cdecl *R_AddCmdDrawStretchPic_t)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, const FLOAT *color, DWORD material);
R_AddCmdDrawStretchPic_t BO1_R_AddCmdDrawStretchPic = (R_AddCmdDrawStretchPic_t)BO1_AddCmdDrawStretchPic_t;

typedef VOID(__cdecl *CG_DrawRotatedPicPhysical)(DWORD ScreenPlacement, FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT angle, const FLOAT *color, int Material);
CG_DrawRotatedPicPhysical BO1_DrawRotated = (CG_DrawRotatedPicPhysical)BO1_DrawRotatedPicPhysical_t;

typedef BOOL(*Dvar_GetBOOL_t)(const char *dvarName);
Dvar_GetBOOL_t BO1_Dvar_GetBOOL = (Dvar_GetBOOL_t)BO1_DvarGetBOOL;

typedef int(*BO1_CanSeeFriendlyHead_)(int unknown, BO1_cEntity * pEnt);
BO1_CanSeeFriendlyHead_ BO1_CanSeeFriendlyHead = (BO1_CanSeeFriendlyHead_)BO1_CanSeeFriend;

typedef VOID(__cdecl *BO1_R_RenderScene_t)(DWORD refdef);
BO1_R_RenderScene_t BO1_R_RenderScene_ = (BO1_R_RenderScene_t)BO1_R_RenderScene;

typedef int(*Key_IsDown)(int ClientNum, int ButtonEnum);
Key_IsDown BO1_Key_Down = (Key_IsDown)BO1_KeyIsDown;

typedef int(*BO1_GetTagPos_t)(int unknown, BO1_cEntity * TheEnt, short tag, FLOAT* out);
static BO1_GetTagPos_t BO1_GetTagPos = (BO1_GetTagPos_t)BO1_GetPos;

typedef int(*BO1_RTGBN) (char* szName);
BO1_RTGBN BO1_GetTagByName = (BO1_RTGBN)BO1_RegTag;
//-----------------------------------------

//------TYPEDEF MW2-------------------------------------------------------------------
typedef VOID(__cdecl *R_AddCmdDrawText_t)(const char *text, int maxChars, DWORD font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, const FLOAT *color, int style);
R_AddCmdDrawText_t MW2_R_AddCmdDrawText = (R_AddCmdDrawText_t)MW2_AddCmdDrawText_t;

typedef VOID(__cdecl *R_AddCmdDrawStretchPic_t)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, const FLOAT *color, DWORD material);
R_AddCmdDrawStretchPic_t MW2_R_AddCmdDrawStretchPic = (R_AddCmdDrawStretchPic_t)MW2_AddCmdDrawStretchPic_t;

typedef VOID(__cdecl *CG_DrawRotatedPicPhysical)(DWORD ScreenPlacement, FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT angle, const FLOAT *color, int Material);
CG_DrawRotatedPicPhysical MW2_DrawRotated = (CG_DrawRotatedPicPhysical)MW2_DrawRotatedPicPhysical_t;

typedef BOOL(*Dvar_GetBOOL_t)(const char *dvarName);
Dvar_GetBOOL_t MW2_Dvar_GetBOOL = (Dvar_GetBOOL_t)MW2_DvarGetBool;

typedef BOOL(*MW2_CanSeePlayer_)(int unknown, MW2_cEntity * pEnt, DWORD mask);
MW2_CanSeePlayer_ MW2_CanSeePlayer = (MW2_CanSeePlayer_)MW2_CG_CanSeePlayer;

typedef int(*Key_IsDown)(int ClientNum, int ButtonEnum);
Key_IsDown MW2_Key_Down = (Key_IsDown)MW2_KeyIsDown;

typedef int(*MW2_GetTagPos_t)(int unknown, MW2_cEntity * TheEnt, short tag, FLOAT* out);
static MW2_GetTagPos_t MW2_GetTagPos = (MW2_GetTagPos_t)MW2_GetPos;

typedef VOID(__cdecl *R_SetLodOrigin_t)(DWORD refdef);
R_SetLodOrigin_t SetLodOrigin = (R_SetLodOrigin_t)R_SetLodOrigin;

typedef VOID(__cdecl *BG_GetSpreadForWeaponMW2_t)(playerState_sMW2 *ps, int weapondef, FLOAT *minSpread, FLOAT *maxSpread);
BG_GetSpreadForWeaponMW2_t BG_GetSpreadForWeaponMW2 = (BG_GetSpreadForWeaponMW2_t)0x820E33F0;

typedef VOID(__cdecl *RandomBulletDirMW2_t)(int randSeed, FLOAT *x, FLOAT *y);
RandomBulletDirMW2_t RandomBulletDirMW2 = (RandomBulletDirMW2_t)0x8212EF78;
//-----------------------------------------

//---------------INI FUNCTIONS AW-----------
VOID setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors);
VOID DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color);
//---------------------------------------

//---------------INI FUNCTIONS GO-----------
VOID GO_setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID GO_setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID GO_DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID GO_DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors);
VOID GO_DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID GO_DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color);
//---------------------------------------

//---------------INI FUNCTIONS MW3-----------
VOID MW3_setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID MW3_setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID MW3_DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID MW3_DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors);
VOID MW3_DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID MW3_DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color);
//---------------------------------------

//---------------INI FUNCTIONS BO2-----------
VOID BO2_setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID BO2_setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID BO2_DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID BO2_DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors);
VOID BO2_DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID BO2_DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *colorInside, const FLOAT *colorOutside);
//---------------------------------------

//---------------INI FUNCTIONS BO1-----------
VOID BO1_setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID BO1_setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID BO1_DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID BO1_DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors);
VOID BO1_DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID BO1_DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color);
//---------------------------------------

//---------------INI FUNCTIONS MW2-----------
VOID MW2_setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID MW2_setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz);
VOID MW2_DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID MW2_DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors);
VOID MW2_DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color);
VOID MW2_DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *colorInside, const FLOAT *colorOutside);
//---------------------------------------

BYTE MyTeam;
#define ENTITIESMAX 12
BOOL bInit = FALSE;

/////////////////////////def AW
cRefdef *RefDef;
cEntity *Entities[12];
CG_t *cg;
CGS *cgs;
PlayerNames *Clients[ENTITIESMAX];
PlayerNames *LocalClient;
clientInfo_t *ci[ENTITIESMAX];
clientInfo_t *Local;
/////////////////////////////////

/////////////////////////def GO
GO_cRefdef *GO_RefDef;
GO_cEntity *GO_Entities[12];
GO_PlayerNames *GO_Clients[ENTITIESMAX];
GO_CG_t *GO_cg;
GO_CGS *GO_cgs;
GO_PlayerNames *GO_LocalClient;
GO_clientInfo_t *GO_ci[ENTITIESMAX];
GO_clientInfo_t *GO_Local;
/////////////////////////////////

/////////////////////////def MW3
MW3_cRefdef *MW3_RefDef;
MW3_cEntity *MW3_Entities[18];
MW3_ClientInfo *MW3_Clients[18];
MW3_CG_t *MW3_cg;
MW3_CGS *MW3_cgs;
MW3_ClientInfo *MW3_LocalClient;
/////////////////////////////////

/////////////////////////def BO2
BO2_cRefdef *BO2_RefDef;
BO2_cEntity *BO2_Entities[12];
BO2_ClientInfo *BO2_Clients[ENTITIESMAX];
BO2_CG_t *BO2_cg;
BO2_CGS * BO2_cgs;
BO2_ClientInfo *BO2_LocalClient;
/////////////////////////////////

/////////////////////////def BO1
BO1_cRefdef *BO1_RefDef;
BO1_cEntity *BO1_Entities[12];
BO1_ClientInfo *BO1_Clients[ENTITIESMAX];
BO1_CG_t *BO1_cg;
BO1_CGS *BO1_cgs;
BO1_ClientInfo *BO1_LocalClient;
/////////////////////////////////

/////////////////////////def MW2
MW2_cRefdef *MW2_RefDef;
MW2_cEntity *MW2_Entities[12];
MW2_ClientInfo *MW2_Clients[12];
MW2_CG_t *MW2_cg;
MW2_CGS *MW2_cgs;
MW2_ClientInfo *MW2_LocalClient;
/////////////////////////////////

// -------------------COLORS-------------------
const FLOAT *change_color;
const FLOAT *friendly_color;
const FLOAT *enemies_color;
const FLOAT *visible_color;
const FLOAT *change_shad_color;
const FLOAT *change_shad_colorInside;
const FLOAT *change_shad_colorOutside;
const FLOAT *crosshair_color;
const FLOAT *shad_e_color;
const FLOAT *shad_f_color;
const FLOAT *shad_v_color;
static const FLOAT L_white[4] = { 1.0f, 1.0f, 1.0f, 0.2f };
static const FLOAT L_black[4] = { 0.0f, 0.0f, 0.0f, 0.2f };
static const FLOAT L_black2[4] = { 0.0f, 0.0f, 0.0f, 0.1f };
static const FLOAT L_grey[4] = { 0.5f, 0.5f, 0.5f, 0.2f };
static const FLOAT L_red[4] = { 1.0f, 0.0f, 0.0f, 0.2f };
static const FLOAT L_red2[4] = { 1.0f, 0.0f, 0.0f, 0.4f };
static const FLOAT L_green[4] = { 0.0f, 1.0f, 0.0f, 0.2f };
static const FLOAT L_green2[4] = { 0.0f, 1.0f, 0.0f, 0.4f };
static const FLOAT L_blue[4] = { 0.0f, 0.0f, 1.0f, 0.2f };
static const FLOAT L_blue2[4] = { 0.0f, 0.0f, 1.0f, 0.4f };
static const FLOAT L_yellow[4] = { 0.9f, 0.9f, 0.0f, 0.2f };
static const FLOAT L_yellow2[4] = { 0.9f, 0.9f, 0.0f, 0.4f };
static const FLOAT L_orange[4] = { 1.f, 0.5, 0.f, 0.2f };
static const FLOAT L_magenta[4] = { 1.0f, 0.0f, 1.0f, 0.2f };
static const FLOAT L_Purple[4] = { 0.6f, 0.2f, 0.8f, 0.2f };
static const FLOAT L_pink[4] = { 1.0f, 0.5607843137254902f, 0.8156862745098039f, 0.2f };
static const FLOAT L_cyan[4] = { 0.0f, 1.0f, 1.0f, 0.2f };
static const FLOAT L_xecheats_yellow[4] = { 0.8f, 1.0f, 0.2f, 0.2f };
static const FLOAT L_xecheats_orange[4] = { 0.8901960784313725f, 0.3372549019607843f,0.0627450980392157f, 0.2f };
static const FLOAT L_xecheats_blue[4] = { 0.0549019607843137f, 0.4509803921568627f,0.8f, 0.2f };

static const FLOAT OrangeStormz[4] = { 0.902f, 0.38f, 0.118f, 1.0f };
static const FLOAT BlueStormz[4] = { 0.118f, 0.639f, 0.902f,1.0f };
static const FLOAT OrangeStormzShader[4] = { 0.902f, 0.38f, 0.118f, 0.2f };
static const FLOAT BlueStormzShader[4] = { 0.118f, 0.639f, 0.902f, 0.2f };
static const FLOAT redShader[4] = { 1.0f, 0.0f, 0.0f, 0.2f };
static const FLOAT white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
static const FLOAT black[4] = { 0.0f, 0.0f, 0.0f, 255.0f };
static const FLOAT black2[4] = { 0.0f, 0.0f, 0.0f, 0.9f };
static const FLOAT blackM[4] = { 0.0f, 0.0f, 0.0f, 0.5f };
static const FLOAT smoke[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
static const FLOAT grey[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
static const FLOAT grey1[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
static const FLOAT grey2[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
static const FLOAT grey3[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
static const FLOAT grey4[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
static const FLOAT red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
static const FLOAT red1[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
static const FLOAT red2[4] = { 0.6f, 0.0f, 0.0f, 1.0f };
static const FLOAT red3[4] = { 1.0f, 0.2f, 0.2f, 1.0f };
static const FLOAT red4[4] = { 0.5f, 0.1f, 0.1f, 1.0f };
static const FLOAT red5[4] = { 0.8f, 0.1f, 0.0f, 1.0f };
static const FLOAT green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
static const FLOAT green1[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
static const FLOAT green2[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
static const FLOAT green3[4] = { 0.0f, 0.4f, 0.0f, 1.0f };
static const FLOAT green4[4] = { 0.0f, 1.0f, 0.4f, 1.0f };
static const FLOAT green5[4] = { 0.2f, 0.6f, 0.4f, 1.0f };
static const FLOAT green6[4] = { 0.4f, 0.6f, 0.2f, 1.0f };
static const FLOAT blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
static const FLOAT blue1[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
static const FLOAT blue2[4] = { 0.0f, 0.4f, 1.0f, 1.0f };
static const FLOAT blue3[4] = { 0.2f, 0.2f, 0.8f, 1.0f };
static const FLOAT blue4[4] = { 0.0f, 0.0f, 0.6f, 1.0f };
static const FLOAT blue5[4] = { 0.4f, 0.2f, 1.0f, 1.0f };
static const FLOAT blue6[4] = { 0.4f, 0.5f, 1.0f, 1.0f };
static const FLOAT yellow[4] = { 0.9f, 0.9f, 0.0f, 1.0 };
static const FLOAT yellow1[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
static const FLOAT yellow2[4] = { 0.6f, 0.6f, 0.0f, 1.0f };
static const FLOAT yellow3[4] = { 0.8f, 1.0f, 0.2f, 1.0f };
static const FLOAT yellow4[4] = { 1.0f, 1.0f, 0.4f, 1.0f };
static const FLOAT yellow5[4] = { 0.6f, 0.4f, 0.2f, 1.0f };
static const FLOAT yellow6[4] = { 1.0f, 0.8f, 0.4f, 1.0f };
static const FLOAT yellow7[4] = { 0.8f, 0.8f, 0.0f, 1.0f };
static const FLOAT yellow8[4] = { 0.8f, 0.8f, 0.2f, 1.0f };
static const FLOAT yellow9[4] = { 0.8f, 0.8f, 0.4f, 1.0f };
static const FLOAT orange[4] = { 1.f, 0.5, 0.f, 1.f };
static const FLOAT orange1[4] = { 1.0f, 0.8f, 0.0f, 1.0f };
static const FLOAT orange2[4] = { 1.0f, 0.6f, 0.0f, 1.0f };
static const FLOAT orange3[4] = { 1.0f, 0.4f, 0.2f, 1.0f };
static const FLOAT orange4[4] = { 0.8f, 0.4f, 0.0f, 1.0f };
static const FLOAT orange5[4] = { 0.9f, 0.5f, 0.0f, 1.0f };
static const FLOAT orange6[4] = { 1.0f, 0.8f, 0.2f, 1.0f };
static const FLOAT magenta[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
static const FLOAT magenta1[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
static const FLOAT magenta2[4] = { 0.6f, 0.2f, 0.4f, 1.0f };
static const FLOAT magenta3[4] = { 1.0f, 0.4f, 0.6f, 1.0f };
static const FLOAT magenta4[4] = { 1.0f, 0.2f, 0.8f, 1.0f };
static const FLOAT Purple[4] = { 0.6f, 0.2f, 0.8f, 1.0f };
static const FLOAT Purple1[4] = { 0.6f, 0.2f, 0.8f, 1.0f };
static const FLOAT Purple2[4] = { 0.8f, 0.2f, 1.0f, 1.0f };
static const FLOAT Purple3[4] = { 0.6f, 0.0f, 1.0f, 1.0f };
static const FLOAT Purple4[4] = { 0.4f, 0.0f, 0.8f, 1.0f };
static const FLOAT Purple5[4] = { 0.6f, 0.0f, 0.8f, 1.0f };
static const FLOAT Purple6[4] = { 0.8f, 0.0f, 0.6f, 1.0f };
static const FLOAT pink[4] = { 1.0f, 0.5607843137254902f, 0.8156862745098039f, 1.0f };
static const FLOAT pink1[4] = { 1.0f, 0.5f, 0.8f, 1.0f };
static const FLOAT pink2[4] = { 1.0f, 0.8f, 0.9f, 1.0f };
static const FLOAT cyan[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
static const FLOAT cyan1[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
static const FLOAT cyan2[4] = { 0.4f, 0.8f, 0.8f, 1.0f };
static const FLOAT cyan3[4] = { 0.0f, 1.0f, 0.6f, 1.0f };
static const FLOAT cyan4[4] = { 0.6f, 1.0f, 1.0f, 1.0f };
static const FLOAT cyan5[4] = { 0.2f, 0.6f, 1.0f, 1.0f };
static const FLOAT cyan6[4] = { 0.2f, 0.6f, 0.6f, 1.0f };
static const FLOAT xecheats_yellow[4] = { 0.8f, 1.0f, 0.2f, 1.0f };
static const FLOAT xecheats_orange[4] = { 0.8901960784313725f, 0.3372549019607843f,0.0627450980392157f, 1.0f };
static const FLOAT xecheats_blue[4] = { 0.0549019607843137f, 0.4509803921568627f,0.8f, 1.0f };
// ---------------------------------------------

int maxChars = 2147372652;

VOID setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, orange, 0);
}
VOID GO_setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	GO_R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, orange, 0);
}
VOID MW3_setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	MW3_R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, orange, 0);
}
VOID BO1_setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	BO1_R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, orange, 0);
}
VOID BO2_setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	BO2_R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, orange, 0);
}
VOID MW2_setText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	MW2_R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, orange, 0);
}


VOID DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	R_AddCmdDrawText(text, 0x7FFFFFFF, titleFont, x, y, xScale, xScale, 0, color, 0);
}
VOID GO_DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	GO_R_AddCmdDrawText(text, 0x7FFFFFFF, bigFont, x, y, xScale, xScale, 0, color, 0);
}
VOID MW3_DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	MW3_R_AddCmdDrawText(text, 0x7FFFFFFF, MW3_bigFont, x, y, xScale, xScale, 0, color, 0);
}
VOID BO1_DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	BO1_R_AddCmdDrawText(text, 0x7FFFFFFF, BO1_bigFont, x, y, xScale, xScale, 0, color, 0);
}
VOID BO2_DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	BO2_R_AddCmdDrawText(text, 0x7FFFFFFF, BO2_bigFont, x, y, xScale, xScale, 0, color, 0);
}
VOID MW2_DrawText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	MW2_R_AddCmdDrawText(text, 0x7FFFFFFF, MW2_bigFont, x, y, xScale, xScale, 0, color, 0);
}


VOID DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color)
{
	//const FLOAT *clr[4] = color[3];

	DrawRect(x - (Width / 2), y - Height, Width, Height, color);

	DrawRect(x - (Width / 2), y - Height, Width, 1, black);//*TOP-LINE
	DrawRect(x - (Width / 2), y, Width, 1, black);//BOTTOM
	DrawRect(x - (Width / 2), y - Height, 1, Height, black);//LEFT
	DrawRect(x + (Width / 2), y - Height, 1, Height, black);//RIGHT
}
VOID GO_DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color)
{
	GO_DrawRect(x - (Width / 2), y - Height, Width, Height, color);

	GO_DrawRect(x - (Width / 2), y - Height, Width, 1, black);//*TOP-LINE
	GO_DrawRect(x - (Width / 2), y, Width, 1, black);//BOTTOM
	GO_DrawRect(x - (Width / 2), y - Height, 1, Height, black);//LEFT
	GO_DrawRect(x + (Width / 2), y - Height, 1, Height, black);//RIGHT
}
VOID MW3_DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color)
{
	MW3_DrawRect(x - (Width / 2), y - Height, Width, Height, color);

	MW3_DrawRect(x - (Width / 2), y - Height, Width, 1, black);//*TOP-LINE
	MW3_DrawRect(x - (Width / 2), y, Width, 1, black);//BOTTOM
	MW3_DrawRect(x - (Width / 2), y - Height, 1, Height, black);//LEFT
	MW3_DrawRect(x + (Width / 2), y - Height, 1, Height, black);//RIGHT
}
VOID BO1_DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color)
{
	BO1_DrawRect(x - (Width / 2), y - Height, Width, Height, color);

	BO1_DrawRect(x - (Width / 2), y - Height, Width, 1, black);//*TOP-LINE
	BO1_DrawRect(x - (Width / 2), y, Width, 1, black);//BOTTOM
	BO1_DrawRect(x - (Width / 2), y - Height, 1, Height, black);//LEFT
	BO1_DrawRect(x + (Width / 2), y - Height, 1, Height, black);//RIGHT
}
VOID BO2_DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *colorInside, const FLOAT *colorOutside)
{
	BO2_DrawRect(x - (Width / 2), y - Height, Width, Height, colorInside);

	BO2_DrawRect(x - (Width / 2), y - Height, Width, 2, colorOutside);//*TOP-LINE
	BO2_DrawRect(x - (Width / 2), y, Width, 2, colorOutside);//BOTTOM
	BO2_DrawRect(x - (Width / 2), y - Height, 2, Height, colorOutside);//LEFT
	BO2_DrawRect(x + (Width / 2), y - Height, 2, Height, colorOutside);//RIGHT
}
VOID MW2_DrawBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *colorInside, const FLOAT *colorOutside)
{
	MW2_DrawRect(x - (Width / 2), y - Height, Width, Height, colorInside);

	MW2_DrawRect(x - (Width / 2), y - Height, Width, 2, colorOutside);//*TOP-LINE
	MW2_DrawRect(x - (Width / 2), y, Width, 2, colorOutside);//BOTTOM
	MW2_DrawRect(x - (Width / 2), y - Height, 2, Height, colorOutside);//LEFT
	MW2_DrawRect(x + (Width / 2), y - Height, 2, Height, colorOutside);//RIGHT
}

VOID BO2_DrawCornerBox(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color)
{
	int WidthThird = Width / 4;
	int HeightThird = WidthThird;
 
	/* draw eight shadowlines */
	//topleft 
	BO2_DrawRect( x - Width / 2 - ( 1 ), y - ( 1 ), WidthThird + 2, 3, black ); //h
	BO2_DrawRect( x - Width / 2 - ( 1 ), y - ( 1 ), 3, HeightThird + 2, black ); //v
 
	//topright
	BO2_DrawRect( ( x + Width / 2 ) - WidthThird - ( 1 ), y - ( 1 ), WidthThird + 2, 3, black ); //h
	BO2_DrawRect( x + Width / 2 - ( 1 ), y - ( 1 ), 3, HeightThird + 2, black ); //v
 
	//bottom left
	BO2_DrawRect( x - Width / 2 - ( 1 ), y + Height - 4, WidthThird + 2, 3, black ); //h
	BO2_DrawRect( x - Width / 2 - ( 1 ), ( y + Height ) - HeightThird - 4, 3, HeightThird + 2, black );//v
 
	//bottom right
	BO2_DrawRect( ( x + Width / 2 ) - WidthThird - ( 1 ), y + Height - 4, WidthThird + 2, 3, black );//h
	BO2_DrawRect( x + Width / 2 - ( 1 ), ( y + Height ) - HeightThird - 4, 3, HeightThird + 3, black ); //v
 
	/* draw eight lines */
	//topleft 
	BO2_DrawRect( x - Width / 2, y, WidthThird, 1, color ); //h
	BO2_DrawRect( x - Width / 2, y, 1, HeightThird, color ); //v
 
	//topright
	BO2_DrawRect( ( x + Width / 2 ) - WidthThird, y, WidthThird, 1, color ); //h
	BO2_DrawRect( x + Width / 2, y, 1, HeightThird, color ); //v
 
	//bottom left
	BO2_DrawRect( x - Width / 2, y + Height - 3, WidthThird, 1, color ); //h
	BO2_DrawRect( x - Width / 2, ( y + Height ) - HeightThird - 3, 1, HeightThird, color );//v
 
	//bottom right
	BO2_DrawRect( ( x + Width / 2 ) - WidthThird, y + Height - 3, WidthThird, 1, color );//h
	BO2_DrawRect( x + Width / 2, ( y + Height ) - HeightThird - 3, 1, HeightThird + 1, color ); //v			
}

VOID DrawBox2(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color)
{
	DrawRect(x, y, Width, Height, color);

	DrawRect(x, y - Height, Width, 1, black);//*TOP-LINE
	DrawRect(x, y, Width, 1, black);//BOTTOM
	DrawRect(x, y, 1, Height, black);//LEFT
	DrawRect(x + Width, y, 1, Height, black);//RIGHT
}
VOID GO_DrawBox2(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color)
{
	GO_DrawRect(x, y, Width, Height, color);

	GO_DrawRect(x, y - Height, Width, 1, black);//*TOP-LINE
	GO_DrawRect(x, y, Width, 1, black);//BOTTOM
	GO_DrawRect(x, y, 1, Height, black);//LEFT
	GO_DrawRect(x + Width, y, 1, Height, black);//RIGHT
}
VOID MW3_DrawBox2(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color)
{
	MW3_DrawRect(x, y, Width, Height, color);

	MW3_DrawRect(x, y - Height, Width, 1, black);//*TOP-LINE
	MW3_DrawRect(x, y, Width, 1, black);//BOTTOM
	MW3_DrawRect(x, y, 1, Height, black);//LEFT
	MW3_DrawRect(x + Width, y, 1, Height, black);//RIGHT
}
VOID BO1_DrawBox2(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color)
{
	BO1_DrawRect(x, y, Width, Height, color);

	BO1_DrawRect(x, y - Height, Width, 1, black);//*TOP-LINE
	BO1_DrawRect(x, y, Width, 1, black);//BOTTOM
	BO1_DrawRect(x, y, 1, Height, black);//LEFT
	BO1_DrawRect(x + Width, y, 1, Height, black);//RIGHT
}
VOID BO2_DrawBox2(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color)
{
	BO2_DrawRect(x, y, Width, Height, color);

	BO2_DrawRect(x, y - Height, Width, 1, black);//*TOP-LINE
	BO2_DrawRect(x, y, Width, 1, black);//BOTTOM
	BO2_DrawRect(x, y, 1, Height, black);//LEFT
	BO2_DrawRect(x + Width, y, 1, Height, black);//RIGHT
}
VOID MW2_DrawBox2(FLOAT x, FLOAT y, FLOAT Width, FLOAT Height, const FLOAT *color)
{
	MW2_DrawRect(x, y, Width, Height, color);

	MW2_DrawRect(x, y - Height, Width, 1, black);//*TOP-LINE
	MW2_DrawRect(x, y, Width, 1, black);//BOTTOM
	MW2_DrawRect(x, y, 1, Height, black);//LEFT
	MW2_DrawRect(x + Width, y, 1, Height, black);//RIGHT
}


VOID DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	DrawText(text, x - 0.85, y, xScale, blackM);
	DrawText(text, x + 0.85, y, xScale, blackM);
	DrawText(text, x, y - 0.85, xScale, blackM);
	DrawText(text, x, y + 0.85, xScale, blackM);

	DrawText(text, x, y, xScale, color);

}
VOID GO_DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	GO_DrawText(text, x - 1, y, xScale, black);
	GO_DrawText(text, x + 1, y, xScale, black);
	GO_DrawText(text, x, y - 1, xScale, black);
	GO_DrawText(text, x, y + 1, xScale, black);

	GO_DrawText(text, x, y, xScale, color);

}
VOID MW3_DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	MW3_DrawText(text, x - 1, y, xScale, black);
	MW3_DrawText(text, x + 1, y, xScale, black);
	MW3_DrawText(text, x, y - 1, xScale, black);
	MW3_DrawText(text, x, y + 1, xScale, black);

	MW3_DrawText(text, x, y, xScale, color);

}
VOID BO1_DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	BO1_DrawText(text, x - 1, y, xScale, black);
	BO1_DrawText(text, x + 1, y, xScale, black);
	BO1_DrawText(text, x, y - 1, xScale, black);
	BO1_DrawText(text, x, y + 1, xScale, black);

	BO1_DrawText(text, x, y, xScale, color);

}
VOID BO2_DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	BO2_DrawText(text, x - 1, y, xScale, black);
	BO2_DrawText(text, x + 1, y, xScale, black);
	BO2_DrawText(text, x, y - 1, xScale, black);
	BO2_DrawText(text, x, y + 1, xScale, black);

	BO2_DrawText(text, x, y, xScale, color);

}
VOID MW2_DrawShadowText(const char* text, FLOAT x, FLOAT y, FLOAT xScale, const FLOAT *color)
{
	MW2_DrawText(text, x - 1, y, xScale, black);
	MW2_DrawText(text, x + 1, y, xScale, black);
	MW2_DrawText(text, x, y - 1, xScale, black);
	MW2_DrawText(text, x, y + 1, xScale, black);

	MW2_DrawText(text, x, y, xScale, color);

}


VOID setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, blue2, 0);
}
VOID GO_setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	GO_R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, blue2, 0);
}
VOID MW3_setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	MW3_R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, blue2, 0);
}
VOID BO1_setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	BO1_R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, blue2, 0);
}
VOID BO2_setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	BO2_R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, blue2, 0);
}
VOID MW2_setText2(const char* text, FLOAT x, FLOAT y, FLOAT xScale, DWORD fontz)
{
	MW2_R_AddCmdDrawText(text, 0x7FFFFFFF, fontz, x, y, xScale, xScale, 0, blue2, 0);
}


VOID DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors)
{
	R_AddCmdDrawStretchPic(x, y, weight, Height, 0, 0, 1, 1, colors, White);
}
VOID GO_DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors)
{
	GO_R_AddCmdDrawStretchPic(x, y, weight, Height, 0, 0, 1, 1, colors, GO_White);
}
VOID MW3_DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors)
{
	MW3_R_AddCmdDrawStretchPic(x, y, weight, Height, 0, 0, 1, 1, colors, MW3_White);
}
VOID BO1_DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors)
{
	BO1_R_AddCmdDrawStretchPic(x, y, weight, Height, 0, 0, 1, 1, colors, BO1_White);
}
VOID BO2_DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors)
{
	BO2_R_AddCmdDrawStretchPic(x, y, weight, Height, 0, 0, 2, 2, colors, BO2_White);
}
VOID MW2_DrawRect(FLOAT x, FLOAT y, FLOAT weight, FLOAT Height, const FLOAT *colors)
{
	MW2_R_AddCmdDrawStretchPic(x, y, weight, Height, 0, 0, 1, 1, colors, MW2_White);
}


FLOAT GetDistance(D3DXVECTOR3 me, D3DXVECTOR3 enemi)
{
	FLOAT dx = me.x - enemi.x;
	FLOAT dy = me.y - enemi.y;
	FLOAT dz = me.z - enemi.z;
	return (FLOAT)sqrt((dx * dx) + (dy * dy) + (dz * dz))  * 0.03048f;
}

char *getWeaponname(short iD)
{
	int ids = (int)iD;
	switch (ids)
	{
	case 72: case 74: case 75: case 76: case 77: case 78: case 79: case 80: case 81: case 82: case 83:
		return "Bal-27";
	case 26: case 27: case 28: case 29: case 30: case 31: case 32: case 33: case 34: case 35: case 36: case 37:
		return "AK12";
	case 38: case 39: case 40: case 41: case 42: case 43: case 44: case 45: case 46: case 47: case 48:
		return "ARX-160";
	case 141: case 142: case 143: case 144: case 145: case 146: case 147: case 148: case 149: case 150: case 151:
		return "HBRa3";
	case 60: case 61: case 62: case 63: case 64: case 65: case 66: case 67: case 68: case 69: case 70: case 71:
		return "ASM1";
	case 152: case 153: case 154: case 155: case 156: case 157:case 158:case 159:case 160: case 161: case 162:
		return "IMR";
	case 199: case 200: case 201: case 202: case 203: case 204: case 205: case 206: case 207: case 208: case 209:
		return "MK14";
	case 177: case 178: case 179: case 180: case 181: case 182: case 183: case 184: case 185: case 186: case 187:
		return "KF5";
	case 259: case 260: case 261: case 262: case 263: case 264: case 265: case 266: case 267: case 268: case 269:
		return "MP11";
	case 321: case 322: case 323: case 324: case 325: case 326: case 327: case 328: case 329: case 330: case 331:
		return "SN6";
	case 310: case 311: case 312: case 313: case 314: case 315: case 316: case 317: case 318: case 319: case 320:
		return "SAC3";
	case 163: case 164: case 165: case 166: case 167: case 168: case 169: case 170: case 171: case 172: case 173:
		return "AMR9";
	case 129: case 130: case 131: case 132: case 133: case 134: case 135: case 136: case 137: case 138: case 139: case 140:
		return "Lynx";
	case 248: case 249: case 250: case 251: case 252: case 253: case 254: case 255: case 256: case 257: case 258:
		return "MORS";
	case 210: case 211: case 212: case 213:  case 214: case 215: case 216: case 217: case 218: case 219: case 220:
		return "NA-45";
	case 337: case 338: case 339: case 340: case 341: case 342: case 343: case 344: case 345: case 346: case 347:
		return "Atlas 20mm";
	case 360: case 361: case 362: case 363: case 364: case 365: case 366: case 367: case 368: case 369: case 370:
		return "Tac-19";
	case 281: case 282: case 283: case 284: case 285: case 286: case 287: case 288: case 289: case 290: case 291:
		return "S-12";
	case 231: case 232: case 233: case 234: case 235: case 236: case 237: case 238: case 239: case 240: case 241: case 242:
		return "Bulldog";
	case 87: case 88: case 89: case 90: case 91: case 92: case 93:  case 94: case 95: case 96: case 97: case 98:
		return "EM1";
	case 188: case 189: case 190: case 191: case 192: case 193: case 194: case 195: case 196: case 197: case 198:
		return "Pytaek";
	case 117: case 118: case 119: case 120: case 121: case 122: case 123: case 124: case 125: case 126: case 127: case 128:
		return "XMG";
	case 99: case 100: case 101: case 102: case 103: case 104: case 105: case 106: case 107: case 108: case 109:
		return "EPM3";
	case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57: case 58: case 59:
		return "Ameli";
	case 292: case 293: case 294: case 295: case 296: case 297: case 298:
		return "Heary Shield";
	case 243: case 244: case 245: case 246: case 247:
		return "MDL";
	case 348: case 349: case 350: case 351: case 352: case 353: case 354: case 355: case 356: case 357: case 358: case 359:
		return "Atlas 45";
	case 299: case 300: case 301: case 302: case 303: case 304: case 305: case 306: case 307: case 308: case 309:
		return "RW1";
	case 270: case 271: case 272: case 273: case 274: case 275: case 276: case 277: case 278: case 279: case 280:
		return "MP443 Grach";
	case 371: case 372: case 373: case 374: case 375: case 376: case 377: case 378: case 379: case 380: case 381:
		return "PDW";
	case 332: case 333: case 334: case 335: case 336:
		return "Stringer M7";
	case 221: case 222: case 223: case 224: case 225:
		return "MAAWS";
	case 226: case 227: case 228: case 229: case 230:
		return "MAHEM";
	case 110: case 111: case 112: case 113: case 114: case 115:
		return "Crossbow";
	case 85: case 86:
		return "Combat Knife";
	default:
		return "Unknow";
	}
}

char *GO_getWeaponname(BYTE iD)
{
	int ids = (int)iD;
	switch (ids)
	{
	case 41:
		return "AK-12";
	case 42:
		return "Ameli";
	case 43:
		return "ARX-160";
	case 44:
		return "SA-805";
	case 45:
		return "CBJ-MS";
	case 46:
		return "FAD";
	case 47:
		return "FP6";
	case 51:
		return "Honey Badger";
	case 53:
		return "K7";
	case 54:
		return "Chain SAW";
	case 55:
		return "Combat Knife";
	case 139:
		return "Gold Knife";
	case 57:
		return "Vector CRB";
	case 59:
		return "LSAT";
	case 60:
		return "M27";
	case 61:
		return "M9A1";
	case 62:
		return "Maaws";
	case 65:
		return ".44 Magnum";
	case 68:
		return "Bulldog";
	case 69:
		return "MTAR-X";
	case 70:
		return "Minigun";
	case 73:
		return "MK32";
	case 75:
		return "MP-443 Grach";
	case 76:
		return "MSBS";
	case 77:
		return "MTS-255";
	case 78:
		return "P226";
	case 80:
		return "Panzerfaust";
	case 81:
		return "PDW";
	case 82:
		return "Bizon";
	case 83:
		return "Remington R5";
	case 84:
		return "Kastet";
	case 85:
		return "Riot Shield";
	case 87:
		return "SC-2010";
	case 90:
		return "Tac 12";
	case 91:
		return "Vepr";
		//case 139: 
		//return "Gold PDW";
	case 48:
		return "MR-28";
	case 72:
		return "MK14 EBR";
	case 52:
		return "IA-2";
	case 88:
		return "SVU";
	case 89:
		return "USR";
	case 58:
		return "L115";
	case 49:
		return "Lynx";
	case 92:
		return "VKS";
	default:
		return "Unknow";
	}
}

char *MW3_getWeaponname(BYTE iD)
{
	int ids = (int)iD;
	switch (ids)
	{
	case 3:
		return "Riot Shield";
	case 4:
		return "Riot Shield Jugg";
	case 5:
		return ".44 Magnum";
	case 6:
		return "USP 45";
	case 7:
		return "USP 45 Jugg";
	case 8:
		return "Desert Eagle";
	case 9:
		return "MP412";
	case 10:
		return "MP412 Jugg";
	case 11:
		return "P99";
	case 12:
		return "Five Seven";
	case 13:
		return "FMG9";
	case 14:
		return "Skorpion";
	case 15:
		return "MP9";
	case 16:
		return "G18";
	case 17:
		return "MP5";
	case 18:
		return "PM-9";
	case 19:
		return "P90";
	case 20:
		return "PP90M1";
	case 21:
		return "UMP45";
	case 22:
		return "MP7";
	case 23:
		return "AK-47";
	case 24:
		return "M16A4";
	case 25:
		return "M4A1";
	case 26:
		return "FAD";
	case 27:
		return "ACR 6.8";
	case 28:
		return "Type 95";
	case 29:
		return "MK14";
	case 30:
		return "SCAR-L";
	case 31:
		return "G36C";
	case 32:
		return "CM-901";
	case 33:
		return "Unknow";
	case 34:
		return "M320 GLM";
	case 35:
		return "RPG-7";
	case 36:
		return "SMAW";
	case 37:
		return "Stinger";
	case 38:
		return "Javelin";
	case 39:
		return "XM25";
	case 40:
		return "Dragunov";
	case 41:
		return "MSR";
	case 42:
		return "Barrett .50";
	case 43:
		return "RSASS";
	case 44:
		return "AS50";
	case 45:
		return "L118A";
	case 46:
		return "KSG 12";
	case 47:
		return "Model 1887";
	case 48:
		return "striker";
	case 49:
		return "AA-12";
	case 50:
		return "USAS 12";
	case 51:
		return "SPAS-12";
	case 52:
		return "M60E4 Jugg";
	case 53:
		return "M60E4";
	case 54:
		return "MK46";
	case 55:
		return "PKP Pecheneg";
	case 56:
		return "L86 LSW";
	case 57:
		return "MG36";
	default:
		return "Unknow";
	}
}

char *BO2_getWeaponname(BYTE iD)
{
	int ids = (int)iD;
	switch (ids)
	{
	case 2:
		return "MP7";
	case 4:
		return "PDW-57";
	case 6:
		return "Vector K10";
	case 8:
		return "MSMC";
	case 10:
		return "Chicom CQB";
	case 12:
		return "Skorpion EVO";
	case 14:
		return "Peacekeeper";
	case 16:
		return "MTAR";
	case 20:
		return "Type 25";
	case 24:
		return "SWAT-556";
	case 28:
		return "FAL OSW";
	case 32:
		return "M27";
	case 36:
		return "SCAR-H";
	case 40:
		return "SMR";
	case 44:
		return "M8A1";
	case 48:
		return "AN-94";
	case 52:
		return "R870 MCS";
	case 53:
		return "S12";
	case 54:
		return "KSG";
	case 55:
		return "M1216";
	case 56:
		return "MK 48";
	case 58:
		return "QBB LSW";
	case 60:
		return "LSAT";
	case 62:
		return "HAMR";
	case 64:
		return "SVU-AS";
	case 65:
		return "DSR 50";
	case 66:
		return "Ballista";
	case 67:
		return "XPR50";
	case 68:
		return "KAP-40";
	case 70:
		return "Tac-45";
	case 72:
		return "Five Seven";
	case 74:
		return "Executor";
	case 76:
		return "B93R";
	case 78:
		return "Five Seven";
	case 79:
		return "Tac-45";
	case 80:
		return "B93R";
	case 81:
		return "Executor";
	case 82:
		return "Kard";
	case 83:
		return "M32";
	case 84:
		return "SMAW";
	case 85:
		return "FHJ-18 AA";
	case 86:
		return "RPG-7";
	case 87:
		return "Knife";
	case 88:
		return "Minigun";
	case 89:
		return "Riot Shield";
	case 90:
		return "Crossbow";
	case 91:
		return "Knife Ballist.";
	default:
		return "Unknow";
	}
}

char *BO1_getWeaponname(short iD)
{
	int ids = (int)iD;
	switch (ids)
	{
	case 34:
		return "SKORPION";
	case 1613:
		return "MAKAROV";
	case 46:
		return "MAC11";
	case 1604:
		return "ASP";
	case 179:
		return "UZI";
	case 329:
		return "KIPARIS";
	case 1638:
		return "RPG-7";
	case 1639:
		return "STRELA-3";
	case 1605:
		return "M1911";
	case 500:
		return "ENFIELD";
	case 1621:
		return "Python";
	case 1625:
		return "CZ75";
	case 623:
		return "M14";
	case 377:
		return "M16";
	case 1466:
		return "G11";
	case 726:
		return "Famas";
	case 82:
		return "AK74";
	case 10:
		return "MP5K";
	case 233:
		return "MPL";
	case 227:
		return "PM63";
	case 281:
		return "Spectre";
	case 1596:
		return "CZ75";
	case 1597:
		return "CZ75";
	case 1470:
		return "STAKEOUT";
	case 1469:
		return "OLYMPIA";
	case 1472:
		return "SPAS-12";
	case 1500:
		return "M60";
	case 1520:
		return "STONER63";
	case 1546:
		return "WA2000";
	case 1578:
		return "PSG1";
	case 1474:
		return "HS10";
	case 972:
		return "AUG";
	case 849:
		return "Galil";
	case 1341:
		return "Commando";
	case 1095:
		return "FN FAL";
	case 1530:
		return "Dragunov";
	case 1562:
		return "L96A1";
	case 1485:
		return "RPK";
	case 1475:
		return "HK21";
	case 1637:
		return "M72 LAW";
	case 1640:
		return "China Lake";
	case 1642:
		return "Crossbow";
	case 1641:
		return "Knife Ballistic";
	case 1702:
		return "Minigun";
	default:
		return "Unknow";
	}
}

PlayerNames *getClientbyIDx(int id)
{
	DWORD ptr = ((DWORD)RefDef - 0x2568);

	return (PlayerNames*)(ptr + ((int)0x12C * id));
}

GO_PlayerNames *GO_getClientbyIDx(int id)
{
	DWORD ptr = ((DWORD)GO_RefDef - 0x1A5B);

	return (GO_PlayerNames*)(ptr + ((int)0xF0 * id));
}

clientInfo_t *ClientbyIDx(int id)
{
	DWORD ptr = ((DWORD)RefDef + 0x68DA0);

	return (clientInfo_t*)(ptr + ((int)0xC34 * id));
}

GO_clientInfo_t *GO_ClientbyIDx(int id)
{
	DWORD ptr = ((DWORD)GO_RefDef + 0x4BEF4);

	return (GO_clientInfo_t*)(ptr + ((int)0x59C * id));
}

MW3_ClientInfo *MW3_getClientbyIDx(int id)
{
	DWORD MW3_CclientInfo = ((DWORD)MW3_RefDef + 0x924DC);

	return (MW3_ClientInfo*)(MW3_CclientInfo + ((int)0x594 * id));
}

BO2_ClientInfo *BO2_getClientbyIDx(int id)
{
	DWORD BO2_CclientInfo = ((DWORD)BO2_RefDef + 0x1C210);

	return (BO2_ClientInfo*)(BO2_CclientInfo + ((int)0x808 * id));
}

BO1_ClientInfo *BO1_getClientbyIDx(int id)
{
	DWORD BO1_CclientInfo = ((DWORD)BO1_RefDef + 0x1C0F8);

	return (BO1_ClientInfo*)(BO1_CclientInfo + ((int)0x5D0 * id));
}

MW2_ClientInfo *MW2_getClientbyIDx(int id)
{
	DWORD MW2_CclientInfo = ((DWORD)MW2_RefDef + 0x8C258);

	return (MW2_ClientInfo*)(MW2_CclientInfo + ((int)0x544 * id));
}

CG_t *getCg()
{
	DWORD cg_ptr = *(DWORD*)cg_t;

	return (CG_t*)cg_ptr;
}

CGS *getCgs()
{
	DWORD cgs_ptr = *(DWORD*)cgs_t;

	return (CGS*)cgs_ptr;
}

MW3_CGS *MW3_getCgs()
{
	DWORD cgs_ptr = *(DWORD*)MW3_cgs_t;

	return (MW3_CGS*)cgs_ptr;
}

GO_CGS *GO_getCgs()
{
	DWORD cgs_ptr = *(DWORD*)GO_cgs_t;

	return (GO_CGS*)cgs_ptr;
}
BO2_CGS *BO2_getCgs()
{
	DWORD cgs_ptr = *(DWORD*)BO2_cgs_t;

	return (BO2_CGS*)cgs_ptr;
}
BO1_CGS *BO1_getCgs()
{
	DWORD cgs_ptr = *(DWORD*)BO1_cgs_t;

	return (BO1_CGS*)cgs_ptr;
}

MW2_CGS *MW2_getCgs()
{
	DWORD cgs_ptr = *(DWORD*)MW2_cgs_t;

	return (MW2_CGS*)cgs_ptr;
}

GO_CG_t *GO_getCg()
{
	DWORD cg_ptr = *(DWORD*)GO_cg_t;

	return (GO_CG_t*)cg_ptr;
}

MW3_CG_t *MW3_getCg()
{
	DWORD cg_ptr = *(DWORD*)MW3_cg_t;

	return (MW3_CG_t*)cg_ptr;
}

BO2_CG_t *BO2_getCg()
{
	DWORD cg_ptr = *(DWORD*)BO2_cg_t;

	return (BO2_CG_t*)cg_ptr;
}

BO1_CG_t *BO1_getCg()
{
	DWORD cg_ptr = *(DWORD*)BO1_cg_t;

	return (BO1_CG_t*)cg_ptr;
}

MW2_CG_t *MW2_getCg()
{
	DWORD cg_ptr = *(DWORD*)MW2_cg_t;

	return (MW2_CG_t*)cg_ptr;
}

clientActive *getCA()
{
	DWORD ca_ptr = *(DWORD*)clientActive_t;

	return (clientActive*)ca_ptr;
}

GO_clientActive *GO_getCA()
{
	DWORD ca_ptr = *(DWORD*)GO_clientActive_t;

	return (GO_clientActive*)ca_ptr;
}

MW3_clientActive *MW3_getCA()
{
	DWORD ca_ptr = *(DWORD*)MW3_clientActive_t;

	return (MW3_clientActive*)ca_ptr;
}


BO2_clientActive *BO2_getCA()
{
	DWORD ca_ptr = *(DWORD*)BO2_clientActive_t;

	return (BO2_clientActive*)ca_ptr;
}

BO1_clientActive *BO1_getCA()
{
	DWORD ca_ptr = *(DWORD*)BO1_clientActive_t;

	return (BO1_clientActive*)ca_ptr;
}

MW2_clientActive *MW2_getCA()
{
	DWORD ca_ptr = *(DWORD*)MW2_clientActive_t;

	return (MW2_clientActive*)ca_ptr;
}

cEntity * GetEntitybyIdx(int x)
{
	DWORD cent_ptr = *(DWORD*)centity_t;

	return (cEntity*)(cent_ptr + ((int)0x20C * x));

}

GO_cEntity * GO_GetEntitybyIdx(int x)
{
	DWORD cent_ptr = *(DWORD*)GO_centity_t;

	return (GO_cEntity*)(cent_ptr + ((int)0x1EC * x));

}

MW3_cEntity * MW3_GetEntitybyIdx(int x)
{
	DWORD cent_ptr = *(DWORD*)MW3_centity_t;

	return (MW3_cEntity*)(cent_ptr + ((int)0x1EC * x));

}

BO2_cEntity * BO2_GetEntitybyIdx(int x)
{
	DWORD cent_ptr = *(DWORD*)BO2_centity_t;

	return (BO2_cEntity*)(cent_ptr + ((int)0x374 * x));

}

BO1_cEntity * BO1_GetEntitybyIdx(int x)
{
	DWORD cent_ptr = *(DWORD*)BO1_centity_t;

	return (BO1_cEntity*)(cent_ptr + ((int)0x31C * x));

}

MW2_cEntity * MW2_GetEntitybyIdx(int x)
{
	DWORD cent_ptr = *(DWORD*)MW2_centity_t;

	return (MW2_cEntity*)(cent_ptr + ((int)0x1F8 * x));

}

VOID ReadGame()
{
	DWORD ref_ptr = *(DWORD*)refdef_s;
	RefDef = (cRefdef*)ref_ptr;
	cg = (CG_t*)getCg();
	cgs = (CGS*)getCgs();

	DWORD cent_ptr = *(DWORD*)centity_t;

	for (int i = 0; i < ENTITIESMAX; i++)
		Entities[i] = (cEntity*)(cent_ptr + ((int)0x20C * i));

	for (int i = 0; i < ENTITIESMAX; i++)
		Clients[i] = (PlayerNames*)getClientbyIDx(i);

	for (int i = 0; i < ENTITIESMAX; i++)
		ci[i] = (clientInfo_t*)ClientbyIDx(i);

	for (int i = 0; i < ENTITIESMAX; i++)
	{
		if (cg->cNum == i) {
			MyTeam = Clients[i]->mTeam;
			break;

		}
	}

	for (int i = 0; i < ENTITIESMAX; i++)
	{
		if (cg->cNum == i) {
			MyTeam = ci[i]->mTeam;
			break;

		}
	}
}

VOID GO_ReadGame()
{
	DWORD ref_ptr = *(DWORD*)GO_refdef_s;
	GO_RefDef = (GO_cRefdef*)ref_ptr;
	GO_cg = (GO_CG_t*)GO_getCg();
	GO_cgs = (GO_CGS*)GO_getCgs();

	DWORD cent_ptr = *(DWORD*)GO_centity_t;

	for (int i = 0; i < ENTITIESMAX; i++)
		GO_Entities[i] = (GO_cEntity*)(cent_ptr + ((int)0x1EC * i));

	for (int i = 0; i < ENTITIESMAX; i++)
		GO_Clients[i] = (GO_PlayerNames*)GO_getClientbyIDx(i);

	for (int i = 0; i < ENTITIESMAX; i++)
		GO_ci[i] = (GO_clientInfo_t*)GO_ClientbyIDx(i);

	for (int i = 0; i < ENTITIESMAX; i++)
	{
		if (GO_cg->cNum == i) {
			MyTeam = GO_Clients[i]->mTeam;
			break;

		}
	}

	for (int i = 0; i < ENTITIESMAX; i++)
	{
		if (GO_cg->cNum == i) {
			MyTeam = GO_ci[i]->mTeam;
			break;

		}
	}
}

VOID MW3_ReadGame()
{
	DWORD ref_ptr = *(DWORD*)MW3_refdef_s;
	MW3_RefDef = (MW3_cRefdef*)ref_ptr;
	MW3_cg = (MW3_CG_t*)MW3_getCg();
	MW3_cgs = (MW3_CGS*)MW3_getCgs();

	DWORD cent_ptr = *(DWORD*)MW3_centity_t;

	for (int i = 0; i < 18; i++)
		MW3_Entities[i] = (MW3_cEntity*)(cent_ptr + ((int)0x1EC * i));

	for (int i = 0; i < 18; i++)
		MW3_Clients[i] = (MW3_ClientInfo*)MW3_getClientbyIDx(i);

	for (int i = 0; i < 18; i++)
	{
		if (MW3_cg->cNum == i)
		{
			MyTeam = MW3_Clients[i]->mTeam;
			break;
		}
	}
}

VOID BO2_ReadGame()
{
	BO2_RefDef = (BO2_cRefdef*)BO2_RefDefCalc;
	BO2_cg = (BO2_CG_t*)BO2_getCg();
	BO2_cgs = (BO2_CGS*)BO2_getCgs();
	DWORD cent_ptr = *(DWORD*)BO2_centity_t;

	for (int i = 0; i < ENTITIESMAX; i++)
		BO2_Entities[i] = (BO2_cEntity*)(cent_ptr + ((int)0x374 * i));

	for (int i = 0; i < ENTITIESMAX; i++)
		BO2_Clients[i] = (BO2_ClientInfo*)BO2_getClientbyIDx(i);

	for (int i = 0; i < ENTITIESMAX; i++)
	{
		if (BO2_cg->cNum == i)
		{
			MyTeam = BO2_Clients[i]->mTeam;
			break;
		}
	}
}

VOID BO1_ReadGame()
{
	BO1_RefDef = (BO1_cRefdef*)BO1_RefDefCalc;
	BO1_cg = (BO1_CG_t*)BO1_getCg();
	BO1_cgs = (BO1_CGS*)BO1_getCgs();
	DWORD cent_ptr = *(DWORD*)BO1_centity_t;

	for (int i = 0; i < ENTITIESMAX; i++)
		BO1_Entities[i] = (BO1_cEntity*)(cent_ptr + ((int)0x31C * i));

	for (int i = 0; i < ENTITIESMAX; i++)
		BO1_Clients[i] = (BO1_ClientInfo*)BO1_getClientbyIDx(i);

	for (int i = 0; i < ENTITIESMAX; i++)
	{
		if (BO1_cg->cNum == i)
		{
			MyTeam = BO1_Clients[i]->mTeam;
			break;
		}
	}
}

VOID MW2_ReadGame()
{
	DWORD refdef_ptr = *(DWORD*)MW2_refdef_s;
	MW2_RefDef = (MW2_cRefdef*)(refdef_ptr - 0x98D0);
	MW2_cg = (MW2_CG_t*)MW2_getCg();
	MW2_cgs = (MW2_CGS*)MW2_getCgs();
	DWORD cent_ptr = *(DWORD*)MW2_centity_t;

	for (int i = 0; i < ENTITIESMAX; i++)
		MW2_Entities[i] = (MW2_cEntity*)(cent_ptr + ((int)0x1F8 * i));

	for (int i = 0; i < ENTITIESMAX; i++)
		MW2_Clients[i] = (MW2_ClientInfo*)MW2_getClientbyIDx(i);

	for (int i = 0; i < ENTITIESMAX; i++)
	{
		if (MW2_cg->cNum == i)
		{
			MyTeam = MW2_Clients[i]->mTeam;
			break;
		}
	}
}

BOOL wS2(FLOAT *flLocation, FLOAT *flOut)
{

	if (!bInit)
	{
		DWORD LEPD = *(DWORD*)refdef_s;
		RefDef = (cRefdef*)LEPD;

		bInit = TRUE;
	}

	Vector vLocal;

	VectorSubtract(flLocation, RefDef->EyePos, vLocal);
	FLOAT flZ = DotProduct(RefDef->ViewAxis[0], vLocal);
	FLOAT flX = DotProduct(RefDef->ViewAxis[1], vLocal);
	FLOAT flY = DotProduct(RefDef->ViewAxis[2], vLocal);

	if (flZ >= 0.0f)
	{
		FLOAT flTmp = 1.0f / flZ;

		flOut[0] = (1.0f - (flX / RefDef->fov_X * flTmp)) * ((FLOAT)1280 / 2);
		flOut[1] = (1.0f - (flY / RefDef->fov_Y * flTmp)) * ((FLOAT)720 / 2);
		return TRUE;
	}
	return FALSE;
}

BOOL GO_wS2(FLOAT *flLocation, FLOAT *flOut)
{

	if (!bInit)
	{
		DWORD LEPD = *(DWORD*)GO_refdef_s;
		GO_RefDef = (GO_cRefdef*)LEPD;

		bInit = TRUE;
	}

	Vector vLocal;

	VectorSubtract(flLocation, GO_RefDef->EyePos, vLocal);
	FLOAT flZ = DotProduct(GO_RefDef->ViewAxis[0], vLocal);
	FLOAT flX = DotProduct(GO_RefDef->ViewAxis[1], vLocal);
	FLOAT flY = DotProduct(GO_RefDef->ViewAxis[2], vLocal);

	if (flZ >= 0.0f)
	{
		FLOAT flTmp = 1.0f / flZ;

		flOut[0] = (1.0f - (flX / GO_RefDef->fov_X * flTmp)) * ((FLOAT)1280 / 2);
		flOut[1] = (1.0f - (flY / GO_RefDef->fov_Y * flTmp)) * ((FLOAT)720 / 2);
		return TRUE;
	}
	return FALSE;
}

BOOL MW3_wS2(FLOAT *flLocation, FLOAT *flOut)
{

	if (!bInit)
	{
		DWORD LEPD = *(DWORD*)MW3_refdef_s;
		MW3_RefDef = (MW3_cRefdef*)LEPD;

		bInit = TRUE;
	}

	Vector vLocal;

	VectorSubtract(flLocation, MW3_RefDef->EyePos, vLocal);
	FLOAT flZ = DotProduct(MW3_RefDef->ViewAxis[0], vLocal);
	FLOAT flX = DotProduct(MW3_RefDef->ViewAxis[1], vLocal);
	FLOAT flY = DotProduct(MW3_RefDef->ViewAxis[2], vLocal);

	if (flZ >= 0.0f)
	{
		FLOAT flTmp = 1.0f / flZ;

		flOut[0] = (1.0f - (flX / MW3_RefDef->fov_X * flTmp)) * ((FLOAT)1280 / 2);
		flOut[1] = (1.0f - (flY / MW3_RefDef->fov_Y * flTmp)) * ((FLOAT)720 / 2);
		return TRUE;
	}
	return FALSE;
}

BOOL BO2_wS2(FLOAT *flLocation, FLOAT *flOut)
{

	if (!bInit)
	{
		BO2_RefDef = (BO2_cRefdef*)BO2_RefDefCalc;

		bInit = TRUE;
	}

	Vector vLocal;

	VectorSubtract(flLocation, BO2_RefDef->EyePos, vLocal);
	FLOAT flZ = DotProduct(BO2_RefDef->ViewAxis[0], vLocal);
	FLOAT flX = DotProduct(BO2_RefDef->ViewAxis[1], vLocal);
	FLOAT flY = DotProduct(BO2_RefDef->ViewAxis[2], vLocal);

	if (flZ >= 0.0f)
	{
		FLOAT flTmp = 1.0f / flZ;

		flOut[0] = (1.0f - (flX / BO2_RefDef->fov_X * flTmp)) * ((FLOAT)1280 / 2);
		flOut[1] = (1.0f - (flY / BO2_RefDef->fov_Y * flTmp)) * ((FLOAT)720 / 2);
		return TRUE;
	}
	return FALSE;
}

BOOL BO1_wS2(FLOAT *flLocation, FLOAT *flOut)
{

	if (!bInit)
	{
		BO1_RefDef = (BO1_cRefdef*)BO1_RefDefCalc;

		bInit = TRUE;
	}

	Vector vLocal;

	VectorSubtract(flLocation, BO1_RefDef->EyePos, vLocal);
	FLOAT flZ = DotProduct(BO1_RefDef->ViewAxis[0], vLocal);
	FLOAT flX = DotProduct(BO1_RefDef->ViewAxis[1], vLocal);
	FLOAT flY = DotProduct(BO1_RefDef->ViewAxis[2], vLocal);

	if (flZ >= 0.0f)
	{
		FLOAT flTmp = 1.0f / flZ;

		flOut[0] = (1.0f - (flX / BO1_RefDef->fov_X * flTmp)) * ((FLOAT)1280 / 2);
		flOut[1] = (1.0f - (flY / BO1_RefDef->fov_Y * flTmp)) * ((FLOAT)720 / 2);
		return TRUE;
	}
	return FALSE;
}

BOOL MW2_wS2(FLOAT *flLocation, FLOAT *flOut)
{

	if (!bInit)
	{
		DWORD ptr = *(DWORD*)MW2_refdef_s;
		MW2_RefDef = (MW2_cRefdef*)(ptr - 0x98D0);

		bInit = TRUE;
	}

	Vector vLocal;

	VectorSubtract(flLocation, MW2_RefDef->EyePos, vLocal);
	FLOAT flZ = DotProduct(MW2_RefDef->ViewAxis[0], vLocal);
	FLOAT flX = DotProduct(MW2_RefDef->ViewAxis[1], vLocal);
	FLOAT flY = DotProduct(MW2_RefDef->ViewAxis[2], vLocal);

	if (flZ >= 0.0f)
	{
		FLOAT flTmp = 1.0f / flZ;

		flOut[0] = (1.0f - (flX / MW2_RefDef->fov_X * flTmp)) * ((FLOAT)1280 / 2);
		flOut[1] = (1.0f - (flY / MW2_RefDef->fov_Y * flTmp)) * ((FLOAT)720 / 2);
		return TRUE;
	}
	return FALSE;
}

VOID DrawLine2(int x1, int y1, int x2, int y2, const FLOAT* color, int shader) {
	DWORD ptr = DrawRotated_R3;

	FLOAT x, y, angle, l1, l2, h1;
	h1 = y2 - y1;
	l1 = x2 - x1;
	l2 = sqrt(l1 * l1 + h1 * h1);
	x = x1 + ((l1 - l2) / 2);
	y = y1 + (h1 / 2);
	angle = atan(h1 / l1)* (180 / 3.14159265358979323846);
	DrawRotated(ptr, x, y, l2, 2, angle, color, shader);
}
VOID GO_DrawLine2(int x1, int y1, int x2, int y2, const FLOAT* color, int shader) {
	DWORD ptr = GO_DrawRotated_R3;

	FLOAT x, y, angle, l1, l2, h1;
	h1 = y2 - y1;
	l1 = x2 - x1;
	l2 = sqrt(l1 * l1 + h1 * h1);
	x = x1 + ((l1 - l2) / 2);
	y = y1 + (h1 / 2);
	angle = atan(h1 / l1)* (180 / 3.14159265358979323846);
	GO_DrawRotated(ptr, x, y, l2, 2, angle, color, shader);
}
VOID MW3_DrawLine2(int x1, int y1, int x2, int y2, const FLOAT* color, int shader) {
	DWORD ptr = MW3_DrawRotated_R3;

	FLOAT x, y, angle, l1, l2, h1;
	h1 = y2 - y1;
	l1 = x2 - x1;
	l2 = sqrt(l1 * l1 + h1 * h1);
	x = x1 + ((l1 - l2) / 2);
	y = y1 + (h1 / 2);
	angle = atan(h1 / l1)* (180 / 3.14159265358979323846);
	MW3_DrawRotated(ptr, x, y, l2, 2, angle, color, shader);
}
VOID BO1_DrawLine2(int x1, int y1, int x2, int y2, const FLOAT* color, int shader) {
	DWORD ptr = BO1_DrawRotated_R3;

	FLOAT x, y, angle, l1, l2, h1;
	h1 = y2 - y1;
	l1 = x2 - x1;
	l2 = sqrt(l1 * l1 + h1 * h1);
	x = x1 + ((l1 - l2) / 2);
	y = y1 + (h1 / 2);
	angle = atan(h1 / l1)* (180 / 3.14159265358979323846);
	BO1_DrawRotated(ptr, x, y, l2, 2, angle, color, shader);
}
VOID BO2_DrawLine2(int x1, int y1, int x2, int y2, const FLOAT* color, int shader) {
	DWORD ptr = BO2_DrawRotated_R3;

	FLOAT x, y, angle, l1, l2, h1;
	h1 = y2 - y1;
	l1 = x2 - x1;
	l2 = sqrt(l1 * l1 + h1 * h1);
	x = x1 + ((l1 - l2) / 2);
	y = y1 + (h1 / 2);
	angle = atan(h1 / l1)* (180 / 3.14159265358979323846);
	BO2_DrawRotated(ptr, x, y, l2, 2, angle, color, shader);
}
VOID MW2_DrawLine2(int x1, int y1, int x2, int y2, const FLOAT* color, int shader) {
	DWORD ptr = MW2_DrawRotated_R3;

	FLOAT x, y, angle, l1, l2, h1;
	h1 = y2 - y1;
	l1 = x2 - x1;
	l2 = sqrt(l1 * l1 + h1 * h1);
	x = x1 + ((l1 - l2) / 2);
	y = y1 + (h1 / 2);
	angle = atan(h1 / l1)* (180 / 3.14159265358979323846);
	MW2_DrawRotated(ptr, x, y, l2, 2, angle, color, shader);
}

VOID MakeESPLine(FLOAT center[3], FLOAT x1, FLOAT y1, FLOAT z1, FLOAT x2, FLOAT y2, FLOAT z2, const FLOAT * color)
{
	FLOAT pointPos1[3] = { center[0] + x1, center[1] + y1, center[2] + z1 };
	FLOAT pointPos2[3] = { center[0] + x2, center[1] + y2, center[2] + z2 };
	FLOAT xy1[2], xy2[2];

	if (wS2(pointPos1, xy1) && wS2(pointPos2, xy2))
		DrawLine2(xy1[0], xy1[1], xy2[0], xy2[1], color, White);
}
VOID GO_MakeESPLine(FLOAT center[3], FLOAT x1, FLOAT y1, FLOAT z1, FLOAT x2, FLOAT y2, FLOAT z2, const FLOAT * color)
{
	FLOAT pointPos1[3] = { center[0] + x1, center[1] + y1, center[2] + z1 };
	FLOAT pointPos2[3] = { center[0] + x2, center[1] + y2, center[2] + z2 };
	FLOAT xy1[2], xy2[2];

	if (GO_wS2(pointPos1, xy1) && GO_wS2(pointPos2, xy2))
		GO_DrawLine2(xy1[0], xy1[1], xy2[0], xy2[1], color, GO_White);
}
VOID MW3_MakeESPLine(FLOAT center[3], FLOAT x1, FLOAT y1, FLOAT z1, FLOAT x2, FLOAT y2, FLOAT z2, const FLOAT * color)
{
	FLOAT pointPos1[3] = { center[0] + x1, center[1] + y1, center[2] + z1 };
	FLOAT pointPos2[3] = { center[0] + x2, center[1] + y2, center[2] + z2 };
	FLOAT xy1[2], xy2[2];

	if (MW3_wS2(pointPos1, xy1) && MW3_wS2(pointPos2, xy2))
		MW3_DrawLine2(xy1[0], xy1[1], xy2[0], xy2[1], color, MW3_White);
}
VOID BO1_MakeESPLine(FLOAT center[3], FLOAT x1, FLOAT y1, FLOAT z1, FLOAT x2, FLOAT y2, FLOAT z2, const FLOAT * color)
{
	FLOAT pointPos1[3] = { center[0] + x1, center[1] + y1, center[2] + z1 };
	FLOAT pointPos2[3] = { center[0] + x2, center[1] + y2, center[2] + z2 };
	FLOAT xy1[2], xy2[2];

	if (BO1_wS2(pointPos1, xy1) && BO1_wS2(pointPos2, xy2))
		BO1_DrawLine2(xy1[0], xy1[1], xy2[0], xy2[1], color, BO1_White);
}
VOID BO2_MakeESPLine(FLOAT center[3], FLOAT x1, FLOAT y1, FLOAT z1, FLOAT x2, FLOAT y2, FLOAT z2, const FLOAT * color)
{
	FLOAT pointPos1[3] = { center[0] + x1, center[1] + y1, center[2] + z1 };
	FLOAT pointPos2[3] = { center[0] + x2, center[1] + y2, center[2] + z2 };
	FLOAT xy1[2], xy2[2];

	if (BO2_wS2(pointPos1, xy1) && BO2_wS2(pointPos2, xy2))
		BO2_DrawLine2(xy1[0], xy1[1], xy2[0], xy2[1], color, BO2_White);
}
VOID MW2_MakeESPLine(FLOAT center[3], FLOAT x1, FLOAT y1, FLOAT z1, FLOAT x2, FLOAT y2, FLOAT z2, const FLOAT * color)
{
	FLOAT pointPos1[3] = { center[0] + x1, center[1] + y1, center[2] + z1 };
	FLOAT pointPos2[3] = { center[0] + x2, center[1] + y2, center[2] + z2 };
	FLOAT xy1[2], xy2[2];

	if (MW2_wS2(pointPos1, xy1) && MW2_wS2(pointPos2, xy2))
		MW2_DrawLine2(xy1[0], xy1[1], xy2[0], xy2[1], color, MW2_White);
}

VOID Make3DBox(FLOAT center[3], FLOAT w, FLOAT h, const FLOAT * color)
{
	//bottom
	MakeESPLine(center, -w, -w, 0, w, -w, 0, color);
	MakeESPLine(center, -w, -w, 0, -w, w, 0, color);
	MakeESPLine(center, w, w, 0, w, -w, 0, color);
	MakeESPLine(center, w, w, 0, -w, w, 0, color);

	//middle
	MakeESPLine(center, -w, -w, 0, -w, -w, h, color);
	MakeESPLine(center, -w, w, 0, -w, w, h, color);
	MakeESPLine(center, w, -w, 0, w, -w, h, color);
	MakeESPLine(center, w, w, 0, w, w, h, color);

	//top
	MakeESPLine(center, -w, -w, h, w, -w, h, color);
	MakeESPLine(center, -w, -w, h, -w, w, h, color);
	MakeESPLine(center, w, w, h, w, -w, h, color);
	MakeESPLine(center, w, w, h, -w, w, h, color);
}
VOID GO_Make3DBox(GO_cEntity *pEntity, FLOAT w, FLOAT h, const FLOAT * color)
{
	//bottom
	GO_MakeESPLine(pEntity->mPos, -w, -w, 0, w, -w, 0, color);
	GO_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, w, 0, color);
	GO_MakeESPLine(pEntity->mPos, w, w, 0, w, -w, 0, color);
	GO_MakeESPLine(pEntity->mPos, w, w, 0, -w, w, 0, color);

	//middle
	GO_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, -w, h, color);
	GO_MakeESPLine(pEntity->mPos, -w, w, 0, -w, w, h, color);
	GO_MakeESPLine(pEntity->mPos, w, -w, 0, w, -w, h, color);
	GO_MakeESPLine(pEntity->mPos, w, w, 0, w, w, h, color);

	//top
	GO_MakeESPLine(pEntity->mPos, -w, -w, h, w, -w, h, color);
	GO_MakeESPLine(pEntity->mPos, -w, -w, h, -w, w, h, color);
	GO_MakeESPLine(pEntity->mPos, w, w, h, w, -w, h, color);
	GO_MakeESPLine(pEntity->mPos, w, w, h, -w, w, h, color);
}
VOID MW3_Make3DBox(MW3_cEntity *pEntity, FLOAT w, FLOAT h, const FLOAT * color)
{
	//bottom
	MW3_MakeESPLine(pEntity->mPos, -w, -w, 0, w, -w, 0, color);
	MW3_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, w, 0, color);
	MW3_MakeESPLine(pEntity->mPos, w, w, 0, w, -w, 0, color);
	MW3_MakeESPLine(pEntity->mPos, w, w, 0, -w, w, 0, color);

	//middle
	MW3_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, -w, h, color);
	MW3_MakeESPLine(pEntity->mPos, -w, w, 0, -w, w, h, color);
	MW3_MakeESPLine(pEntity->mPos, w, -w, 0, w, -w, h, color);
	MW3_MakeESPLine(pEntity->mPos, w, w, 0, w, w, h, color);

	//top
	MW3_MakeESPLine(pEntity->mPos, -w, -w, h, w, -w, h, color);
	MW3_MakeESPLine(pEntity->mPos, -w, -w, h, -w, w, h, color);
	MW3_MakeESPLine(pEntity->mPos, w, w, h, w, -w, h, color);
	MW3_MakeESPLine(pEntity->mPos, w, w, h, -w, w, h, color);
}
VOID BO1_Make3DBox(BO1_cEntity *pEntity, FLOAT w, FLOAT h, const FLOAT * color)
{
	//bottom
	BO1_MakeESPLine(pEntity->mPos, -w, -w, 0, w, -w, 0, color);
	BO1_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, w, 0, color);
	BO1_MakeESPLine(pEntity->mPos, w, w, 0, w, -w, 0, color);
	BO1_MakeESPLine(pEntity->mPos, w, w, 0, -w, w, 0, color);

	//middle
	BO1_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, -w, h, color);
	BO1_MakeESPLine(pEntity->mPos, -w, w, 0, -w, w, h, color);
	BO1_MakeESPLine(pEntity->mPos, w, -w, 0, w, -w, h, color);
	BO1_MakeESPLine(pEntity->mPos, w, w, 0, w, w, h, color);

	//top
	BO1_MakeESPLine(pEntity->mPos, -w, -w, h, w, -w, h, color);
	BO1_MakeESPLine(pEntity->mPos, -w, -w, h, -w, w, h, color);
	BO1_MakeESPLine(pEntity->mPos, w, w, h, w, -w, h, color);
	BO1_MakeESPLine(pEntity->mPos, w, w, h, -w, w, h, color);
}
VOID BO2_Make3DBox(BO2_cEntity *pEntity, FLOAT w, FLOAT h, const FLOAT * color)
{
	//bottom
	BO2_MakeESPLine(pEntity->mPos, -w, -w, 0, w, -w, 0, color);
	BO2_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, w, 0, color);
	BO2_MakeESPLine(pEntity->mPos, w, w, 0, w, -w, 0, color);
	BO2_MakeESPLine(pEntity->mPos, w, w, 0, -w, w, 0, color);

	//middle
	BO2_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, -w, h, color);
	BO2_MakeESPLine(pEntity->mPos, -w, w, 0, -w, w, h, color);
	BO2_MakeESPLine(pEntity->mPos, w, -w, 0, w, -w, h, color);
	BO2_MakeESPLine(pEntity->mPos, w, w, 0, w, w, h, color);

	//top
	BO2_MakeESPLine(pEntity->mPos, -w, -w, h, w, -w, h, color);
	BO2_MakeESPLine(pEntity->mPos, -w, -w, h, -w, w, h, color);
	BO2_MakeESPLine(pEntity->mPos, w, w, h, w, -w, h, color);
	BO2_MakeESPLine(pEntity->mPos, w, w, h, -w, w, h, color);
}
VOID MW2_Make3DBox(MW2_cEntity *pEntity, FLOAT w, FLOAT h, const FLOAT * color)
{
	//bottom
	MW2_MakeESPLine(pEntity->mPos, -w, -w, 0, w, -w, 0, color);
	MW2_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, w, 0, color);
	MW2_MakeESPLine(pEntity->mPos, w, w, 0, w, -w, 0, color);
	MW2_MakeESPLine(pEntity->mPos, w, w, 0, -w, w, 0, color);

	//middle
	MW2_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, -w, h, color);
	MW2_MakeESPLine(pEntity->mPos, -w, w, 0, -w, w, h, color);
	MW2_MakeESPLine(pEntity->mPos, w, -w, 0, w, -w, h, color);
	MW2_MakeESPLine(pEntity->mPos, w, w, 0, w, w, h, color);

	//top
	MW2_MakeESPLine(pEntity->mPos, -w, -w, h, w, -w, h, color);
	MW2_MakeESPLine(pEntity->mPos, -w, -w, h, -w, w, h, color);
	MW2_MakeESPLine(pEntity->mPos, w, w, h, w, -w, h, color);
	MW2_MakeESPLine(pEntity->mPos, w, w, h, -w, w, h, color);
}

VOID GO_MakePyramid(GO_cEntity *pEntity, FLOAT w, FLOAT h, const FLOAT * color)
{
	GO_MakeESPLine(pEntity->mPos, -w, -w, 0, w, -w, 0, color);
	GO_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, w, 0, color);
	GO_MakeESPLine(pEntity->mPos, w, w, 0, w, -w, 0, color);
	GO_MakeESPLine(pEntity->mPos, w, w, 0, -w, w, 0, color);

	GO_MakeESPLine(pEntity->mPos, -w, -w, 0, 0, 0, h, color);
	GO_MakeESPLine(pEntity->mPos, -w, w, 0, 0, 0, h, color);
	GO_MakeESPLine(pEntity->mPos, w, -w, 0, 0, 0, h, color);
	GO_MakeESPLine(pEntity->mPos, w, w, 0, 0, 0, h, color);
}
VOID BO2_MakePyramid(BO2_cEntity *pEntity, FLOAT w, FLOAT h, const FLOAT * color)
{
	BO2_MakeESPLine(pEntity->mPos, -w, -w, 0, w, -w, 0, color);
	BO2_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, w, 0, color);
	BO2_MakeESPLine(pEntity->mPos, w, w, 0, w, -w, 0, color);
	BO2_MakeESPLine(pEntity->mPos, w, w, 0, -w, w, 0, color);

	BO2_MakeESPLine(pEntity->mPos, -w, -w, 0, 0, 0, h, color);
	BO2_MakeESPLine(pEntity->mPos, -w, w, 0, 0, 0, h, color);
	BO2_MakeESPLine(pEntity->mPos, w, -w, 0, 0, 0, h, color);
	BO2_MakeESPLine(pEntity->mPos, w, w, 0, 0, 0, h, color);
}
VOID MW3_MakePyramid(MW3_cEntity *pEntity, FLOAT w, FLOAT h, const FLOAT * color)
{
	MW3_MakeESPLine(pEntity->mPos, -w, -w, 0, w, -w, 0, color);
	MW3_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, w, 0, color);
	MW3_MakeESPLine(pEntity->mPos, w, w, 0, w, -w, 0, color);
	MW3_MakeESPLine(pEntity->mPos, w, w, 0, -w, w, 0, color);

	MW3_MakeESPLine(pEntity->mPos, -w, -w, 0, 0, 0, h, color);
	MW3_MakeESPLine(pEntity->mPos, -w, w, 0, 0, 0, h, color);
	MW3_MakeESPLine(pEntity->mPos, w, -w, 0, 0, 0, h, color);
	MW3_MakeESPLine(pEntity->mPos, w, w, 0, 0, 0, h, color);
}
VOID BO1_MakePyramid(BO1_cEntity *pEntity, FLOAT w, FLOAT h, const FLOAT * color)
{
	BO1_MakeESPLine(pEntity->mPos, -w, -w, 0, w, -w, 0, color);
	BO1_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, w, 0, color);
	BO1_MakeESPLine(pEntity->mPos, w, w, 0, w, -w, 0, color);
	BO1_MakeESPLine(pEntity->mPos, w, w, 0, -w, w, 0, color);

	BO1_MakeESPLine(pEntity->mPos, -w, -w, 0, 0, 0, h, color);
	BO1_MakeESPLine(pEntity->mPos, -w, w, 0, 0, 0, h, color);
	BO1_MakeESPLine(pEntity->mPos, w, -w, 0, 0, 0, h, color);
	BO1_MakeESPLine(pEntity->mPos, w, w, 0, 0, 0, h, color);
}
VOID MW2_MakePyramid(MW2_cEntity *pEntity, FLOAT w, FLOAT h, const FLOAT * color)
{
	MW2_MakeESPLine(pEntity->mPos, -w, -w, 0, w, -w, 0, color);
	MW2_MakeESPLine(pEntity->mPos, -w, -w, 0, -w, w, 0, color);
	MW2_MakeESPLine(pEntity->mPos, w, w, 0, w, -w, 0, color);
	MW2_MakeESPLine(pEntity->mPos, w, w, 0, -w, w, 0, color);

	MW2_MakeESPLine(pEntity->mPos, -w, -w, 0, 0, 0, h, color);
	MW2_MakeESPLine(pEntity->mPos, -w, w, 0, 0, 0, h, color);
	MW2_MakeESPLine(pEntity->mPos, w, -w, 0, 0, 0, h, color);
	MW2_MakeESPLine(pEntity->mPos, w, w, 0, 0, 0, h, color);
}

FLOAT GetBoxWidth() {
	return 17;
}
FLOAT GetBoxHeight() {
	return 60;
}


//Test Barrel stuff :D
VOID AngleVectors(D3DXVECTOR3 angles, D3DXVECTOR3 forward, D3DXVECTOR3 right, D3DXVECTOR3 up)
{
	FLOAT		angle;
	static FLOAT		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = angles[1] * (M_PI * 2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[0] * (M_PI * 2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[2] * (M_PI * 2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = (-1 * sr*sp*cy + -1 * cr*-sy);
		right[1] = (-1 * sr*sp*sy + -1 * cr*cy);
		right[2] = -1 * sr*cp;
	}
	if (up)
	{
		up[0] = (cr*sp*cy + -sr*-sy);
		up[1] = (cr*sp*sy + -sr*cy);
		up[2] = cr*cp;
	}
}


FLOAT* PlayerViewPoint(FLOAT Line, D3DXVECTOR3 Angle, D3DXVECTOR3 ClientPos)
{
	D3DXVECTOR3 Trail;
	D3DXVECTOR3 teub, teub2;

	AngleVectors(Angle, Trail, teub, teub);
	Trail[2] = -Trail[2]; // same as 0.0f

	D3DXVECTOR3 Pos;
	VectorCopy(ClientPos, Pos);

	Pos[0] += (Trail[0] * Line);
	Pos[1] += (Trail[1] * Line);
	Pos[2] += (Trail[2] * Line);

	return Pos;
}//ghetto fixed
 //tu veux test sur quel cod ? nimp aw
VOID EnemyIsAimingAtYou(cEntity * pEnt)
{
	D3DXVECTOR3 Position;
	Vector vOut, vOutXY;

	VectorCopy(Entities[cg->cNum]->mPos, Position);//get ta position a toi pd ! tg

	if (wS2(PlayerViewPoint(1000.0f, pEnt->ViewAngle2, Position), vOut))
	{

		if (wS2(Position, vOutXY))
		{

			D3DXVECTOR3 PlayerViewPoint;
			PlayerViewPoint[0] = vOut.x;
			PlayerViewPoint[1] = vOut.y;
			PlayerViewPoint[2] = 0.0f;

			if (PlayerViewPoint[0] == vOut.x && PlayerViewPoint[1] == vOut.y)
			{
				DrawShadowText("ENEMY IS AIMING AT YOU!", 250, 80, 0.7, red2);//bon le barrel esp now
			}
		}
	}
}
D3DXVECTOR3 ConverteAngulos(D3DXVECTOR3 Angulos, D3DXVECTOR3 PosicaoCliente)
{
	FLOAT ValorLinha = 300;
	D3DXVECTOR3 trazeira, NULLz;

	AngleVectors(Angulos, trazeira, NULLz, NULLz);
	trazeira[2] = -trazeira[2];

	D3DXVECTOR3 Posicao(PosicaoCliente);
	Posicao += trazeira * ValorLinha;

	return Posicao;
}

VOID Barrel(cEntity* pEnt)
{
	Vector Out, Out2;
	D3DXVECTOR3 Posicao;



	VectorCopy(pEnt->mPos, Posicao);
	Posicao[2] += 65.0f;

	if (wS2(ConverteAngulos(pEnt->ViewAngle2, Posicao), Out))
	{
		if (wS2(Posicao, Out2))
			DrawLine2(Out[0], Out[1], Out2[0], Out2[1], Purple, White);
	}

}

/*
VOID Barrel(cEntity* pEnt)
{
D3DXVECTOR3 up, right, forward, origin, screen;
FLOAT xy1[2], xy2[2];
VectorCopy(pEnt->mPos,origin);
AngleVectors(pEnt->ViewAngle2, forward, right, up);
forward[2] = -forward[2];

D3DXVECTOR3 pos (origin);

pos [0] += forward[0] * 10;
pos [1] += forward[1] * 10;
pos [2] += forward[2] * 10;

screen = pos + forward* 2;


if (wS2(origin, xy1) && wS2(screen, xy2))
DrawLine2(xy1[0], xy1[1], xy2[0], xy2[1],Purple, White);
//DrawLine2(pos , screen ,Red);
}*/
FLOAT GetFontSize1(FLOAT PlayerDisctance)
{
	if (PlayerDisctance <= 10)
		return 0.37f;

	else if (PlayerDisctance <= 20)
		return 0.36f;

	else if (PlayerDisctance <= 30)
		return 0.35f;

	else if (PlayerDisctance <= 40)
		return 0.34f;

	else if (PlayerDisctance <= 50)
		return 0.33f;

	else if (PlayerDisctance <= 60)
		return 0.32f;

	else if (PlayerDisctance <= 70)
		return 0.31f;

	else if (PlayerDisctance <= 80)
		return 0.30f;

	else if (PlayerDisctance <= 90)
		return 0.29f;

	else if (PlayerDisctance <= 99.9999)
		return 0.28f;

	else if (PlayerDisctance >= 100)
		return 0.28f;
	
	//return (100 * PlayerDisctance) /1000;
}

FLOAT GetFontSize(FLOAT PlayerDisctance)
{
	if (PlayerDisctance <= 10)
		return 0.3f;

	else if (PlayerDisctance <= 20)
		return 0.28f;

	else if (PlayerDisctance <= 30)
		return 0.27f;

	else if (PlayerDisctance <= 40)
		return 0.26f;

	else if (PlayerDisctance <= 50)
		return 0.25f;

	else if (PlayerDisctance <= 60)
		return 0.24f;

	else if (PlayerDisctance <= 70)
		return 0.23f;

	else if (PlayerDisctance <= 80)
		return 0.22f;

	else if (PlayerDisctance <= 90)
		return 0.21f;

	else if (PlayerDisctance <= 99.9999)
		return 0.20f;

	else if (PlayerDisctance >= 100)
		return 0.20f;

	//return (100 * PlayerDisctance) /1000;
}

FLOAT GetTextPos(FLOAT PlayerDisctance)
{
	if (PlayerDisctance <= 10)
		return 18;

	else if (PlayerDisctance <= 20)
		return 15;

	else if (PlayerDisctance <= 30)
		return 12;

	else if (PlayerDisctance <= 40)
		return 11;

	else if (PlayerDisctance <= 50)
		return 10;

	else if (PlayerDisctance <= 60)
		return 9;

	else if (PlayerDisctance <= 70)
		return 8;

	else if (PlayerDisctance <= 80)
		return 7;

	else if (PlayerDisctance <= 90)
		return 6;

	else if (PlayerDisctance <= 99.9999)
		return 5;

	else if (PlayerDisctance >= 100)
		return 5;
	
}

FLOAT GetTextPos1(FLOAT PlayerDisctance)
{
	if (PlayerDisctance <= 10)
		return 26;

	else if (PlayerDisctance <= 20)
		return 23;

	else if (PlayerDisctance <= 30)
		return 20;

	else if (PlayerDisctance <= 40)
		return 19;

	else if (PlayerDisctance <= 50)
		return 18;

	else if (PlayerDisctance <= 60)
		return 17;

	else if (PlayerDisctance <= 70)
		return 16;

	else if (PlayerDisctance <= 80)
		return 15;

	else if (PlayerDisctance <= 90)
		return 14;

	else if (PlayerDisctance <= 99.9999)
		return 13;

	else if (PlayerDisctance >= 100)
		return 13;
	
}

BOOL ProximityWarning(FLOAT PlayerDisctance)
{
	if (PlayerDisctance <= 15)
		return TRUE;

	else return FALSE;
}
#define POW(x)					((x) * (x))
#define ScreenDistance(a,b)		(sqrt(POW(a[0]-b[0]) + POW(a[1]-b[1])))
#define DistanceBox(a,b,c)		((c*((FLOAT)(*(int*)0x2A81000))/pRefDef->flFOV) * 18.0f / (VectorDistance(a,b)/55.0f))
int VectorToScreenDistance(FLOAT *flOrigin1, FLOAT *flOrigin2)
{
	Vector vScreen1, vScreen2;
	wS2(flOrigin1, vScreen1);
	wS2(flOrigin2, vScreen2);
	return (int)ScreenDistance(vScreen1, vScreen2);
}

VOID DrawRotatedShader(BOOL bCenter, int x, int y, int w, int h, FLOAT flAngle, const FLOAT * clrColor, int pShader = White)
{

	if (bCenter)
	{
		x -= w / 2;
		y -= h / 2;
	}

	DrawRotated(DrawRotated_R3, x, y, w, h, flAngle, clrColor, pShader);


}

int DrawPlayerBox(cEntity *pEntity, short tag, FLOAT size, const FLOAT * clrColor)
{
	Vector vTop;
	Vector vBot = pEntity->mPos;
	if (!GetTagPos(0, pEntity, tag, vTop))
		return 0;
	vTop.z += 10;
	vBot.z -= 5;

	int iDistance = VectorToScreenDistance(vTop, vBot);
	int iWidth = size;//DistanceBox(RefDef->EyePos,vTop,25);
					  //if(pEntity->iFlags&FL_CROUCH)
					  //iWidth=(int)DistanceBox(pRefDef->vViewOrg,vTop,35);

	Vector vScreen;
	if (wS2(vTop, vScreen))
	{
		int x = (int)vScreen[0] - iWidth / 2;
		int y = (int)vScreen[1];

		DrawRotatedShader(FALSE, x - 1, y, 1, iDistance, 0, clrColor);
		DrawRotatedShader(FALSE, x + iWidth, y, 1, iDistance, 0, clrColor);
		DrawRotatedShader(FALSE, x - 1, y - 1, iWidth + 2, 1, 0, clrColor);
		DrawRotatedShader(FALSE, x - 1, y + iDistance, iWidth + 2, 1, 0, clrColor);

		return iDistance;
	}

	return 0;
}
int GO_VectorToScreenDistance(FLOAT *flOrigin1, FLOAT *flOrigin2)
{
	Vector vScreen1, vScreen2;
	GO_wS2(flOrigin1, vScreen1);
	GO_wS2(flOrigin2, vScreen2);
	return (int)ScreenDistance(vScreen1, vScreen2);
}

VOID GO_DrawRotatedShader(BOOL bCenter, int x, int y, int w, int h, FLOAT flAngle, const FLOAT * clrColor, int pShader = GO_White)
{

	if (bCenter)
	{
		x -= w / 2;
		y -= h / 2;
	}

	GO_DrawRotated(GO_DrawRotated_R3, x, y, w, h, flAngle, clrColor, pShader);


}

int GO_DrawPlayerBox(GO_cEntity *pEntity, short tag, FLOAT size, const FLOAT * clrColor)
{
	Vector vTop;
	Vector vBot = pEntity->mPos;
	if (!GO_GetTagPos(0, pEntity, tag, vTop))
		return 0;
	vTop.z += 10;
	vBot.z -= 5;

	int iDistance = GO_VectorToScreenDistance(vTop, vBot);//Crash   
	int iWidth = size;//DistanceBox(RefDef->EyePos,vTop,25);
					  //if(pEntity->iFlags&FL_CROUCH)
					  //iWidth=(int)DistanceBox(pRefDef->vViewOrg,vTop,35);

	Vector vScreen;
	if (GO_wS2(vTop, vScreen))
	{
		int x = (int)vScreen[0] - iWidth / 2;
		int y = (int)vScreen[1];

		GO_DrawRotatedShader(FALSE, x - 1, y, 2, iDistance, 0, clrColor);
		GO_DrawRotatedShader(FALSE, x + iWidth, y, 2, iDistance, 0, clrColor);
		GO_DrawRotatedShader(FALSE, x - 1, y - 1, iWidth + 2, 2, 0, clrColor);
		GO_DrawRotatedShader(FALSE, x - 1, y + iDistance, iWidth + 2, 2, 0, clrColor);

		return iDistance;
	}

	return 0;
}
int MW3_VectorToScreenDistance(FLOAT *flOrigin1, FLOAT *flOrigin2)
{
	Vector vScreen1, vScreen2;
	MW3_wS2(flOrigin1, vScreen1);
	MW3_wS2(flOrigin2, vScreen2);
	return (int)ScreenDistance(vScreen1, vScreen2);
}

VOID MW3_DrawRotatedShader(BOOL bCenter, int x, int y, int w, int h, FLOAT flAngle, const FLOAT * clrColor, int pShader = MW3_White)
{

	if (bCenter)
	{
		x -= w / 2;
		y -= h / 2;
	}

	MW3_DrawRotated(MW3_DrawRotated_R3, x, y, w, h, flAngle, clrColor, pShader);


}

int MW3_DrawPlayerBox(MW3_cEntity *pEntity, short tag, FLOAT size, const FLOAT * clrColor)
{
	Vector vTop;
	Vector vBot = pEntity->mPos;
	if (!MW3_GetTagPos(0, pEntity, tag, vTop))
		return 0;
	vTop.z += 10;
	vBot.z -= 5;

	int iDistance = MW3_VectorToScreenDistance(vTop, vBot);
	int iWidth = size;//DistanceBox(RefDef->EyePos,vTop,25);
					  //if(pEntity->iFlags&FL_CROUCH)
					  //iWidth=(int)DistanceBox(pRefDef->vViewOrg,vTop,35);

	Vector vScreen;
	if (MW3_wS2(vTop, vScreen))
	{
		int x = (int)vScreen[0] - iWidth / 2;
		int y = (int)vScreen[1];

		MW3_DrawRotatedShader(FALSE, x - 1, y, 2, iDistance, 0, clrColor);
		MW3_DrawRotatedShader(FALSE, x + iWidth, y, 2, iDistance, 0, clrColor);
		MW3_DrawRotatedShader(FALSE, x - 1, y - 1, iWidth + 2, 2, 0, clrColor);
		MW3_DrawRotatedShader(FALSE, x - 1, y + iDistance, iWidth + 2, 2, 0, clrColor);

		return iDistance;
	}

	return 0;
}

int BO2_VectorToScreenDistance(FLOAT *flOrigin1, FLOAT *flOrigin2)
{
	Vector vScreen1, vScreen2;
	BO2_wS2(flOrigin1, vScreen1);
	BO2_wS2(flOrigin2, vScreen2);
	return (int)ScreenDistance(vScreen1, vScreen2);
}

VOID BO2_DrawRotatedShader(BOOL bCenter, int x, int y, int w, int h, FLOAT flAngle, const FLOAT * clrColor, int pShader = BO2_White)
{

	if (bCenter)
	{
		x -= w / 2;
		y -= h / 2;
	}

	BO2_DrawRotated(BO2_DrawRotated_R3, x, y, w, h, flAngle, clrColor, pShader);


}

int BO2_DrawPlayerBox(BO2_cEntity *pEntity, short tag, FLOAT size, const FLOAT * clrColor)
{
	Vector vTop;
	Vector vBot = pEntity->mPos;
	if (!BO2_GetTagPos(pEntity, tag, vTop))
		return 0;

	vTop.z += 10;
	vBot.z -= 5;

	int iDistance = BO2_VectorToScreenDistance(vTop, vBot);
	int iWidth = size;//DistanceBox(RefDef->EyePos,vTop,25);
					  //if(pEntity->iFlags&FL_CROUCH)
					  //iWidth=(int)DistanceBox(pRefDef->vViewOrg,vTop,35);

	Vector vScreen;
	if (BO2_wS2(vTop, vScreen))
	{
		int x = (int)vScreen[0] - iWidth / 2;
		int y = (int)vScreen[1];

		BO2_DrawRotatedShader(FALSE, x - 1, y, 2, iDistance, 0, clrColor);
		BO2_DrawRotatedShader(FALSE, x + iWidth, y, 2, iDistance, 0, clrColor);
		BO2_DrawRotatedShader(FALSE, x - 1, y - 1, iWidth + 2, 2, 0, clrColor);
		BO2_DrawRotatedShader(FALSE, x - 1, y + iDistance, iWidth + 2, 2, 0, clrColor);

		return iDistance;
	}

	return 0;
}

int BO1_VectorToScreenDistance(FLOAT *flOrigin1, FLOAT *flOrigin2)
{
	Vector vScreen1, vScreen2;
	BO1_wS2(flOrigin1, vScreen1);
	BO1_wS2(flOrigin2, vScreen2);
	return (int)ScreenDistance(vScreen1, vScreen2);
}

VOID BO1_DrawRotatedShader(BOOL bCenter, int x, int y, int w, int h, FLOAT flAngle, const FLOAT * clrColor, int pShader = BO1_White)
{

	if (bCenter)
	{
		x -= w / 2;
		y -= h / 2;
	}

	BO1_DrawRotated(BO1_DrawRotated_R3, x, y, w, h, flAngle, clrColor, pShader);


}

int BO1_DrawPlayerBox(BO1_cEntity *pEntity, short tag, FLOAT size, const FLOAT * clrColor)
{
	Vector vTop;
	Vector vBot = pEntity->mPos;
	if (!BO1_GetTagPos(0, pEntity, tag, vTop))
		return 0;
	vTop.z += 10;
	vBot.z -= 5;

	int iDistance = BO1_VectorToScreenDistance(vTop, vBot);
	int iWidth = size;//DistanceBox(RefDef->EyePos,vTop,25);
					  //if(pEntity->iFlags&FL_CROUCH)
					  //iWidth=(int)DistanceBox(pRefDef->vViewOrg,vTop,35);

	Vector vScreen;
	if (BO1_wS2(vTop, vScreen))
	{
		int x = (int)vScreen[0] - iWidth / 2;
		int y = (int)vScreen[1];

		BO1_DrawRotatedShader(FALSE, x - 1, y, 1, iDistance, 0, clrColor);
		BO1_DrawRotatedShader(FALSE, x + iWidth, y, 1, iDistance, 0, clrColor);
		BO1_DrawRotatedShader(FALSE, x - 1, y - 1, iWidth + 2, 1, 0, clrColor);
		BO1_DrawRotatedShader(FALSE, x - 1, y + iDistance, iWidth + 2, 1, 0, clrColor);

		return iDistance;
	}

	return 0;
}

int MW2_VectorToScreenDistance(FLOAT *flOrigin1, FLOAT *flOrigin2)
{
	Vector vScreen1, vScreen2;
	MW2_wS2(flOrigin1, vScreen1);
	MW2_wS2(flOrigin2, vScreen2);
	return (int)ScreenDistance(vScreen1, vScreen2);
}

VOID MW2_DrawRotatedShader(BOOL bCenter, int x, int y, int w, int h, FLOAT flAngle, const FLOAT * clrColor, int pShader = MW2_White)
{

	if (bCenter)
	{
		x -= w / 2;
		y -= h / 2;
	}

	MW2_DrawRotated(MW2_DrawRotated_R3, x, y, w, h, flAngle, clrColor, pShader);


}

int MW2_DrawPlayerBox(MW2_cEntity *pEntity, short tag, FLOAT size, const FLOAT * clrColor)
{
	Vector vTop;
	Vector vBot = pEntity->mPos;
	if (!MW2_GetTagPos(0, pEntity, tag, vTop))
		return 0;
	vTop.z += 10;
	vBot.z -= 5;

	int iDistance = MW2_VectorToScreenDistance(vTop, vBot);
	int iWidth = size;//DistanceBox(RefDef->EyePos,vTop,25);
					  //if(pEntity->iFlags&FL_CROUCH)
					  //iWidth=(int)DistanceBox(pRefDef->vViewOrg,vTop,35);

	Vector vScreen;
	if (MW2_wS2(vTop, vScreen))
	{
		int x = (int)vScreen[0] - iWidth / 2;
		int y = (int)vScreen[1];

		MW2_DrawRotatedShader(FALSE, x - 1, y, 2, iDistance, 0, clrColor);
		MW2_DrawRotatedShader(FALSE, x + iWidth, y, 2, iDistance, 0, clrColor);
		MW2_DrawRotatedShader(FALSE, x - 1, y - 1, iWidth + 2, 2, 0, clrColor);
		MW2_DrawRotatedShader(FALSE, x - 1, y + iDistance, iWidth + 2, 2, 0, clrColor);

		return iDistance;
	}

	return 0;
}


//NewStuff
BOOL onlyOnce = FALSE;
short tagsBO2[20] = { 0 };

char tagnames[20][32] = {
	"j_helmet",
	"j_head",
	"j_neck",
	"j_shoulder_le",
	"j_shoulder_ri",
	"j_elbow_le",
	"j_elbow_ri",
	"j_wrist_le",
	"j_wrist_ri",
	"j_gun",
	"j_mainroot",
	"j_spineupper",
	"j_spinelower",
	"j_spine4",
	"j_hip_le",
	"j_hip_ri",
	"j_knee_le",
	"j_knee_ri",
	"j_ankle_le",
	"j_ankle_ri"
};

BOOL BO2_GetTagPosReg(BO2_cEntity* pEnt, char* szTag, FLOAT* flOut) {
	WORD wTag = BO2_GetTagByName(szTag);
	if (!wTag)
		return FALSE;

	BO2_GetTagPos(pEnt, wTag, flOut);

	return TRUE;
}
VOID BO2_DrawBones(BO2_cEntity* Entity, CHAR* BoneName1, CHAR* BoneName2, const FLOAT *Color) {
	FLOAT Screen[2], Screen2[2];
	Vector BonePos1, BonePos2;

	//for ( int j = 0; j < 20; j++ ) {
	BO2_GetTagPosReg(Entity, BoneName1, BonePos1);
	BO2_GetTagPosReg(Entity, BoneName2, BonePos2);

	if (BO2_wS2(BonePos1, Screen) && BO2_wS2(BonePos2, Screen2)) {
		BO2_DrawLine2(Screen[0], Screen[1], Screen2[0], Screen2[1], Color, BO2_White);

		//BO2_DrawShadowText (".",Screen[0], Screen[1],0.5f,Purple);
		//}
	}
}

VOID BO2_DrawBoneEsp(BO2_cEntity* pEnt, const FLOAT* color)
{
	// register are tags so we dont get errors
	if (!tagsBO2[0]) {
		for (int i = 0; i < 20; i++)
			tagsBO2[i] = registerTagBO2(tagnames[i]);
	}

	// top: j_helmet to j_neck
	BO2_DrawBones(pEnt, (char*)tagsBO2[0], (char*)tagsBO2[1], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[1], (char*)tagsBO2[2], color);

	// left arm: j_neck to j_wrist_le
	BO2_DrawBones(pEnt, (char*)tagsBO2[2], (char*)tagsBO2[3], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[3], (char*)tagsBO2[5], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[5], (char*)tagsBO2[7], color);

	// right arm: j_neck to j_wrist_ri
	BO2_DrawBones(pEnt, (char*)tagsBO2[2], (char*)tagsBO2[4], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[4], (char*)tagsBO2[6], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[6], (char*)tagsBO2[8], color);

	// center: j_neck to j_spinelower
	BO2_DrawBones(pEnt, (char*)tagsBO2[2], (char*)tagsBO2[10], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[10], (char*)tagsBO2[11], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[11], (char*)tagsBO2[12], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[12], (char*)tagsBO2[13], color);

	// left leg: j_spine4 to j_ankle_le
	BO2_DrawBones(pEnt, (char*)tagsBO2[13], (char*)tagsBO2[14], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[14], (char*)tagsBO2[16], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[16], (char*)tagsBO2[18], color);

	// right leg: j_spine4 to j_ankle_ri
	BO2_DrawBones(pEnt, (char*)tagsBO2[13], (char*)tagsBO2[15], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[15], (char*)tagsBO2[17], color);
	BO2_DrawBones(pEnt, (char*)tagsBO2[17], (char*)tagsBO2[19], color);
}

BOOL BO1_GetTagPosReg(BO1_cEntity* pEnt, char* szTag, FLOAT* flOut) {
	WORD wTag = BO1_GetTagByName(szTag);
	if (!wTag)
		return FALSE;

	BO1_GetTagPos(0, pEnt, wTag, flOut);

	return TRUE;
}
VOID BO1_DrawBones(BO1_cEntity* Entity, CHAR* BoneName1, CHAR* BoneName2, const FLOAT *Color) {
	FLOAT Screen[2], Screen2[2];
	Vector BonePos1, BonePos2;

	//for ( int j = 0; j < 20; j++ ) {
	BO1_GetTagPosReg(Entity, BoneName1, BonePos1);
	BO1_GetTagPosReg(Entity, BoneName2, BonePos2);

	if (BO1_wS2(BonePos1, Screen) && BO1_wS2(BonePos2, Screen2)) {
		BO1_DrawLine2(Screen[0], Screen[1], Screen2[0], Screen2[1], Color, BO1_White);

		//BO1_DrawShadowText (".",Screen[0], Screen[1],0.5f,Purple);
		//}
	}
}

VOID BO1_DrawBoneEsp(BO1_cEntity* pEnt, const FLOAT* color)
{
	BO1_DrawBones(pEnt, "j_neck", "j_shoulder_le", color);
	BO1_DrawBones(pEnt, "j_neck", "j_shoulder_ri", color);
	BO1_DrawBones(pEnt, "j_shoulder_le", "j_elbow_le", color);
	BO1_DrawBones(pEnt, "j_shoulder_ri", "j_elbow_ri", color);
	BO1_DrawBones(pEnt, "j_elbow_le", "j_wrist_le", color);
	BO1_DrawBones(pEnt, "j_elbow_ri", "j_wrist_ri", color);
	BO1_DrawBones(pEnt, "j_neck", "j_spinelower", color);
	BO1_DrawBones(pEnt, "j_spinelower", "j_knee_le", color);
	BO1_DrawBones(pEnt, "j_spinelower", "j_knee_ri", color);
	BO1_DrawBones(pEnt, "j_knee_le", "j_ankle_le", color);
	BO1_DrawBones(pEnt, "j_knee_ri", "j_ankle_ri", color);
}


BOOL GO_GetTagPosReg(GO_cEntity* pEnt, char* szTag, FLOAT* flOut) {
	WORD wTag = GetTagByName(szTag);
	if (!wTag)
		return FALSE;

	GO_GetTagPos(0, pEnt, wTag, flOut);

	return TRUE;
}
VOID GO_DrawBones(GO_cEntity* Entity, CHAR* BoneName1, CHAR* BoneName2, const FLOAT *Color) {
	FLOAT Screen[2], Screen2[2];
	Vector BonePos1, BonePos2;

	//for ( int j = 0; j < 20; j++ ) {
	GO_GetTagPosReg(Entity, BoneName1, BonePos1);
	GO_GetTagPosReg(Entity, BoneName2, BonePos2);

	if (GO_wS2(BonePos1, Screen) && GO_wS2(BonePos2, Screen2)) {
		GO_DrawLine2(Screen[0], Screen[1], Screen2[0], Screen2[1], Color, GO_White);

		//GO_DrawShadowText (".",Screen[0], Screen[1],0.5f,Purple);
		//}
	}
}

VOID GO_DrawBoneEsp(GO_cEntity* pEnt, const FLOAT* color)
{
	GO_DrawBones(pEnt, "j_neck", "j_shoulder_le", color);
	GO_DrawBones(pEnt, "j_neck", "j_shoulder_ri", color);
	GO_DrawBones(pEnt, "j_shoulder_le", "j_elbow_le", color);
	GO_DrawBones(pEnt, "j_shoulder_ri", "j_elbow_ri", color);
	GO_DrawBones(pEnt, "j_elbow_le", "j_wrist_le", color);
	GO_DrawBones(pEnt, "j_elbow_ri", "j_wrist_ri", color);
	GO_DrawBones(pEnt, "j_neck", "j_spinelower", color);
	GO_DrawBones(pEnt, "j_spinelower", "j_knee_le", color);
	GO_DrawBones(pEnt, "j_spinelower", "j_knee_ri", color);
	GO_DrawBones(pEnt, "j_knee_le", "j_ankle_le", color);
	GO_DrawBones(pEnt, "j_knee_ri", "j_ankle_ri", color);
}


BOOL MW3_GetTagPosReg(MW3_cEntity* pEnt, char* szTag, FLOAT* flOut) {
	WORD wTag = MW3_GetTagByName(szTag);
	if (!wTag)
		return FALSE;

	MW3_GetTagPos(0, pEnt, wTag, flOut);

	return TRUE;
}
VOID MW3_DrawBones(MW3_cEntity* Entity, CHAR* BoneName1, CHAR* BoneName2, const FLOAT *Color) {
	FLOAT Screen[2], Screen2[2];
	Vector BonePos1, BonePos2;

	//for ( int j = 0; j < 20; j++ ) {
	MW3_GetTagPosReg(Entity, BoneName1, BonePos1);
	MW3_GetTagPosReg(Entity, BoneName2, BonePos2);

	if (MW3_wS2(BonePos1, Screen) && MW3_wS2(BonePos2, Screen2)) {
		MW3_DrawLine2(Screen[0], Screen[1], Screen2[0], Screen2[1], Color, MW3_White);

		//MW3_DrawShadowText (".",Screen[0], Screen[1],0.5f,Purple);
		//}
	}
}

VOID MW3_DrawBoneEsp(MW3_cEntity* pEnt, const FLOAT* color)
{
	MW3_DrawBones(pEnt, "j_neck", "j_shoulder_le", color);
	MW3_DrawBones(pEnt, "j_neck", "j_shoulder_ri", color);
	MW3_DrawBones(pEnt, "j_shoulder_le", "j_elbow_le", color);
	MW3_DrawBones(pEnt, "j_shoulder_ri", "j_elbow_ri", color);
	MW3_DrawBones(pEnt, "j_elbow_le", "j_wrist_le", color);
	MW3_DrawBones(pEnt, "j_elbow_ri", "j_wrist_ri", color);
	MW3_DrawBones(pEnt, "j_neck", "j_spinelower", color);
	MW3_DrawBones(pEnt, "j_spinelower", "j_knee_le", color);
	MW3_DrawBones(pEnt, "j_spinelower", "j_knee_ri", color);
	MW3_DrawBones(pEnt, "j_knee_le", "j_ankle_le", color);
	MW3_DrawBones(pEnt, "j_knee_ri", "j_ankle_ri", color);
}
BOOL IsNearToCrosshair(D3DXVECTOR3 spot)
{
	D3DXVECTOR3 originScreen;

	if (!BO2_wS2(spot, originScreen))
		return FALSE;

	if (abs(1280 / 2.0f - originScreen.x) <= 50 && abs(720 / 2.0f - originScreen.y) <= 50)
		return TRUE;

	return FALSE;
}

VOID AngleVectorsZ(D3DXVECTOR3 Angles, D3DXVECTOR3& Forward)
{
	FLOAT SP;
	FLOAT SY;
	FLOAT CP;
	FLOAT CY;

	FLOAT Angle;
	Angle = Angles[1] * (M_PI * 2 / 360);
	SY = sin(Angle);
	CY = cos(Angle);

	Angle = Angles[0] * (M_PI * 2 / 360);
	SP = sin(Angle);
	CP = cos(Angle);


	Forward[0] = CP * CY;
	Forward[1] = CP * SY;
	Forward[2] = -SP;

	/*
	FLOAT		angle;
	static FLOAT		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = angles[1] * (M_PI*2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[0] * (M_PI*2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[2] * (M_PI*2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
	forward[0] = cp*cy;
	forward[1] = cp*sy;
	forward[2] = -sp;
	};*/
}

VOID GO_toogleHack()
{
	//Recoil
	*(int*)GO_NoRecoil = 0x60000000;
	//UavON
	*(int*)GO_Radar = 0x3B600001;
	//SwayON
	*(int*)GO_WeaponSway = 0x60000000;
	*(int*)GO_ZommingSway = 0x60000000;
}

VOID MW3_toogleHack()
{
	//Recoil
	*(int*)MW3_NoRecoil = 0x60000000;
	//UavON
	*(int*)MW3_Radar = 0x3B800001;
	//SwayON
	*(int*)MW3_WeaponSway = 0x60000000;
	*(int*)MW3_ZommingSway = 0x60000000;
}

VOID BO1_toogleHack()
{
	//Recoil
	*(int*)BO1_NoRecoil = 0x60000000;
	//UavON
	*(int*)BO1_Radar = 0x2B0B0001;
	//SwayON
	*(int*)BO1_WeaponSway = 0x60000000;
	*(int*)BO1_ZommingSway = 0x60000000;
}


VOID BO2_toogleHack()
{
	//Recoil
	*(int*)BO2_NoRecoil = 0x60000000;
	//UavON
	*(int*)BO2_Radar = 0x2B0B0001;
	//SwayON
	*(int*)BO2_WeaponSway = 0x60000000;
	*(int*)BO2_ZommingSway = 0x60000000;
}

VOID MW2_toogleHack()
{
	//Recoil
	*(int*)MW2_NoRecoil = 0x60000000;
	//UavON
	*(int*)MW2_Radar = 0x3BA00001;
	//SwayON
	*(int*)MW2_WeaponSway = 0x60000000;
	*(int*)MW2_ZommingSway = 0x60000000;
}

GO_cEntity* GO_GetTargetByCrosshair()
{
	FLOAT ScreenX = (FLOAT)1280 / 2.0f;
	FLOAT ScreenY = (FLOAT)720 / 2.0f;
	FLOAT Nearest = (FLOAT)INT_MAX;
	FLOAT PosX = 0.0f, PosY = 0.0f;

	GO_cEntity* pRet = NULL;
	GO_ReadGame();

	Vector vOut;

	for (int i = 0; i < 12; i++)
	{
		if (i == GO_cg->cNum)
			continue;

		if ((GO_Entities[i]->Valid) && (GO_Entities[i]->State & 1))
		{
			if (strcmp(GO_cgs->gametype, "dm") == 0 || strcmp(GO_cgs->gametype, "gun") == 0)
			{
				if (GO_wS2(GO_Entities[i]->mPos2, vOut))
				{
					PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
					PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

					FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

					if (Temp > Nearest)
						continue;

					pRet = GO_Entities[i];
					Nearest = Temp;
				}
			}
			else
			{
				if (GO_ci[i]->mTeam != GO_ci[GO_cg->cNum]->mTeam)
				{
					if (GO_wS2(GO_Entities[i]->mPos2, vOut))
					{
						PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
						PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

						FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

						if (Temp > Nearest)
							continue;

						pRet = GO_Entities[i];
						Nearest = Temp;
					}
				}
			}
		}
	}
	return pRet;
}

GO_cEntity* GO_GetVisibleTarget()
{
	FLOAT ScreenX = (FLOAT)1280 / 2.0f;
	FLOAT ScreenY = (FLOAT)720 / 2.0f;
	FLOAT Nearest = (FLOAT)INT_MAX;
	FLOAT PosX = 0.0f, PosY = 0.0f;

	GO_cEntity* pRet = NULL;
	GO_ReadGame();

	Vector vOut;

	for (int i = 0; i < 12; i++)
	{
		if (i == GO_cg->cNum)
			continue;

		if ((GO_Entities[i]->Valid) && (GO_Entities[i]->State & 1))
		{
			if (strcmp(GO_cgs->gametype, "dm") == 0 || strcmp(GO_cgs->gametype, "gun") == 0)
			{
				if (GO_CanSeePlayer(0, GO_Entities[i], 0x280F803))
				{
					PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
					PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

					FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

					pRet = GO_Entities[i];
					Nearest = Temp;
				}
			}
			else
			{
				if (GO_ci[i]->mTeam != GO_ci[GO_cg->cNum]->mTeam)
				{
					if (GO_CanSeePlayer(0, GO_Entities[i], 0x280F803))
					{
						PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
						PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

						FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

						pRet = GO_Entities[i];
						Nearest = Temp;
					}
				}
			}
		}
	}
	return pRet;
}

MW3_cEntity* MW3_GetTargetByCrosshair()
{
	FLOAT ScreenX = (FLOAT)1280 / 2.0f;
	FLOAT ScreenY = (FLOAT)720 / 2.0f;
	FLOAT Nearest = (FLOAT)INT_MAX;
	FLOAT PosX = 0.0f, PosY = 0.0f;

	MW3_cEntity* pRet = NULL;
	MW3_ReadGame();

	Vector vOut;

	for (int i = 0; i < 18; i++)
	{
		if (i == MW3_cg->cNum)
			continue;

		if ((MW3_Entities[i]->Valid) && (MW3_Entities[i]->State & 1))
		{
			if (strcmp(MW3_cgs->gametype, "dm") == 0)
			{
				if (MW3_wS2(MW3_Entities[i]->mPos2, vOut))
				{
					PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
					PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

					FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

					if (Temp > Nearest)
						continue;

					pRet = MW3_Entities[i];
					Nearest = Temp;
				}
			}
			else
			{
				if (MW3_Clients[i]->mTeam != MW3_Clients[MW3_cg->cNum]->mTeam)
				{
					if (MW3_wS2(MW3_Entities[i]->mPos2, vOut))
					{
						PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
						PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

						FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

						if (Temp > Nearest)
							continue;

						pRet = MW3_Entities[i];
						Nearest = Temp;
					}
				}
			}
		}
	}
	return pRet;
}

MW3_cEntity* MW3_GetVisibleTarget()
{
	FLOAT ScreenX = (FLOAT)1280 / 2.0f;
	FLOAT ScreenY = (FLOAT)720 / 2.0f;
	FLOAT Nearest = (FLOAT)INT_MAX;
	FLOAT PosX = 0.0f, PosY = 0.0f;

	MW3_cEntity* pRet = NULL;
	MW3_ReadGame();

	Vector vOut;

	for (int i = 0; i < 18; i++)
	{
		if (i == MW3_cg->cNum)
			continue;

		if ((MW3_Entities[i]->Valid) && (MW3_Entities[i]->State & 1))
		{
			if (strcmp(MW3_cgs->gametype, "dm") == 0)
			{
				if (MW3_CanSeePlayer(0, MW3_Entities[i], 0x280F823))
				{
					PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
					PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

					FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

					pRet = MW3_Entities[i];
					Nearest = Temp;
				}
			}
			else
			{
				if (MW3_Clients[i]->mTeam != MW3_Clients[MW3_cg->cNum]->mTeam)
				{
					if (MW3_CanSeePlayer(0, MW3_Entities[i], 0x280F823))
					{
						PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
						PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

						FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

						pRet = MW3_Entities[i];
						Nearest = Temp;
					}
				}
			}
		}
	}
	return pRet;
}

MW2_cEntity* MW2_GetTargetByCrosshair()
{
	FLOAT ScreenX = (FLOAT)1280 / 2.0f;
	FLOAT ScreenY = (FLOAT)720 / 2.0f;
	FLOAT Nearest = (FLOAT)INT_MAX;
	FLOAT PosX = 0.0f, PosY = 0.0f;

	MW2_cEntity* pRet = NULL;
	MW2_ReadGame();

	Vector vOut;

	for (int i = 0; i < 12; i++)
	{
		if (i == MW2_cg->cNum)
			continue;

		if ((MW2_Entities[i]->Valid) && (MW2_Entities[i]->State & 1))
		{
			if (strcmp(MW2_cgs->gametype, "dm") == 0)
			{
				if (MW2_wS2(MW2_Entities[i]->mPos2, vOut))
				{
					PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
					PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

					FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

					if (Temp > Nearest)
						continue;

					pRet = MW2_Entities[i];
					Nearest = Temp;
				}
			}
			else
			{
				if (MW2_Clients[i]->mTeam != MW2_Clients[MW2_cg->cNum]->mTeam)
				{
					if (MW2_wS2(MW2_Entities[i]->mPos2, vOut))
					{
						PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
						PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

						FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

						if (Temp > Nearest)
							continue;

						pRet = MW2_Entities[i];
						Nearest = Temp;
					}
				}
			}
		}
	}
	return pRet;
}

MW2_cEntity* MW2_GetVisibleTarget()
{
	FLOAT ScreenX = (FLOAT)1280 / 2.0f;
	FLOAT ScreenY = (FLOAT)720 / 2.0f;
	FLOAT Nearest = (FLOAT)INT_MAX;
	FLOAT PosX = 0.0f, PosY = 0.0f;

	MW2_cEntity* pRet = NULL;
	MW2_ReadGame();

	Vector vOut;

	for (int i = 0; i < 12; i++)
	{
		if (i == MW2_cg->cNum)
			continue;

		if ((MW2_Entities[i]->Valid) && (MW2_Entities[i]->State & 1))
		{
			if (strcmp(MW2_cgs->gametype, "dm") == 0)
			{
				if (MW2_CanSeePlayer(0, MW2_Entities[i], 0x280F823))
				{
					PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
					PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

					FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

					pRet = MW2_Entities[i];
					Nearest = Temp;
				}
			}
			else
			{
				if (MW2_Clients[i]->mTeam != MW2_Clients[MW2_cg->cNum]->mTeam)
				{
					if (MW2_CanSeePlayer(0, MW2_Entities[i], 0x280F823))
					{
						PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
						PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

						FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

						pRet = MW2_Entities[i];
						Nearest = Temp;
					}
				}
			}
		}
	}
	return pRet;
}

BO1_cEntity* BO1_GetTargetByCrosshair()
{
	FLOAT ScreenX = (FLOAT)1280 / 2.0f;
	FLOAT ScreenY = (FLOAT)720 / 2.0f;
	FLOAT Nearest = (FLOAT)INT_MAX;
	FLOAT PosX = 0.0f, PosY = 0.0f;

	BO1_cEntity* pRet = NULL;
	BO1_ReadGame();

	Vector vOut;

	for (int i = 0; i < 12; i++)
	{
		if (i == BO1_cg->cNum)
			continue;

		if ((BO1_Entities[i]->Valid) && (BO1_Entities[i]->State == 64))
		{
			if (strcmp(BO1_cgs->gametype, "dm") == 0)
			{
				if (BO1_wS2(BO1_Entities[i]->mPos2, vOut))
				{
					PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
					PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

					FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

					if (Temp > Nearest)
						continue;

					pRet = BO1_Entities[i];
					Nearest = Temp;
				}
			}
			else
			{
				if (BO1_Clients[i]->mTeam != BO1_Clients[BO1_cg->cNum]->mTeam)
				{
					if (BO1_wS2(BO1_Entities[i]->mPos2, vOut))
					{
						PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
						PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

						FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

						if (Temp > Nearest)
							continue;

						pRet = BO1_Entities[i];
						Nearest = Temp;
					}
				}
			}
		}
	}
	return pRet;
}

BO1_cEntity* BO1_GetVisibleTarget()
{
	FLOAT ScreenX = (FLOAT)1280 / 2.0f;
	FLOAT ScreenY = (FLOAT)720 / 2.0f;
	FLOAT Nearest = (FLOAT)INT_MAX;
	FLOAT PosX = 0.0f, PosY = 0.0f;

	BO1_cEntity* pRet = NULL;
	BO1_ReadGame();

	Vector vOut;

	for (int i = 0; i < 12; i++)
	{
		if (i == BO1_cg->cNum)
			continue;

		if ((BO1_Entities[i]->Valid) && (BO1_Entities[i]->State == 64))
		{
			if (strcmp(BO1_cgs->gametype, "dm") == 0)
			{
				if (BO1_CanSeeFriendlyHead(0, BO1_Entities[i]) == 1)
				{
					PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
					PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

					FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

					pRet = BO1_Entities[i];
					Nearest = Temp;
				}
			}
			else
			{
				if (BO1_Clients[i]->mTeam != BO1_Clients[BO1_cg->cNum]->mTeam)
				{
					if (BO1_CanSeeFriendlyHead(0, BO1_Entities[i]) == 1)
					{
						PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
						PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

						FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

						pRet = BO1_Entities[i];
						Nearest = Temp;
					}
				}
			}
		}
	}
	return pRet;
}

BO2_cEntity* BO2_GetVisibleTarget()
{
	FLOAT ScreenX = (FLOAT)1280 / 2.0f;
	FLOAT ScreenY = (FLOAT)720 / 2.0f;
	FLOAT Nearest = (FLOAT)INT_MAX;
	FLOAT PosX = 0.0f, PosY = 0.0f;

	BO2_cEntity* pRet = NULL;
	BO2_ReadGame();

	Vector vOut;

	for (int i = 0; i < 12; i++)
	{
		if (i == BO2_cg->cNum)
			continue;

		if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
		{
			if (strcmp(BO2_cgs->gametype, "dm") == 0 || strcmp(BO2_cgs->gametype, "gun") == 0 || strcmp(BO2_cgs->gametype, "oic") == 0 || strcmp(BO2_cgs->gametype, "shrp") == 0 || strcmp(BO2_cgs->gametype, "sas") == 0)
			{
				if (BO2_CanSeeFriendlyHead(0, BO2_Entities[i]) == 1)
				{
					PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
					PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

					FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

					pRet = BO2_Entities[i];
					Nearest = Temp;
				}
			}
			else
			{
				if (BO2_Clients[i]->mTeam != BO2_Clients[BO2_cg->cNum]->mTeam)
				{
					if (BO2_CanSeeFriendlyHead(0, BO2_Entities[i]) == 1)
					{
						PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
						PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

						FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

						pRet = BO2_Entities[i];
						Nearest = Temp;
					}
				}
			}
		}
	}
	return pRet;
}

BO2_cEntity* BO2_GetTargetByCrosshair()
{
	FLOAT ScreenX = (FLOAT)1280 / 2.0f;
	FLOAT ScreenY = (FLOAT)720 / 2.0f;
	FLOAT Nearest = (FLOAT)INT_MAX;
	FLOAT PosX = 0.0f, PosY = 0.0f;

	BO2_cEntity* pRet = NULL;
	BO2_ReadGame();

	Vector vOut;

	for (int i = 0; i < 12; i++)
	{
		if (i == BO2_cg->cNum)
			continue;

		if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
		{
			if (strcmp(BO2_cgs->gametype, "dm") == 0 || strcmp(BO2_cgs->gametype, "gun") == 0 || strcmp(BO2_cgs->gametype, "oic") == 0 || strcmp(BO2_cgs->gametype, "shrp") == 0 || strcmp(BO2_cgs->gametype, "sas") == 0)
			{
				if (BO2_wS2(BO2_Entities[i]->mPos2, vOut))
				{
					PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
					PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

					FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

					if (Temp > Nearest)
						continue;

					pRet = BO2_Entities[i];
					Nearest = Temp;
				}
			}
			else
			{
				if (BO2_Clients[i]->mTeam != BO2_Clients[BO2_cg->cNum]->mTeam)
				{
					if (BO2_wS2(BO2_Entities[i]->mPos2, vOut))
					{
						PosX = vOut.x > ScreenX ? vOut.x - ScreenX : ScreenX - vOut.x;
						PosY = vOut.y > ScreenY ? vOut.y - ScreenY : ScreenY - vOut.y;

						FLOAT Temp = sqrt(PosX*PosX + PosY*PosY);

						if (Temp > Nearest)
							continue;

						pRet = BO2_Entities[i];
						Nearest = Temp;
					}
				}
			}
		}
	}
	return pRet;
}

struct Vec3
{
	FLOAT x, y, z;
};
FLOAT angles[3];
Vector vectoangles(Vec3 Angles)
{
	FLOAT forward;
	FLOAT yaw, pitch;
	FLOAT PI = 3.1415926535897931;
	if (Angles.x == 0 && Angles.y == 0)
	{
		yaw = 0;
		if (Angles.z > 0) pitch = 90.00;
		else pitch = 270.00;
	}
	else
	{
		if (Angles.x != -1) yaw = (FLOAT)(atan2((double)Angles.y, (double)Angles.x) * 180.00 / PI);
		else if (Angles.y > 0) yaw = 90.00;
		else yaw = 270;
		if (yaw < 0) yaw += 360.00;

		forward = (FLOAT)sqrt((double)(Angles.x * Angles.x + Angles.y * Angles.y));
		pitch = (FLOAT)(atan2((double)Angles.z, (double)forward) * 180.00 / PI);
		if (pitch < 0) pitch += 360.00;
	}
	angles[0] = -pitch;
	angles[1] = yaw;
	angles[2] = 0;

	return angles;
}



Vec3 Difference;
Vec3 GetVec(D3DXVECTOR3 Attacker, D3DXVECTOR3 Target)
{
	Difference.x = (Target.x - Attacker.x);
	Difference.y = (Target.y - Attacker.y);
	Difference.z = (Target.z - Attacker.z);
	return Difference;
}

vec2 normalize(const vec2 &u)
{
	return u / sqrt(u.x * u.x + u.y * u.y);
}

VOID VectorAngles(const FLOAT* forward, FLOAT* angles)
{
	FLOAT tmp, yaw, pitch;

	if (forward[2] == 0 && forward[0] == 0)
	{
		yaw = 0;

		if (forward[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = (atan2(forward[2], -forward[0]) * 180 / M_PI) - 90;

		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[2] * forward[2]);
		pitch = (atan2(forward[1], tmp) * 180 / M_PI);

		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = -pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

BOOL vbackupaim = FALSE;

__declspec(naked) int SaveStubPacket(int a1)
{
	__asm
	{
		nop
		nop
		nop
		nop
		nop
		nop
		nop
	}
}

/////////////////////////////////////// Ghosts Menu /////////////////////////////////////

BOOL FirstTimeInGameGO = FALSE;
BOOL openGO = FALSE;
int scrollGO = 0, maxoptionsGO = 7;
char optionsGO[][32] = {
	"Aimbot Options",
	"ESP Options",
	"No Recoil",
	"Advanced UAV",
	"No Sway",
	"Visible Warning",
	"Crosshair"
};
BOOL boptionsGO[7] = { FALSE };
BOOL *norecoilGO = &boptionsGO[2];
BOOL *advanceduavGO = &boptionsGO[3];
BOOL *noswayGO = &boptionsGO[4];
BOOL *VisibleWarningGO = &boptionsGO[5];
BOOL *CrosshairGO = &boptionsGO[6];

int ESPscrollGO = 1, ESPmaxoptionsGO = 9;
char ESPoptionsGO[][32] = {
	"ESP Type",
	"",
	"ESP Name",
	"ESP Distance",
	"ESP Weapon",
	"ESP Bones",
	"ESP Snap Lines",
	"ESP Enemy",
	"ESP Friendly"
};
BOOL ESPboptionsGO[9] = { FALSE };
BOOL *ESPNamesGO = &ESPboptionsGO[2];
BOOL *ESPDistanceGO = &ESPboptionsGO[3];
BOOL *ESPWeaponGO = &ESPboptionsGO[4];
BOOL *ESPBonesGO = &ESPboptionsGO[5];
BOOL *ESPSnaplinesGO = &ESPboptionsGO[6];
BOOL *ESPEnemysGO = &ESPboptionsGO[7];
BOOL *ESPFriendlyGO = &ESPboptionsGO[8];

int AimscrollGO = 1, AimmaxoptionsGO = 10;
char AimoptionsGO[][32] = {
	"Aimbot Type",
	"",
	"Auto Aim",
	"Auto Shoot",
	"Slow Spin",
	"Fast Spin",
	"Spin Bot",
	"Fake Aim Down",
	"Fake Aim Up",
	"Anti Aim"
};
BOOL AimboptionsGO[10] = { FALSE };
BOOL *AutoAimGO = &AimboptionsGO[2];
BOOL *AutoShootGO = &AimboptionsGO[3];
BOOL *SlowSpinBotGO = &AimboptionsGO[4];
BOOL *FastSpinBotGO = &AimboptionsGO[5];
BOOL *SpinBotGO = &AimboptionsGO[6];
BOOL *AimDownGO = &AimboptionsGO[7];
BOOL *AimUpGO = &AimboptionsGO[8];
BOOL *AntiAimGO = &AimboptionsGO[9];

char AimbotTypeGO[5][32] = {
	"   Nearest",
	"   Visible",
	"   Crosshair",
	"   Silent Aim",
	"   OFF"
};

char espTypeGO[4][32] = {
	"   2D",
	"   3D",
	"   Pyramid",
	"   OFF"
};

int GO_MAIN = 0, GO_AIMBOT = 1, GO_ESP = 2;
int currentGO = GO_MAIN;
int GO_NEAREST = 0, GO_ONSCREEN = 1, GO_CROSSHAIR = 2, GO_Silent = 3, GO_AIMBOT_OFF = 4;
int CurrentGOAimbot = GO_AIMBOT_OFF;
int GO_TWO_D = 0, GO_THREE_D = 1, GO_PYRAMID = 2, GO_ESP_OFF = 3;
int CurrentGOESP = GO_ESP_OFF;

VOID GO_Draw()
{
	char cNum[200];
	char count[250];
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;
	BOOL isFriend = FALSE;
	//GO_toogleHack();

	if (GO_Dvar_GetBOOL("cl_ingame"))
	{
		if (*CrosshairGO)
		{
			GO_DrawRect(CenterX - 10, CenterY, 21, 2, white);
			GO_DrawRect(CenterX, CenterY - 10, 2, 21, white);
		}

		GO_ReadGame();
		Vector vScreen;
		int playercount = 0;

		for (int i = 0; i < ENTITIESMAX; i++)
		{
			if ((GO_Entities[i]->Valid) && (GO_Entities[i]->WeaponID != 0x00) && (GO_Entities[i]->State & 1))
			{
				playercount++;

				D3DXVECTOR3 outpos;
				GO_cEntity *pEntity = GO_Entities[i];

				FLOAT DistancePlayer = GetDistance(GO_RefDef->EyePos, GO_Entities[i]->mPos);

				if (GO_wS2(pEntity->mPos, vScreen))
				{
					if (DistancePlayer > 3)
					{
						int x = vScreen[0];
						int y = vScreen[1];

						FLOAT fWidth = (60.0f * 15.0f) / DistancePlayer;
						FLOAT fHeight = (100.0f * 15.0f) / DistancePlayer;

						D3DXVECTOR3 vHead;
						short mTag = *(short*)GO_j_head;

						GO_GetTagPos(0, GO_Entities[i], mTag, vHead);

						D3DXVECTOR3 BoxPos;
						int newX = x + (fWidth / 2) + 1;
						GO_wS2(vHead, BoxPos);
						int newY = BoxPos.y;
						int HeadX = BoxPos.x;
						int HeadY = BoxPos.y;
						if ((GO_ci[i]->mTeam == GO_ci[GO_cg->cNum]->mTeam) && (strcmp(GO_cgs->gametype, "dm") != 0) && (strcmp(GO_cgs->gametype, "gun") != 0))
						{
							if (*ESPFriendlyGO)
							{
								if (*ESPNamesGO && *ESPDistanceGO)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s [^5%.fm^7]", GO_Clients[GO_ci[i]->mNum]->mName, GetDistance(GO_RefDef->EyePos, GO_Entities[i]->mPos));
									GO_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.3f, white);
								}
								else if (*ESPNamesGO)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s", GO_Clients[GO_ci[i]->mNum]->mName);
									GO_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.3f, white);
								}
								else if (*ESPDistanceGO)
								{
									char clientdetails[256];
									sprintf(clientdetails, "[^5%.fm^7]", GetDistance(GO_RefDef->EyePos, GO_Entities[i]->mPos));
									GO_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.3f, white);
								}

								if (*ESPWeaponGO)
								{
									char clientweapon[256];
									sprintf(clientweapon, "%s", GO_getWeaponname(GO_Entities[i]->WeaponID));
									GO_DrawText(clientweapon, x - strlen(clientweapon) * 2.5, y + GetTextPos1(DistancePlayer), 0.3f, white);
								}

								if (*ESPBonesGO)
								{
									GO_DrawBoneEsp(GO_Entities[i], white);
								}

								if (CurrentGOESP == GO_TWO_D)
								{
									GO_DrawPlayerBox(GO_Entities[i], mTag, fWidth, green);
								}
								else if (CurrentGOESP == GO_THREE_D)
								{
									GO_Make3DBox(GO_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, green);
								}
								else if (CurrentGOESP == GO_PYRAMID)
								{
									GO_MakePyramid(GO_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, green);
								}

								if (*ESPSnaplinesGO)
								{
									GO_DrawLine2(CenterX, CenterY, x, y, green, GO_White);
								}
							}
						}
						else
						{
							change_color = GO_CanSeePlayer(0, GO_Entities[i], 0x280F803) ? blue : red;
							change_shad_color = GO_CanSeePlayer(0, GO_Entities[i], 0x280F803) ? blue : red;

							if (*ESPEnemysGO)
							{
								if (*ESPNamesGO && *ESPDistanceGO)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s [^5%.fm^7]", GO_Clients[GO_ci[i]->mNum]->mName, GetDistance(GO_RefDef->EyePos, GO_Entities[i]->mPos));
									GO_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.3f, white);
								}
								else if (*ESPNamesGO)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s", GO_Clients[GO_ci[i]->mNum]->mName);
									GO_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.3f, white);
								}
								else if (*ESPDistanceGO)
								{
									char clientdetails[256];
									sprintf(clientdetails, "[^5%.fm^7]", GetDistance(GO_RefDef->EyePos, GO_Entities[i]->mPos));
									GO_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.3f, white);
								}

								if (*ESPWeaponGO)
								{
									char clientweapon[256];
									sprintf(clientweapon, "%s", GO_getWeaponname(GO_Entities[i]->WeaponID));
									GO_DrawText(clientweapon, x - strlen(clientweapon) * 2.5, y + GetTextPos1(DistancePlayer), 0.3f, white);
								}

								if (*ESPBonesGO)
								{
									GO_DrawBoneEsp(GO_Entities[i], white);
								}

								if (CurrentGOESP == GO_TWO_D)
								{
									GO_DrawPlayerBox(GO_Entities[i], mTag, fWidth, change_color);
								}
								else if (CurrentGOESP == GO_THREE_D)
								{
									GO_Make3DBox(GO_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, change_color);
								}
								else if (CurrentGOESP == GO_PYRAMID)
								{
									GO_MakePyramid(GO_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, change_color);
								}

								if (*ESPSnaplinesGO)
								{
									GO_DrawLine2(CenterX, CenterY, x, y, change_color, GO_White);
								}
							}

							if (*VisibleWarningGO)
							{
								if (GO_CanSeePlayer(0, GO_Entities[i], 0x280F803))
								{
									char msg[250];
									sprintf(msg, "*Enemy Visible*");
									GO_DrawText(msg, CenterX - 60, CenterY + 150, 0.4f, Purple);
								}
							}
						}
					}
				}
			}
		}
	}
}

VOID GO_Aimbot()
{
	if (GO_Dvar_GetBOOL("cl_ingame"))
	{
		DWORD pd = *(DWORD*)GO_clientActive_t;
		GO_clientActive *cmdInput = (GO_clientActive*)pd;
		GO_UserCmd_t* cmd = cmdInput->GetCmd(cmdInput->CmdNum);
		GO_clientActive *Ca = GO_getCA();

		GO_UserCmd_t* oldCmd = cmdInput->GetCmd(cmdInput->CmdNum - 1);

		GO_ReadGame();
		GO_cEntity * Target = NULL;
		GO_clientActive * ca = NULL;
		ca = (GO_clientActive*)GO_getCA();
		Vector vAngles;

		FLOAT objectDistance = 0.f;
		FLOAT flClosestDistance = (FLOAT)INT_MAX;

		D3DXVECTOR3 vHead, vBestHead;
		short mTag = *(short*)GO_j_neck;

		if (CurrentGOAimbot == GO_NEAREST)
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == GO_cg->cNum)
					continue;

				if ((GO_Entities[i]->Valid) && (GO_Entities[i]->State & 1))
				{
					if (strcmp(GO_cgs->gametype, "dm") == 0 || strcmp(GO_cgs->gametype, "gun") == 0)
					{
						objectDistance = GetDistance(GO_RefDef->EyePos, GO_Entities[i]->mPos);

						Target = GO_Entities[i];
						GO_GetTagPos(0, Target, mTag, vHead);

						if (objectDistance < flClosestDistance)
						{
							vBestHead = vHead;
							flClosestDistance = objectDistance;
						}
					}
					else
					{
						if (GO_ci[i]->mTeam != GO_ci[GO_cg->cNum]->mTeam)
						{
							objectDistance = GetDistance(GO_RefDef->EyePos, GO_Entities[i]->mPos);

							Target = GO_Entities[i];
							GO_GetTagPos(0, Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
				}
			}

			if (GO_Key_Down(0, 0x12) || *AutoAimGO)
			{
				if ((VALID(Target)) && (VALID(ca)))
				{
					vAngles = vectoangles(GetVec(GO_RefDef->EyePos, vBestHead));
					ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
					ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;

					if (*AutoShootGO)
					{
						oldCmd->Button |= 1;
					}
				}
			}
		}
		else if (CurrentGOAimbot == GO_ONSCREEN)
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == GO_cg->cNum)
					continue;

				if ((GO_Entities[i]->Valid) && (GO_Entities[i]->State & 1))
				{
					if (strcmp(GO_cgs->gametype, "dm") == 0 || strcmp(GO_cgs->gametype, "gun") == 0)
					{
						if (GO_CanSeePlayer(0, GO_Entities[i], 0x280F803))
						{
							objectDistance = GetDistance(GO_RefDef->EyePos, GO_Entities[i]->mPos);

							Target = GO_Entities[i];
							GO_GetTagPos(0, Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
					else
					{
						if (GO_ci[i]->mTeam != GO_ci[GO_cg->cNum]->mTeam)
						{
							if (GO_CanSeePlayer(0, GO_Entities[i], 0x280F803))
							{
								objectDistance = GetDistance(GO_RefDef->EyePos, GO_Entities[i]->mPos);

								Target = GO_Entities[i];
								GO_GetTagPos(0, Target, mTag, vHead);

								if (objectDistance < flClosestDistance)
								{
									vBestHead = vHead;
									flClosestDistance = objectDistance;
								}
							}
						}
					}
				}
			}

			if (GO_Key_Down(0, 0x12) || *AutoAimGO)
			{
				if ((VALID(Target)) && (VALID(ca)))
				{
					vAngles = vectoangles(GetVec(GO_RefDef->EyePos, vBestHead));
					ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
					ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;

					if (*AutoShootGO)
					{
						oldCmd->Button |= 1;
					}
				}
			}
		}
		else if (CurrentGOAimbot == GO_CROSSHAIR)
		{
			if (LastFrameAimKey && pSavedTarget2)
				Target = pSavedTarget1;
			else
				Target = GO_GetTargetByCrosshair();

			if (GO_Key_Down(0, 0x12) || *AutoAimGO)
			{
				if ((VALID(Target)) && (VALID(ca)))
				{
					D3DXVECTOR3 vHead;

					GO_GetTagPos(0, Target, mTag, vHead);

					vAngles = vectoangles(GetVec(GO_RefDef->EyePos, vHead));

					if (*AutoShootGO)
					{
						oldCmd->Button |= 1;
					}

					ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
					ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;
				}
			}
			pSavedTarget1 = Target;
			LastFrameAimKey = TRUE;
		}
	}
}

VOID SlowSpinBotGOThread(GO_UserCmd_t *UserCommand) {
	Vector SpinBotView;
	int SpinBotAngles = -1;

	SpinBotAngles++;
	if (SpinBotAngles > 360) {
		SpinBotAngles = 0;
	}

	SpinBotView.y = SpinBotAngles;

	UserCommand->ViewAngle[1] = AngleToShort(SpinBotView.y);
}

VOID SpinBotGOThread(GO_UserCmd_t *UserCommand) {
	Vector SpinBotView;
	int SpinBotAngles = -1;

	SpinBotAngles++;
	SpinBotAngles * 4;
	if (SpinBotAngles > 360) {
		SpinBotAngles = 0;
	}

	SpinBotView.y = SpinBotAngles;

	UserCommand->ViewAngle[1] = AngleToShort(SpinBotView.y);
}

VOID FastSpinBotGOThread(GO_UserCmd_t *UserCommand) {
	Vector SpinBotView;
	int SpinBotAngles = -1;

	SpinBotAngles++ * 8;
	if (SpinBotAngles > 360) {
		SpinBotAngles = 0;
	}

	SpinBotView.y = SpinBotAngles;

	UserCommand->ViewAngle[1] = AngleToShort(SpinBotView.y);
}

VOID AimDownGOThread(GO_UserCmd_t *UserCommand) {
	Vector SpinBotAngles;
	SpinBotAngles.x = 80;
	if (SpinBotAngles.x != 80)
		SpinBotAngles.x = 80;
	UserCommand->ViewAngle[0] = AngleToShort(SpinBotAngles.x);
}

VOID AimUpGOThread(GO_UserCmd_t *UserCommand) {
	Vector SpinBotAngles;
	SpinBotAngles.x = -80;
	if (SpinBotAngles.x != -80)
		SpinBotAngles.x = -80;
	UserCommand->ViewAngle[0] = AngleToShort(SpinBotAngles.x);
}

VOID AntiAimGOThread(GO_UserCmd_t *UserCommand)
{
	UserCommand->ViewAngle[0] = 180 + int(rand() % 10);
	UserCommand->ViewAngle[1] = (vec_t)fmod(-360.0f, 360.0f) + int(rand() % 10);
	UserCommand->ViewAngle[2] = UserCommand->ViewAngle[1] + 110 + int(rand() % 10);
	if (UserCommand->ViewAngle[0] < -180.0f)UserCommand->ViewAngle[0] += 360.0f;
	if (UserCommand->ViewAngle[0] > 180.0f)UserCommand->ViewAngle[0] -= 360.0f;
	if (UserCommand->ViewAngle[1] < -180.0f)UserCommand->ViewAngle[1] += 360.0f;
	if (UserCommand->ViewAngle[1] > 180.0f)UserCommand->ViewAngle[1] -= 360.0f;
	if (UserCommand->ViewAngle[2] < -180.0f)UserCommand->ViewAngle[2] += 360.0f;
	if (UserCommand->ViewAngle[2] > 180.0f)UserCommand->ViewAngle[2] -= 360.0f;
}

VOID GO_CL_WritePackethk(int a1)
{
	if (GO_Dvar_GetBOOL("cl_ingame"))
	{
		DWORD pd = *(DWORD*)GO_clientActive_t;
		GO_clientActive *cmdInput = (GO_clientActive*)pd;
		GO_UserCmd_t* cmd = cmdInput->GetCmd(cmdInput->CmdNum);
		GO_clientActive *Ca = GO_getCA();

		GO_UserCmd_t* oldCmd = cmdInput->GetCmd(cmdInput->CmdNum - 1);
		Vector vMyAngles = Ca->ViewAngles;

		*oldCmd = *cmd;
		oldCmd->ServerTime--;

		GO_ReadGame();
		GO_cEntity * Target = NULL;
		GO_clientActive * ca = NULL;
		ca = (GO_clientActive*)GO_getCA();
		Vector vAngles;

		FLOAT objectDistance = 0.f;
		FLOAT flClosestDistance = (FLOAT)INT_MAX;

		D3DXVECTOR3 vHead, vBestHead;
		short mTag = *(short*)GO_j_neck;

		if (CurrentGOAimbot == GO_Silent)
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == GO_cg->cNum)
					continue;

				if ((GO_Entities[i]->Valid) && (GO_Entities[i]->State & 1))
				{
					if (strcmp(GO_cgs->gametype, "dm") == 0 || strcmp(GO_cgs->gametype, "gun") == 0)
					{
						if (GO_CanSeePlayer(0, GO_Entities[i], 0x280F803))
						{
							objectDistance = GetDistance(GO_RefDef->EyePos, GO_Entities[i]->mPos);

							Target = GO_Entities[i];
							GO_GetTagPos(0, Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
					else
					{
						if (GO_ci[i]->mTeam != GO_ci[GO_cg->cNum]->mTeam)
						{
							if (GO_CanSeePlayer(0, GO_Entities[i], 0x280F803))
							{
								objectDistance = GetDistance(GO_RefDef->EyePos, GO_Entities[i]->mPos);

								Target = GO_Entities[i];
								GO_GetTagPos(0, Target, mTag, vHead);

								if (objectDistance < flClosestDistance)
								{
									vBestHead = vHead;
									flClosestDistance = objectDistance;
								}
							}
						}
					}
				}
			}

			if ((VALID(Target)) && (VALID(ca)))
			{
				vAngles = vectoangles(GetVec(GO_RefDef->EyePos, vBestHead));
				SilentAngles.x = vAngles.x - ca->RefAngles.x;
				SilentAngles.y = vAngles.y - ca->RefAngles.y;
				SilentAngles.z = 0;

				vMyAngles.x = SilentAngles.x;
				vMyAngles.y = SilentAngles.y;

				oldCmd->moveTonpere = cmd->moveTonpere;
				oldCmd->ViewAngle[0] = ANGLE2SHORT(vMyAngles.x);
				oldCmd->ViewAngle[1] = ANGLE2SHORT(vMyAngles.y);

				if (*AutoShootGO)
				{
					oldCmd->Button |= 1;
				}
			}
		}

		if (*SlowSpinBotGO)
		{
			SlowSpinBotGOThread(cmd);
		}

		if (*SpinBotGO)
		{
			SpinBotGOThread(cmd);
		}

		if (*FastSpinBotGO)
		{
			FastSpinBotGOThread(cmd);
		}

		if (*AimDownGO)
		{
			AimDownGOThread(cmd);
		}

		if (*AimUpGO)
		{
			AimUpGOThread(cmd);
		}

		if (*AntiAimGO)
		{
			AntiAimGOThread(cmd);
		}
	}

	SaveStubPacket(a1);
}

VOID drawMaterialGO(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT angle, const FLOAT *color) {
	GO_DrawRotated(GO_DrawRotated_R3, x, y, width, height, angle, color, GO_White);
}

VOID drawRectangleGO(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT lineWidth, const FLOAT *color) {
	drawMaterialGO(x, y, width, lineWidth, 0.0f, color);
	drawMaterialGO(x + width, y, lineWidth, height, 0.0f, color);
	drawMaterialGO(x, y + height - lineWidth, width, lineWidth, 0.0f, color);
	drawMaterialGO(x, y, lineWidth, height, 0.0f, color);
}

XINPUT_STATE XInputStateGO;
short currentButtonGO, previousButtonGO;
#define buttonHitGO(button) ((currentButtonGO & ~previousButtonGO) & button)
VOID updateInputGO() {
	XInputGetState(0, &XInputStateGO);
	previousButtonGO = currentButtonGO;
	currentButtonGO = ~XInputStateGO.Gamepad.wButtons & 0xFBFF;

	if (buttonHitGO(XINPUT_GAMEPAD_DPAD_LEFT)) {
		openGO = !openGO;
	}

	if (openGO) {
		if (currentGO == GO_AIMBOT || currentGO == GO_ESP) {
			if (buttonHitGO(XINPUT_GAMEPAD_B)) {
				currentGO = GO_MAIN;
			}
		}

		if (buttonHitGO(XINPUT_GAMEPAD_DPAD_UP)) {
			if (currentGO == GO_MAIN)
			{
				scrollGO--;
				if (scrollGO == -1) {
					scrollGO = maxoptionsGO - 1;
				}
			}
			else
			{
				if (currentGO == GO_AIMBOT)
				{
					AimscrollGO--;
					if (AimscrollGO == 0 || AimscrollGO == -1) {
						AimscrollGO = AimmaxoptionsGO - 1;
					}
				}
				else
				{
					if (currentGO == GO_ESP)
					{
						ESPscrollGO--;
						if (ESPscrollGO == 0 || ESPscrollGO == -1) {
							ESPscrollGO = ESPmaxoptionsGO - 1;
						}
					}
				}
			}
		}

		if (buttonHitGO(XINPUT_GAMEPAD_DPAD_DOWN)) {
			if (currentGO == GO_MAIN)
			{
				scrollGO++;
				if (scrollGO == maxoptionsGO) {
					scrollGO = 0;
				}
			}
			else
			{
				if (currentGO == GO_AIMBOT)
				{
					AimscrollGO++;
					if (AimscrollGO == 0 || AimscrollGO == AimmaxoptionsGO) {
						AimscrollGO = 1;
					}
				}
				else
				{
					if (currentGO == GO_ESP)
					{
						ESPscrollGO++;
						if (ESPscrollGO == 0 || ESPscrollGO == ESPmaxoptionsGO) {
							ESPscrollGO = 1;
						}
					}
				}
			}
		}

		if (buttonHitGO(XINPUT_GAMEPAD_X)) {
			if (currentGO == GO_MAIN)
			{
				if (scrollGO == 0)
				{
					currentGO = GO_AIMBOT;
				}
				else
				{
					if (scrollGO == 1)
					{
						currentGO = GO_ESP;
					}
					else
					{
						boptionsGO[scrollGO] = !boptionsGO[scrollGO];
					}
				}
			}
			else
			{
				if (currentGO == GO_AIMBOT)
				{
					if (AimscrollGO == 1) {
						switch (CurrentGOAimbot) {
						case 0:
							CurrentGOAimbot = GO_ONSCREEN;
							break;
						case 1:
							CurrentGOAimbot = GO_CROSSHAIR;
							break;
						case 2:
							CurrentGOAimbot = GO_Silent;
							break;
						case 3:
							CurrentGOAimbot = GO_AIMBOT_OFF;
							break;
						case 4:
							CurrentGOAimbot = GO_NEAREST;
							break;
						default:
							CurrentGOAimbot = GO_AIMBOT_OFF;
							break;
						}
					}
					else {
						AimboptionsGO[AimscrollGO] = !AimboptionsGO[AimscrollGO];
					}
				}
				else
				{
					if (currentGO == GO_ESP)
					{
						if (ESPscrollGO == 1) {
							switch (CurrentGOESP) {
							case 0:
								CurrentGOESP = GO_THREE_D;
								break;
							case 1:
								CurrentGOESP = GO_PYRAMID;
								break;
							case 2:
								CurrentGOESP = GO_ESP_OFF;
								break;
							case 3:
								CurrentGOESP = GO_TWO_D;
								break;
							default:
								CurrentGOESP = GO_ESP_OFF;
								break;
							}
						}
						else {
							ESPboptionsGO[ESPscrollGO] = !ESPboptionsGO[ESPscrollGO];
						}
					}
				}
			}
		}
	}
}

VOID updateOptionsGO()
{
	if (*norecoilGO)
	{
		*(int*)GO_NoRecoil = 0x60000000;
	}
	else
	{
		*(int*)GO_NoRecoil = 0x4BF8AE4D;
	}
	if (*advanceduavGO)
	{
		*(int*)GO_Radar = 0x3B600001;
	}
	else
	{
		*(int*)GO_Radar = 0x551B7FFE;
	}
	if (*noswayGO)
	{
		*(int*)GO_WeaponSway = 0x60000000;
		*(int*)GO_ZommingSway = 0x60000000;
	}
	else
	{
		*(int*)GO_WeaponSway = 0x4BFFEAA1;
		*(int*)GO_ZommingSway = 0x4BFFFBA5;
	}
}

VOID GO_Menu_Hook()
{
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;
	FLOAT middleGO = 950.0f;
	FLOAT centerGO = 100.0f;
	FLOAT bottomGO = 700.0f;

	updateInputGO();
	updateOptionsGO();

	if (GO_Dvar_GetBOOL("cl_ingame"))
	{
		if (!FirstTimeInGameGO)
		{
			openGO = FALSE;
			currentGO = 0;
			CurrentGOESP = 3;
			CurrentGOAimbot = 4;
			scrollGO = 0;
			AimscrollGO = 1;
			ESPscrollGO = 1;
			FirstTimeInGameGO = TRUE;
		}

		if (openGO)
		{
			if (currentGO == GO_MAIN)
			{
				drawMaterialGO(middleGO - 1, centerGO, 250.0f, 30.0f + (maxoptionsGO * 30.0f), 0, L_black);
				drawRectangleGO(middleGO - 1, centerGO, 250.0f, 30.0f + (maxoptionsGO * 30.0f), 2.0f, Purple);
				GO_DrawText("OutHere Menu", middleGO + 20, centerGO - 1, 0.4f, Purple);
				GO_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomGO, 0.3f, white);

				for (int i = 0; i < maxoptionsGO; i++) {
					if (i == scrollGO) {
						GO_DrawText(optionsGO[i], middleGO + 20, centerGO + 40.0f + (30.0f * i), 0.3f, Purple);
					}
					else {
						GO_DrawText(optionsGO[i], middleGO + 20, centerGO + 40.0f + (30.0f * i), 0.3f, white);
					}
					if (i == 0)
					{
						// fix
					}
					else if (i == 1)
					{
						// fix
					}
					else if (boptionsGO[i])
					{
						GO_DrawText("ON", middleGO + 190, centerGO + 40.0f + (30.0f * i), 0.3f, L_green2);
					}
					else
					{
						GO_DrawText("OFF", middleGO + 190, centerGO + 40.0f + (30.0f * i), 0.3f, Purple);
					}
				}
			}
			else
			{
				if (currentGO == GO_AIMBOT)
				{
					drawMaterialGO(middleGO - 1, centerGO, 250.0f, 30.0f + (AimmaxoptionsGO * 30.0f), 0, L_black);
					drawRectangleGO(middleGO - 1, centerGO, 250.0f, 30.0f + (AimmaxoptionsGO * 30.0f), 2.0f, Purple);
					GO_DrawText("Aimbot Options", middleGO + 20, centerGO - 1, 0.4f, Purple);
					GO_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomGO, 0.3f, white);

					for (int i = 0; i < AimmaxoptionsGO; i++) {
						if (i == AimscrollGO) {
							GO_DrawText(AimoptionsGO[i], middleGO + 20, centerGO + 40.0f + (30.0f * i), 0.3f, Purple);
						}
						else {
							GO_DrawText(AimoptionsGO[i], middleGO + 20, centerGO + 40.0f + (30.0f * i), 0.3f, white);
						}
						if (i == 0) {
							// fix
						}
						else if (i == 1) {
							GO_DrawText(AimbotTypeGO[CurrentGOAimbot], middleGO + 20, centerGO + 40.0f + (30.0f * i), 0.4f, AimscrollGO == 1 ? Purple : white);//Menu Text
						}
						else {
							if (AimboptionsGO[i])
							{
								GO_DrawText("ON", middleGO + 190, centerGO + 40.0f + (30.0f * i), 0.3f, L_green2);
							}
							else
							{
								GO_DrawText("OFF", middleGO + 190, centerGO + 40.0f + (30.0f * i), 0.3f, Purple);
							}
						}
					}
				}
				else
				{
					if (currentGO == GO_ESP)
					{
						drawMaterialGO(middleGO - 1, centerGO, 250.0f, 30.0f + (ESPmaxoptionsGO * 30.0f), 0, L_black);
						drawRectangleGO(middleGO - 1, centerGO, 250.0f, 30.0f + (ESPmaxoptionsGO * 30.0f), 2.0f, Purple);
						GO_DrawText("ESP Options", middleGO + 20, centerGO - 1, 0.4f, Purple);
						GO_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomGO, 0.3f, white);

						for (int i = 0; i < ESPmaxoptionsGO; i++) {
							if (i == ESPscrollGO) {
								GO_DrawText(ESPoptionsGO[i], middleGO + 20, centerGO + 40.0f + (30.0f * i), 0.3f, Purple);
							}
							else {
								GO_DrawText(ESPoptionsGO[i], middleGO + 20, centerGO + 40.0f + (30.0f * i), 0.3f, white);
							}
							if (i == 0) {
								// fix
							}
							else if (i == 1) {
								GO_DrawText(espTypeGO[CurrentGOESP], middleGO + 20, centerGO + 40.0f + (30.0f * i), 0.4f, ESPscrollGO == 1 ? Purple : white);//Menu Text
							}
							else {
								if (ESPboptionsGO[i])
								{
									GO_DrawText("ON", middleGO + 190, centerGO + 40.0f + (30.0f * i), 0.3f, L_green2);
								}
								else
								{
									GO_DrawText("OFF", middleGO + 190, centerGO + 40.0f + (30.0f * i), 0.3f, Purple);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			GO_DrawText("Press D-PAD Left To ^2Open^7 OutHere Menu", 30.0f, bottomGO, 0.3f, white);//Bottom shit
		}
	}
	else
	{
		openGO = FALSE;
	}
}

HRESULT GO_CL_DrawTextHook(const char *text, int maxChars, DWORD font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, const FLOAT *color, int style)
{
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;

	FLOAT rotation = 0;
	GO_R_AddCmdDrawText(text, maxChars, font, x, y, xScale, yScale, rotation, color, style);

	//GO_dMenu.BuildMenu("\n\n^2Masked Menu^7",(CenterX - 160),50,280,150,black,blackM,Purple);

	if (GO_Key_Down(0, 0x12))
	{
		//printf("aiming");
	}
	else
	{
		LastFrameAimKey = FALSE;
	}

	GO_Menu_Hook();
	GO_Draw();
	GO_Aimbot();

	return S_OK;
}

VOID GO_Start()
{
	PatchInJump((DWORD *)0x8253E140, (DWORD)GO_CL_DrawTextHook, FALSE);
	HookFunctionStart((DWORD *)0x822D4918, (PDWORD)SaveStubPacket, (DWORD)GO_CL_WritePackethk);
}

/////////////////////////////////////// Ghosts Menu END /////////////////////////////////////

/////////////////////////////////////// BO2 Menu /////////////////////////////////////

BOOL resetpromod = FALSE;
BOOL SnapDoneBO2 = FALSE;
BOOL CanDrawTargetedESPBO2 = FALSE;
BOOL DrawTargetDetailsBO2 = FALSE;
BOOL FirstTimeInGameBO2 = FALSE;
BOOL openBO2 = FALSE;
BOOL resetnightvis = FALSE;
int scrollBO2 = 0, maxoptionsBO2 = 4;
char optionsBO2[][32] = {
	"Main Menu",
	"Aimbot Menu",
	"ESP Menu",
	"Misc Menu"
};
BOOL boptionsBO2[4] = { FALSE };

int MainscrollBO2 = 0, MainmaxoptionsBO2 = 7;
char MainoptionsBO2[][32] = {
	"No Recoil",
	"Advanced UAV",
	"No Sway",
	"Red Boxes",
	"Chams",
	"Crosshair",
	"Pro Mod"
};
BOOL MainboptionsBO2[7] = { FALSE };
BOOL *norecoilBO2 = &MainboptionsBO2[0];
BOOL *advanceduavBO2 = &MainboptionsBO2[1];
BOOL *noswayBO2 = &MainboptionsBO2[2];
BOOL *redboxesBO2 = &MainboptionsBO2[3];
BOOL *chamsBO2 = &MainboptionsBO2[4];
BOOL *promodBO2 = &MainboptionsBO2[6];

int ESPscrollBO2 = 0, ESPmaxoptionsBO2 = 9;
char ESPoptionsBO2[][32] = {
	"ESP",
	"ESP Type:",
	"ESP Name",
	"ESP Distance",
	"ESP Weapon",
	"ESP Team:",
	"Snap Lines",
	"Snap Line Pos:",
	"Snap Line Team:"
};
BOOL ESPboptionsBO2[9] = { FALSE };
BOOL *EnableESPBO2 = &ESPboptionsBO2[0];
BOOL *ESPNamesBO2 = &ESPboptionsBO2[2];
BOOL *ESPDistanceBO2 = &ESPboptionsBO2[3];
BOOL *ESPWeaponBO2 = &ESPboptionsBO2[4];
BOOL *ESPSnapLinesBO2 = &ESPboptionsBO2[6];

int AimscrollBO2 = 0, AimmaxoptionsBO2 = 6;
char AimoptionsBO2[][32] = {
	"Aimbot",
	"Aimbot Type:",
	"Auto Aim",
	"Visible Warning",
	"Proximity Warning",
	"Target Details"
};
BOOL AimboptionsBO2[6] = { FALSE };
BOOL *AimbotBO2 = &AimboptionsBO2[0];
BOOL *AutoAimBO2 = &AimboptionsBO2[2];
BOOL *VisibleWarningBO2 = &AimboptionsBO2[3];
BOOL *ProximityWarningBO2 = &AimboptionsBO2[4];
BOOL *TargetDetailsBO2 = &AimboptionsBO2[5];

int MiscscrollBO2 = 0, MiscmaxoptionsBO2 = 5;
char MiscoptionsBO2[][32] = {
	"Scroll Bar",
	"Watermark",
	"Draw ON/OFF",
	"Show Host",
	"End Game"
};
BOOL MiscboptionsBO2[5] = { FALSE };
BOOL *scrollbarBO2 = &MiscboptionsBO2[0];
BOOL *watermarkBO2 = &MiscboptionsBO2[1];
BOOL *drawonoffBO2 = &MiscboptionsBO2[2];
BOOL *showhostBO2 = &MiscboptionsBO2[3];
BOOL *endgameBO2 = &MiscboptionsBO2[4];

char AimbotTypeBO2[4][32] = {
	"Nearest",
	"On Screen",
	"Crosshair",
	"Snap"
};

char espTypeBO2[5][32] = {
	"2D",
	"3D",
	"Pyramid",
	"Filled",
	"Corner"
};

char ESPTeamTypeBO2[3][32] = {
	"Enemy",
	"Friendly",
	"Both"
};

char SnapLineTypeBO2[3][32] = {
	"Middle",
	"Bottom",
	"Top"
};

char SnapLineTeamTypeBO2[3][32] = {
	"Enemy",
	"Friendly",
	"Both"
};

char CrossHairTypeBO2[2][32] = {
	"+",
	"X"
};

int BO2_MENU = 0, BO2_MAIN_MENU = 1, BO2_AIMBOT = 2, BO2_ESP = 3, BO2_MISC = 4;
int currentBO2 = BO2_MENU;
int BO2_NEAREST = 0, BO2_ONSCREEN = 1, BO2_CROSSHAIR = 2, BO2_SNAP = 3;
int CurrentBO2Aimbot = BO2_NEAREST;
int BO2_TWO_D = 0, BO2_THREE_D = 1, BO2_PYRAMID = 2, BO2_FILLED = 3, BO2_CORNER = 4;
int CurrentBO2ESP = BO2_TWO_D;
int ESPEnemyBO2 = 0, ESPFriendlyBO2 = 1, ESPBothBO2 = 2;
int ESPBO2 = ESPEnemyBO2;
int SnapLineMiddleBO2 = 0, SnapLineBottomBO2 = 1, SnapLineTopBO2 = 2;
int SnapLinePosBO2 = SnapLineMiddleBO2;
int SnapLineEnemyBO2 = 0, SnapLineFriendlyBO2 = 1, SnapLineTeamBothBO2 = 2;
int SnapLineTeamBO2 = SnapLineEnemyBO2;
int PlusCrosshairBO2 = 0, XCrosshairBO2 = 1, CrosshairOffBO2 = 2;
int CrossHairBO2 = CrosshairOffBO2;

VOID BO2_Aimbot()
{
	if (strcmp(BO2_cgs->gametype, "zclassic") != 0 || strcmp(BO2_cgs->gametype, "zstandard") != 0)
	{
		if (BO2_Dvar_GetBOOL("cl_ingame"))
		{
			BO2_ReadGame();
			BO2_cEntity * Target = NULL;
			BO2_clientActive * ca = NULL;
			ca = (BO2_clientActive*)BO2_getCA();
			Vector vAngles;

			FLOAT objectDistance = 0.f;
			FLOAT flClosestDistance = (FLOAT)INT_MAX;

			D3DXVECTOR3 vHead, vBestHead;
			short mTag = *(short*)BO2_j_neck;

			if (*AimbotBO2)
			{
				if (CurrentBO2Aimbot == BO2_NEAREST)
				{
					for (int i = 0; i < 12; i++)
					{
						if (i == BO2_cg->cNum)
							continue;

						if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
						{
							if (strcmp(BO2_cgs->gametype, "dm") == 0 || strcmp(BO2_cgs->gametype, "gun") == 0 || strcmp(BO2_cgs->gametype, "oic") == 0 || strcmp(BO2_cgs->gametype, "shrp") == 0 || strcmp(BO2_cgs->gametype, "sas") == 0)
							{
								objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

								Target = BO2_Entities[i];
								BO2_GetTagPos(Target, mTag, vHead);

								if (objectDistance < flClosestDistance)
								{
									vBestHead = vHead;
									flClosestDistance = objectDistance;
								}
							}
							else
							{
								if (BO2_Clients[i]->mTeam != BO2_Clients[BO2_cg->cNum]->mTeam)
								{
									objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

									Target = BO2_Entities[i];
									BO2_GetTagPos(Target, mTag, vHead);

									if (objectDistance < flClosestDistance)
									{
										vBestHead = vHead;
										flClosestDistance = objectDistance;
									}
								}
							}
						}
					}

					if ((VALID(Target)) && (VALID(ca)))
					{
						CanDrawTargetedESPBO2 = TRUE;
						DrawTargetDetailsBO2 = TRUE;
						if (BO2_Key_Down(0, 0x12) || *AutoAimBO2)
						{
							vAngles = vectoangles(GetVec(BO2_RefDef->EyePos, vBestHead));
							ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
							ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;
						}
					}
				}
				else if (CurrentBO2Aimbot == BO2_ONSCREEN)
				{
					for (int i = 0; i < 12; i++)
					{
						if (i == BO2_cg->cNum)
							continue;

						if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
						{
							if (strcmp(BO2_cgs->gametype, "dm") == 0 || strcmp(BO2_cgs->gametype, "gun") == 0 || strcmp(BO2_cgs->gametype, "oic") == 0 || strcmp(BO2_cgs->gametype, "shrp") == 0 || strcmp(BO2_cgs->gametype, "sas") == 0)
							{
								if (BO2_CanSeeFriendlyHead(0, BO2_Entities[i]) == 1)
								{
									objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

									Target = BO2_Entities[i];
									BO2_GetTagPos(Target, mTag, vHead);

									if (objectDistance < flClosestDistance)
									{
										vBestHead = vHead;
										flClosestDistance = objectDistance;
									}
								}
							}
							else
							{
								if (BO2_Clients[i]->mTeam != BO2_Clients[BO2_cg->cNum]->mTeam)
								{
									if (BO2_CanSeeFriendlyHead(0, BO2_Entities[i]) == 1)
									{
										objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

										Target = BO2_Entities[i];
										BO2_GetTagPos(Target, mTag, vHead);

										if (objectDistance < flClosestDistance)
										{
											vBestHead = vHead;
											flClosestDistance = objectDistance;
										}
									}
								}
							}
						}
					}

					if ((VALID(Target)) && (VALID(ca)))
					{
						CanDrawTargetedESPBO2 = TRUE;
						DrawTargetDetailsBO2 = TRUE;
						if (BO2_Key_Down(0, 0x12) || *AutoAimBO2)
						{
							vAngles = vectoangles(GetVec(BO2_RefDef->EyePos, vBestHead));
							ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
							ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;
						}
					}
				}
				else if (CurrentBO2Aimbot == BO2_CROSSHAIR)
				{
					if (LastFrameAimKey && pSavedTarget4)
						Target = pSavedTarget4;
					else
						Target = BO2_GetTargetByCrosshair();

					if ((VALID(Target)) && (VALID(ca)))
					{
						CanDrawTargetedESPBO2 = TRUE;
						DrawTargetDetailsBO2 = TRUE;
						if (BO2_Key_Down(0, 0x12) || *AutoAimBO2)
						{
							D3DXVECTOR3 vHead;
							BO2_GetTagPos(Target, mTag, vHead);

							vAngles = vectoangles(GetVec(BO2_RefDef->EyePos, vHead));
							ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
							ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;
						}
						pSavedTarget4 = Target;
						LastFrameAimKey = TRUE;
					}
				}
				else if (CurrentBO2Aimbot == BO2_SNAP)
				{
					for (int i = 0; i < 12; i++)
					{
						if (i == BO2_cg->cNum)
							continue;

						if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
						{
							if (strcmp(BO2_cgs->gametype, "dm") == 0 || strcmp(BO2_cgs->gametype, "gun") == 0 || strcmp(BO2_cgs->gametype, "oic") == 0 || strcmp(BO2_cgs->gametype, "shrp") == 0 || strcmp(BO2_cgs->gametype, "sas") == 0)
							{
								if (BO2_CanSeeFriendlyHead(0, BO2_Entities[i]) == 1)
								{
									objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

									Target = BO2_Entities[i];
									BO2_GetTagPos(Target, mTag, vHead);

									if (objectDistance < flClosestDistance)
									{
										vBestHead = vHead;
										flClosestDistance = objectDistance;
									}
								}
							}
							else
							{
								if (BO2_Clients[i]->mTeam != BO2_Clients[BO2_cg->cNum]->mTeam)
								{
									if (BO2_CanSeeFriendlyHead(0, BO2_Entities[i]) == 1)
									{
										objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

										Target = BO2_Entities[i];
										BO2_GetTagPos(Target, mTag, vHead);

										if (objectDistance < flClosestDistance)
										{
											vBestHead = vHead;
											flClosestDistance = objectDistance;
										}
									}
								}
							}
						}
					}

					if ((VALID(Target)) && (VALID(ca)))
					{
						SnapDoneBO2 = FALSE;
						CanDrawTargetedESPBO2 = TRUE;
						DrawTargetDetailsBO2 = TRUE;
						if (BO2_Key_Down(0, 0x12) || *AutoAimBO2)
						{
							vAngles = vectoangles(GetVec(BO2_RefDef->EyePos, vBestHead));
							ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
							ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;
						}
					}
					else
					{
						if (!SnapDoneBO2)
						{
							ca->ViewAngles.y = ca->ViewAngles.y - 45;
							SnapDoneBO2 = TRUE;
						}
					}
				}
			}
		}
	}
}

/*VOID SpinBotBO2Thread(BO2_UserCMD *UserCommand) {
Vector SpinBotAngles;
SpinBotAngles.y += 45 * (rand() % 8);
UserCommand->ViewAngle[1] += AngleToShort(SpinBotAngles.y);
}

VOID BO2_CL_WritePackethk(int a1)
{
if (BO2_Dvar_GetBOOL("cl_ingame"))
{
DWORD pd = *(DWORD*)BO2_clientActive_t;
BO2_clientActive *cmdInput = (BO2_clientActive*)pd;
BO2_UserCMD* cmd = cmdInput->GetCmd(cmdInput->CmdNum);
BO2_UserCMD* oldCmd = cmdInput->GetCmd(cmdInput->CmdNum - 1);

BO2_clientActive *Ca = BO2_getCA();
Vector vMyAngles = Ca->ViewAngles;

*oldCmd = *cmd;
oldCmd->ServerTime--;

BO2_ReadGame();
BO2_cEntity * Target = NULL;
BO2_clientActive * ca = NULL;
ca = (BO2_clientActive*)BO2_getCA();
Vector vAngles;

FLOAT objectDistance = 0.f;
FLOAT flClosestDistance = (FLOAT)INT_MAX;

D3DXVECTOR3 vHead, vBestHead;
short mTag = *(short*)BO2_j_neck;;

if (CurrentBO2Aimbot == BO2_Silent)
{
for (int i = 0; i < 12; i++)
{
if (i == BO2_cg->cNum)
continue;

if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
{
if (strcmp(BO2_cgs->gametype, "dm") == 0 || strcmp(BO2_cgs->gametype, "gun") == 0 || strcmp(BO2_cgs->gametype, "oic") == 0 || strcmp(BO2_cgs->gametype, "shrp") == 0 || strcmp(BO2_cgs->gametype, "sas") == 0)
{
if (BO2_CanSeeFriendlyHead(0, BO2_Entities[i]) == 1)
{
objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

Target = BO2_Entities[i];
BO2_GetTagPos(Target, mTag, vHead);

if (objectDistance < flClosestDistance)
{
vBestHead = vHead;
flClosestDistance = objectDistance;
}
}
}
else
{
if (BO2_Clients[i]->mTeam != BO2_Clients[BO2_cg->cNum]->mTeam)
{
if (BO2_CanSeeFriendlyHead(0, BO2_Entities[i]) == 1)
{
objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

Target = BO2_Entities[i];
BO2_GetTagPos(Target, mTag, vHead);

if (objectDistance < flClosestDistance)
{
vBestHead = vHead;
flClosestDistance = objectDistance;
}
}
}
}
}
}

if ((VALID(Target)) && (VALID(ca)))
{
vAngles = vectoangles(GetVec(BO2_RefDef->EyePos, vBestHead));
SilentAngles.x = vAngles.x - ca->RefAngles.x;
SilentAngles.y = vAngles.y - ca->RefAngles.y;
SilentAngles.z = 0;

vMyAngles.x = SilentAngles.x;
vMyAngles.y = SilentAngles.y;

oldCmd->moveTonpere = cmd->moveTonpere;
oldCmd->ViewAngle[0] = ANGLE2SHORT(vMyAngles.x);
oldCmd->ViewAngle[1] = ANGLE2SHORT(vMyAngles.y);

if (*AutoShootBO2)
{
oldCmd->Button |= 1;
}
}
}

//if (!BO2_Key_Down(0, 0x12))
//{
if (*SpinBotBO2)
{
SpinBotBO2Thread(cmd);
}
//}
}

SaveStubPacket(a1);
}*/

VOID drawMaterialBO2(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT angle, const FLOAT *color) {
	BO2_DrawRotated(BO2_DrawRotated_R3, x, y, width, height, angle, color, BO2_White);
}

VOID drawRectangleBO2(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT lineWidth, const FLOAT *color) {
	drawMaterialBO2(x, y, width, lineWidth, 0.0f, color);
	drawMaterialBO2(x + width, y, lineWidth, height, 0.0f, color);
	drawMaterialBO2(x, y + height - lineWidth, width, lineWidth, 0.0f, color);
	drawMaterialBO2(x, y, lineWidth, height, 0.0f, color);
}

int ClosetsClientBO2()
{
	BO2_ReadGame();
	FLOAT objectDistance = 0.f;
	FLOAT flClosestDistance = (FLOAT)INT_MAX;
	int ClosestClient = -1;

	for (int i = 0; i < 12; i++)
	{
		if (i == BO2_cg->cNum)
			continue;

		if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
		{
			if (strcmp(BO2_cgs->gametype, "dm") == 0 || strcmp(BO2_cgs->gametype, "gun") == 0 || strcmp(BO2_cgs->gametype, "oic") == 0 || strcmp(BO2_cgs->gametype, "shrp") == 0 || strcmp(BO2_cgs->gametype, "sas") == 0)
			{
				objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

				if (objectDistance < flClosestDistance)
				{
					flClosestDistance = objectDistance;
					ClosestClient = i;
				}
			}
			else
			{
				if (BO2_Clients[i]->mTeam != BO2_Clients[BO2_cg->cNum]->mTeam)
				{
					objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

					if (objectDistance < flClosestDistance)
					{
						flClosestDistance = objectDistance;
						ClosestClient = i;
					}
				}
			}
		}
	}

	return ClosestClient;
}

VOID ClientDetailsBO2()
{
	if (BO2_Dvar_GetBOOL("cl_ingame"))
	{
		if (*TargetDetailsBO2)
		{
			drawMaterialBO2(105.0f, 245.0f, 250.0f, 80.0f, 0, L_black);
			drawRectangleBO2(105.0f, 245.0f, 250.0f, 80.0f, 2.0f, Purple);
			BO2_DrawText("Target Details:", 105.0f, 241.0f, 0.4f, white);

			int ClientValid = ClosetsClientBO2();
			char* targettext;
			if (DrawTargetDetailsBO2)
			{
				char targetbuffer[3096];
				sprintf(targetbuffer, "Name: %s\nDistance: %.fm\nWeapon: %s", BO2_Clients[BO2_Clients[ClientValid]->mNum]->mName, GetDistance(BO2_RefDef->EyePos, BO2_Entities[ClientValid]->mPos), BO2_getWeaponname(BO2_Entities[ClientValid]->WeaponID));
				if (targetbuffer != "")
					targettext = targetbuffer;
				else
					targettext = "Target Details Error";
				DrawTargetDetailsBO2 = FALSE;
			}
			else
			{
				targettext = "Name: N/A\nDistance: N/A\nWeapon: N/A";
			}
			BO2_DrawText(targettext, 115.0f, 275.0f, 0.4f, white);
		}
	}
}

VOID BO2_Draw()
{
	BO2_ReadGame();
	if (strcmp(BO2_cgs->gametype, "zclassic") != 0 || strcmp(BO2_cgs->gametype, "zstandard") != 0)
	{

		char cNum[200];
		char count[250];
		int CenterX = 1280 / 2;
		int CenterY = 720 / 2;
		BOOL isFriend = FALSE;
		//BO2_toogleHack();

		if (CrossHairBO2 == PlusCrosshairBO2)
		{
			BO2_DrawRect(CenterX - 10, CenterY, 21, 2, white);
			BO2_DrawRect(CenterX, CenterY - 10, 2, 21, white);
		}
		else if (CrossHairBO2 == XCrosshairBO2)
		{
			BO2_DrawLine2(625, 345, 638, 358, yellow, BO2_White);
			BO2_DrawLine2(655, 345, 642, 358, yellow, BO2_White);
			BO2_DrawLine2(625, 375, 638, 362, yellow, BO2_White);
			BO2_DrawLine2(655, 375, 642, 362, yellow, BO2_White);
		}

		Vector vScreen;
		Vector vScreenClient;
		int ClientValid = ClosetsClientBO2();
		BO2_cEntity *pEntityValid = BO2_Entities[ClientValid];
		int playercount = 0;

		for (int i = 0; i < ENTITIESMAX; i++)
		{
			if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
			{
				playercount++;
				BO2_cEntity *pEntity = BO2_Entities[i];

				FLOAT DistancePlayer = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

				if (BO2_wS2(pEntity->mPos, vScreen))
				{
					if (DistancePlayer > 3)
					{
						int x = vScreen[0];
						int y = vScreen[1];

						FLOAT fWidth = (60.0f * 15.0f) / DistancePlayer;
						FLOAT fHeight = (100.0f * 15.0f) / DistancePlayer;
						BOOL bVisible = (BO2_CanSeeFriendlyHead(0, BO2_Entities[i]) == 1);
						int newX = x + (fWidth / 2) + 1;
						int newY = y - (fHeight);

						D3DXVECTOR3 vHead;
						short mTag = *(short*)BO2_j_head;
						BO2_GetTagPos(BO2_Entities[i], mTag, vHead);
						D3DXVECTOR3 BoxPos;
						BO2_wS2(vHead, BoxPos);
						int HeadX = BoxPos.x;
						int HeadY = BoxPos.y;
						if ((BO2_Clients[i]->mTeam == BO2_Clients[BO2_cg->cNum]->mTeam) && (strcmp(BO2_cgs->gametype, "dm") != 0) && (strcmp(BO2_cgs->gametype, "gun") != 0) && (strcmp(BO2_cgs->gametype, "oic") != 0) && (strcmp(BO2_cgs->gametype, "shrp") != 0) && (strcmp(BO2_cgs->gametype, "sas") != 0))
						{
							if (ESPBO2 == ESPFriendlyBO2 && *EnableESPBO2 || ESPBO2 == ESPBothBO2 && *EnableESPBO2)
							{
								if (*ESPNamesBO2 && *ESPDistanceBO2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s ^7[^5%.fm^7]", BO2_Clients[BO2_Clients[i]->mNum]->mName, GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos));
									BO2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, Purple);
								}
								else if (*ESPNamesBO2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s", BO2_Clients[BO2_Clients[i]->mNum]->mName);
									BO2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, Purple);
								}
								else if (*ESPDistanceBO2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "^7[^5%.fm^7]", GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos));
									BO2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, Purple);
								}

								if (*ESPWeaponBO2)
								{
									char clientweapon[256];
									sprintf(clientweapon, "%s", BO2_getWeaponname(BO2_Entities[i]->WeaponID));
									BO2_DrawText(clientweapon, x - strlen(clientweapon) * 2.5, y + GetTextPos1(DistancePlayer), 0.4f, Purple);
								}

								if (CurrentBO2ESP == BO2_TWO_D)
								{
									BO2_DrawPlayerBox(BO2_Entities[i], mTag, fWidth, green);
								}
								else if (CurrentBO2ESP == BO2_THREE_D)
								{
									BO2_Make3DBox(BO2_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, green);
								}
								else if (CurrentBO2ESP == BO2_PYRAMID)
								{
									BO2_MakePyramid(BO2_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, green);
								}
								else if (CurrentBO2ESP == BO2_FILLED)
								{
									BO2_DrawBox(x, y, fWidth, fHeight, L_green, green);
								}
								else if (CurrentBO2ESP == BO2_CORNER)
								{
									BO2_DrawCornerBox(HeadX, HeadY, fWidth, fHeight, green);
								}
							}

							if (SnapLineTeamBO2 == SnapLineFriendlyBO2 && *ESPSnapLinesBO2 || SnapLineTeamBO2 == SnapLineTeamBothBO2 && *ESPSnapLinesBO2)
							{
								if (SnapLinePosBO2 == SnapLineMiddleBO2)
								{
									BO2_DrawLine2(CenterX, CenterY, x, y, green, BO2_White);
								}
								else if (SnapLinePosBO2 == SnapLineBottomBO2)
								{
									BO2_DrawLine2(CenterX, 700.0f, x, y, green, BO2_White);
								}
								else if (SnapLinePosBO2 == SnapLineTopBO2)
								{
									BO2_DrawLine2(CenterX, 30.0f, x, y, green, BO2_White);
								}
							}
						}
						else
						{
							change_color = bVisible ? blue : red;
							change_shad_colorInside = bVisible ? L_blue : L_red;
							change_shad_colorOutside = bVisible ? blue : red;

							if (ESPBO2 == ESPEnemyBO2 && *EnableESPBO2 || ESPBO2 == ESPBothBO2 && *EnableESPBO2)
							{
								if (*ESPNamesBO2 && *ESPDistanceBO2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s ^7[^5%.fm^7]", BO2_Clients[BO2_Clients[i]->mNum]->mName, GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos));
									BO2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, change_color);
								}
								else if (*ESPNamesBO2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s", BO2_Clients[BO2_Clients[i]->mNum]->mName);
									BO2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, change_color);
								}
								else if (*ESPDistanceBO2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "^7[^5%.fm^7]", GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos));
									BO2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, change_color);
								}

								if (*ESPWeaponBO2)
								{
									char clientweapon[256];
									sprintf(clientweapon, "%s", BO2_getWeaponname(BO2_Entities[i]->WeaponID));
									BO2_DrawText(clientweapon, x - strlen(clientweapon) * 2.5, y + GetTextPos1(DistancePlayer), 0.4f, change_color);
								}

								if (CurrentBO2ESP == BO2_TWO_D)
								{
									BO2_DrawPlayerBox(BO2_Entities[i], mTag, fWidth, change_color);
								}
								else if (CurrentBO2ESP == BO2_THREE_D)
								{
									BO2_Make3DBox(BO2_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, change_color);
								}
								else if (CurrentBO2ESP == BO2_PYRAMID)
								{
									BO2_MakePyramid(BO2_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, change_color);
								}
								else if (CurrentBO2ESP == BO2_FILLED)
								{
									BO2_DrawBox(x, y, fWidth, fHeight, change_shad_colorInside, change_shad_colorOutside);
								}
								else if (CurrentBO2ESP == BO2_CORNER)
								{
									BO2_DrawCornerBox(HeadX, HeadY, fWidth, fHeight, change_color);
								}
							}

							if (SnapLineTeamBO2 == SnapLineEnemyBO2 && *ESPSnapLinesBO2 || SnapLineTeamBO2 == SnapLineTeamBothBO2 && *ESPSnapLinesBO2)
							{
								if (SnapLinePosBO2 == SnapLineMiddleBO2)
								{
									BO2_DrawLine2(CenterX, CenterY, x, y, change_color, BO2_White);
								}
								else if (SnapLinePosBO2 == SnapLineBottomBO2)
								{
									BO2_DrawLine2(CenterX, 700.0f, x, y, change_color, BO2_White);
								}
								else if (SnapLinePosBO2 == SnapLineTopBO2)
								{
									BO2_DrawLine2(CenterX, 30.0f, x, y, change_color, BO2_White);
								}
							}

							if (*VisibleWarningBO2)
							{
								if (bVisible)
								{
									char msg[250];
									sprintf(msg, "*Enemy Visible*");
									BO2_DrawText(msg, CenterX - strlen(msg) * 2.5, CenterY + 150, 0.6f, Purple);
								}
							}

							if (*ProximityWarningBO2)
							{
								if (ProximityWarning(DistancePlayer))
								{
									char msg[250];
									sprintf(msg, "Proximity Warning!");
									BO2_DrawText(msg, CenterX - strlen(msg) * 2.5, CenterY + 170, 0.6f, Purple);
								}
							}
						}
					}
				}
			}
		}
	}
}

VOID BO2_ZombiesAimbot()
{
	BO2_ReadGame();
	if (strcmp(BO2_cgs->gametype, "zclassic") == 0 || strcmp(BO2_cgs->gametype, "zstandard") == 0)
	{
		BO2_cEntity * pObject = NULL;
		FLOAT objectDistance = 0.f;
		FLOAT flClosestDistance = 9999.f;
		BO2_clientActive * ca = NULL;

		D3DXVECTOR3 vBestZombieHead, vZombieHead;
		D3DXVECTOR3 vOutPos;
		Vector vAngles;

		ca = (BO2_clientActive*)BO2_getCA();

		for (int i = 4; i < 2048; i++)
		{

			pObject = BO2_GetEntitybyIdx(i);

			if (pObject == NULL || !(pObject->AliveZombie == 64))//
				continue;

			if (!(pObject->Valid & ET_ZOMBIE_MP))//zombieType only
				continue;

			D3DXVECTOR3 vHead, vHeadScreen, vLegScreen, vLeg, v3Muzzle;

			objectDistance = GetDistance(pObject->mPos, BO2_RefDef->EyePos);

			//todo check if alive for going further....

			short mTag = *(short*)BO2_j_neck;

			if (!BO2_GetTagPos(pObject, mTag, vZombieHead))//get Head ...
				continue;

			if (objectDistance < flClosestDistance)
			{
				vBestZombieHead = vZombieHead;
				flClosestDistance = objectDistance;
			}
		}

		if (objectDistance < 200)
		{
			vAngles = vectoangles(GetVec(BO2_RefDef->EyePos, vBestZombieHead));
			ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
			ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;
		}
	}
}

VOID BO2_ZombiesESP()
{
	BO2_ReadGame();
	if (strcmp(BO2_cgs->gametype, "zclassic") == 0 || strcmp(BO2_cgs->gametype, "zstandard") == 0)
	{
		BO2_toogleHack();
		int CenterX = 1280 / 2;
		int CenterY = 720 / 2;
		FLOAT Screen[2];
		char Px[25];
		char Pdistance[25];
		if (BO2_Dvar_GetBOOL("cl_ingame"))
		{
			for (int i = 4; i < 2048; i++)
			{
				BO2_cEntity* pEnt = BO2_GetEntitybyIdx(i);
				FLOAT distance = GetDistance(pEnt->mPos, BO2_RefDef->EyePos);

				if (pEnt == NULL || !(pEnt->AliveZombie == 64))//
															   //if((BO2_Entities[i]->Valid ) && (BO2_Entities[i]->State == 64))
					continue;

				if (!(pEnt->Valid & ET_ZOMBIE_MP))//zombieType only
					continue;

				short mTag = *(short*)BO2_j_neck;
				if (BO2_wS2(pEnt->mPos, Screen))
				{
					int x = Screen[0];
					int y = Screen[1];

					FLOAT fWidth = (60.0f * 15.0f) / distance;
					FLOAT fHeight = (100.0f * 15.0f) / distance;
					int newX = x + (fWidth / 2) + 1;
					int newY = y - (fHeight);

					if (distance < 200)
					{
						if (ESPBO2 == ESPEnemyBO2)
						{
							if (*ESPNamesBO2)
							{
								sprintf(Px, "Zombie");
								BO2_DrawShadowText(Px, newX, newY, GetFontSize1(distance), Purple);
							}

							if (*ESPDistanceBO2)
							{
								sprintf(Pdistance, "[%0.1fm]", distance);
								BO2_DrawShadowText(Pdistance, newX, newY + 14, GetFontSize1(distance), Purple);
							}

							if (CurrentBO2ESP == BO2_TWO_D)
							{
								BO2_DrawPlayerBox(pEnt, mTag, fWidth, Purple);
							}
							else if (CurrentBO2ESP == BO2_THREE_D)
							{
								BO2_Make3DBox(pEnt, GetBoxWidth(), GetBoxHeight() + 10, Purple);
							}
							else if (CurrentBO2ESP == BO2_PYRAMID)
							{
								BO2_MakePyramid(pEnt, GetBoxWidth(), GetBoxHeight() + 10, Purple);
							}
						}

						if (SnapLineTeamBO2 == SnapLineEnemyBO2)
						{
							if (SnapLinePosBO2 == SnapLineMiddleBO2)
							{
								BO2_DrawLine2(CenterX, CenterY, x, y, change_color, BO2_White);
							}
							else if (SnapLinePosBO2 == SnapLineBottomBO2)
							{
								BO2_DrawLine2(CenterX, 700.0f, x, y, change_color, BO2_White);
							}
							else if (SnapLinePosBO2 == SnapLineTopBO2)
							{
								BO2_DrawLine2(CenterX, 30.0f, x, y, change_color, BO2_White);
							}
						}
					}
				}
			}
		}
	}
}

XINPUT_STATE XInputStateBO2;
short currentButtonBO2, previousButtonBO2;
#define buttonHitBO2(button) ((currentButtonBO2 & ~previousButtonBO2) & button)
VOID updateInputBO2() {
	XInputGetState(0, &XInputStateBO2);
	previousButtonBO2 = currentButtonBO2;
	currentButtonBO2 = ~XInputStateBO2.Gamepad.wButtons & 0xFBFF;

	if (buttonHitBO2(XINPUT_GAMEPAD_DPAD_LEFT)) {
		openBO2 = !openBO2;
	}

	if (openBO2) {
		if (currentBO2 == BO2_MAIN_MENU || currentBO2 == BO2_AIMBOT || currentBO2 == BO2_ESP || currentBO2 == BO2_MISC) {
			if (buttonHitBO2(XINPUT_GAMEPAD_B)) {
				currentBO2 = BO2_MENU;
			}
		}

		if (buttonHitBO2(XINPUT_GAMEPAD_DPAD_UP)) {
			if (currentBO2 == BO2_MENU)
			{
				scrollBO2--;
				if (scrollBO2 == -1) {
					scrollBO2 = maxoptionsBO2 - 1;
				}
			}
			else
			{
				if (currentBO2 == BO2_MAIN_MENU)
				{
					MainscrollBO2--;
					if (MainscrollBO2 == -1) {
						MainscrollBO2 = MainmaxoptionsBO2 - 1;
					}
				}
				else
				{
					if (currentBO2 == BO2_AIMBOT)
					{
						AimscrollBO2--;
						if (AimscrollBO2 == -1) {
							AimscrollBO2 = AimmaxoptionsBO2 - 1;
						}
					}
					else
					{
						if (currentBO2 == BO2_ESP)
						{
							ESPscrollBO2--;
							if (ESPscrollBO2 == -1) {
								ESPscrollBO2 = ESPmaxoptionsBO2 - 1;
							}
						}
						else
						{
							if (currentBO2 == BO2_MISC)
							{
								MiscscrollBO2--;
								if (MiscscrollBO2 == -1) {
									MiscscrollBO2 = MiscmaxoptionsBO2 - 1;
								}
							}
						}
					}
				}
			}
		}

		if (buttonHitBO2(XINPUT_GAMEPAD_DPAD_DOWN)) {
			if (currentBO2 == BO2_MENU)
			{
				scrollBO2++;
				if (scrollBO2 == maxoptionsBO2) {
					scrollBO2 = 0;
				}
			}
			else
			{
				if (currentBO2 == BO2_MAIN_MENU)
				{
					MainscrollBO2++;
					if (MainscrollBO2 == MainmaxoptionsBO2) {
						MainscrollBO2 = 0;
					}
				}
				else
				{
					if (currentBO2 == BO2_AIMBOT)
					{
						AimscrollBO2++;
						if (AimscrollBO2 == AimmaxoptionsBO2) {
							AimscrollBO2 = 0;
						}
					}
					else
					{
						if (currentBO2 == BO2_ESP)
						{
							ESPscrollBO2++;
							if (ESPscrollBO2 == ESPmaxoptionsBO2) {
								ESPscrollBO2 = 0;
							}
						}
						else
						{
							if (currentBO2 == BO2_MISC)
							{
								MiscscrollBO2++;
								if (MiscscrollBO2 == MiscmaxoptionsBO2) {
									MiscscrollBO2 = 0;
								}
							}
						}
					}
				}
			}
		}

		if (buttonHitBO2(XINPUT_GAMEPAD_X)) {
			if (currentBO2 == BO2_MENU)
			{
				if (scrollBO2 == 0)
				{
					currentBO2 = BO2_MAIN_MENU;
				}
				else
				{
					if (scrollBO2 == 1)
					{
						currentBO2 = BO2_AIMBOT;
					}
					else
					{
						if (scrollBO2 == 2)
						{
							currentBO2 = BO2_ESP;
						}
						else
						{
							if (scrollBO2 == 3)
							{
								currentBO2 = BO2_MISC;
							}
							else
							{
								boptionsBO2[scrollBO2] = !boptionsBO2[scrollBO2];
							}
						}
					}
				}
			}
			else
			{
				if (currentBO2 == BO2_MAIN_MENU)
				{
					if (MainscrollBO2 == 5)
					{
						switch (CrossHairBO2) {
						case 0:
							CrossHairBO2 = XCrosshairBO2;
							break;
						case 1:
							CrossHairBO2 = CrosshairOffBO2;
							break;
						case 2:
							CrossHairBO2 = PlusCrosshairBO2;
							break;
						default:
							CrossHairBO2 = CrosshairOffBO2;
							break;
						}
					}
					else {
						MainboptionsBO2[MainscrollBO2] = !MainboptionsBO2[MainscrollBO2];
					}
				}
				else
				{
					if (currentBO2 == BO2_AIMBOT)
					{
						if (AimscrollBO2 == 1) {
							switch (CurrentBO2Aimbot) {
							case 0:
								CurrentBO2Aimbot = BO2_ONSCREEN;
								break;
							case 1:
								CurrentBO2Aimbot = BO2_CROSSHAIR;
								break;
							case 2:
								CurrentBO2Aimbot = BO2_SNAP;
								break;
							case 3:
								CurrentBO2Aimbot = BO2_NEAREST;
								break;
							default:
								CurrentBO2Aimbot = BO2_SNAP;
								break;
							}
						}
						else {
							AimboptionsBO2[AimscrollBO2] = !AimboptionsBO2[AimscrollBO2];
						}
					}
					else
					{
						if (currentBO2 == BO2_ESP)
						{
							if (ESPscrollBO2 == 1) {
								switch (CurrentBO2ESP) {
								case 0:
									CurrentBO2ESP = BO2_THREE_D;
									break;
								case 1:
									CurrentBO2ESP = BO2_PYRAMID;
									break;
								case 2:
									CurrentBO2ESP = BO2_FILLED;
									break;
								case 3:
									CurrentBO2ESP = BO2_CORNER;
									break;
								case 4:
									CurrentBO2ESP = BO2_TWO_D;
									break;
								default:
									CurrentBO2ESP = BO2_CORNER;
									break;
								}
							}
							else if (ESPscrollBO2 == 5) {
								switch (ESPBO2) {
								case 0:
									ESPBO2 = ESPFriendlyBO2;
									break;
								case 1:
									ESPBO2 = ESPBothBO2;
									break;
								case 2:
									ESPBO2 = ESPEnemyBO2;
									break;
								default:
									ESPBO2 = ESPBothBO2;
									break;
								}
							}
							else if (ESPscrollBO2 == 7) {
								switch (SnapLinePosBO2) {
								case 0:
									SnapLinePosBO2 = SnapLineBottomBO2;
									break;
								case 1:
									SnapLinePosBO2 = SnapLineTopBO2;
									break;
								case 2:
									SnapLinePosBO2 = SnapLineMiddleBO2;
									break;
								default:
									SnapLinePosBO2 = SnapLineTopBO2;
									break;
								}
							}
							else if (ESPscrollBO2 == 8) {
								switch (SnapLineTeamBO2) {
								case 0:
									SnapLineTeamBO2 = SnapLineFriendlyBO2;
									break;
								case 1:
									SnapLineTeamBO2 = SnapLineTeamBothBO2;
									break;
								case 2:
									SnapLineTeamBO2 = SnapLineEnemyBO2;
									break;
								default:
									SnapLineTeamBO2 = SnapLineTeamBothBO2;
									break;
								}
							}
							else {
								ESPboptionsBO2[ESPscrollBO2] = !ESPboptionsBO2[ESPscrollBO2];
							}
						}
						else
						{
							if (currentBO2 == BO2_MISC)
							{
								MiscboptionsBO2[MiscscrollBO2] = !MiscboptionsBO2[MiscscrollBO2];
							}
						}
					}
				}
			}
		}
	}
}

VOID updateOptionsBO2()
{
	if (*norecoilBO2)
	{
		*(int*)BO2_NoRecoil = 0x60000000;
	}
	else
	{
		*(int*)BO2_NoRecoil = 0x48461341;
	}
	if (*advanceduavBO2)
	{
		*(int*)BO2_Radar = 0x2B0B0001;
	}
	else
	{
		*(int*)BO2_Radar = 0x2B0B0000;
	}
	if (*noswayBO2)
	{
		*(int*)BO2_WeaponSway = 0x60000000;
		*(int*)BO2_ZommingSway = 0x60000000;
	}
	else
	{
		*(int*)BO2_WeaponSway = 0x4BFFE975;
		*(int*)BO2_ZommingSway = 0x4BFFFA85;
	}
	if (*redboxesBO2)
	{
		*(int*)BO2_Red_Boxes = 0x38600001;
	}
	else
	{
		*(int*)BO2_Red_Boxes = 0x38600000;
	}
	if (*chamsBO2)
	{
		*(int*)0x821fc04c = 0x38c0ffff;
	}
	else
	{
		*(int*)0x821fc04c = 0x7fa6eb120;
	}
	if(*promodBO2)
	{
		if(!*endgameBO2)
		{
			Cbuf_AddTextBO2(0, "cg_fov 90");
		}
		resetpromod = TRUE;
	}

	else
	{
		if(resetpromod)
		{
			Cbuf_AddTextBO2(0, "cg_fov 65");
			resetpromod = FALSE;
		}
	}

	
	if (*showhostBO2)
	{
		BYTE NewByte[0x12];
		SetMemory((PVOID)0x82003f70, NewByte, 0x12);
		*(BOOL*)0x83c5a3bb = TRUE;
	}
	else
	{
		BYTE NewByte[0x12];
		SetMemory((PVOID)0x82003f70, NewByte, 0x12);
		*(BOOL*)0x83c5a3bb = FALSE;
	}
	if (*endgameBO2)
	{
		char buffer[100];
		sprintf(buffer, "cmd mr %i 3 endround;", *(int*)0x82c15758);
		Cbuf_AddTextBO2(0, buffer);
		*endgameBO2 = FALSE;
	}
}

VOID BO2_Menu_Hook()
{
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;
	FLOAT middleBO2 = 950.0f;
	FLOAT centerBO2 = 100.0f;
	FLOAT centerMBO2 = 80.0f;
	FLOAT centerM2BO2 = 83.0f;
	FLOAT bottomBO2 = 700.0f;

	updateInputBO2();
	updateOptionsBO2();

	if (BO2_Dvar_GetBOOL("cl_ingame"))
	{
		if (!FirstTimeInGameBO2)
		{
			openBO2 = FALSE;
			currentBO2 = 0;
			CurrentBO2ESP = 0;
			CurrentBO2Aimbot = 0;
			scrollBO2 = 0;
			MainscrollBO2 = 0;
			AimscrollBO2 = 0;
			ESPscrollBO2 = 0;
			SnapLinePosBO2 = 0;
			SnapLineTeamBO2 = 0;
			CrossHairBO2 = 2;
			ESPBO2 = 0;
			CrossHairBO2 = 2;
			*watermarkBO2 = TRUE;
			FirstTimeInGameBO2 = TRUE;
		}

		if (openBO2)
		{
			if (currentBO2 == BO2_MENU)
			{
				drawMaterialBO2(middleBO2 - 1, centerBO2, 250.0f, 30.0f + (maxoptionsBO2 * 30.0f), 0, L_black);
				drawRectangleBO2(middleBO2 - 1, centerBO2, 250.0f, 30.0f + (maxoptionsBO2 * 30.0f), 2.0f, Purple);
				BO2_DrawText("OutHere Menu", middleBO2 + 20, centerBO2 - 1, 0.6f, Purple);

				for (int i = 0; i < maxoptionsBO2; i++) {
					if (i == scrollBO2) {
						if (*scrollbarBO2) drawMaterialBO2(middleBO2 + 1, centerMBO2 + 40.0f + (30.0f * i), 248.0f, 20.0f, 0, grey);
						BO2_DrawText(optionsBO2[i], middleBO2 + 20, centerBO2 + 40.0f + (30.0f * i), 0.4f, Purple);
					}
					else {
						BO2_DrawText(optionsBO2[i], middleBO2 + 20, centerBO2 + 40.0f + (30.0f * i), 0.4f, white);
					}
				}
			}
			else
			{
				if (currentBO2 == BO2_MAIN_MENU)
				{
					drawMaterialBO2(middleBO2 - 1, centerBO2, 250.0f, 30.0f + (MainmaxoptionsBO2 * 30.0f), 0, L_black);
					drawRectangleBO2(middleBO2 - 1, centerBO2, 250.0f, 30.0f + (MainmaxoptionsBO2 * 30.0f), 2.0f, Purple);
					BO2_DrawText("Main Menu", middleBO2 + 20, centerBO2 - 1, 0.6f, Purple);

					for (int i = 0; i < MainmaxoptionsBO2; i++) {
						if (i == MainscrollBO2) {
							if (*scrollbarBO2) drawMaterialBO2(middleBO2 + 1, centerMBO2 + 40.0f + (30.0f * i), 248.0f, 20.0f, 0, grey);
							BO2_DrawText(MainoptionsBO2[i], middleBO2 + 20, centerBO2 + 40.0f + (30.0f * i), 0.4f, Purple);
						}
						else {
							BO2_DrawText(MainoptionsBO2[i], middleBO2 + 20, centerBO2 + 40.0f + (30.0f * i), 0.4f, white);
						}
						if (i == 5)
						{
							BO2_DrawText(CrossHairTypeBO2[CrossHairBO2], middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, BlueStormz);
							if (CrossHairBO2 == CrosshairOffBO2)
							{
								if (*drawonoffBO2)
								{
									BO2_DrawText("OFF", middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, Purple);
								}
								else
								{
									drawMaterialBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 0, L_black);
									drawRectangleBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 2.0f, Purple);
								}
							}
						}
						else if (i == 8)
						{
							// fix
						}
						else if (i == 9)
						{
							// fix
						}
						else if (MainboptionsBO2[i])
						{
							if (*drawonoffBO2)
							{
								BO2_DrawText("ON", middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, green);
							}
							else
							{
								drawMaterialBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 0, L_black);
								drawRectangleBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 2.0f, Purple);
							}
						}
						else
						{
							if (*drawonoffBO2)
							{
								BO2_DrawText("OFF", middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, Purple);
							}
							else
							{
								drawMaterialBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 0, L_black);
								drawRectangleBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 2.0f, Purple);
							}
						}
					}
				}
				else
				{
					if (currentBO2 == BO2_AIMBOT)
					{
						drawMaterialBO2(middleBO2 - 1, centerBO2, 250.0f, 30.0f + (AimmaxoptionsBO2 * 30.0f), 0, L_black);
						drawRectangleBO2(middleBO2 - 1, centerBO2, 250.0f, 30.0f + (AimmaxoptionsBO2 * 30.0f), 2.0f, Purple);
						BO2_DrawText("Aimbot Menu", middleBO2 + 20, centerBO2 - 1, 0.6f, Purple);

						for (int i = 0; i < AimmaxoptionsBO2; i++) {
							if (i == AimscrollBO2) {
								if (*scrollbarBO2) drawMaterialBO2(middleBO2 + 1, centerMBO2 + 40.0f + (30.0f * i), 248.0f, 20.0f, 0, grey);
								BO2_DrawText(AimoptionsBO2[i], middleBO2 + 20, centerBO2 + 40.0f + (30.0f * i), 0.4f, Purple);
							}
							else {
								BO2_DrawText(AimoptionsBO2[i], middleBO2 + 20, centerBO2 + 40.0f + (30.0f * i), 0.4f, white);
							}
							if (i == 1) {
								BO2_DrawText(AimbotTypeBO2[CurrentBO2Aimbot], middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, BlueStormz);
							}
							else {
								if (AimboptionsBO2[i])
								{
									if (*drawonoffBO2)
									{
										BO2_DrawText("ON", middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, green);
									}
									else
									{
										drawMaterialBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 0, L_black);
										drawRectangleBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 2.0f, Purple);
									}
								}
								else
								{
									if (*drawonoffBO2)
									{
										BO2_DrawText("OFF", middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, Purple);
									}
									else
									{
										drawMaterialBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 0, L_black);
										drawRectangleBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 2.0f, Purple);
									}
								}
							}
						}
					}
					else
					{
						if (currentBO2 == BO2_ESP)
						{
							drawMaterialBO2(middleBO2 - 1, centerBO2, 250.0f, 30.0f + (ESPmaxoptionsBO2 * 30.0f), 0, L_black);
							drawRectangleBO2(middleBO2 - 1, centerBO2, 250.0f, 30.0f + (ESPmaxoptionsBO2 * 30.0f), 2.0f, Purple);
							BO2_DrawText("ESP Menu", middleBO2 + 20, centerBO2 - 1, 0.6f, Purple);

							for (int i = 0; i < ESPmaxoptionsBO2; i++) {
								if (i == ESPscrollBO2) {
									if (*scrollbarBO2) drawMaterialBO2(middleBO2 + 1, centerMBO2 + 40.0f + (30.0f * i), 248.0f, 20.0f, 0, grey);
									BO2_DrawText(ESPoptionsBO2[i], middleBO2 + 20, centerBO2 + 40.0f + (30.0f * i), 0.4f, Purple);
								}
								else {
									BO2_DrawText(ESPoptionsBO2[i], middleBO2 + 20, centerBO2 + 40.0f + (30.0f * i), 0.4f, white);
								}
								if (i == 1) {
									BO2_DrawText(espTypeBO2[CurrentBO2ESP], middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, BlueStormz);
								}
								else if (i == 5) {
									BO2_DrawText(ESPTeamTypeBO2[ESPBO2], middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, BlueStormz);
								}
								else if (i == 7) {
									BO2_DrawText(SnapLineTypeBO2[SnapLinePosBO2], middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, BlueStormz);
								}
								else if (i == 8) {
									BO2_DrawText(SnapLineTeamTypeBO2[SnapLineTeamBO2], middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, BlueStormz);
								}
								else {
									if (ESPboptionsBO2[i])
									{
										if (*drawonoffBO2)
										{
											BO2_DrawText("ON", middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, green);
										}
										else
										{
											drawMaterialBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 0, L_black);
											drawRectangleBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 2.0f, Purple);
										}
									}
									else
									{
										if (*drawonoffBO2)
										{
											BO2_DrawText("OFF", middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, Purple);
										}
										else
										{
											drawMaterialBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 0, L_black);
											drawRectangleBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 2.0f, Purple);
										}
									}
								}
							}
						}
						else
						{
							if (currentBO2 == BO2_MISC)
							{
								drawMaterialBO2(middleBO2 - 1, centerBO2, 250.0f, 30.0f + (MiscmaxoptionsBO2 * 30.0f), 0, L_black);
								drawRectangleBO2(middleBO2 - 1, centerBO2, 250.0f, 30.0f + (MiscmaxoptionsBO2 * 30.0f), 2.0f, Purple);
								BO2_DrawText("Misc Menu", middleBO2 + 20, centerBO2 - 1, 0.6f, Purple);

								for (int i = 0; i < MiscmaxoptionsBO2; i++) {
									if (i == MiscscrollBO2) {
										if (*scrollbarBO2) drawMaterialBO2(middleBO2 + 1, centerMBO2 + 40.0f + (30.0f * i), 248.0f, 20.0f, 0, grey);
										BO2_DrawText(MiscoptionsBO2[i], middleBO2 + 20, centerBO2 + 40.0f + (30.0f * i), 0.4f, Purple);
									}
									else {
										BO2_DrawText(MiscoptionsBO2[i], middleBO2 + 20, centerBO2 + 40.0f + (30.0f * i), 0.4f, white);
									}
									if (i == 4)
									{
										// fix
									}
									else if (MiscboptionsBO2[i])
									{
										if (*drawonoffBO2)
										{
											BO2_DrawText("ON", middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, green);
										}
										else
										{
											drawMaterialBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 0, L_black);
											drawRectangleBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 2.0f, Purple);
										}
									}
									else
									{
										if (*drawonoffBO2)
										{
											BO2_DrawText("OFF", middleBO2 + 190, centerBO2 + 40.0f + (30.0f * i), 0.4f, Purple);
										}
										else
										{
											drawMaterialBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 0, L_black);
											drawRectangleBO2(middleBO2 + 190, centerM2BO2 + 40.0f + (30.0f * i), 15.0f, 15.0f, 2.0f, Purple);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (*watermarkBO2)
		{
			drawMaterialBO2(40.0f, 680.0f, 380.0f, 20.0f, 0, L_black);
			drawRectangleBO2(40.0f, 680.0f, 380.0f, 20.0f, 2.0f, Purple);
			BO2_DrawText("Press DPAD_Left  To ^2Open/^1Close OutHere Menu", 44.0f, 700.0f, 0.4f, white);//Bottom shit
		}
	}
	else
	{
		openBO2 = FALSE;
	}
}

HRESULT BO2_CL_RenderScenehk(DWORD a1)
{
	//int CenterX = 1280/2;
	//int CenterY = 720/2;

	BO2_R_RenderScene_(a1);//Call back
	BO2_RefDefCalc = a1;
	//BO2_dMenu.BuildMenu("\n\n^2Masked Menu^7",(CenterX - 300),50,300,150,black,blackM,Purple);

	if (BO2_Key_Down(0, 0x12) && CurrentBO2Aimbot == BO2_NEAREST || *AutoAimBO2)
	{
		//printf("aiming");
		BO2_ZombiesAimbot();
	}
	else
	{
		LastFrameAimKey = FALSE;
	}

	BO2_Menu_Hook();
	BO2_Draw();
	BO2_ZombiesESP();
	BO2_Aimbot();
	ClientDetailsBO2();

	return S_OK;
}

VOID BO2_Start()
{
	PatchInJump((DWORD *)0x82262978, (DWORD)BO2_CL_RenderScenehk, FALSE);
	//HookFunctionStart((DWORD *)0x8225E930, (PDWORD)SaveStubPacket, (DWORD)BO2_CL_WritePackethk);
}

/////////////////////////////////// BO2 Menu END /////////////////////////////////////

/////////////////////////////////////// MW3 Menu /////////////////////////////////////

BOOL CanDrawTargetedESPMW3 = FALSE;
BOOL DrawTargetDetailsMW3 = FALSE;
BOOL FirstTimeInGameMW3 = FALSE;
BOOL openMW3 = FALSE;
int scrollMW3 = 0, maxoptionsMW3 = 6;
char optionsMW3[][32] = {
	"Aimbot Options",
	"ESP Options",
	"No Recoil",
	"Advanced UAV",
	"No Sway",
	"Crosshair"
};
BOOL boptionsMW3[6] = { FALSE };
BOOL *norecoilMW3 = &boptionsMW3[2];
BOOL *advanceduavMW3 = &boptionsMW3[3];
BOOL *noswayMW3 = &boptionsMW3[4];

int ESPscrollMW3 = 0, ESPmaxoptionsMW3 = 9;
char ESPoptionsMW3[][32] = {
	"ESP Type:",
	"ESP Name",
	"ESP Distance",
	"ESP Weapon",
	"ESP Bones",
	"ESP Team",
	"Snap Line Screen Pos:",
	"Snap Line Team:",
	"Targeted Client ESP"
};
BOOL ESPboptionsMW3[9] = { FALSE };
BOOL *ESPNamesMW3 = &ESPboptionsMW3[1];
BOOL *ESPDistanceMW3 = &ESPboptionsMW3[2];
BOOL *ESPWeaponMW3 = &ESPboptionsMW3[3];
BOOL *ESPBonesMW3 = &ESPboptionsMW3[4];
BOOL *ESPTargetedMW3 = &ESPboptionsMW3[8];

int AimscrollMW3 = 0, AimmaxoptionsMW3 = 12;
char AimoptionsMW3[][32] = {
	"Aimbot Type:",
	"Auto Aim",
	"Auto Shoot",
	"Spin Bot",
	"Spin Bot Slow",
	"Spin Bot Fast",
	"Fake Aim Down",
	"Fake Aim Up",
	"Anti Aim",
	"Visible Warning",
	"Proximity Warning",
	"Target Details"
};
BOOL AimboptionsMW3[12] = { FALSE };
BOOL *AutoAimMW3 = &AimboptionsMW3[1];
BOOL *AutoShootMW3 = &AimboptionsMW3[2];
BOOL *SpinBotMW3 = &AimboptionsMW3[3];
BOOL *SlowSpinBotMW3 = &AimboptionsMW3[4];
BOOL *FastSpinBotMW3 = &AimboptionsMW3[5];
BOOL *FakeAimDownMW3 = &AimboptionsMW3[6];
BOOL *FakeAimUpMW3 = &AimboptionsMW3[7];
BOOL *AntiAimMW3 = &AimboptionsMW3[8];
BOOL *VisibleWarningMW3 = &AimboptionsMW3[9];
BOOL *ProximityWariningMW3 = &AimboptionsMW3[10];
BOOL *TargetDetailsMW3 = &AimboptionsMW3[11];

char AimbotTypeMW3[5][32] = {
	"Nearest",
	"Visible",
	"Crosshair",
	"Silent Aim",
	"OFF"
};

char espTypeMW3[4][32] = {
	"2D",
	"3D",
	"Pyramid",
	"OFF"
};

char ESPTeamTypeMW3[4][32] = {
	"Enemy",
	"Friendly",
	"Both",
	"OFF"
};

char SnapLineTypeMW3[4][32] = {
	"Middle",
	"Bottom",
	"Top",
	"OFF"
};

char SnapLineTeamTypeMW3[4][32] = {
	"Enemy",
	"Friendly",
	"Both",
	"OFF"
};

char CrossHairTypeMW3[3][32] = {
	"+",
	"X",
	"OFF"
};

int MW3_MAIN = 0, MW3_AIMBOT = 1, MW3_ESP = 2;
int currentMW3 = MW3_MAIN;
int MW3_NEAREST = 0, MW3_ONSCREEN = 1, MW3_CROSSHAIR = 2, MW3_Silent = 3, MW3_AIMBOT_OFF = 4;
int CurrentMW3Aimbot = MW3_AIMBOT_OFF;
int MW3_TWO_D = 0, MW3_THREE_D = 1, MW3_PYRAMID = 2, MW3_ESP_OFF = 3;
int CurrentMW3ESP = MW3_ESP_OFF;
int ESPEnemyMW3 = 0, ESPFriendlyMW3 = 1, ESPBothMW3 = 2, ESPOffMW3 = 3;
int ESPMW3 = ESPOffMW3;
int SnapLineMiddleMW3 = 0, SnapLineBottomMW3 = 1, SnapLineTopMW3 = 2, SnapLinesOffMW3 = 3;
int SnapLinePosMW3 = SnapLinesOffMW3;
int SnapLineEnemyMW3 = 0, SnapLineFriendlyMW3 = 1, SnapLineTeamBothMW3 = 2, SnapLineTeamOffMW3 = 3;
int SnapLineTeamMW3 = SnapLineTeamOffMW3;
int PlusCrosshairMW3 = 0, XCrosshairMW3 = 1, CrosshairOffMW3 = 2;
int CrossHairMW3 = CrosshairOffMW3;

VOID MW3_Aimbot()
{
	if (MW3_Dvar_GetBOOL("cl_ingame"))
	{
		DWORD pd = *(DWORD*)MW3_clientActive_t;
		MW3_clientActive *cmdInput = (MW3_clientActive*)pd;
		MW3_UserCmd_t* cmd = cmdInput->GetCmd(cmdInput->CmdNum);
		MW3_clientActive *Ca = MW3_getCA();

		MW3_UserCmd_t* oldCmd = cmdInput->GetCmd(cmdInput->CmdNum - 1);

		MW3_ReadGame();
		MW3_cEntity * Target = NULL;
		MW3_clientActive * ca = NULL;
		ca = (MW3_clientActive*)MW3_getCA();
		Vector vAngles;

		FLOAT objectDistance = 0.f;
		FLOAT flClosestDistance = (FLOAT)INT_MAX;

		D3DXVECTOR3 vHead, vBestHead;
		short mTag = *(short*)MW3_j_neck;

		if (CurrentMW3Aimbot == MW3_NEAREST)
		{
			for (int i = 0; i < 18; i++)
			{
				if (i == MW3_cg->cNum)
					continue;

				if ((MW3_Entities[i]->Valid) && (MW3_Entities[i]->State & 1))
				{
					if (strcmp(MW3_cgs->gametype, "dm") == 0)
					{
						objectDistance = GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos);

						Target = MW3_Entities[i];
						MW3_GetTagPos(0, Target, mTag, vHead);

						if (objectDistance < flClosestDistance)
						{
							vBestHead = vHead;
							flClosestDistance = objectDistance;
						}
					}
					else
					{
						if (MW3_Clients[i]->mTeam != MW3_Clients[MW3_cg->cNum]->mTeam)
						{
							objectDistance = GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos);

							Target = MW3_Entities[i];
							MW3_GetTagPos(0, Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
				}
			}

			if ((VALID(Target)) && (VALID(ca)))
			{
				CanDrawTargetedESPMW3 = TRUE;
				DrawTargetDetailsMW3 = TRUE;
				if (MW3_Key_Down(0, 0x12) || *AutoAimMW3)
				{
					vAngles = vectoangles(GetVec(MW3_RefDef->EyePos, vBestHead));
					ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
					ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;

					if (*AutoShootMW3)
					{
						oldCmd->Button |= 1;
					}
				}
			}
		}
		else if (CurrentMW3Aimbot == MW3_ONSCREEN)
		{
			for (int i = 0; i < 18; i++)
			{
				if (i == MW3_cg->cNum)
					continue;

				if ((MW3_Entities[i]->Valid) && (MW3_Entities[i]->State & 1))
				{
					if (strcmp(MW3_cgs->gametype, "dm") == 0)
					{
						if (MW3_CanSeePlayer(0, MW3_Entities[i], 0x2805823))
						{
							objectDistance = GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos);

							Target = MW3_Entities[i];
							MW3_GetTagPos(0, Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
					else
					{
						if (MW3_Clients[i]->mTeam != MW3_Clients[MW3_cg->cNum]->mTeam)
						{
							if (MW3_CanSeePlayer(0, MW3_Entities[i], 0x2805823))
							{
								objectDistance = GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos);

								Target = MW3_Entities[i];
								MW3_GetTagPos(0, Target, mTag, vHead);

								if (objectDistance < flClosestDistance)
								{
									vBestHead = vHead;
									flClosestDistance = objectDistance;
								}
							}
						}
					}
				}
			}

			if ((VALID(Target)) && (VALID(ca)))
			{
				CanDrawTargetedESPMW3 = TRUE;
				DrawTargetDetailsMW3 = TRUE;
				if (MW3_Key_Down(0, 0x12) || *AutoAimMW3)
				{
					vAngles = vectoangles(GetVec(MW3_RefDef->EyePos, vBestHead));
					ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
					ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;

					if (*AutoShootMW3)
					{
						oldCmd->Button |= 1;
					}
				}
			}
		}
		else if (CurrentMW3Aimbot == MW3_CROSSHAIR)
		{
			if (LastFrameAimKey && pSavedTarget2)
				Target = pSavedTarget2;
			else
				Target = MW3_GetTargetByCrosshair();

			if ((VALID(Target)) && (VALID(ca)))
			{
				CanDrawTargetedESPMW3 = TRUE;
				DrawTargetDetailsMW3 = TRUE;
				if (MW3_Key_Down(0, 0x12) || *AutoAimMW3)
				{
					D3DXVECTOR3 vHead;

					MW3_GetTagPos(0, Target, mTag, vHead);

					vAngles = vectoangles(GetVec(MW3_RefDef->EyePos, vHead));

					if (*AutoShootMW3)
					{
						oldCmd->Button |= 1;
					}

					ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
					ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;
				}
				pSavedTarget2 = Target;
				LastFrameAimKey = TRUE;
			}
		}
		else if (CurrentMW3Aimbot == MW3_Silent)
		{
			for (int i = 0; i < 18; i++)
			{
				if (i == MW3_cg->cNum)
					continue;

				if ((MW3_Entities[i]->Valid) && (MW3_Entities[i]->State & 1))
				{
					if (strcmp(MW3_cgs->gametype, "dm") == 0)
					{
						if (MW3_CanSeePlayer(0, MW3_Entities[i], 0x2805823))
						{
							objectDistance = GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos);

							Target = MW3_Entities[i];
							MW3_GetTagPos(0, Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
					else
					{
						if (MW3_Clients[i]->mTeam != MW3_Clients[MW3_cg->cNum]->mTeam)
						{
							if (MW3_CanSeePlayer(0, MW3_Entities[i], 0x2805823))
							{
								objectDistance = GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos);

								Target = MW3_Entities[i];
								MW3_GetTagPos(0, Target, mTag, vHead);

								if (objectDistance < flClosestDistance)
								{
									vBestHead = vHead;
									flClosestDistance = objectDistance;
								}
							}
						}
					}
				}
			}

			if ((VALID(Target)) && (VALID(ca)))
			{
				CanDrawTargetedESPMW3 = TRUE;
				DrawTargetDetailsMW3 = TRUE;

				vAngles = vectoangles(GetVec(MW3_RefDef->EyePos, vBestHead));
				SilentAngles.x = vAngles.x - ca->RefAngles.x;
				SilentAngles.y = vAngles.y - ca->RefAngles.y;

				//oldCmd->moveTonpere = cmd->moveTonpere;
				oldCmd->ViewAngle[0] = ANGLE2SHORT(SilentAngles.x);
				oldCmd->ViewAngle[1] = ANGLE2SHORT(SilentAngles.y);

				if (*AutoShootMW3)
				{
					oldCmd->Button |= 1;
				}
			}
		}
	}
}

VOID SpinBotMW3Thread(MW3_UserCmd_t *UserCommand) {
	Vector SpinBotView;
	int SpinBotAngles = -1;

	SpinBotAngles++;
	if (SpinBotAngles = 360) {
		SpinBotAngles = -1;
	}

	SpinBotView.y = SpinBotAngles;

	UserCommand->ViewAngle[1] = AngleToShort(SpinBotView.y);
}

VOID SlowSpinBotMW3Thread(MW3_UserCmd_t *UserCommand) {
	Vector SpinBotView;
	int SpinBotAngles = -1;

	SpinBotAngles++;
	SpinBotAngles * 2;
	if (SpinBotAngles = 360) {
		SpinBotAngles = -1;
	}

	SpinBotView.y = SpinBotAngles;

	UserCommand->ViewAngle[1] = AngleToShort(SpinBotView.y);
}

VOID FastSpinBotMW3Thread(MW3_UserCmd_t *UserCommand) {
	Vector SpinBotView;
	int SpinBotAngles = -1;

	SpinBotAngles++;
	SpinBotAngles * 4;
	if (SpinBotAngles = 360) {
		SpinBotAngles = -1;
	}

	SpinBotView.y = SpinBotAngles;

	UserCommand->ViewAngle[1] = AngleToShort(SpinBotView.y);
}

VOID AimDownMW3Thread(MW3_UserCmd_t *UserCommand) {
	Vector SpinBotAngles;
	SpinBotAngles.x = 80;
	if (SpinBotAngles.x < 79 || SpinBotAngles.x > 81)
		SpinBotAngles.x = 80;
	UserCommand->ViewAngle[0] = AngleToShort(SpinBotAngles.x);
}

VOID AimUpMW3Thread(MW3_UserCmd_t *UserCommand) {
	Vector SpinBotAngles;
	SpinBotAngles.x = -80;
	if (SpinBotAngles.x < -79 || SpinBotAngles.x > -81)
		SpinBotAngles.x = -80;
	UserCommand->ViewAngle[0] = AngleToShort(SpinBotAngles.x);
}

VOID AntiAimMW3Thread(MW3_UserCmd_t *UserCommand)
{
	/*UserCommand->ViewAngles[0] = 180 * (rand() % 10);
	UserCommand->ViewAngles[1] = (vec_t)fmod(-360.0f, 360.0f) * (rand() % 10);
	UserCommand->ViewAngles[2] = UserCommand->ViewAngles[1] + 110 * (rand() % 10);
	if (UserCommand->ViewAngles[0] < -180.0f)UserCommand->ViewAngles[0] += 360.0f;
	if (UserCommand->ViewAngles[0] > 180.0f)UserCommand->ViewAngles[0] -= 360.0f;
	if (UserCommand->ViewAngles[1] < -180.0f)UserCommand->ViewAngles[1] += 360.0f;
	if (UserCommand->ViewAngles[1] > 180.0f)UserCommand->ViewAngles[1] -= 360.0f;
	if (UserCommand->ViewAngles[2] < -180.0f)UserCommand->ViewAngles[2] += 360.0f;
	if (UserCommand->ViewAngles[2] > 180.0f)UserCommand->ViewAngles[2] -= 360.0f;*/

	MW3_ReadGame();
	MW3_clientActive * ca = NULL;
	ca = (MW3_clientActive*)MW3_getCA();
	D3DXVECTOR3 AntiAimAngles;

	//AntiAimAngles.x = ca->ViewAngles.x + 180;
	AntiAimAngles.y = ca->ViewAngles.y + 180;

	//UserCommand->ViewAngles[0] = ANGLE2SHORT(AntiAimAngles.x);
	UserCommand->ViewAngle[1] = ANGLE2SHORT(AntiAimAngles.y);
}

VOID MW3_CL_WritePackethk(int a1)
{
	if (MW3_Dvar_GetBOOL("cl_ingame"))
	{
		DWORD pd = *(DWORD*)MW3_clientActive_t;
		MW3_clientActive *cmdInput = (MW3_clientActive*)pd;
		MW3_UserCmd_t* cmd = cmdInput->GetCmd(cmdInput->CmdNum);
		MW3_clientActive *Ca = MW3_getCA();

		MW3_UserCmd_t* oldCmd = cmdInput->GetCmd(cmdInput->CmdNum - 1);
		Vector vMyAngles = Ca->ViewAngles;

		*oldCmd = *cmd;
		oldCmd->ServerTime--;

		if (*SpinBotMW3)
		{
			SpinBotMW3Thread(cmd);
		}

		if (*SlowSpinBotMW3)
		{
			SlowSpinBotMW3Thread(cmd);
		}

		if (*FastSpinBotMW3)
		{
			FastSpinBotMW3Thread(cmd);
		}

		if (*FakeAimDownMW3)
		{
			AimDownMW3Thread(cmd);
		}

		if (*FakeAimUpMW3)
		{
			AimUpMW3Thread(cmd);
		}

		if (*AntiAimMW3)
		{
			AntiAimMW3Thread(cmd);
		}
	}

	SaveStubPacket(a1);
}

VOID drawMaterialMW3(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT angle, const FLOAT *color) {
	MW3_DrawRotated(MW3_DrawRotated_R3, x, y, width, height, angle, color, MW3_White);
}

VOID drawRectangleMW3(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT lineWidth, const FLOAT *color) {
	drawMaterialMW3(x, y, width, lineWidth, 0.0f, color);
	drawMaterialMW3(x + width, y, lineWidth, height, 0.0f, color);
	drawMaterialMW3(x, y + height - lineWidth, width, lineWidth, 0.0f, color);
	drawMaterialMW3(x, y, lineWidth, height, 0.0f, color);
}

int ClosetsClientMW3()
{
	MW3_ReadGame();
	FLOAT objectDistance = 0.f;
	FLOAT flClosestDistance = (FLOAT)INT_MAX;
	int ClosestClient = -1;

	for (int i = 0; i < 18; i++)
	{
		if (i == MW3_cg->cNum)
			continue;

		if ((MW3_Entities[i]->Valid) && (MW3_Entities[i]->State & 1))
		{
			if (strcmp(MW3_cgs->gametype, "dm") == 0)
			{
				objectDistance = GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos);
				if (objectDistance < flClosestDistance)
				{
					flClosestDistance = objectDistance;
					ClosestClient = i;
				}
			}
			else
			{
				if (MW3_Clients[i]->mTeam != MW3_Clients[MW3_cg->cNum]->mTeam)
				{
					objectDistance = GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos);
					if (objectDistance < flClosestDistance)
					{
						flClosestDistance = objectDistance;
						ClosestClient = i;
					}
				}
			}
		}
	}

	return ClosestClient;
}

VOID ClientDetailsMW3()
{
	if (MW3_Dvar_GetBOOL("cl_ingame"))
	{
		if (*TargetDetailsMW3)
		{
			drawMaterialMW3(105.0f, 245.0f, 250.0f, 80.0f, 0, L_black);
			drawRectangleMW3(105.0f, 245.0f, 250.0f, 80.0f, 2.0f, Purple);
			MW3_DrawText("Target Details:", 105.0f, 241.0f, 0.4f, white);

			int ClientValid = ClosetsClientMW3();
			char* targettext;
			if (DrawTargetDetailsMW3)
			{
				char targetbuffer[3096];
				sprintf(targetbuffer, "Name: %s\nDistance: %.fm\nWeapon: %s", MW3_Clients[MW3_Clients[ClientValid]->mNum]->mName, GetDistance(MW3_RefDef->EyePos, MW3_Entities[ClientValid]->mPos), MW3_getWeaponname(MW3_Entities[ClientValid]->WeaponID));
				if (targetbuffer != "")
					targettext = targetbuffer;
				else
					targettext = "Target Details Error";
				DrawTargetDetailsMW3 = FALSE;
			}
			else
			{
				targettext = "Name: N/A\nDistance: N/A\nWeapon: N/A";
			}
			MW3_DrawText(targettext, 115.0f, 275.0f, 0.4f, white);
		}
	}
}

VOID MW3_Draw()
{
	char cNum[200];
	char count[250];
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;
	BOOL isFriend = FALSE;
	//MW3_toogleHack();

	if (MW3_Dvar_GetBOOL("cl_ingame"))
	{
		if (CrossHairMW3 == PlusCrosshairMW3)
		{
			MW3_DrawRect(CenterX - 10, CenterY, 21, 2, white);
			MW3_DrawRect(CenterX, CenterY - 10, 2, 21, white);
		}
		else if (CrossHairMW3 == XCrosshairMW3)
		{
			MW3_DrawLine2(615, 335, 628, 348, Purple, MW3_White);
			MW3_DrawLine2(645, 335, 632, 348, Purple, MW3_White);
			MW3_DrawLine2(615, 365, 628, 352, Purple, MW3_White);
			MW3_DrawLine2(645, 365, 632, 352, Purple, MW3_White);
		}

		MW3_ReadGame();
		Vector vScreen;
		Vector vScreenClient;
		int ClientValid = ClosetsClientMW3();
		MW3_cEntity *pEntityClient = MW3_Entities[ClientValid];
		int playercount = 0;

		for (int i = 0; i < 18; i++)
		{
			if ((MW3_Entities[i]->Valid) && (MW3_Entities[i]->State & 1))
			{
				playercount++;

				D3DXVECTOR3 outpos;
				MW3_cEntity *pEntity = MW3_Entities[i];

				FLOAT DistancePlayer = GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos);

				if (MW3_wS2(pEntity->mPos, vScreen))
				{
					if (i != MW3_cg->cNum)
					{
						int x = vScreen[0];
						int y = vScreen[1];

						FLOAT fWidth = (60.0f * 15.0f) / DistancePlayer;
						FLOAT fHeight = (100.0f * 15.0f) / DistancePlayer;

						D3DXVECTOR3 vHead;
						short mTag = *(short*)MW3_j_head;

						MW3_GetTagPos(0, MW3_Entities[i], mTag, vHead);

						D3DXVECTOR3 BoxPos;
						int newX = x + (fWidth / 2) + 1;
						MW3_wS2(vHead, BoxPos);
						int newY = BoxPos.y;
						int HeadX = BoxPos.x;
						int HeadY = BoxPos.y;
						if ((MW3_Clients[i]->mTeam == MW3_Clients[MW3_cg->cNum]->mTeam) && (strcmp(MW3_cgs->gametype, "dm") != 0))
						{
							if (ESPMW3 == ESPFriendlyMW3 || ESPMW3 == ESPBothMW3)
							{
								if (*ESPNamesMW3 && *ESPDistanceMW3)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s ^7[^5%.fm^7]", MW3_Clients[MW3_Clients[i]->mNum]->mName, GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos));
									MW3_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, Purple);
								}
								else if (*ESPNamesMW3)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s", MW3_Clients[MW3_Clients[i]->mNum]->mName);
									MW3_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, Purple);
								}
								else if (*ESPDistanceMW3)
								{
									char clientdetails[256];
									sprintf(clientdetails, "^7[^5%.fm^7]", GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos));
									MW3_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, Purple);
								}

								if (*ESPWeaponMW3)
								{
									char clientweapon[256];
									sprintf(clientweapon, "%s", MW3_getWeaponname(MW3_Entities[i]->WeaponID));
									MW3_DrawText(clientweapon, x - strlen(clientweapon) * 2.5, y + GetTextPos1(DistancePlayer), 0.4f, Purple);
								}

								if (CurrentMW3ESP == MW3_TWO_D)
								{
									MW3_DrawPlayerBox(MW3_Entities[i], mTag, fWidth, green);
								}
								else if (CurrentMW3ESP == MW3_THREE_D)
								{
									MW3_Make3DBox(MW3_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, green);
								}
								else if (CurrentMW3ESP == MW3_PYRAMID)
								{
									MW3_MakePyramid(MW3_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, green);
								}

								if (*ESPBonesMW3)
								{
									MW3_DrawBoneEsp(MW3_Entities[i], BlueStormz);
								}
							}

							if (SnapLineTeamMW3 == SnapLineFriendlyMW3 || SnapLineTeamMW3 == SnapLineTeamBothMW3)
							{
								if (SnapLinePosMW3 == SnapLineMiddleMW3)
								{
									MW3_DrawLine2(CenterX, CenterY, x, y, Purple, MW3_White);
								}
								else if (SnapLinePosMW3 == SnapLineBottomMW3)
								{
									MW3_DrawLine2(CenterX, 700.0f, x, y, Purple, MW3_White);
								}
								else if (SnapLinePosMW3 == SnapLineTopMW3)
								{
									MW3_DrawLine2(CenterX, 30.0f, x, y, Purple, MW3_White);
								}
							}
						}
						else
						{
							change_color = MW3_CanSeePlayer(0, MW3_Entities[i], 0x2805823) ? blue : red;
							change_shad_color = MW3_CanSeePlayer(0, MW3_Entities[i], 0x2805823) ? blue : red;

							if (ESPMW3 == ESPEnemyMW3 || ESPMW3 == ESPBothMW3)
							{
								if (*ESPNamesMW3 && *ESPDistanceMW3)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s ^7[^5%.fm^7]", MW3_Clients[MW3_Clients[i]->mNum]->mName, GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos));
									MW3_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, Purple);
								}
								else if (*ESPNamesMW3)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s", MW3_Clients[MW3_Clients[i]->mNum]->mName);
									MW3_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, Purple);
								}
								else if (*ESPDistanceMW3)
								{
									char clientdetails[256];
									sprintf(clientdetails, "^7[^5%.fm^7]", GetDistance(MW3_RefDef->EyePos, MW3_Entities[i]->mPos));
									MW3_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, Purple);
								}

								if (*ESPWeaponMW3)
								{
									char clientweapon[256];
									sprintf(clientweapon, "%s", MW3_getWeaponname(MW3_Entities[i]->WeaponID));
									MW3_DrawText(clientweapon, x - strlen(clientweapon) * 2.5, y + GetTextPos1(DistancePlayer), 0.4f, Purple);
								}

								if (*ESPTargetedMW3 && CanDrawTargetedESPMW3)
								{
									if (CurrentMW3ESP == MW3_TWO_D)
									{
										MW3_DrawPlayerBox(MW3_Entities[ClientValid], mTag, fWidth, Purple);
									}
									else if (CurrentMW3ESP == MW3_THREE_D)
									{
										MW3_Make3DBox(MW3_Entities[ClientValid], GetBoxWidth(), GetBoxHeight() + 10, Purple);
									}
									else if (CurrentMW3ESP == MW3_PYRAMID)
									{
										MW3_MakePyramid(MW3_Entities[ClientValid], GetBoxWidth(), GetBoxHeight() + 10, Purple);
									}
								}
								if (CurrentMW3ESP == MW3_TWO_D)
								{
									MW3_DrawPlayerBox(MW3_Entities[i], mTag, fWidth, change_color);
								}
								else if (CurrentMW3ESP == MW3_THREE_D)
								{
									MW3_Make3DBox(MW3_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, change_color);
								}
								else if (CurrentMW3ESP == MW3_PYRAMID)
								{
									MW3_MakePyramid(MW3_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, change_color);
								}

								if (*ESPBonesMW3)
								{
									MW3_DrawBoneEsp(MW3_Entities[i], BlueStormz);
								}
							}

							if (SnapLineTeamMW3 == SnapLineEnemyMW3 || SnapLineTeamMW3 == SnapLineTeamBothMW3)
							{
								if (*ESPTargetedMW3 && CanDrawTargetedESPMW3)
								{
									if (MW3_wS2(pEntityClient->mPos, vScreenClient))
									{
										if (SnapLinePosMW3 == SnapLineMiddleMW3)
										{
											MW3_DrawLine2(CenterX, CenterY, vScreenClient[0], vScreenClient[1], Purple, MW3_White);
										}
										else if (SnapLinePosMW3 == SnapLineBottomMW3)
										{
											MW3_DrawLine2(CenterX, 700.0f, vScreenClient[0], vScreenClient[1], Purple, MW3_White);
										}
										else if (SnapLinePosMW3 == SnapLineTopMW3)
										{
											MW3_DrawLine2(CenterX, 30.0f, vScreenClient[0], vScreenClient[1], Purple, MW3_White);
										}
									}
								}
								if (SnapLinePosMW3 == SnapLineMiddleMW3)
								{
									MW3_DrawLine2(CenterX, CenterY, x, y, change_color, MW3_White);
								}
								else if (SnapLinePosMW3 == SnapLineBottomMW3)
								{
									MW3_DrawLine2(CenterX, 700.0f, x, y, change_color, MW3_White);
								}
								else if (SnapLinePosMW3 == SnapLineTopMW3)
								{
									MW3_DrawLine2(CenterX, 30.0f, x, y, change_color, MW3_White);
								}
							}

							if (*VisibleWarningMW3)
							{
								if (MW3_CanSeePlayer(0, MW3_Entities[i], 0x2805823))
								{
									char msg[250];
									sprintf(msg, "*Enemy Visible*");
									MW3_DrawText("*Enemy Visible*", CenterX - strlen(msg) * 2.5, CenterY + 150, 0.6f, Purple);
								}
							}

							if (*ProximityWariningMW3)
							{
								if (ProximityWarning(DistancePlayer))
								{
									char msg[250];
									sprintf(msg, "Proximity Warning!");
									MW3_DrawText(msg, CenterX - strlen(msg) * 2.5, CenterY + 170, 0.6f, Purple);
								}
							}
						}
					}
				}
			}
		}
	}
}

XINPUT_STATE XInputStateMW3;
short currentButtonMW3, previousButtonMW3;
#define buttonHitMW3(button) ((currentButtonMW3 & ~previousButtonMW3) & button)
VOID updateInputMW3() {
	XInputGetState(0, &XInputStateMW3);
	previousButtonMW3 = currentButtonMW3;
	currentButtonMW3 = ~XInputStateMW3.Gamepad.wButtons & 0xFBFF;

	if (buttonHitMW3(XINPUT_GAMEPAD_DPAD_LEFT)) {
		openMW3 = !openMW3;
	}

	if (openMW3) {
		if (currentMW3 == MW3_AIMBOT || currentMW3 == MW3_ESP) {
			if (buttonHitMW3(XINPUT_GAMEPAD_B)) {
				currentMW3 = MW3_MAIN;
			}
		}

		if (buttonHitMW3(XINPUT_GAMEPAD_DPAD_UP)) {
			if (currentMW3 == MW3_MAIN)
			{
				scrollMW3--;
				if (scrollMW3 == -1) {
					scrollMW3 = maxoptionsMW3 - 1;
				}
			}
			else
			{
				if (currentMW3 == MW3_AIMBOT)
				{
					AimscrollMW3--;
					if (AimscrollMW3 == -1) {
						AimscrollMW3 = AimmaxoptionsMW3 - 1;
					}
				}
				else
				{
					if (currentMW3 == MW3_ESP)
					{
						ESPscrollMW3--;
						if (ESPscrollMW3 == -1) {
							ESPscrollMW3 = ESPmaxoptionsMW3 - 1;
						}
					}
				}
			}
		}

		if (buttonHitMW3(XINPUT_GAMEPAD_DPAD_DOWN)) {
			if (currentMW3 == MW3_MAIN)
			{
				scrollMW3++;
				if (scrollMW3 == maxoptionsMW3) {
					scrollMW3 = 0;
				}
			}
			else
			{
				if (currentMW3 == MW3_AIMBOT)
				{
					AimscrollMW3++;
					if (AimscrollMW3 == AimmaxoptionsMW3) {
						AimscrollMW3 = 0;
					}
				}
				else
				{
					if (currentMW3 == MW3_ESP)
					{
						ESPscrollMW3++;
						if (ESPscrollMW3 == ESPmaxoptionsMW3) {
							ESPscrollMW3 = 0;
						}
					}
				}
			}
		}

		if (buttonHitMW3(XINPUT_GAMEPAD_X)) {
			if (currentMW3 == MW3_MAIN)
			{
				if (scrollMW3 == 0)
				{
					currentMW3 = MW3_AIMBOT;
				}
				else
				{
					if (scrollMW3 == 1)
					{
						currentMW3 = MW3_ESP;
					}
					else if (scrollMW3 == 5)
					{
						switch (CrossHairMW3) {
						case 0:
							CrossHairMW3 = XCrosshairMW3;
							break;
						case 1:
							CrossHairMW3 = CrosshairOffMW3;
							break;
						case 2:
							CrossHairMW3 = PlusCrosshairMW3;
							break;
						default:
							CrossHairMW3 = CrosshairOffMW3;
							break;
						}
					}
					else
					{
						boptionsMW3[scrollMW3] = !boptionsMW3[scrollMW3];
					}
				}
			}
			else
			{
				if (currentMW3 == MW3_AIMBOT)
				{
					if (AimscrollMW3 == 0) {
						switch (CurrentMW3Aimbot) {
						case 0:
							CurrentMW3Aimbot = MW3_ONSCREEN;
							break;
						case 1:
							CurrentMW3Aimbot = MW3_CROSSHAIR;
							break;
						case 2:
							CurrentMW3Aimbot = MW3_Silent;
							break;
						case 3:
							CurrentMW3Aimbot = MW3_AIMBOT_OFF;
							break;
						case 4:
							CurrentMW3Aimbot = MW3_NEAREST;
							break;
						default:
							CurrentMW3Aimbot = MW3_AIMBOT_OFF;
							break;
						}
					}
					else {
						AimboptionsMW3[AimscrollMW3] = !AimboptionsMW3[AimscrollMW3];
					}
				}
				else
				{
					if (currentMW3 == MW3_ESP)
					{
						if (ESPscrollMW3 == 0) {
							switch (CurrentMW3ESP) {
							case 0:
								CurrentMW3ESP = MW3_THREE_D;
								break;
							case 1:
								CurrentMW3ESP = MW3_PYRAMID;
								break;
							case 2:
								CurrentMW3ESP = MW3_ESP_OFF;
								break;
							case 3:
								CurrentMW3ESP = MW3_TWO_D;
								break;
							default:
								CurrentMW3ESP = MW3_ESP_OFF;
								break;
							}
						}
						else if (ESPscrollMW3 == 5) {
							switch (ESPMW3) {
							case 0:
								ESPMW3 = ESPFriendlyMW3;
								break;
							case 1:
								ESPMW3 = ESPBothMW3;
								break;
							case 2:
								ESPMW3 = ESPOffMW3;
								break;
							case 3:
								ESPMW3 = ESPEnemyMW3;
								break;
							default:
								ESPMW3 = ESPOffMW3;
								break;
							}
						}
						else if (ESPscrollMW3 == 6) {
							switch (SnapLinePosMW3) {
							case 0:
								SnapLinePosMW3 = SnapLineBottomMW3;
								break;
							case 1:
								SnapLinePosMW3 = SnapLineTopMW3;
								break;
							case 2:
								SnapLinePosMW3 = SnapLinesOffMW3;
								break;
							case 3:
								SnapLinePosMW3 = SnapLineMiddleMW3;
								break;
							default:
								SnapLinePosMW3 = SnapLinesOffMW3;
								break;
							}
						}
						else if (ESPscrollMW3 == 7) {
							switch (SnapLineTeamMW3) {
							case 0:
								SnapLineTeamMW3 = SnapLineFriendlyMW3;
								break;
							case 1:
								SnapLineTeamMW3 = SnapLineTeamBothMW3;
								break;
							case 2:
								SnapLineTeamMW3 = SnapLineTeamOffMW3;
								break;
							case 3:
								SnapLineTeamMW3 = SnapLineEnemyMW3;
								break;
							default:
								SnapLineTeamMW3 = SnapLineTeamOffMW3;
								break;
							}
						}
						else {
							ESPboptionsMW3[ESPscrollMW3] = !ESPboptionsMW3[ESPscrollMW3];
						}
					}
				}
			}
		}
	}
}

VOID updateOptionsMW3()
{
	if (*norecoilMW3)
	{
		*(int*)MW3_NoRecoil = 0x60000000;
	}
	else
	{
		*(int*)MW3_NoRecoil = 0x4BFA0395;
	}
	if (*advanceduavMW3)
	{
		*(int*)MW3_Radar = 0x3B800001;
	}
	else
	{
		*(int*)MW3_Radar = 0x557C87FE;
	}
	if (*noswayMW3)
	{
		*(int*)MW3_WeaponSway = 0x60000000;
		*(int*)MW3_ZommingSway = 0x60000000;
	}
	else
	{
		*(int*)MW3_WeaponSway = 0x4BFFEA61;
		*(int*)MW3_ZommingSway = 0x4BFFFB9D;
	}
}

VOID MW3_Menu_Hook()
{
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;
	FLOAT middleMW3 = 950.0f;
	FLOAT centerMW3 = 100.0f;
	FLOAT bottomMW3 = 700.0f;

	updateInputMW3();
	updateOptionsMW3();

	if (MW3_Dvar_GetBOOL("cl_ingame"))
	{
		if (!FirstTimeInGameMW3)
		{
			openMW3 = FALSE;
			currentMW3 = 0;
			CurrentMW3ESP = 3;
			CurrentMW3Aimbot = 4;
			scrollMW3 = 0;
			AimscrollMW3 = 0;
			ESPscrollMW3 = 0;
			SnapLinePosMW3 = 3;
			SnapLineTeamMW3 = 3;
			ESPMW3 = 3;
			CrossHairMW3 = 2;
			FirstTimeInGameMW3 = TRUE;
		}

		if (openMW3)
		{
			if (currentMW3 == MW3_MAIN)
			{
				drawMaterialMW3(middleMW3 - 1, centerMW3, 250.0f, 30.0f + (maxoptionsMW3 * 30.0f), 0, L_black);
				drawRectangleMW3(middleMW3 - 1, centerMW3, 250.0f, 30.0f + (maxoptionsMW3 * 30.0f), 2.0f, Purple);
				MW3_DrawText("OutHere Menu", middleMW3 + 20, centerMW3 - 1, 0.6f, Purple);
				MW3_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomMW3, 0.4f, white);

				for (int i = 0; i < maxoptionsMW3; i++) {
					if (i == scrollMW3) {
						MW3_DrawText(optionsMW3[i], middleMW3 + 20, centerMW3 + 40.0f + (30.0f * i), 0.4f, Purple);
					}
					else {
						MW3_DrawText(optionsMW3[i], middleMW3 + 20, centerMW3 + 40.0f + (30.0f * i), 0.4f, white);
					}
					if (i == 0)
					{
						// fix
					}
					else if (i == 1)
					{
						// fix
					}
					else if (i == 5)
					{
						MW3_DrawText(CrossHairTypeMW3[CrossHairMW3], middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, CrossHairMW3 == CrosshairOffMW3 ? L_red2 : BlueStormz);
					}
					else if (boptionsMW3[i])
					{
						MW3_DrawText("ON", middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, L_green2);
					}
					else
					{
						MW3_DrawText("OFF", middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, Purple);
					}
				}
			}
			else
			{
				if (currentMW3 == MW3_AIMBOT)
				{
					drawMaterialMW3(middleMW3 - 1, centerMW3, 250.0f, 30.0f + (AimmaxoptionsMW3 * 30.0f), 0, L_black);
					drawRectangleMW3(middleMW3 - 1, centerMW3, 250.0f, 30.0f + (AimmaxoptionsMW3 * 30.0f), 2.0f, Purple);
					MW3_DrawText("Aimbot Options", middleMW3 + 20, centerMW3 - 1, 0.6f, Purple);
					MW3_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomMW3, 0.4f, white);

					for (int i = 0; i < AimmaxoptionsMW3; i++) {
						if (i == AimscrollMW3) {
							MW3_DrawText(AimoptionsMW3[i], middleMW3 + 20, centerMW3 + 40.0f + (30.0f * i), 0.4f, Purple);
						}
						else {
							MW3_DrawText(AimoptionsMW3[i], middleMW3 + 20, centerMW3 + 40.0f + (30.0f * i), 0.4f, white);
						}
						if (i == 0) {
							MW3_DrawText(AimbotTypeMW3[CurrentMW3Aimbot], middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, CurrentMW3Aimbot == MW3_AIMBOT_OFF ? L_red2 : BlueStormz);
						}
						else {
							if (AimboptionsMW3[i])
							{
								MW3_DrawText("ON", middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, L_green2);
							}
							else
							{
								MW3_DrawText("OFF", middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, Purple);
							}
						}
					}
				}
				else
				{
					if (currentMW3 == MW3_ESP)
					{
						drawMaterialMW3(middleMW3 - 1, centerMW3, 250.0f, 30.0f + (ESPmaxoptionsMW3 * 30.0f), 0, L_black);
						drawRectangleMW3(middleMW3 - 1, centerMW3, 250.0f, 30.0f + (ESPmaxoptionsMW3 * 30.0f), 2.0f, Purple);
						MW3_DrawText("ESP Options", middleMW3 + 20, centerMW3 - 1, 0.6f, Purple);
						MW3_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomMW3, 0.4f, white);

						for (int i = 0; i < ESPmaxoptionsMW3; i++) {
							if (i == ESPscrollMW3) {
								MW3_DrawText(ESPoptionsMW3[i], middleMW3 + 20, centerMW3 + 40.0f + (30.0f * i), 0.4f, Purple);
							}
							else {
								MW3_DrawText(ESPoptionsMW3[i], middleMW3 + 20, centerMW3 + 40.0f + (30.0f * i), 0.4f, white);
							}
							if (i == 0) {
								MW3_DrawText(espTypeMW3[CurrentMW3ESP], middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, CurrentMW3ESP == MW3_ESP_OFF ? L_red2 : BlueStormz);
							}
							else if (i == 5) {
								MW3_DrawText(ESPTeamTypeMW3[ESPMW3], middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, ESPMW3 == ESPOffMW3 ? L_red2 : BlueStormz);
							}
							else if (i == 6) {
								MW3_DrawText(SnapLineTypeMW3[SnapLinePosMW3], middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, SnapLinePosMW3 == SnapLinesOffMW3 ? L_red2 : BlueStormz);
							}
							else if (i == 7) {
								MW3_DrawText(SnapLineTeamTypeMW3[SnapLineTeamMW3], middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, SnapLineTeamMW3 == SnapLineTeamOffMW3 ? L_red2 : BlueStormz);
							}
							else {
								if (ESPboptionsMW3[i])
								{
									MW3_DrawText("ON", middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, L_green2);
								}
								else
								{
									MW3_DrawText("OFF", middleMW3 + 190, centerMW3 + 40.0f + (30.0f * i), 0.4f, Purple);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			MW3_DrawText("Press D-PAD Left To ^2Open^7 OutHere Menu", 30.0f, bottomMW3, 0.4f, white);//Bottom shit
		}
	}
	else
	{
		openMW3 = FALSE;
	}
}

HRESULT MW3_CL_DrawTextHook(const char *text, int maxChars, DWORD font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, const FLOAT *color, int style)
{
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;

	FLOAT rotation = 0;
	MW3_R_AddCmdDrawText(text, maxChars, font, x, y, xScale, yScale, rotation, color, style);

	//MW3_dMenu.BuildMenu("\n\n^2Masked Menu^7",(CenterX - 160),50,280,150,black,blackM,Purple);

	if (MW3_Key_Down(0, 0x12))
	{
		//printf("aiming");
	}
	else
	{
		LastFrameAimKey = FALSE;
	}

	MW3_Menu_Hook();
	MW3_Draw();
	MW3_Aimbot();
	ClientDetailsMW3();

	return S_OK;
}

VOID MW3_Start()
{
	PatchInJump((DWORD *)0x82174C78, (DWORD)MW3_CL_DrawTextHook, FALSE);
	HookFunctionStart((DWORD *)0x8216D998, (PDWORD)SaveStubPacket, (DWORD)MW3_CL_WritePackethk);
}

/////////////////////////////////////// MW3 Menu END /////////////////////////////////////

/////////////////////////////////////// BO1 Menu /////////////////////////////////////

BOOL FirstTimeInGameBO1 = FALSE;
BOOL openBO1 = FALSE;
int scrollBO1 = 0, maxoptionsBO1 = 8;
char optionsBO1[][32] = {
	"Aimbot Options",
	"ESP Options",
	"No Recoil",
	"Advanced UAV",
	"No Sway",
	"Visible Warning",
	"Proximity Warning",
	"Crosshair"
};
BOOL boptionsBO1[8] = { FALSE };
BOOL *norecoilBO1 = &boptionsBO1[2];
BOOL *advanceduavBO1 = &boptionsBO1[3];
BOOL *noswayBO1 = &boptionsBO1[4];
BOOL *VisibleWarningBO1 = &boptionsBO1[5];
BOOL *ProximityWariningBO1 = &boptionsBO1[6];
BOOL *CrosshairBO1 = &boptionsBO1[7];

int ESPscrollBO1 = 1, ESPmaxoptionsBO1 = 10;
char ESPoptionsBO1[][32] = {
	"ESP Type",
	"",
	"ESP Name",
	"ESP Distance",
	"ESP Weapon",
	"ESP Bones",
	"ESP Snap Lines",
	"ESP Enemy",
	"ESP Friendly",
	"Enemy Details"
};
BOOL ESPboptionsBO1[10] = { FALSE };
BOOL *ESPNamesBO1 = &ESPboptionsBO1[2];
BOOL *ESPDistanceBO1 = &ESPboptionsBO1[3];
BOOL *ESPWeaponBO1 = &ESPboptionsBO1[4];
BOOL *ESPBonesBO1 = &ESPboptionsBO1[5];
BOOL *ESPSnaplinesBO1 = &ESPboptionsBO1[6];
BOOL *ESPEnemysBO1 = &ESPboptionsBO1[7];
BOOL *ESPFriendlyBO1 = &ESPboptionsBO1[8];
BOOL *EnemyDetailsBO1 = &ESPboptionsBO1[9];

int AimscrollBO1 = 1, AimmaxoptionsBO1 = 10;
char AimoptionsBO1[][32] = {
	"Aimbot Type",
	"",
	"Auto Aim",
	"Auto Shoot",
	"Auto Wall",
	"No Spread",
	"Spin Bot",
	"Fake Aim Down",
	"Fake Aim Up",
	"Anti Aim"
};
BOOL AimboptionsBO1[10] = { FALSE };
BOOL *AutoAimBO1 = &AimboptionsBO1[2];
BOOL *AutoShootBO1 = &AimboptionsBO1[3];
BOOL *SpinBotBO1 = &AimboptionsBO1[6];
BOOL *FakeAimDownBO1 = &AimboptionsBO1[7];
BOOL *FakeAimUpBO1 = &AimboptionsBO1[8];
BOOL *AntiAimBO1 = &AimboptionsBO1[9];

char AimbotTypeBO1[5][32] = {
	"   Nearest",
	"   Visible",
	"   Crosshair",
	"   Silent Aim",
	"   OFF"
};

char espTypeBO1[4][32] = {
	"   2D",
	"   3D",
	"   Pyramid",
	"   OFF"
};

int BO1_MAIN = 0, BO1_AIMBOT = 1, BO1_ESP = 2;
int currentBO1 = BO1_MAIN;
int BO1_NEAREST = 0, BO1_ONSCREEN = 1, BO1_CROSSHAIR = 2, BO1_Silent = 3, BO1_AIMBOT_OFF = 4;
int CurrentBO1Aimbot = BO1_AIMBOT_OFF;
int BO1_TWO_D = 0, BO1_THREE_D = 1, BO1_PYRAMID = 2, BO1_ESP_OFF = 3;
int CurrentBO1ESP = BO1_ESP_OFF;

VOID BO1_Draw()
{
	char cNum[200];
	char count[250];
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;
	BOOL isFriend = FALSE;
	//BO1_toogleHack();

	//if (BO1_Dvar_GetBOOL("cl_ingame"))
	//{
	if (*CrosshairBO1)
	{
		BO1_DrawRect(CenterX - 10, CenterY, 21, 2, white);
		BO1_DrawRect(CenterX, CenterY - 10, 2, 21, white);
	}

	BO1_ReadGame();
	Vector vScreen;
	int playercount = 0;

	for (int i = 0; i < 12; i++)
	{
		if ((BO1_Entities[i]->Valid) && (BO1_Entities[i]->State == 64))
		{
			playercount++;

			D3DXVECTOR3 outpos;
			BO1_cEntity *pEntity = BO1_Entities[i];

			FLOAT DistancePlayer = GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos);

			if (BO1_wS2(pEntity->mPos, vScreen))
			{
				if (i != BO1_cg->cNum)
				{
					int x = vScreen[0];
					int y = vScreen[1];

					FLOAT fWidth = (60.0f * 15.0f) / DistancePlayer;
					FLOAT fHeight = (100.0f * 15.0f) / DistancePlayer;

					D3DXVECTOR3 vHead;
					short mTag = *(short*)BO1_j_head;

					BO1_GetTagPos(0, BO1_Entities[i], mTag, vHead);

					D3DXVECTOR3 BoxPos;
					int newX = x + (fWidth / 2) + 1;
					BO1_wS2(vHead, BoxPos);
					int newY = BoxPos.y;
					int HeadX = BoxPos.x;
					int HeadY = BoxPos.y;
					BOOL bVisible = (BO1_CanSeeFriendlyHead(0, BO1_Entities[i]) == 1);
					if ((BO1_Clients[i]->mTeam == BO1_Clients[BO1_cg->cNum]->mTeam) && (strcmp(BO1_cgs->gametype, "dm") != 0))
					{
						if (*ESPFriendlyBO1)
						{
							if (*ESPNamesBO1 && *ESPDistanceBO1)
							{
								char clientdetails[256];
								sprintf(clientdetails, "%s [^5%.fm^7]", BO1_Clients[BO1_Clients[i]->mNum]->mName, GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos));
								BO1_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
							}
							else if (*ESPNamesBO1)
							{
								char clientdetails[256];
								sprintf(clientdetails, "%s", BO1_Clients[BO1_Clients[i]->mNum]->mName);
								BO1_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
							}
							else if (*ESPDistanceBO1)
							{
								char clientdetails[256];
								sprintf(clientdetails, "[^5%.fm^7]", GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos));
								BO1_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
							}

							if (*ESPWeaponBO1)
							{
								char clientweapon[256];
								sprintf(clientweapon, "%s", BO1_getWeaponname(BO1_Entities[i]->WeaponID));
								BO1_DrawText(clientweapon, x - strlen(clientweapon) * 2.5, y + GetTextPos1(DistancePlayer), 0.4f, white);
							}

							if (CurrentBO1ESP == BO1_TWO_D)
							{
								BO1_DrawPlayerBox(BO1_Entities[i], mTag, fWidth, green);
							}
							else if (CurrentBO1ESP == BO1_THREE_D)
							{
								BO1_Make3DBox(BO1_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, green);
							}
							else if (CurrentBO1ESP == BO1_PYRAMID)
							{
								BO1_MakePyramid(BO1_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, green);
							}

							if (*ESPBonesBO1)
							{
								BO1_DrawBoneEsp(BO1_Entities[i], white);
							}

							if (*ESPSnaplinesBO1)
							{
								BO1_DrawLine2(CenterX, CenterY, x, y, green, BO1_White);
							}
						}
					}
					else
					{
						change_color = bVisible ? blue : red;
						change_shad_color = bVisible ? blue : red;

						if (*ESPEnemysBO1)
						{
							if (*ESPNamesBO1 && *ESPDistanceBO1)
							{
								char clientdetails[256];
								sprintf(clientdetails, "%s [^5%.fm^7]", BO1_Clients[BO1_Clients[i]->mNum]->mName, GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos));
								BO1_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
							}
							else if (*ESPNamesBO1)
							{
								char clientdetails[256];
								sprintf(clientdetails, "%s", BO1_Clients[BO1_Clients[i]->mNum]->mName);
								BO1_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
							}
							else if (*ESPDistanceBO1)
							{
								char clientdetails[256];
								sprintf(clientdetails, "[^5%.fm^7]", GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos));
								BO1_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
							}

							if (*ESPWeaponBO1)
							{
								char clientweapon[256];
								sprintf(clientweapon, "%s", BO1_getWeaponname(BO1_Entities[i]->WeaponID));
								BO1_DrawText(clientweapon, x - strlen(clientweapon) * 2.5, y + GetTextPos1(DistancePlayer), 0.4f, white);
							}

							if (CurrentBO1ESP == BO1_TWO_D)
							{
								BO1_DrawPlayerBox(BO1_Entities[i], mTag, fWidth, change_color);
							}
							else if (CurrentBO1ESP == BO1_THREE_D)
							{
								BO1_Make3DBox(BO1_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, change_color);
							}
							else if (CurrentBO1ESP == BO1_PYRAMID)
							{
								BO1_MakePyramid(BO1_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, change_color);
							}

							if (*ESPBonesBO1)
							{
								BO1_DrawBoneEsp(BO1_Entities[i], white);
							}

							if (*ESPSnaplinesBO1)
							{
								BO1_DrawLine2(CenterX, CenterY, x, y, change_color, BO1_White);
							}
						}

						if (*VisibleWarningBO1)
						{
							if (bVisible)
							{
								char msg[250];
								sprintf(msg, "*Enemy Visible*");
								BO1_DrawText(msg, CenterX - strlen(msg) * 2.5, CenterY + 150, 0.6f, Purple);
							}
						}

						if (*ProximityWariningBO1)
						{
							if (ProximityWarning(DistancePlayer))
							{
								char msg[250];
								sprintf(msg, "Proximity Warning!");
								BO1_DrawText(msg, CenterX - strlen(msg) * 2.5, CenterY + 170, 0.6f, Purple);
							}
						}
					}
				}
			}
		}
	}
	//}
}

VOID BO1_Aimbot()
{
	//if (BO1_Dvar_GetBOOL("cl_ingame"))
	//{
	DWORD pd = *(DWORD*)BO1_clientActive_t;
	BO1_clientActive *cmdInput = (BO1_clientActive*)pd;
	BO1_UserCmd* cmd = cmdInput->GetUserCmd(cmdInput->CurrentCmd);
	BO1_clientActive *Ca = BO1_getCA();

	BO1_UserCmd* oldCmd = cmdInput->GetUserCmd(cmdInput->CurrentCmd - 1);

	BO1_ReadGame();
	BO1_cEntity * Target = NULL;
	BO1_clientActive * ca = NULL;
	ca = (BO1_clientActive*)BO1_getCA();
	Vector vAngles;

	FLOAT objectDistance = 0.f;
	FLOAT flClosestDistance = (FLOAT)INT_MAX;

	D3DXVECTOR3 vHead, vBestHead;
	short mTag = *(short*)BO1_j_neck;

	if (CurrentBO1Aimbot == BO1_NEAREST)
	{
		for (int i = 0; i < 12; i++)
		{
			if (i == BO1_cg->cNum)
				continue;

			if ((BO1_Entities[i]->Valid) && (BO1_Entities[i]->State == 64))
			{
				if (strcmp(BO1_cgs->gametype, "dm") == 0)
				{
					objectDistance = GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos);

					Target = BO1_Entities[i];
					BO1_GetTagPos(0, Target, mTag, vHead);

					if (objectDistance < flClosestDistance)
					{
						vBestHead = vHead;
						flClosestDistance = objectDistance;
					}
				}
				else
				{
					if (BO1_Clients[i]->mTeam != BO1_Clients[BO1_cg->cNum]->mTeam)
					{
						objectDistance = GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos);

						Target = BO1_Entities[i];
						BO1_GetTagPos(0, Target, mTag, vHead);

						if (objectDistance < flClosestDistance)
						{
							vBestHead = vHead;
							flClosestDistance = objectDistance;
						}
					}
				}
			}
		}

		if (BO1_Key_Down(0, 0x12) || *AutoAimBO1)
		{
			if ((VALID(Target)) && (VALID(ca)))
			{
				vAngles = vectoangles(GetVec(BO1_RefDef->EyePos, vBestHead));
				ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
				ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;

				if (*AutoShootBO1)
				{
					oldCmd->buttons |= 1;
				}
			}
		}
	}
	else if (CurrentBO1Aimbot == BO1_ONSCREEN)
	{
		for (int i = 0; i < 12; i++)
		{
			if (i == BO1_cg->cNum)
				continue;

			if ((BO1_Entities[i]->Valid) && (BO1_Entities[i]->State == 64))
			{
				if (strcmp(BO1_cgs->gametype, "dm") == 0)
				{
					if (BO1_CanSeeFriendlyHead(0, BO1_Entities[i]) == 1)
					{
						objectDistance = GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos);

						Target = BO1_Entities[i];
						BO1_GetTagPos(0, Target, mTag, vHead);

						if (objectDistance < flClosestDistance)
						{
							vBestHead = vHead;
							flClosestDistance = objectDistance;
						}
					}
				}
				else
				{
					if (BO1_Clients[i]->mTeam != BO1_Clients[BO1_cg->cNum]->mTeam)
					{
						if (BO1_CanSeeFriendlyHead(0, BO1_Entities[i]) == 1)
						{
							objectDistance = GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos);

							Target = BO1_Entities[i];
							BO1_GetTagPos(0, Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
				}
			}
		}

		if (BO1_Key_Down(0, 0x12) || *AutoAimBO1)
		{
			if ((VALID(Target)) && (VALID(ca)))
			{
				vAngles = vectoangles(GetVec(BO1_RefDef->EyePos, vBestHead));
				ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
				ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;

				if (*AutoShootBO1)
				{
					oldCmd->buttons |= 1;
				}
			}
		}
	}
	else if (CurrentBO1Aimbot == BO1_CROSSHAIR)
	{
		if (LastFrameAimKey && pSavedTarget2)
			Target = pSavedTarget3;
		else
			Target = BO1_GetTargetByCrosshair();

		if (BO1_Key_Down(0, 0x12) || *AutoAimBO1)
		{
			if ((VALID(Target)) && (VALID(ca)))
			{
				D3DXVECTOR3 vHead;

				BO1_GetTagPos(0, Target, mTag, vHead);

				vAngles = vectoangles(GetVec(BO1_RefDef->EyePos, vHead));

				if (*AutoShootBO1)
				{
					oldCmd->buttons |= 1;
				}

				ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
				ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;
			}
		}
		pSavedTarget3 = Target;
		LastFrameAimKey = TRUE;
	}
	//}
}

VOID SpinBotBO1Thread(BO1_UserCmd *UserCommand) {
	Vector SpinBotAngles;
	SpinBotAngles.y += 45 * (rand() % 8);
	UserCommand->viewangles[1] = AngleToShort(SpinBotAngles.y);
}

VOID AimDownBO1Thread(BO1_UserCmd *UserCommand) {
	Vector SpinBotAngles;
	SpinBotAngles.x = 80;
	if (SpinBotAngles.x < 79 || SpinBotAngles.x > 81)
		SpinBotAngles.x = 80;
	UserCommand->viewangles[0] = AngleToShort(SpinBotAngles.x);
}

VOID AimUpBO1Thread(BO1_UserCmd *UserCommand) {
	Vector SpinBotAngles;
	SpinBotAngles.x = -80;
	if (SpinBotAngles.x < -79 || SpinBotAngles.x > -81)
		SpinBotAngles.x = -80;
	UserCommand->viewangles[0] = AngleToShort(SpinBotAngles.x);
}

VOID AntiAimBO1Thread(BO1_UserCmd *UserCommand)
{
	UserCommand->viewangles[0] = 180 + int(rand() % 10);
	UserCommand->viewangles[1] = (vec_t)fmod(-360.0f, 360.0f) + int(rand() % 10);
	UserCommand->viewangles[2] = UserCommand->viewangles[1] + 110 + int(rand() % 10);
	if (UserCommand->viewangles[0] < -180.0f)UserCommand->viewangles[0] += 360.0f;
	if (UserCommand->viewangles[0] > 180.0f)UserCommand->viewangles[0] -= 360.0f;
	if (UserCommand->viewangles[1] < -180.0f)UserCommand->viewangles[1] += 360.0f;
	if (UserCommand->viewangles[1] > 180.0f)UserCommand->viewangles[1] -= 360.0f;
	if (UserCommand->viewangles[2] < -180.0f)UserCommand->viewangles[2] += 360.0f;
	if (UserCommand->viewangles[2] > 180.0f)UserCommand->viewangles[2] -= 360.0f;
}

VOID BO1_CL_WritePackethk(int a1)
{
	//if (BO1_Dvar_GetBOOL("cl_ingame"))
	//{
	DWORD pd = *(DWORD*)BO1_clientActive_t;
	BO1_clientActive *cmdInput = (BO1_clientActive*)pd;
	BO1_UserCmd* cmd = cmdInput->GetUserCmd(cmdInput->CurrentCmd);
	BO1_clientActive *Ca = BO1_getCA();

	BO1_UserCmd* oldCmd = cmdInput->GetUserCmd(cmdInput->CurrentCmd - 1);
	Vector vMyAngles = Ca->ViewAngles;

	*oldCmd = *cmd;
	oldCmd->servertime--;

	BO1_ReadGame();
	BO1_cEntity * Target = NULL;
	BO1_clientActive * ca = NULL;
	ca = (BO1_clientActive*)BO1_getCA();
	Vector vAngles;

	FLOAT objectDistance = 0.f;
	FLOAT flClosestDistance = (FLOAT)INT_MAX;

	D3DXVECTOR3 vHead, vBestHead;
	short mTag = *(short*)BO1_j_neck;

	if (CurrentBO1Aimbot == BO1_Silent)
	{
		for (int i = 0; i < 12; i++)
		{
			if (i == BO1_cg->cNum)
				continue;

			if ((BO1_Entities[i]->Valid) && (BO1_Entities[i]->State == 64))
			{
				if (strcmp(BO1_cgs->gametype, "dm") == 0)
				{
					if (BO1_CanSeeFriendlyHead(0, BO1_Entities[i]) == 1)
					{
						objectDistance = GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos);

						Target = BO1_Entities[i];
						BO1_GetTagPos(0, Target, mTag, vHead);

						if (objectDistance < flClosestDistance)
						{
							vBestHead = vHead;
							flClosestDistance = objectDistance;
						}
					}
				}
				else
				{
					if (BO1_Clients[i]->mTeam != BO1_Clients[BO1_cg->cNum]->mTeam)
					{
						if (BO1_CanSeeFriendlyHead(0, BO1_Entities[i]) == 1)
						{
							objectDistance = GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos);

							Target = BO1_Entities[i];
							BO1_GetTagPos(0, Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
				}
			}
		}

		if ((VALID(Target)) && (VALID(ca)))
		{
			vAngles = vectoangles(GetVec(BO1_RefDef->EyePos, vBestHead));
			SilentAngles.x = vAngles.x - ca->RefAngles.x;
			SilentAngles.y = vAngles.y - ca->RefAngles.y;
			SilentAngles.z = 0;

			vMyAngles.x = SilentAngles.x;
			vMyAngles.y = SilentAngles.y;

			oldCmd->moveTonpere = cmd->moveTonpere;
			oldCmd->viewangles[0] = ANGLE2SHORT(vMyAngles.x);
			oldCmd->viewangles[1] = ANGLE2SHORT(vMyAngles.y);

			if (*AutoShootBO1)
			{
				oldCmd->buttons |= 1;
			}
		}
	}

	if (*SpinBotBO1)
	{
		SpinBotBO1Thread(cmd);
	}

	if (*FakeAimDownBO1)
	{
		AimDownBO1Thread(cmd);
	}

	if (*FakeAimUpBO1)
	{
		AimUpBO1Thread(cmd);
	}

	if (*AntiAimBO1)
	{
		AntiAimBO1Thread(cmd);
	}
	//}

	SaveStubPacket(a1);
}

VOID drawMaterialBO1(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT angle, const FLOAT *color) {
	BO1_DrawRotated(BO1_DrawRotated_R3, x, y, width, height, angle, color, BO1_White);
}

VOID drawRectangleBO1(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT lineWidth, const FLOAT *color) {
	drawMaterialBO1(x, y, width, lineWidth, 0.0f, color);
	drawMaterialBO1(x + width, y, lineWidth, height, 0.0f, color);
	drawMaterialBO1(x, y + height - lineWidth, width, lineWidth, 0.0f, color);
	drawMaterialBO1(x, y, lineWidth, height, 0.0f, color);
}

VOID ClientDetailsBO1()
{
	//if (BO1_Dvar_GetBOOL("cl_ingame"))
	//{
	BO1_ReadGame();
	FLOAT objectDistance = 0.f;
	FLOAT flClosestDistance = (FLOAT)INT_MAX;
	int ClientValid = -1;

	if (*EnemyDetailsBO1)
	{
		drawMaterialBO1(105.0f, 245.0f, 250.0f, 80.0f, 0, L_black);
		drawRectangleBO1(105.0f, 245.0f, 250.0f, 80.0f, 2.0f, Purple);
		BO1_DrawText("Client Details:", 105.0f, 241.0f, 0.4f, white);
		if (CurrentBO1Aimbot != BO1_AIMBOT_OFF)
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == BO1_cg->cNum)
					continue;

				if ((BO1_Entities[i]->Valid) && (BO1_Entities[i]->State == 64))
				{
					if (strcmp(BO1_cgs->gametype, "dm") == 0)
					{
						if (BO1_CanSeeFriendlyHead(0, BO1_Entities[i]) == 1)
						{
							objectDistance = GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos);
							if (objectDistance < flClosestDistance)
							{
								flClosestDistance = objectDistance;
								ClientValid = i;
							}
						}
					}
					else
					{
						if (BO1_Clients[i]->mTeam != BO1_Clients[BO1_cg->cNum]->mTeam)
						{
							if (BO1_CanSeeFriendlyHead(0, BO1_Entities[i]) == 1)
							{
								objectDistance = GetDistance(BO1_RefDef->EyePos, BO1_Entities[i]->mPos);
								if (objectDistance < flClosestDistance)
								{
									flClosestDistance = objectDistance;
									ClientValid = i;
								}
							}
						}
					}
				}

				if (BO1_CanSeeFriendlyHead(0, BO1_Entities[i]) == 1 && ClientValid != -1)
				{
					char clientname[256];
					sprintf(clientname, "Name: %s", BO1_Clients[BO1_Clients[ClientValid]->mNum]->mName);
					BO1_DrawText(clientname, 115.0f, 275.0f, 0.4f, white);

					char clientdistance[256];
					sprintf(clientdistance, "Distance: %.fm", GetDistance(BO1_RefDef->EyePos, BO1_Entities[ClientValid]->mPos));
					BO1_DrawText(clientdistance, 115.0f, 295.0f, 0.4f, white);

					char clientweapon[256];
					sprintf(clientweapon, "Weapon: %s", BO1_getWeaponname(BO1_Entities[ClientValid]->WeaponID));
					BO1_DrawText(clientweapon, 115.0f, 315.0f, 0.4f, white);
				}
			}
		}
	}
	//}
}

XINPUT_STATE XInputStateBO1;
short currentButtonBO1, previousButtonBO1;
#define buttonHitBO1(button) ((currentButtonBO1 & ~previousButtonBO1) & button)
VOID updateInputBO1() {
	XInputGetState(0, &XInputStateBO1);
	previousButtonBO1 = currentButtonBO1;
	currentButtonBO1 = ~XInputStateBO1.Gamepad.wButtons & 0xFBFF;

	if (buttonHitBO1(XINPUT_GAMEPAD_DPAD_LEFT)) {
		openBO1 = !openBO1;
	}

	if (openBO1) {
		if (currentBO1 == BO1_AIMBOT || currentBO1 == BO1_ESP) {
			if (buttonHitBO1(XINPUT_GAMEPAD_B)) {
				currentBO1 = BO1_MAIN;
			}
		}

		if (buttonHitBO1(XINPUT_GAMEPAD_DPAD_UP)) {
			if (currentBO1 == BO1_MAIN)
			{
				scrollBO1--;
				if (scrollBO1 == -1) {
					scrollBO1 = maxoptionsBO1 - 1;
				}
			}
			else
			{
				if (currentBO1 == BO1_AIMBOT)
				{
					AimscrollBO1--;
					if (AimscrollBO1 == 0 || AimscrollBO1 == -1) {
						AimscrollBO1 = AimmaxoptionsBO1 - 1;
					}
				}
				else
				{
					if (currentBO1 == BO1_ESP)
					{
						ESPscrollBO1--;
						if (ESPscrollBO1 == 0 || ESPscrollBO1 == -1) {
							ESPscrollBO1 = ESPmaxoptionsBO1 - 1;
						}
					}
				}
			}
		}

		if (buttonHitBO1(XINPUT_GAMEPAD_DPAD_DOWN)) {
			if (currentBO1 == BO1_MAIN)
			{
				scrollBO1++;
				if (scrollBO1 == maxoptionsBO1) {
					scrollBO1 = 0;
				}
			}
			else
			{
				if (currentBO1 == BO1_AIMBOT)
				{
					AimscrollBO1++;
					if (AimscrollBO1 == 0 || AimscrollBO1 == AimmaxoptionsBO1) {
						AimscrollBO1 = 1;
					}
				}
				else
				{
					if (currentBO1 == BO1_ESP)
					{
						ESPscrollBO1++;
						if (ESPscrollBO1 == 0 || ESPscrollBO1 == ESPmaxoptionsBO1) {
							ESPscrollBO1 = 1;
						}
					}
				}
			}
		}

		if (buttonHitBO1(XINPUT_GAMEPAD_X)) {
			if (currentBO1 == BO1_MAIN)
			{
				if (scrollBO1 == 0)
				{
					currentBO1 = BO1_AIMBOT;
				}
				else
				{
					if (scrollBO1 == 1)
					{
						currentBO1 = BO1_ESP;
					}
					else
					{
						boptionsBO1[scrollBO1] = !boptionsBO1[scrollBO1];
					}
				}
			}
			else
			{
				if (currentBO1 == BO1_AIMBOT)
				{
					if (AimscrollBO1 == 1) {
						switch (CurrentBO1Aimbot) {
						case 0:
							CurrentBO1Aimbot = BO1_ONSCREEN;
							break;
						case 1:
							CurrentBO1Aimbot = BO1_CROSSHAIR;
							break;
						case 2:
							CurrentBO1Aimbot = BO1_Silent;
							break;
						case 3:
							CurrentBO1Aimbot = BO1_AIMBOT_OFF;
							break;
						case 4:
							CurrentBO1Aimbot = BO1_NEAREST;
							break;
						default:
							CurrentBO1Aimbot = BO1_AIMBOT_OFF;
							break;
						}
					}
					else {
						AimboptionsBO1[AimscrollBO1] = !AimboptionsBO1[AimscrollBO1];
					}
				}
				else
				{
					if (currentBO1 == BO1_ESP)
					{
						if (ESPscrollBO1 == 1) {
							switch (CurrentBO1ESP) {
							case 0:
								CurrentBO1ESP = BO1_THREE_D;
								break;
							case 1:
								CurrentBO1ESP = BO1_PYRAMID;
								break;
							case 2:
								CurrentBO1ESP = BO1_ESP_OFF;
								break;
							case 3:
								CurrentBO1ESP = BO1_TWO_D;
								break;
							default:
								CurrentBO1ESP = BO1_ESP_OFF;
								break;
							}
						}
						else {
							ESPboptionsBO1[ESPscrollBO1] = !ESPboptionsBO1[ESPscrollBO1];
						}
					}
				}
			}
		}
	}
}

VOID updateOptionsBO1()
{
	if (*norecoilBO1)
	{
		*(int*)BO1_NoRecoil = 0x60000000;
	}
	else
	{
		*(int*)BO1_NoRecoil = 0x4BF67EC5;
	}
	if (*advanceduavBO1)
	{
		*(int*)BO1_Radar = 0x2B0B0001;
	}
	else
	{
		*(int*)BO1_Radar = 0x2B0B0000;
	}
	if (*noswayBO1)
	{
		*(int*)BO1_WeaponSway = 0x60000000;
		*(int*)BO1_ZommingSway = 0x60000000;
	}
	else
	{
		*(int*)BO1_WeaponSway = 0x4BFFEB09;
		*(int*)BO1_ZommingSway = 0x4BFFFA85;
	}
}

VOID BO1_Menu_Hook()
{
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;
	FLOAT middleBO1 = 950.0f;
	FLOAT centerBO1 = 100.0f;
	FLOAT bottomBO1 = 700.0f;

	updateInputBO1();
	updateOptionsBO1();

	//if (BO1_Dvar_GetBOOL("cl_ingame"))
	//{
	if (!FirstTimeInGameBO1)
	{
		openBO1 = FALSE;
		currentBO1 = 0;
		CurrentBO1ESP = 3;
		CurrentBO1Aimbot = 4;
		scrollBO1 = 0;
		AimscrollBO1 = 1;
		ESPscrollBO1 = 1;
		FirstTimeInGameBO1 = TRUE;
	}

	if (openBO1)
	{
		if (currentBO1 == BO1_MAIN)
		{
			drawMaterialBO1(middleBO1 - 1, centerBO1, 250.0f, 30.0f + (maxoptionsBO1 * 30.0f), 0, L_black);
			drawRectangleBO1(middleBO1 - 1, centerBO1, 250.0f, 30.0f + (maxoptionsBO1 * 30.0f), 2.0f, Purple);
			BO1_DrawText("OutHere Menu", middleBO1 + 20, centerBO1 - 1, 0.6f, Purple);
			BO1_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomBO1, 0.4f, white);

			for (int i = 0; i < maxoptionsBO1; i++) {
				if (i == scrollBO1) {
					BO1_DrawText(optionsBO1[i], middleBO1 + 20, centerBO1 + 40.0f + (30.0f * i), 0.4f, Purple);
				}
				else {
					BO1_DrawText(optionsBO1[i], middleBO1 + 20, centerBO1 + 40.0f + (30.0f * i), 0.4f, white);
				}
				if (i == 0)
				{
					// fix
				}
				else if (i == 1)
				{
					// fix
				}
				else if (boptionsBO1[i])
				{
					BO1_DrawText("ON", middleBO1 + 190, centerBO1 + 40.0f + (30.0f * i), 0.4f, L_green2);
				}
				else
				{
					BO1_DrawText("OFF", middleBO1 + 190, centerBO1 + 40.0f + (30.0f * i), 0.4f, L_red2);
				}
			}
		}
		else
		{
			if (currentBO1 == BO1_AIMBOT)
			{
				drawMaterialBO1(middleBO1 - 1, centerBO1, 250.0f, 30.0f + (AimmaxoptionsBO1 * 30.0f), 0, L_black);
				drawRectangleBO1(middleBO1 - 1, centerBO1, 250.0f, 30.0f + (AimmaxoptionsBO1 * 30.0f), 2.0f, Purple);
				BO1_DrawText("Aimbot Options", middleBO1 + 20, centerBO1 - 1, 0.6f, Purple);
				BO1_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomBO1, 0.4f, white);

				for (int i = 0; i < AimmaxoptionsBO1; i++) {
					if (i == AimscrollBO1) {
						BO1_DrawText(AimoptionsBO1[i], middleBO1 + 20, centerBO1 + 40.0f + (30.0f * i), 0.4f, Purple);
					}
					else {
						BO1_DrawText(AimoptionsBO1[i], middleBO1 + 20, centerBO1 + 40.0f + (30.0f * i), 0.4f, white);
					}
					if (i == 0) {
						// fix
					}
					else if (i == 1) {
						BO1_DrawText(AimbotTypeBO1[CurrentBO1Aimbot], middleBO1 + 20, centerBO1 + 40.0f + (30.0f * i), 0.6f, AimscrollBO1 == 1 ? Purple : white);//Menu Text
					}
					else {
						if (AimboptionsBO1[i])
						{
							BO1_DrawText("ON", middleBO1 + 190, centerBO1 + 40.0f + (30.0f * i), 0.4f, L_green2);
						}
						else
						{
							BO1_DrawText("OFF", middleBO1 + 190, centerBO1 + 40.0f + (30.0f * i), 0.4f, L_red2);
						}
					}
				}
			}
			else
			{
				if (currentBO1 == BO1_ESP)
				{
					drawMaterialBO1(middleBO1 - 1, centerBO1, 250.0f, 30.0f + (ESPmaxoptionsBO1 * 30.0f), 0, L_black);
					drawRectangleBO1(middleBO1 - 1, centerBO1, 250.0f, 30.0f + (ESPmaxoptionsBO1 * 30.0f), 2.0f, Purple);
					BO1_DrawText("ESP Options", middleBO1 + 20, centerBO1 - 1, 0.6f, Purple);
					BO1_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomBO1, 0.4f, white);

					for (int i = 0; i < ESPmaxoptionsBO1; i++) {
						if (i == ESPscrollBO1) {
							BO1_DrawText(ESPoptionsBO1[i], middleBO1 + 20, centerBO1 + 40.0f + (30.0f * i), 0.4f, Purple);
						}
						else {
							BO1_DrawText(ESPoptionsBO1[i], middleBO1 + 20, centerBO1 + 40.0f + (30.0f * i), 0.4f, white);
						}
						if (i == 0) {
							// fix
						}
						else if (i == 1) {
							BO1_DrawText(espTypeBO1[CurrentBO1ESP], middleBO1 + 20, centerBO1 + 40.0f + (30.0f * i), 0.6f, ESPscrollBO1 == 1 ? Purple : white);//Menu Text
						}
						else {
							if (ESPboptionsBO1[i])
							{
								BO1_DrawText("ON", middleBO1 + 190, centerBO1 + 40.0f + (30.0f * i), 0.4f, L_green2);
							}
							else
							{
								BO1_DrawText("OFF", middleBO1 + 190, centerBO1 + 40.0f + (30.0f * i), 0.4f, Purple);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		BO1_DrawText("Press D-PAD Left To ^2Open^7 OutHere Menu", 30.0f, bottomBO1, 0.6f, white);//Bottom shit
	}
	//}
	//else
	//{
	//openBO1 = FALSE;
	//}
}

HRESULT BO1_CL_RenderScenehk(DWORD a1)
{
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;

	BO1_R_RenderScene_(a1);//Call back
	BO1_RefDefCalc = a1;
	//BO1_dMenu.BuildMenu("\n\n^2Masked Menu^7",(1280 - 160),50,280,150,black,blackM,Purple);

	if (BO1_Key_Down(0, 0x12))
	{
		//printf("aiming");
	}
	else
	{
		LastFrameAimKey = FALSE;
	}

	BO1_Menu_Hook();
	BO1_Draw();
	BO1_Aimbot();
	ClientDetailsBO1();

	return S_OK;
}

VOID BO1_Start()
{
	PatchInJump((DWORD *)0x8222B5D8, (DWORD)BO1_CL_RenderScenehk, FALSE);
	//HookFunctionStart((DWORD *)0x82239508, (PDWORD)SaveStubPacket, (DWORD)BO1_CL_WritePackethk);
}

/////////////////////////////////// BO1 Menu END /////////////////////////////////////

/////////////////////////////////////// MW2 Menu /////////////////////////////////////

BOOL DrawTargetDetailsMW2 = FALSE;
BOOL FirstTimeInGameMW2 = FALSE;
BOOL openMW2 = FALSE;
int scrollMW2 = 0, maxoptionsMW2 = 6;
char optionsMW2[][32] = {
	"Aimbot Options",
	"ESP Options",
	"No Recoil",
	"Advanced UAV",
	"No Sway",
	"Crosshair"
};
BOOL boptionsMW2[6] = { FALSE };
BOOL *norecoilMW2 = &boptionsMW2[2];
BOOL *advanceduavMW2 = &boptionsMW2[3];
BOOL *noswayMW2 = &boptionsMW2[4];

int ESPscrollMW2 = 0, ESPmaxoptionsMW2 = 9;
char ESPoptionsMW2[][32] = {
	"ESP Type:",
	"ESP Name",
	"ESP Distance",
	"ESP Weapon",
	"ESP Bones",
	"ESP Team",
	"Snap Line Screen Pos:",
	"Snap Line Team:",
	"Targeted Client ESP"
};
BOOL ESPboptionsMW2[9] = { FALSE };
BOOL *ESPNamesMW2 = &ESPboptionsMW2[1];
BOOL *ESPDistanceMW2 = &ESPboptionsMW2[2];
BOOL *ESPWeaponMW2 = &ESPboptionsMW2[3];
BOOL *ESPBonesMW2 = &ESPboptionsMW2[4];

int AimscrollMW2 = 0, AimmaxoptionsMW2 = 10;
char AimoptionsMW2[][32] = {
	"Aimbot Type:",
	"Auto Aim",
	"Auto Shoot",
	//"No Spread",
	"Spin Bot",
	"Fake Aim Down",
	"Fake Aim Up",
	"Anti Aim",
	"Visible Warning",
	"Proximity Warning",
	"Target Details"
};
BOOL AimboptionsMW2[11] = { FALSE };
BOOL *AutoAimMW2 = &AimboptionsMW2[1];
BOOL *AutoShootMW2 = &AimboptionsMW2[2];
BOOL *NoSpreadMW2 = &AimboptionsMW2[3];
BOOL *SpinBotMW2 = &AimboptionsMW2[4];
BOOL *AimDownMW2 = &AimboptionsMW2[5];
BOOL *AimUpMW2 = &AimboptionsMW2[6];
BOOL *AntiAimMW2 = &AimboptionsMW2[7];
BOOL *VisibleWarningMW2 = &AimboptionsMW2[8];
BOOL *ProximityWarningMW2 = &AimboptionsMW2[9];
BOOL *TargetDetailsMW2 = &AimboptionsMW2[10];

char AimbotTypeMW2[5][32] = {
	"Nearest",
	"Visible",
	"Crosshair",
	"Silent Aim",
	"OFF"
};

char espTypeMW2[5][32] = {
	"2D",
	"3D",
	"Pyramid",
	"Filled",
	"OFF"
};

char ESPTeamTypeMW2[4][32] = {
	"Enemy",
	"Friendly",
	"Both",
	"OFF"
};

char SnapLineTypeMW2[4][32] = {
	"Middle",
	"Bottom",
	"Top",
	"OFF"
};

char SnapLineTeamTypeMW2[4][32] = {
	"Enemy",
	"Friendly",
	"Both",
	"OFF"
};

char CrossHairTypeMW2[3][32] = {
	"+",
	"X",
	"OFF"
};

int MW2_MAIN = 0, MW2_AIMBOT = 1, MW2_ESP = 2;
int currentMW2 = MW2_MAIN;
int MW2_NEAREST = 0, MW2_ONSCREEN = 1, MW2_CROSSHAIR = 2, MW2_Silent = 3, MW2_AIMBOT_OFF = 4;
int CurrentMW2Aimbot = MW2_AIMBOT_OFF;
int MW2_TWO_D = 0, MW2_THREE_D = 1, MW2_PYRAMID = 2, MW2_FILLED = 3, MW2_ESP_OFF = 4;
int CurrentMW2ESP = MW2_ESP_OFF;
int ESPEnemyMW2 = 0, ESPFriendlyMW2 = 1, ESPBothMW2 = 2, ESPOffMW2 = 3;
int ESPMW2 = ESPOffMW2;
int SnapLineMiddleMW2 = 0, SnapLineBottomMW2 = 1, SnapLineTopMW2 = 2, SnapLinesOffMW2 = 3;
int SnapLinePosMW2 = SnapLinesOffMW2;
int SnapLineEnemyMW2 = 0, SnapLineFriendlyMW2 = 1, SnapLineTeamBothMW2 = 2, SnapLineTeamOffMW2 = 3;
int SnapLineTeamMW2 = SnapLineTeamOffMW2;
int PlusCrosshairMW2 = 0, XCrosshairMW2 = 1, CrosshairOffMW2 = 2;
int CrossHairMW2 = CrosshairOffMW2;

VOID MW2_Aimbot()
{
	if (MW2_Dvar_GetBOOL("cl_ingame"))
	{
		DWORD pd = *(DWORD*)MW2_clientActive_t;
		MW2_clientActive *cmdInput = (MW2_clientActive*)pd;
		MW2_UserCmd_t* cmd = cmdInput->GetCmd(cmdInput->CmdNum);
		MW2_clientActive *Ca = MW2_getCA();

		MW2_UserCmd_t* oldCmd = cmdInput->GetCmd(cmdInput->CmdNum - 1);

		MW2_ReadGame();
		MW2_cEntity * Target = NULL;
		MW2_clientActive * ca = NULL;
		ca = (MW2_clientActive*)MW2_getCA();
		Vector vAngles;

		FLOAT objectDistance = 0.f;
		FLOAT flClosestDistance = (FLOAT)INT_MAX;

		D3DXVECTOR3 vHead, vBestHead;
		short mTag = *(short*)MW2_j_neck;

		if (CurrentMW2Aimbot == MW2_NEAREST)
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == MW2_cg->cNum)
					continue;

				if ((MW2_Entities[i]->Valid) && (MW2_Entities[i]->State & 1))
				{
					if (strcmp(MW2_cgs->gametype, "dm") == 0)
					{
						objectDistance = GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos);

						Target = MW2_Entities[i];
						MW2_GetTagPos(0, Target, mTag, vHead);

						if (objectDistance < flClosestDistance)
						{
							vBestHead = vHead;
							flClosestDistance = objectDistance;
						}
					}
					else
					{
						if (MW2_Clients[i]->mTeam != MW2_Clients[MW2_cg->cNum]->mTeam)
						{
							objectDistance = GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos);

							Target = MW2_Entities[i];
							MW2_GetTagPos(0, Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
				}
			}

			if ((VALID(Target)) && (VALID(ca)))
			{
				DrawTargetDetailsMW2 = TRUE;
				if (MW2_Key_Down(0, 0x12) || *AutoAimMW2)
				{
					vAngles = vectoangles(GetVec(MW2_RefDef->EyePos, vBestHead));
					ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
					ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;

					if (*AutoShootMW2)
					{
						oldCmd->Button |= 1;
					}
				}
			}
		}
		else if (CurrentMW2Aimbot == MW2_ONSCREEN)
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == MW2_cg->cNum)
					continue;

				if ((MW2_Entities[i]->Valid) && (MW2_Entities[i]->State & 1))
				{
					if (strcmp(MW2_cgs->gametype, "dm") == 0)
					{
						if (MW2_CanSeePlayer(0, MW2_Entities[i], 0x280F803))
						{
							objectDistance = GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos);

							Target = MW2_Entities[i];
							MW2_GetTagPos(0, Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
					else
					{
						if (MW2_Clients[i]->mTeam != MW2_Clients[MW2_cg->cNum]->mTeam)
						{
							if (MW2_CanSeePlayer(0, MW2_Entities[i], 0x280F803))
							{
								objectDistance = GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos);

								Target = MW2_Entities[i];
								MW2_GetTagPos(0, Target, mTag, vHead);

								if (objectDistance < flClosestDistance)
								{
									vBestHead = vHead;
									flClosestDistance = objectDistance;
								}
							}
						}
					}
				}
			}

			if ((VALID(Target)) && (VALID(ca)))
			{
				DrawTargetDetailsMW2 = TRUE;
				if (MW2_Key_Down(0, 0x12) || *AutoAimMW2)
				{
					vAngles = vectoangles(GetVec(MW2_RefDef->EyePos, vBestHead));
					ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
					ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;

					if (*AutoShootMW2)
					{
						oldCmd->Button |= 1;
					}
				}
			}
		}
		else if (CurrentMW2Aimbot == MW2_CROSSHAIR)
		{
			if (LastFrameAimKey && pSavedTarget5)
				Target = pSavedTarget5;
			else
				Target = MW2_GetTargetByCrosshair();

			if ((VALID(Target)) && (VALID(ca)))
			{
				DrawTargetDetailsMW2 = TRUE;
				if (MW2_Key_Down(0, 0x12) || *AutoAimMW2)
				{
					D3DXVECTOR3 vHead;

					MW2_GetTagPos(0, Target, mTag, vHead);

					vAngles = vectoangles(GetVec(MW2_RefDef->EyePos, vHead));

					if (*AutoShootMW2)
					{
						oldCmd->Button |= 1;
					}

					ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
					ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;
				}
				pSavedTarget5 = Target;
				LastFrameAimKey = TRUE;
			}
		}
		else if (CurrentMW2Aimbot == MW2_Silent)
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == MW2_cg->cNum)
					continue;

				if ((MW2_Entities[i]->Valid) && (MW2_Entities[i]->State & 1))
				{
					if (strcmp(MW2_cgs->gametype, "dm") == 0)
					{
						if (MW2_CanSeePlayer(0, MW2_Entities[i], 0x280F803))
						{
							objectDistance = GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos);

							Target = MW2_Entities[i];
							MW2_GetTagPos(0, Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
					else
					{
						if (MW2_Clients[i]->mTeam != MW2_Clients[MW2_cg->cNum]->mTeam)
						{
							if (MW2_CanSeePlayer(0, MW2_Entities[i], 0x280F803))
							{
								objectDistance = GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos);

								Target = MW2_Entities[i];
								MW2_GetTagPos(0, Target, mTag, vHead);

								if (objectDistance < flClosestDistance)
								{
									vBestHead = vHead;
									flClosestDistance = objectDistance;
								}
							}
						}
					}
				}
			}

			if ((VALID(Target)) && (VALID(ca)))
			{
				DrawTargetDetailsMW2 = TRUE;

				vAngles = vectoangles(GetVec(MW2_RefDef->EyePos, vBestHead));
				SilentAngles.x = vAngles.x - ca->RefAngles.x;
				SilentAngles.y = vAngles.y - ca->RefAngles.y;

				//oldCmd->moveTonpere = cmd->moveTonpere;
				oldCmd->ViewAngles[0] = ANGLE2SHORT(SilentAngles.x);
				oldCmd->ViewAngles[1] = ANGLE2SHORT(SilentAngles.y);

				if (*AutoShootMW2)
				{
					oldCmd->Button |= 1;
				}
			}
		}
	}
}

VOID MW2_Draw()
{
	char cNum[200];
	char count[250];
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;
	BOOL isFriend = FALSE;
	//MW2_toogleHack();

	if (MW2_Dvar_GetBOOL("cl_ingame"))
	{
		if (CrossHairMW2 == PlusCrosshairMW2)
		{
			MW2_DrawRect(CenterX - 10, CenterY, 21, 2, white);
			MW2_DrawRect(CenterX, CenterY - 10, 2, 21, white);
		}
		else if (CrossHairMW2 == XCrosshairMW2)
		{
			MW2_DrawLine2(625, 345, 638, 358, yellow, MW2_White);
			MW2_DrawLine2(655, 345, 642, 358, yellow, MW2_White);
			MW2_DrawLine2(625, 375, 638, 362, yellow, MW2_White);
			MW2_DrawLine2(655, 375, 642, 362, yellow, MW2_White);
		}

		MW2_ReadGame();
		Vector vScreen;
		int playercount = 0;

		for (int i = 0; i < 12; i++)
		{
			if ((MW2_Entities[i]->Valid) && (MW2_Entities[i]->State & 1))
			{
				playercount++;

				D3DXVECTOR3 outpos;
				MW2_cEntity *pEntity = MW2_Entities[i];

				FLOAT DistancePlayer = GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos);

				if (MW2_wS2(pEntity->mPos, vScreen))
				{
					if (i != MW2_cg->cNum)
					{
						int x = vScreen[0];
						int y = vScreen[1];

						FLOAT fWidth = (60.0f * 15.0f) / DistancePlayer;
						FLOAT fHeight = (100.0f * 15.0f) / DistancePlayer;

						D3DXVECTOR3 vHead;
						short mTag = *(short*)MW2_j_head;

						MW2_GetTagPos(0, MW2_Entities[i], mTag, vHead);

						D3DXVECTOR3 BoxPos;
						int newX = x + (fWidth / 2) + 1;
						MW2_wS2(vHead, BoxPos);
						int newY = BoxPos.y;
						int HeadX = BoxPos.x;
						int HeadY = BoxPos.y;
						if ((MW2_Clients[i]->mTeam == MW2_Clients[MW2_cg->cNum]->mTeam) && (strcmp(MW2_cgs->gametype, "dm") != 0))
						{
							if (ESPMW2 == ESPFriendlyMW2 || ESPMW2 == ESPBothMW2)
							{
								if (*ESPNamesMW2 && *ESPDistanceMW2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s [^5%.fm^7]", MW2_Clients[MW2_Clients[i]->mNum]->mName, GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos));
									MW2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
								}
								else if (*ESPNamesMW2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s", MW2_Clients[MW2_Clients[i]->mNum]->mName);
									MW2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
								}
								else if (*ESPDistanceMW2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "[^5%.fm^7]", GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos));
									MW2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
								}

								if (CurrentMW2ESP == MW2_TWO_D)
								{
									MW2_DrawPlayerBox(MW2_Entities[i], mTag, fWidth, green);
								}
								else if (CurrentMW2ESP == MW2_THREE_D)
								{
									MW2_Make3DBox(MW2_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, green);
								}
								else if (CurrentMW2ESP == MW2_PYRAMID)
								{
									MW2_MakePyramid(MW2_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, green);
								}
								else if (CurrentMW2ESP == MW2_FILLED)
								{
									MW2_DrawBox(x, y, fWidth, fHeight, L_green, green);
								}
							}

							if (SnapLineTeamMW2 == SnapLineFriendlyMW2 || SnapLineTeamMW2 == SnapLineTeamBothMW2)
							{
								if (SnapLinePosMW2 == SnapLineMiddleMW2)
								{
									MW2_DrawLine2(CenterX, CenterY, x, y, green, MW2_White);
								}
								else if (SnapLinePosMW2 == SnapLineBottomMW2)
								{
									MW2_DrawLine2(CenterX, 700.0f, x, y, green, MW2_White);
								}
								else if (SnapLinePosMW2 == SnapLineTopMW2)
								{
									MW2_DrawLine2(CenterX, 30.0f, x, y, green, MW2_White);
								}
							}
						}
						else
						{
							change_color = MW2_CanSeePlayer(0, MW2_Entities[i], 0x280F803) ? blue : Purple;
							change_shad_color = MW2_CanSeePlayer(0, MW2_Entities[i], 0x280F803) ? blue : red;
							change_shad_colorInside = MW2_CanSeePlayer(0, MW2_Entities[i], 0x280F803) ? L_blue : L_red;
							change_shad_colorOutside = MW2_CanSeePlayer(0, MW2_Entities[i], 0x280F803) ? blue : red;

							if (ESPMW2 == ESPEnemyMW2 || ESPMW2 == ESPBothMW2)
							{
								if (*ESPNamesMW2 && *ESPDistanceMW2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s [^5%.fm^7]", MW2_Clients[MW2_Clients[i]->mNum]->mName, GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos));
									MW2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
								}
								else if (*ESPNamesMW2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "%s", MW2_Clients[MW2_Clients[i]->mNum]->mName);
									MW2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
								}
								else if (*ESPDistanceMW2)
								{
									char clientdetails[256];
									sprintf(clientdetails, "[^5%.fm^7]", GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos));
									MW2_DrawText(clientdetails, HeadX - strlen(clientdetails) * 2.5, HeadY - GetTextPos(DistancePlayer), 0.4f, white);
								}

								if (CurrentMW2ESP == MW2_TWO_D)
								{
									MW2_DrawPlayerBox(MW2_Entities[i], mTag, fWidth, change_color);
								}
								else if (CurrentMW2ESP == MW2_THREE_D)
								{
									MW2_Make3DBox(MW2_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, change_color);
								}
								else if (CurrentMW2ESP == MW2_PYRAMID)
								{
									MW2_MakePyramid(MW2_Entities[i], GetBoxWidth(), GetBoxHeight() + 10, change_color);
								}
								else if (CurrentMW2ESP == MW2_FILLED)
								{
									MW2_DrawBox(x, y, fWidth, fHeight, change_shad_colorInside, change_shad_colorOutside);
								}
							}

							if (SnapLineTeamMW2 == SnapLineEnemyMW2 || SnapLineTeamMW2 == SnapLineTeamBothMW2)
							{
								if (SnapLinePosMW2 == SnapLineMiddleMW2)
								{
									MW2_DrawLine2(CenterX, CenterY, x, y, change_color, MW2_White);
								}
								else if (SnapLinePosMW2 == SnapLineBottomMW2)
								{
									MW2_DrawLine2(CenterX, 700.0f, x, y, change_color, MW2_White);
								}
								else if (SnapLinePosMW2 == SnapLineTopMW2)
								{
									MW2_DrawLine2(CenterX, 30.0f, x, y, change_color, MW2_White);
								}
							}

							if (*VisibleWarningMW2)
							{
								if (MW2_CanSeePlayer(0, MW2_Entities[i], 0x280F803))
								{
									char msg[250];
									sprintf(msg, "*Enemy Visible*");
									MW2_DrawText(msg, CenterX - 60, CenterY + 150, 0.6f, Purple);
								}
							}

							if (*ProximityWarningMW2)
							{
								if (ProximityWarning(DistancePlayer))
								{
									char msg[250];
									sprintf(msg, "Proximity Warning!");
									MW2_DrawText(msg, CenterX - strlen(msg) * 2.5, CenterY + 170, 0.6f, Purple);
								}
							}
						}
					}
				}
			}
		}
	}
}

VOID SpinBotMW2Thread(MW2_UserCmd_t *UserCommand) {
	Vector SpinBotAngles;
	SpinBotAngles.y += 45 * (rand() % 8);
	UserCommand->ViewAngles[1] = AngleToShort(SpinBotAngles.y);
}

VOID AimDownMW2Thread(MW2_UserCmd_t *UserCommand) {
	Vector SpinBotAngles;
	SpinBotAngles.x = 80;
	if (SpinBotAngles.x != 80)
		SpinBotAngles.x = 80;
	UserCommand->ViewAngles[0] = AngleToShort(SpinBotAngles.x);
}

VOID AimUpMW2Thread(MW2_UserCmd_t *UserCommand) {
	Vector SpinBotAngles;
	SpinBotAngles.x = -80;
	if (SpinBotAngles.x != -80)
		SpinBotAngles.x = -80;
	UserCommand->ViewAngles[0] = AngleToShort(SpinBotAngles.x);
}

VOID AntiAimMW2Thread(MW2_UserCmd_t *UserCommand)
{
	/*UserCommand->ViewAngles[0] = 180 * (rand() % 10);
	UserCommand->ViewAngles[1] = (vec_t)fmod(-360.0f, 360.0f) * (rand() % 10);
	UserCommand->ViewAngles[2] = UserCommand->ViewAngles[1] + 110 * (rand() % 10);
	if (UserCommand->ViewAngles[0] < -180.0f)UserCommand->ViewAngles[0] += 360.0f;
	if (UserCommand->ViewAngles[0] > 180.0f)UserCommand->ViewAngles[0] -= 360.0f;
	if (UserCommand->ViewAngles[1] < -180.0f)UserCommand->ViewAngles[1] += 360.0f;
	if (UserCommand->ViewAngles[1] > 180.0f)UserCommand->ViewAngles[1] -= 360.0f;
	if (UserCommand->ViewAngles[2] < -180.0f)UserCommand->ViewAngles[2] += 360.0f;
	if (UserCommand->ViewAngles[2] > 180.0f)UserCommand->ViewAngles[2] -= 360.0f;*/

	MW2_ReadGame();
	MW2_clientActive * ca = NULL;
	ca = (MW2_clientActive*)MW2_getCA();
	D3DXVECTOR3 AntiAimAngles;

	//AntiAimAngles.x = ca->ViewAngles.x + 180;
	AntiAimAngles.y = ca->ViewAngles.y + 180;

	//UserCommand->ViewAngles[0] = ANGLE2SHORT(AntiAimAngles.x);
	UserCommand->ViewAngles[1] = ANGLE2SHORT(AntiAimAngles.y);
}

VOID NoSpreadMW2Thread(MW2_UserCmd_t *UserCommand) {
	FLOAT minSpread, maxSpread;
	int weapon = MW2_Entities[MW2_cg->cNum]->WeaponID;
	BG_GetSpreadForWeaponMW2(&MW2_cgs->ps, weapon, &minSpread, &maxSpread);

	FLOAT right, up;
	int randSeed = MW2_cgs->ps.cmdTime;
	RandomBulletDirMW2(randSeed, &right, &up);

	FLOAT aimSpreadScale = MW2_cgs->aimSpreadScale / 255.0f;
	FLOAT aimSpreadAmount = minSpread + ((maxSpread - minSpread) * aimSpreadScale);

	FLOAT pitch = up * aimSpreadAmount;
	FLOAT yaw = right * aimSpreadAmount;

	UserCommand->ViewAngles[0] += AngleToShort(pitch);
	UserCommand->ViewAngles[1] += AngleToShort(yaw);
}

VOID MW2_CL_WritePackethk(int a1)
{
	if (MW2_Dvar_GetBOOL("cl_ingame"))
	{
		DWORD pd = *(DWORD*)MW2_clientActive_t;
		MW2_clientActive *cmdInput = (MW2_clientActive*)pd;
		MW2_UserCmd_t* cmd = cmdInput->GetCmd(cmdInput->CmdNum);
		MW2_clientActive *Ca = MW2_getCA();

		MW2_UserCmd_t* oldCmd = cmdInput->GetCmd(cmdInput->CmdNum - 1);
		Vector vMyAngles = Ca->ViewAngles;

		*oldCmd = *cmd;
		oldCmd->ServerTime--;

		if (*SpinBotMW2)
		{
			SpinBotMW2Thread(cmd);
		}

		if (*AimDownMW2)
		{
			AimDownMW2Thread(cmd);
		}

		if (*AimUpMW2)
		{
			AimUpMW2Thread(cmd);
		}

		if (*AntiAimMW2)
		{
			AntiAimMW2Thread(cmd);
		}

		if (*NoSpreadMW2)
		{
			NoSpreadMW2Thread(cmd);
		}
	}

	SaveStubPacket(a1);
}

VOID drawMaterialMW2(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT angle, const FLOAT *color) {
	MW2_DrawRotated(MW2_DrawRotated_R3, x, y, width, height, angle, color, MW2_White);
}

VOID drawRectangleMW2(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT lineWidth, const FLOAT *color) {
	drawMaterialMW2(x, y, width, lineWidth, 0.0f, color);
	drawMaterialMW2(x + width, y, lineWidth, height, 0.0f, color);
	drawMaterialMW2(x, y + height - lineWidth, width, lineWidth, 0.0f, color);
	drawMaterialMW2(x, y, lineWidth, height, 0.0f, color);
}

int ClosetsClientMW2()
{
	MW2_ReadGame();
	FLOAT objectDistance = 0.f;
	FLOAT flClosestDistance = (FLOAT)INT_MAX;
	int ClosestClient = -1;

	for (int i = 0; i < 12; i++)
	{
		if (i == MW2_cg->cNum)
			continue;

		if ((MW2_Entities[i]->Valid) && (MW2_Entities[i]->State & 1))
		{
			if (strcmp(MW2_cgs->gametype, "dm") == 0)
			{
				objectDistance = GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos);
				if (objectDistance < flClosestDistance)
				{
					flClosestDistance = objectDistance;
					ClosestClient = i;
				}
			}
			else
			{
				if (MW2_Clients[i]->mTeam != MW2_Clients[MW2_cg->cNum]->mTeam)
				{
					objectDistance = GetDistance(MW2_RefDef->EyePos, MW2_Entities[i]->mPos);
					if (objectDistance < flClosestDistance)
					{
						flClosestDistance = objectDistance;
						ClosestClient = i;
					}
				}
			}
		}
	}

	return ClosestClient;
}

VOID ClientDetailsMW2()
{
	if (MW2_Dvar_GetBOOL("cl_ingame"))
	{
		if (*TargetDetailsMW2)
		{
			drawMaterialMW2(105.0f, 245.0f, 250.0f, 60.0f, 0, L_black);
			drawRectangleMW2(105.0f, 245.0f, 250.0f, 60.0f, 2.0f, Purple);
			MW2_DrawText("Target Details:", 105.0f, 241.0f, 0.4f, white);

			int ClientValid = ClosetsClientMW2();
			char* targettext;
			if (DrawTargetDetailsMW2)
			{
				char targetbuffer[3096];
				sprintf(targetbuffer, "Name: %s\nDistance: %.fm", MW2_Clients[MW2_Clients[ClientValid]->mNum]->mName, GetDistance(MW2_RefDef->EyePos, MW2_Entities[ClientValid]->mPos));
				if (targetbuffer != "")
					targettext = targetbuffer;
				else
					targettext = "Target Details Error";
				DrawTargetDetailsMW2 = FALSE;
			}
			else
			{
				targettext = "Name: N/A\nDistance: N/A";
			}
			MW2_DrawText(targettext, 115.0f, 275.0f, 0.4f, white);
		}
	}
}

XINPUT_STATE XInputStateMW2;
short currentButtonMW2, previousButtonMW2;
#define buttonHitMW2(button) ((currentButtonMW2 & ~previousButtonMW2) & button)
VOID updateInputMW2() {
	XInputGetState(0, &XInputStateMW2);
	previousButtonMW2 = currentButtonMW2;
	currentButtonMW2 = ~XInputStateMW2.Gamepad.wButtons & 0xFBFF;

	if (buttonHitMW2(XINPUT_GAMEPAD_DPAD_LEFT)) {
		openMW2 = !openMW2;
	}

	if (openMW2) {
		if (currentMW2 == MW2_AIMBOT || currentMW2 == MW2_ESP) {
			if (buttonHitMW2(XINPUT_GAMEPAD_B)) {
				currentMW2 = MW2_MAIN;
			}
		}

		if (buttonHitMW2(XINPUT_GAMEPAD_DPAD_UP)) {
			if (currentMW2 == MW2_MAIN)
			{
				scrollMW2--;
				if (scrollMW2 == -1) {
					scrollMW2 = maxoptionsMW2 - 1;
				}
			}
			else
			{
				if (currentMW2 == MW2_AIMBOT)
				{
					AimscrollMW2--;
					if (AimscrollMW2 == -1) {
						AimscrollMW2 = AimmaxoptionsMW2 - 1;
					}
				}
				else
				{
					if (currentMW2 == MW2_ESP)
					{
						ESPscrollMW2--;
						if (ESPscrollMW2 == -1) {
							ESPscrollMW2 = ESPmaxoptionsMW2 - 1;
						}
					}
				}
			}
		}

		if (buttonHitMW2(XINPUT_GAMEPAD_DPAD_DOWN)) {
			if (currentMW2 == MW2_MAIN)
			{
				scrollMW2++;
				if (scrollMW2 == maxoptionsMW2) {
					scrollMW2 = 0;
				}
			}
			else
			{
				if (currentMW2 == MW2_AIMBOT)
				{
					AimscrollMW2++;
					if (AimscrollMW2 == AimmaxoptionsMW2) {
						AimscrollMW2 = 0;
					}
				}
				else
				{
					if (currentMW2 == MW2_ESP)
					{
						ESPscrollMW2++;
						if (ESPscrollMW2 == ESPmaxoptionsMW2) {
							ESPscrollMW2 = 0;
						}
					}
				}
			}
		}

		if (buttonHitMW2(XINPUT_GAMEPAD_X)) {
			if (currentMW2 == MW2_MAIN)
			{
				if (scrollMW2 == 0)
				{
					currentMW2 = MW2_AIMBOT;
				}
				else
				{
					if (scrollMW2 == 1)
					{
						currentMW2 = MW2_ESP;
					}
					else
					{
						if (scrollMW2 == 5)
						{
							switch (CrossHairMW2) {
							case 0:
								CrossHairMW2 = XCrosshairMW2;
								break;
							case 1:
								CrossHairMW2 = CrosshairOffMW2;
								break;
							case 2:
								CrossHairMW2 = PlusCrosshairMW2;
								break;
							default:
								CrossHairMW2 = CrosshairOffMW2;
								break;
							}
						}
						else
						{
							boptionsMW2[scrollMW2] = !boptionsMW2[scrollMW2];
						}
					}
				}
			}
			else
			{
				if (currentMW2 == MW2_AIMBOT)
				{
					if (AimscrollMW2 == 0) {
						switch (CurrentMW2Aimbot) {
						case 0:
							CurrentMW2Aimbot = MW2_ONSCREEN;
							break;
						case 1:
							CurrentMW2Aimbot = MW2_CROSSHAIR;
							break;
						case 2:
							CurrentMW2Aimbot = MW2_Silent;
							break;
						case 3:
							CurrentMW2Aimbot = MW2_AIMBOT_OFF;
							break;
						case 4:
							CurrentMW2Aimbot = MW2_NEAREST;
							break;
						default:
							CurrentMW2Aimbot = MW2_AIMBOT_OFF;
							break;
						}
					}
					else {
						AimboptionsMW2[AimscrollMW2] = !AimboptionsMW2[AimscrollMW2];
					}
				}
				else
				{
					if (currentMW2 == MW2_ESP)
					{
						if (ESPscrollMW2 == 0) {
							switch (CurrentMW2ESP) {
							case 0:
								CurrentMW2ESP = MW2_THREE_D;
								break;
							case 1:
								CurrentMW2ESP = MW2_PYRAMID;
								break;
							case 2:
								CurrentMW2ESP = MW2_FILLED;
								break;
							case 3:
								CurrentMW2ESP = MW2_ESP_OFF;
								break;
							case 4:
								CurrentMW2ESP = MW2_TWO_D;
								break;
							default:
								CurrentMW2ESP = MW2_ESP_OFF;
								break;
							}
						}
						else if (ESPscrollMW2 == 5) {
							switch (ESPMW2) {
							case 0:
								ESPMW2 = ESPFriendlyMW2;
								break;
							case 1:
								ESPMW2 = ESPBothMW2;
								break;
							case 2:
								ESPMW2 = ESPOffMW2;
								break;
							case 3:
								ESPMW2 = ESPEnemyMW2;
								break;
							default:
								ESPMW2 = ESPOffMW2;
								break;
							}
						}
						else if (ESPscrollMW2 == 6) {
							switch (SnapLinePosMW2) {
							case 0:
								SnapLinePosMW2 = SnapLineBottomMW2;
								break;
							case 1:
								SnapLinePosMW2 = SnapLineTopMW2;
								break;
							case 2:
								SnapLinePosMW2 = SnapLinesOffMW2;
								break;
							case 3:
								SnapLinePosMW2 = SnapLineMiddleMW2;
								break;
							default:
								SnapLinePosMW2 = SnapLinesOffMW2;
								break;
							}
						}
						else if (ESPscrollMW2 == 7) {
							switch (SnapLineTeamMW2) {
							case 0:
								SnapLineTeamMW2 = SnapLineFriendlyMW2;
								break;
							case 1:
								SnapLineTeamMW2 = SnapLineTeamBothMW2;
								break;
							case 2:
								SnapLineTeamMW2 = SnapLineTeamOffMW2;
								break;
							case 3:
								SnapLineTeamMW2 = SnapLineEnemyMW2;
								break;
							default:
								SnapLineTeamMW2 = SnapLineTeamOffMW2;
								break;
							}
						}
						else {
							ESPboptionsMW2[ESPscrollMW2] = !ESPboptionsMW2[ESPscrollMW2];
						}
					}
				}
			}
		}
	}
}

VOID updateOptionsMW2()
{
	if (*norecoilMW2)
	{
		*(int*)MW2_NoRecoil = 0x60000000;
	}
	else
	{
		*(int*)MW2_NoRecoil = 0x4BFAD5FD;
	}
	if (*advanceduavMW2)
	{
		*(int*)MW2_Radar = 0x3BA00001;
	}
	else
	{
		*(int*)MW2_Radar = 0x561D063E;
	}
	if (*noswayMW2)
	{
		*(int*)MW2_WeaponSway = 0x60000000;
		*(int*)MW2_ZommingSway = 0x60000000;
	}
	else
	{
		*(int*)MW2_WeaponSway = 0x4BFFEAA9;
		*(int*)MW2_ZommingSway = 0x4BFFFBC5;
	}
}

VOID MW2_Menu_Hook()
{
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;
	FLOAT middleMW2 = 950.0f;
	FLOAT centerMW2 = 100.0f;
	FLOAT bottomMW2 = 700.0f;

	updateInputMW2();
	updateOptionsMW2();

	if (MW2_Dvar_GetBOOL("cl_ingame"))
	{
		if (!FirstTimeInGameMW2)
		{
			openMW2 = FALSE;
			currentMW2 = 0;
			CurrentMW2ESP = 4;
			CurrentMW2Aimbot = 4;
			scrollMW2 = 0;
			AimscrollMW2 = 0;
			ESPscrollMW2 = 0;
			FirstTimeInGameMW2 = TRUE;
		}

		if (openMW2)
		{
			if (currentMW2 == MW2_MAIN)
			{
				drawMaterialMW2(middleMW2 - 1, centerMW2, 250.0f, 30.0f + (maxoptionsMW2 * 30.0f), 0, L_black2);
				drawRectangleMW2(middleMW2 - 1, centerMW2, 250.0f, 30.0f + (maxoptionsMW2 * 30.0f), 2.0f, Purple);
				MW2_DrawText("OutHere Menu", middleMW2 + 20, centerMW2 - 1, 0.6f, Purple);
				MW2_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomMW2, 0.4f, white);

				for (int i = 0; i < maxoptionsMW2; i++) {
					if (i == scrollMW2) {
						MW2_DrawText(optionsMW2[i], middleMW2 + 20, centerMW2 + 40.0f + (30.0f * i), 0.4f, Purple);
					}
					else {
						MW2_DrawText(optionsMW2[i], middleMW2 + 20, centerMW2 + 40.0f + (30.0f * i), 0.4f, white);
					}
					if (i == 0)
					{
						// fix
					}
					else if (i == 1)
					{
						// fix
					}
					else if (i == 5)
					{
						MW2_DrawText(CrossHairTypeMW2[CrossHairMW2], middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, CrossHairMW2 == CrosshairOffMW2 ? L_red2 : BlueStormz);
					}
					else if (boptionsMW2[i])
					{
						MW2_DrawText("ON", middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, L_green2);
					}
					else
					{
						MW2_DrawText("OFF", middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, Purple);
					}
				}
			}
			else
			{
				if (currentMW2 == MW2_AIMBOT)
				{
					drawMaterialMW2(middleMW2 - 1, centerMW2, 250.0f, 30.0f + (AimmaxoptionsMW2 * 30.0f), 0, L_black2);
					drawRectangleMW2(middleMW2 - 1, centerMW2, 250.0f, 30.0f + (AimmaxoptionsMW2 * 30.0f), 2.0f, Purple);
					MW2_DrawText("Aimbot Options", middleMW2 + 20, centerMW2 - 1, 0.6f, Purple);
					MW2_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomMW2, 0.4f, white);

					for (int i = 0; i < AimmaxoptionsMW2; i++) {
						if (i == AimscrollMW2) {
							MW2_DrawText(AimoptionsMW2[i], middleMW2 + 20, centerMW2 + 40.0f + (30.0f * i), 0.4f, Purple);
						}
						else {
							MW2_DrawText(AimoptionsMW2[i], middleMW2 + 20, centerMW2 + 40.0f + (30.0f * i), 0.4f, white);
						}
						if (i == 0) {
							MW2_DrawText(AimbotTypeMW2[CurrentMW2Aimbot], middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, CurrentMW2Aimbot == MW2_AIMBOT_OFF ? L_red2 : BlueStormz);
						}
						else {
							if (AimboptionsMW2[i])
							{
								MW2_DrawText("ON", middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, L_green2);
							}
							else
							{
								MW2_DrawText("OFF", middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, Purple);
							}
						}
					}
				}
				else
				{
					if (currentMW2 == MW2_ESP)
					{
						drawMaterialMW2(middleMW2 - 1, centerMW2, 250.0f, 30.0f + (ESPmaxoptionsMW2 * 30.0f), 0, L_black2);
						drawRectangleMW2(middleMW2 - 1, centerMW2, 250.0f, 30.0f + (ESPmaxoptionsMW2 * 30.0f), 2.0f, Purple);
						MW2_DrawText("ESP Options", middleMW2 + 20, centerMW2 - 1, 0.6f, Purple);
						MW2_DrawText("Press D-PAD Left To ^1Close^7 OutHere Menu", 30.0f, bottomMW2, 0.4f, white);

						for (int i = 0; i < ESPmaxoptionsMW2; i++) {
							if (i == ESPscrollMW2) {
								MW2_DrawText(ESPoptionsMW2[i], middleMW2 + 20, centerMW2 + 40.0f + (30.0f * i), 0.4f, Purple);
							}
							else {
								MW2_DrawText(ESPoptionsMW2[i], middleMW2 + 20, centerMW2 + 40.0f + (30.0f * i), 0.4f, white);
							}
							if (i == 0) {
								MW2_DrawText(espTypeMW2[CurrentMW2ESP], middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, CurrentMW2ESP == MW2_ESP_OFF ? L_red2 : BlueStormz);
							}
							else if (i == 5) {
								MW2_DrawText(ESPTeamTypeMW2[ESPMW2], middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, ESPMW2 == ESPOffMW2 ? L_red2 : BlueStormz);
							}
							else if (i == 6) {
								MW2_DrawText(SnapLineTypeMW2[SnapLinePosMW2], middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, SnapLinePosMW2 == SnapLinesOffMW2 ? L_red2 : BlueStormz);
							}
							else if (i == 7) {
								MW2_DrawText(SnapLineTeamTypeMW2[SnapLineTeamMW2], middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, SnapLineTeamMW2 == SnapLineTeamOffMW2 ? L_red2 : BlueStormz);
							}
							else {
								if (ESPboptionsMW2[i])
								{
									MW2_DrawText("ON", middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, L_green2);
								}
								else
								{
									MW2_DrawText("OFF", middleMW2 + 190, centerMW2 + 40.0f + (30.0f * i), 0.4f, Purple);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			MW2_DrawText("Press D-PAD Left To ^2Open^7 OutHere Menu", 30.0f, bottomMW2, 0.4f, white);//Bottom shit
		}
	}
	else
	{
		openMW2 = FALSE;
	}
}

HRESULT MW2_CL_DrawTextHook(const char *text, int maxChars, DWORD font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, const FLOAT *color, int style)
{
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;

	FLOAT rotation = 0;
	MW2_R_AddCmdDrawText(text, maxChars, font, x, y, xScale, yScale, rotation, color, style);

	//MW2_dMenu.BuildMenu("\n\n^2Masked Menu^7",(CenterX - 160),50,280,150,black,blackM,Purple);

	if (MW2_Key_Down(0, 0x12))
	{
		//printf("aiming");
	}
	else
	{
		LastFrameAimKey = FALSE;
	}

	MW2_Menu_Hook();
	MW2_Draw();
	MW2_Aimbot();
	ClientDetailsMW2();

	return S_OK;
}

VOID MW2_Start()
{
	PatchInJump((DWORD *)0x82146148, (DWORD)MW2_CL_DrawTextHook, false);//82146178 Tu8
	HookFunctionStart((DWORD *)0x821401E0, (PDWORD)SaveStubPacket, (DWORD)MW2_CL_WritePackethk);//82140220 tu8
}

/////////////////////////////////////// MW2 Menu END /////////////////////////////////////