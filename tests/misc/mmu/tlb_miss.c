// 
// Copyright 2015 Jeff Bush
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// 

#include <stdio.h>
#include <string.h>

extern void tlb_miss_handler();

// The TLB handle makes memory wrap every 4 MB
// The MMU maps these to 0x100000.
char *tmp1 = (char*) 0x500000;
char *tmp2 = (char*) 0x900000;

int main(int argc, const char *argv[])
{
	// Set up miss handler
	__builtin_nyuzi_write_control_reg(7, tlb_miss_handler);
	__builtin_nyuzi_write_control_reg(4, (1 << 2));	// Turn on MMU in flags

	// This dflush should cause a TLB miss and fill.
	asm("dflush %0" : : "s" (tmp1));

	// Test that stores are properly translated. Test harness will read
	// physical memory. This should be written to 1MB.
	strcpy(tmp1, "Test String");

	// Test that loads are properly mapped. This should alias to tmp1
	printf("read %p \"%s\"\n", tmp2, tmp2);

	// Make sure to flush first address so it will be in memory dump.
	asm("dflush %0" : : "s" (tmp1));

	return 0;
}