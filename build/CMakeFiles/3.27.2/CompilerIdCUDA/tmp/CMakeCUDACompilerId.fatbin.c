#ifndef __SKIP_INTERNAL_FATBINARY_HEADERS
#include "fatbinary_section.h"
#endif
#define __CUDAFATBINSECTION  ".nvFatBinSegment"
#define __CUDAFATBINDATASECTION  ".nv_fatbin"
asm(
".section .nv_fatbin, \"a\"\n"
".align 8\n"
"fatbinData:\n"
".quad 0x00100001ba55ed50,0x00000000000005b0,0x0000005801010002,0x0000000000000330\n"
".quad 0x000000000000032f,0x0000003400010007,0x0000001600000040,0x0000000000002013\n"
".quad 0x0000000000000000,0x0000000000000908,0x445543656b614d43,0x656c69706d6f4341\n"
".quad 0x000075632e644972,0x010102464c457fa2,0x0002660001000733,0x98210001007a00be\n"
".quad 0x0704d80031000708,0x00340534000ef500,0x0040000200380040,0x68732e000001000f\n"
".quad 0x2e00626174727473,0xf100086d79270008,0x0078646e68735f05,0x2e7466752e766e2e\n"
".quad 0xf4000e7972746e65,0x642e006f666e6902,0x6172665f67756265,0x6e696c40000d656d\n"
".quad 0x57000f5f16002265,0x770014737361735f,0x127478745f787470,0x65725f4c004e0000\n"
".quad 0x7435001805002a67,0x6262618600627079,0x6900232e00766572,0x0011616c65722e00\n"
".quad 0x0f63616d32000c02,0x6c61632e766ec100,0x000e68706172676c,0x00566f746f727052\n"
".quad 0x612e7f003f766e20,0xff01146e6f697463,0x004d8c0400010f04,0x0001000500030000\n"
".quad 0x0100062c00185911,0x00072c00186d1100,0x0a2c0018af110001,0x2c0018bd11000100\n"
".quad 0x0018da110001000b,0x18e9110001000c2c,0x05200001000d2c00,0x0001000e2c001801\n"
".quad 0x000010e003341611,0x01000a0efb010100,0x001a0f000d010101,0x762e0df100010409\n"
".quad 0x38206e6f69737265,0x677261742e00322e,0x32355f6d73207465,0x2e0002f00169202c\n"
".quad 0x5f73736572646461,0x35343620657a6973,0x091a015265732100,0x1d0001007b2c01b7\n"
".quad 0x290f01b401002b7d,0x1e7b1101e0041100,0x130825001101c000,0x040e1b0610080305\n"
".quad 0xf100010000d96c11,0x666e65676c01081b,0x3620474445203a65,0x614d43000400342e\n"
".quad 0x6f4341445543656b,0x644972656c69706d,0x2f07f1002f75632e,0x7562752f656d6f68\n"
".quad 0x622f72702f75746e,0xd600312f646c6975,0x2e332f73656c6946,0x00003a2f322e3732\n"
".quad 0xff0050003b010048,0x08fe130008ffffff,0x08fc130008fd1300,0x116f000100732500\n"
".quad 0x2c00010036050025,0x2200010e02080111,0x0001143d00010040,0x400b1f0001080030\n"
".quad 0x13004001542f0400,0x1300010a01651315,0x020006d811001568,0x0012018b09100024\n"
".quad 0x2e00a80018260001,0x0095401e00010001,0x00404d1f0200800f,0x1f0b0040001a2f0c\n"
".quad 0x00405a1f04004059,0x74260400406d1f14,0x1f0900400f03f003,0x06c823130400407f\n"
".quad 0x40971f0c0040001f,0x2f0c0040af1f2c00,0x40bd1f0b00400010,0x702f004033130400\n"
".quad 0x0040da1f0b004000,0x1f03003004a32f04,0x0067011e00050801,0x0020220040a40023\n"
".quad 0x00170305ba040001,0x400b1f0530080027,0x1b040100c81f0000,0x04005c0613000808\n"
".quad 0x08701b00010c0880,0x00380f00c8081700,0x000000000000501c,0x0000009001010001\n"
".quad 0x0000000000000198,0x0000004000000192,0x0000003400080002,0x0000001600000078\n"
".quad 0x0000000000002013,0x0000000000000000,0x00000000000002ac,0x0000002d00000048\n"
".quad 0x6f642d2d20672d20,0x656772656d2d746e,0x6c6263697361622d,0x722d2d20736b636f\n"
".quad 0x74612d6e72757465,0x00000020646e652d,0x445543656b614d43,0x656c69706d6f4341\n"
".quad 0x000075632e644972,0x762e23f000010a13,0x38206e6f69737265,0x677261742e0a322e\n"
".quad 0x32355f6d73207465,0x0a6775626564202c,0x737365726464612e,0x343620657a69735f\n"
".quad 0x6974636573910035,0x00f200232e096e6f,0x0a7665726262615f,0x06312038622e0a7b\n"
".quad 0x0630110007371100,0x063811000d331200,0x3511000739312100,0x3611001933180006\n"
".quad 0x3332180006020007,0x807d1d0006301900,0x007e6f666e694100,0x0034303120323352\n"
".quad 0x2032333a00343215,0x00ac0200730700a8,0x3330312c383003f1,0x3031312c3130312c\n"
".quad 0x35e0000c3230312c,0x2c39362c32332c38,0x76000c31372c3836,0x5c352c36342c3435\n"
".quad 0xc1000c3420383700,0x37392c37372c3736,0x365000423730312c,0x3620003f35382c37\n"
".quad 0x2c313131a0000c35,0x006131312c393031,0x00750000792c3522,0x30100080372c3430\n"
".quad 0x371f00213910005f,0x656e696c410000bf,0x3416005e34100017,0x00132c313030004f\n"
".quad 0x000738392c373151,0x000c36312100c601,0x3431460070373422,0x10007e050021342c\n"
".quad 0x372b00b90d001630,0x210027351000a130,0x352c304300ff3135,0x00f9373424000935\n"
".quad 0x0f010e071000dc0f,0xce63616d350601cb,0x7d0a0a3020388001,0x000000000000000a\n"
".text\n");
#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned long long fatbinData[184];
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
static const __fatBinC_Wrapper_t __fatDeviceText __attribute__ ((aligned (8))) __attribute__ ((section (__CUDAFATBINSECTION)))= 
	{ 0x466243b1, 1, fatbinData, 0 };
#ifdef __cplusplus
}
#endif
