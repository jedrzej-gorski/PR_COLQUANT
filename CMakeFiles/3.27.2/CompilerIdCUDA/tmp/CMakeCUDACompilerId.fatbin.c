#ifndef __SKIP_INTERNAL_FATBINARY_HEADERS
#include "fatbinary_section.h"
#endif
#define __CUDAFATBINSECTION  ".nvFatBinSegment"
#define __CUDAFATBINDATASECTION  ".nv_fatbin"
asm(
".section .nv_fatbin, \"a\"\n"
".align 8\n"
"fatbinData:\n"
".quad 0x00100001ba55ed50,0x00000000000005a8,0x0000005801010002,0x0000000000000330\n"
".quad 0x000000000000032c,0x0000003400010007,0x0000001600000040,0x0000000000002013\n"
".quad 0x0000000000000000,0x0000000000000900,0x445543656b614d43,0x656c69706d6f4341\n"
".quad 0x000075632e644972,0x010102464c457fa2,0x0002660001000733,0x90210001007a00be\n"
".quad 0x0704d00031000708,0x00340534000ef500,0x0040000200380040,0x68732e000001000f\n"
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
".quad 0xf100010000d96611,0x666e65676c01081b,0x3620474445203a65,0x614d43000400342e\n"
".quad 0x6f4341445543656b,0x644972656c69706d,0x2f01f1002f75632e,0x7562752f656d6f68\n"
".quad 0x2b2f72702f75746e,0x2f73656c6946d600,0x342f322e37322e33,0x0001001300420000\n"
".quad 0x130008ffffffff40,0x650008fd130008fe,0x00010073fffffffc,0x010036050025116f\n"
".quad 0x010e020001112c00,0x143d000100402200,0x1f00010800300001,0x4001542f0400400b\n"
".quad 0x010a015d13151300,0x06d8110015681300,0x0183091000240200,0xa800182600010012\n"
".quad 0x401e000100012e00,0x4d1f0200800f0095,0x0040001a2f0c0040,0x5a1f040040591f0b\n"
".quad 0x0400406d1f140040,0x00400f03e8037426,0x23130400407f1f09,0x1f0c0040001f06c0\n"
".quad 0x0040af1f2c004097,0x1f0b004000102f0c,0x00403313040040bd,0xda1f0b0040006a2f\n"
".quad 0x0030049d2f040040,0x015b000500011f03,0xa013000100700000,0x0400010020220040\n"
".quad 0x08002700170305b2,0x1f0000400b1f0528,0x0008081b040100c0,0x0c087804005c0613\n"
".quad 0x08170008701b0001,0x00501c00380f00c8,0x0000000000000000,0x0000009001010001\n"
".quad 0x0000000000000190,0x000000400000018e,0x0000003400080002,0x0000001600000078\n"
".quad 0x0000000000002013,0x0000000000000000,0x0000000000000296,0x0000002d00000048\n"
".quad 0x6f642d2d20672d20,0x656772656d2d746e,0x6c6263697361622d,0x722d2d20736b636f\n"
".quad 0x74612d6e72757465,0x00000020646e652d,0x445543656b614d43,0x656c69706d6f4341\n"
".quad 0x000075632e644972,0x762e23f000010a13,0x38206e6f69737265,0x677261742e0a322e\n"
".quad 0x32355f6d73207465,0x0a6775626564202c,0x737365726464612e,0x343620657a69735f\n"
".quad 0x6974636573910035,0x00f200232e096e6f,0x0a7665726262615f,0x06312038622e0a7b\n"
".quad 0x0630110007371100,0x063811000d331200,0x3511000739312100,0x3611001933180006\n"
".quad 0x3332180006020007,0x807d1d0006301900,0x007e6f666e694100,0x2832303120323361\n"
".quad 0x32333a0034321500,0xac0200730700a820,0x30312c383003f100,0x31312c3130312c33\n"
".quad 0xe0000c3230312c30,0x39362c32332c3835,0x000c31372c38362c,0x352c36342c343576\n"
".quad 0x000c34203837005c,0x392c37372c3736c1,0x5000423730312c37,0x20003f35382c3736\n"
".quad 0x313131a0000c3536,0x6131312c3930312c,0x750000792c352200,0x100080372c343000\n"
".quad 0x1f00213910005f30,0x6e696c410000bf37,0x16005e3410001765,0x132c313030004f34\n"
".quad 0x0738392c37315100,0x0c36312100c60100,0x3120007037342200,0x372b00a30d000b34\n"
".quad 0x2100273510008b30,0x352c304300e93135,0xc20f003c02000935,0x00c6090143030000\n"
".quad 0x350601b50f00f807, 0x20388001b863616d, 0x0000000a7d0a0a30\n"
".text\n");
#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned long long fatbinData[183];
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
