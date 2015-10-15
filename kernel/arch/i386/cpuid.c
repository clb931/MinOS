#include "kernel/common.h"
#include "kernel/cpuid.h"
#include "kernel/amd_cpu_tables.h"
#include "kernel/intel_cpu_tables.h"
#include <stdio.h>


#define cpuid(in, a, b, c, d) __asm__("cpuid" : "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));


typedef union {
	struct {
		DWORD eax, ebx, ecx, edx;
	};

	struct {
		char str16[16];
	};

	struct {
		DWORD unused;
		char str12[12];
	};
} vendor_string_t;

static int cpuid_intel()
{
	DWORD eax, unused;
	cpuid(0x80000000, eax, unused, unused, unused);
	if (eax > 0x80000000) {
		vendor_string_t str1, str2, str3;
		if (eax >= 0x80000002)
			cpuid(0x80000002, str1.eax, str1.ebx, str1.ecx, str1.edx);
		if (eax >= 0x80000003)
			cpuid(0x80000003, str2.eax, str2.ebx, str2.ecx, str2.edx);
		if (eax >= 0x80000004)
			cpuid(0x80000004, str3.eax, str3.ebx, str3.ecx, str3.edx);
		printf("\n    Processor: %.16s%.16s%.16s", str1.str16, str2.str16, str3.str16);

	} else {
		DWORD ebx;
		cpuid(1, unused, ebx, unused, unused);
		BYTE brandid = ebx & 0xFF;
		if (brandid > 0x00 && brandid < 0x10) {
			if (brandid == 0x03 && eax == 0x000006B1)
				brandid = 0x10;
			if (brandid == 0x08 && eax >= 0x00000F13)
				brandid = 0x11;
			if (brandid == 0x08 && eax < 0x00000F13)
				brandid = 0x12;
			if (brandid == 0x08 && eax < 0x00000F13)
				brandid = 0x13;
			printf("\n    Processor: %s", intel_brandid[brandid]);
		} else {
			return -1;
		}
	}

	return 0;
}

static int cpuid_amd()
{
	DWORD eax, unused;
	cpuid(0x80000000, eax, unused, unused, unused);
	if (eax > 0x80000000) {
		vendor_string_t str1, str2, str3;
		if (eax >= 0x80000002)
			cpuid(0x80000002, str1.eax, str1.ebx, str1.ecx, str1.edx);
		if (eax >= 0x80000003)
			cpuid(0x80000003, str2.eax, str2.ebx, str2.ecx, str2.edx);
		if (eax >= 0x80000004)
			cpuid(0x80000004, str3.eax, str3.ebx, str3.ecx, str3.edx);
		printf("\n    Processor: %.16s%.16s%.16s", str1.str16, str2.str16, str3.str16);

	} else {
		return -2;
	}

	return 0;
}

int cpuid_init()
{
	vendor_string_t str = { 0 };
	cpuid(0, str.unused, str.ebx, str.edx, str.ecx);
	printf("\n    Vendor:    %.12s", str.str12);
	if (str.ebx == 0x756E6547 && str.ecx == 0x49656E69 && str.edx == 0x6C65746E)
		return cpuid_intel();
	else if (str.ebx == 0x68747541 && str.ecx == 0x69746E65 && str.edx == 0x444D4163)
		return cpuid_amd();
	else
		return -3;
}
