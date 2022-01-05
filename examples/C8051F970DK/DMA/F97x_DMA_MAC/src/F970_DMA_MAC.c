//-----------------------------------------------------------------------------
// F97x_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Do Multiply and Accumulate by module MAC0
//
// This program demos how to use module MAC0. Two tasks are done by MAC0
//     1. Initialize xram
//     2. Multiply and accumulate corresponding members of two array
//  The LED5 will be flashing if the result is expeced.
//  The LED5 will be flashing if the result is not expeced.
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (SY)
//    - 1 Dec 2015

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"
#include <string.h>  // For memcmp()

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define SYSCLK       20000000/8             // SYSCLK frequency in Hz

// How many bytes will be set by DMA each time in Mem_Set_Xram_With_DMA0_MAC0()
// Possible value: 1/2. It takes less time to set the memory if the value is 2.
// If the value is 2, then the input length should be even.
#define MEM_SET_DMA_ACCESS_BYTES      1
#define MEM_SET_VALUE                 10

// Definition for multiply and accumulate
#define MEM_SET_ARRAY_LEN             1001
#define MAC_READ_RESULT_WITH_DMA      1     // Read the accumulation result by DMA
#define MAC_ARRARY_LEN                6

#define MAC_ARRARY_2_IN_TWO_BYTES     0     // 1 means a[i]*b[i] saved in 2 bytes, otherwise in 4 bytes

SI_SBIT(LED5, P0, 5);                          // 0 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void Mem_Set_Xram_With_DMA0_MAC0(uint16_t addr, uint16_t len, uint8_t value);
void MUL_ACC_With_DMA0_MAC0(uint16_t xdata_array_a, uint16_t xdata_array_b, uint8_t len);
void MUL_ARRAY_With_DMA0_MAC0(uint16_t xdata_array_a, uint16_t xdata_array_b, uint8_t len, uint16_t xdata_array_c);


xdata uint16_t array_a[MAC_ARRARY_LEN] = {    3,    120,   600,   314,  198,   338};
xdata uint16_t array_b[MAC_ARRARY_LEN] = {    9,     30,   800,   120,  101,   124};
#if MAC_ARRARY_2_IN_TWO_BYTES
xdata uint16_t array_c[MAC_ARRARY_LEN] = {0};
xdata uint16_t array_exp_c[MAC_ARRARY_LEN] = {   27,   3600, 21248,37680, 19998, 41912};
#else
xdata uint32_t array_c[MAC_ARRARY_LEN] = {0};
xdata uint32_t array_exp_c[MAC_ARRARY_LEN] = {   27,   3600,480000,37680, 19998, 41912};
#endif
xdata uint16_t MAC_init_value          = 100;
xdata uint32_t MAC_result_value        = 0x12345678;
// MEM_SET_ARRAY_LEN bytes are needed. Initialize the next byte to 0 for checking.
xdata uint8_t  Memset_array[MEM_SET_ARRAY_LEN + 1]   = {0};

SI_INTERRUPT_PROTO(Timer2_ISR, TIMER2_IRQn);

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
    uint32_t result_hw;
    uint32_t result_sw;
    uint8_t  i;

    enter_DefaultMode_from_RESET();

    // ---------------- Memory set ----------------
    Mem_Set_Xram_With_DMA0_MAC0((uint16_t)Memset_array, MEM_SET_ARRAY_LEN, 10);

    // ---------------- Calculate a[i]*b[i] ----------------
    MUL_ARRAY_With_DMA0_MAC0((uint16_t)array_a, (uint16_t)array_b, MAC_ARRARY_LEN, (uint16_t)array_c);

    // ---------------- Calculate sum of a[i]*b[i] ----------------
    MUL_ACC_With_DMA0_MAC0((uint16_t)array_a, (uint16_t)array_b, MAC_ARRARY_LEN);
    SFRPAGE = MAC0_PAGE;
    // Get the result. Also check MAC0OVF if the result is more than 32 bits
#if MAC_READ_RESULT_WITH_DMA
    result_hw = MAC_result_value;
#else
    result_hw = ((uint32_t)MAC0ACC3 << 24) | ((uint32_t)MAC0ACC2 << 16) | ((uint32_t)MAC0ACC1 << 8) |((uint32_t)MAC0ACC0 << 0);
#endif
    // Result calculated by software
    result_sw = MAC_init_value;
    for (i = 0; i < MAC_ARRARY_LEN; i++)
    {
	    result_sw += (uint32_t)array_a[i]*array_b[i];
    }

    // Check the results
    // Enable interrupt to flash LED if the result is expected.
    if ((result_sw == result_hw) &&
	    (Memset_array[0] == MEM_SET_VALUE) &&
	    (Memset_array[MEM_SET_ARRAY_LEN/2]   == MEM_SET_VALUE) &&
	    (Memset_array[MEM_SET_ARRAY_LEN - 1] == MEM_SET_VALUE) &&
	    (Memset_array[MEM_SET_ARRAY_LEN]     != MEM_SET_VALUE) &&
	    (memcmp(array_c, array_exp_c, MAC_ARRARY_LEN) == 0))
    {
        IE_EA = 1;                        // Enable interrupt to flash LED
    }
    else
    {
	    LED5 = 0;                          // LED on to indicate error
    }


    while (1);                            // Spin forever
}

//-----------------------------------------------------------------------------
// MUL_ACC_With_DMA0_MAC0
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    xdata_array_a     address of uint16_t array a in xram
//                xdata_array_b     address of uint16_t array b in xram
//                len               length of array
//
// Calculate the sum of product of corresponding member of two arrays
//    sum = a[0]*b[0] + a[1]*b[1] + ... + a[n-1]*b[n-1]
// The result is in MAC0's registor.
//
// At least 3 channels are needed for this scenario:
//     1 for MAC0A
//     1 for MAC0B
//     1 for XRAM->ACC (only once at the start)
// 1 more channel will be needed if ACC is read to xram by DMA.
//-----------------------------------------------------------------------------
void MUL_ACC_With_DMA0_MAC0(uint16_t xdata_array_a, uint16_t xdata_array_b, uint8_t len)
{
	uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

	SFRPAGE = DMA0_PAGE;

	// DMA0 channel0 for XRAM -> MAC0's A register
	DMA0SEL  = DMA0SEL_SELECT__CH0;
	// Disable channel 0
	DMA0EN   = (DMA0EN & (~DMA0EN_CH0EN__BMASK)) | DMA0EN_CH0EN__DISABLED;
    // Clear interrupt of channel 0
	DMA0INT &= ~DMA0INT_CH0I__BMASK;
    // XRAM->MAC's A
	// Endian bit's definition is the same as LSB of UU16 of compiler
	DMA0NCF  = DMA0NCF_PERIPH__XRAM_TO_MAC_A | (LSB << DMA0NCF_ENDIAN__SHIFT) | DMA0NCF_IEN__ENABLED;
	DMA0NBA  = xdata_array_a;
	DMA0NAO  = 0;
	DMA0NSZ  = len<<1;  // Fetch 2 bytes to MAC0's A register every time


	// DMA0 channel1 for XRAM -> MAC0's B register
	DMA0SEL  = DMA0SEL_SELECT__CH1;
	// Disable channel 1
	DMA0EN   = (DMA0EN & (~DMA0EN_CH1EN__BMASK)) | DMA0EN_CH1EN__DISABLED;
    // Clear interrupt of channel 1
	DMA0INT &= ~DMA0INT_CH1I__BMASK;
    // XRAM->MAC's B
	// Endian bit's definition is the same as LSB of UU16 of compiler
	DMA0NCF  = DMA0NCF_PERIPH__XRAM_TO_MAC_B | (LSB << DMA0NCF_ENDIAN__SHIFT) | DMA0NCF_IEN__ENABLED;
	DMA0NBA  = xdata_array_b;
	DMA0NAO  = 0;
	DMA0NSZ  = len<<1;  // Fetch 2 bytes to MAC0's B register every time

	// DMA0 channel2 for XRAM -> MAC0's ACC
	// This channel only copy data once at the beginning of the accumulation
	DMA0SEL  = DMA0SEL_SELECT__CH2;
	// Disable channel 2
	DMA0EN   = (DMA0EN & (~DMA0EN_CH2EN__BMASK)) | DMA0EN_CH2EN__DISABLED;
    // Clear interrupt of channel 2
	DMA0INT &= ~DMA0INT_CH2I__BMASK;
    // XRAM -> ACC
	// Endian bit's definition is the same as LSB of UU16 of compiler
	DMA0NCF  =  DMA0NCF_PERIPH__XRAM_TO_MAC_ACC | (LSB << DMA0NCF_ENDIAN__SHIFT) | DMA0NCF_IEN__ENABLED;
	DMA0NBA  = (uint16_t)&MAC_init_value;  // The initial value for ACC. Set the array to 0 if initial value is not necessary
	DMA0NAO  = 0;
	DMA0NSZ  = len<<1;  // Fetch a number of the array every time
	//-------------------------------

#if MAC_READ_RESULT_WITH_DMA
	// DMA0 channel3 setting
	// Choose channel 3 for MAC0's ACC -> XRAM
	DMA0SEL  = DMA0SEL_SELECT__CH3;
	// Disable channel 3
	DMA0EN   = (DMA0EN & (~DMA0EN_CH3EN__BMASK)) | DMA0EN_CH3EN__DISABLED;
    // Clear interrupt of channel 3
	DMA0INT &= ~DMA0INT_CH3I__BMASK;
    // ACC -> XRAM
	// Endian bit's definition is the same as LSB of UU16 of compiler
	DMA0NCF  = DMA0NCF_PERIPH__MAC_ACC_TO_XRAM | (LSB << DMA0NCF_ENDIAN__SHIFT) | DMA0NCF_IEN__ENABLED;
	DMA0NBA  = (uint16_t)&MAC_result_value;
	DMA0NAO  = 0;
    // ACC will be copied to the result after len times MUL and ACC are done if ACC->XRAM DMA channel is enabled
	MAC0ITER = len;
	DMA0NSZ  = 4;  // number of bytes should be not less than the unit of ACC->XRAM and can accommodate the result
#else
	MAC0ITER = len;
#endif

#if MAC_READ_RESULT_WITH_DMA
	DMA0EN   = DMA0EN_CH0EN__ENABLED | DMA0EN_CH1EN__ENABLED | DMA0EN_CH2EN__ENABLED | DMA0EN_CH3EN__ENABLED;
#else
	DMA0EN   = DMA0EN_CH0EN__ENABLED | DMA0EN_CH1EN__ENABLED | DMA0EN_CH2EN__ENABLED;
#endif
	// MAC0 setting
	// No need to change page register as MAC0 and DMA0 registers are in same page.
	// Clear accumulator, Multiply and Accumulate mode
	MAC0CF0 = MAC0CF0_CLRACC__START | MAC0CF0_ACCMD__MAC_MODE;
	// MAC0CF0 = MAC0CF0_CLRACC__START | MAC0CF0_ACCMD__MUL_MODE;
	// Select unsigned mode, MAC A/B data fetched by DMA from XRAM
	MAC0CF1 = MAC0CF1_MAC0BC__XRAM | MAC0CF1_ADMASRC__XRAM;
	// Disable rounding, saturation and alignment logic

	MAC0CF2 = MAC0CF2_ACCDMAIN__2_BYTES | MAC0CF2_ACCDMAOUT__4_BYTES;
	MAC0STA  = MAC0STA_BUSY__SET;    // Set BUSY to start MAC operation
	while (!(DMA0INT & DMA0INT_CH0I__BMASK)); // Poll for DMA Channel 0 completion interrupt
	// All len bytes will be initialized to value
	SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// MUL_ARRAY_With_DMA0_MAC0
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    xdata_array_a     address of uint16_t array a in xram
//                xdata_array_b     address of uint16_t array b in xram
//                len               length of array
//                xdata_array_c     address of uint32_t array c in xram, used
//                                  to save result
//
// Calculate the sum of product of corresponding member of two arrays
//    c[i] = a[i]*b[i]
//
// 3 channels are needed for this scenario:
//     1 for MAC0A
//     1 for MAC0B
//     1 for ACC->xram (only once at the start)
//-----------------------------------------------------------------------------
void MUL_ARRAY_With_DMA0_MAC0(uint16_t xdata_array_a, uint16_t xdata_array_b, uint8_t len, uint16_t xdata_array_c)
{
	uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

	SFRPAGE = DMA0_PAGE;

	// DMA0 channel0 for XRAM -> MAC0's A register
	DMA0SEL  = DMA0SEL_SELECT__CH0;
	// Disable channel 0
	DMA0EN   = (DMA0EN & (~DMA0EN_CH0EN__BMASK)) | DMA0EN_CH0EN__DISABLED;
    // Clear interrupt of channel 0
	DMA0INT &= ~DMA0INT_CH0I__BMASK;
    // XRAM->MAC's A
	// Endian bit's definition is the same as LSB of UU16 of compiler
	DMA0NCF  = DMA0NCF_PERIPH__XRAM_TO_MAC_A | (LSB << DMA0NCF_ENDIAN__SHIFT) | DMA0NCF_IEN__ENABLED;
	DMA0NBA  = xdata_array_a;
	DMA0NAO  = 0;
	DMA0NSZ  = len<<1;  // Fetch 2 bytes to MAC0's A register every time


	// DMA0 channel1 for XRAM -> MAC0's B register
	DMA0SEL  = DMA0SEL_SELECT__CH1;
	// Disable channel 1
	DMA0EN   = (DMA0EN & (~DMA0EN_CH1EN__BMASK)) | DMA0EN_CH1EN__DISABLED;
    // Clear interrupt of channel 1
	DMA0INT &= ~DMA0INT_CH1I__BMASK;
    // XRAM->MAC's B
	// Endian bit's definition is the same as LSB of UU16 of compiler
	DMA0NCF  = DMA0NCF_PERIPH__XRAM_TO_MAC_B | (LSB << DMA0NCF_ENDIAN__SHIFT) | DMA0NCF_IEN__ENABLED;
	DMA0NBA  = xdata_array_b;
	DMA0NAO  = 0;
	DMA0NSZ  = len<<1;  // Fetch 2 bytes to MAC0's B register every time

    // Value before calculating X_i*Y_i is not used. So no DMA channel is needed for XRAM->ACC

	// DMA0 channel3 setting
	// Choose channel 3 for MAC0's ACC -> XRAM
	DMA0SEL  = DMA0SEL_SELECT__CH3;
	// Disable channel 3
	DMA0EN   = (DMA0EN & (~DMA0EN_CH3EN__BMASK)) | DMA0EN_CH3EN__DISABLED;
    // Clear interrupt of channel 3
	DMA0INT &= ~DMA0INT_CH3I__BMASK;
    // ACC -> XRAM
	// Endian bit's definition is the same as LSB of UU16 of compiler
	DMA0NCF  = DMA0NCF_PERIPH__MAC_ACC_TO_XRAM | (LSB << DMA0NCF_ENDIAN__SHIFT) | DMA0NCF_IEN__ENABLED;
	DMA0NBA  = xdata_array_c;
	DMA0NAO  = 0;
	MAC0ITER = 1;                    // Copy ACC->xram every time MUL is done
#if MAC_ARRARY_2_IN_TWO_BYTES
	DMA0NSZ  = len<<1;  // This number can be bigger becasue DMA will stop after other channel is over.
#else
	DMA0NSZ  = len<<2;  // This number can be bigger becasue DMA will stop after other channel is over.
#endif
	DMA0EN   = DMA0EN_CH0EN__ENABLED | DMA0EN_CH1EN__ENABLED | DMA0EN_CH3EN__ENABLED;

	// MAC0 setting
	// No need to change page register as MAC0 and DMA0 registers are in same page.
	// Clear accumulator, Multiply mode only
	MAC0CF0 = MAC0CF0_CLRACC__START | MAC0CF0_ACCMD__MUL_MODE;
	// Select unsigned mode, MAC A/B data fetched by DMA from XRAM
	MAC0CF1 = MAC0CF1_MAC0BC__XRAM | MAC0CF1_ADMASRC__XRAM;
	// Disable rounding, saturation and alignment logic
#if MAC_ARRARY_2_IN_TWO_BYTES
	MAC0CF2 = MAC0CF2_ACCDMAIN__2_BYTES | MAC0CF2_ACCDMAOUT__2_BYTES;
#else
	MAC0CF2 = MAC0CF2_ACCDMAIN__2_BYTES | MAC0CF2_ACCDMAOUT__4_BYTES;
#endif
	MAC0STA  = MAC0STA_BUSY__SET;    // Set BUSY to start MAC operation
	while (!(DMA0INT & DMA0INT_CH0I__BMASK)); // Poll for DMA Channel 0 completion interrupt
	// All len bytes will be initialized to value
	SFRPAGE = SFRPAGE_save;
}



//-----------------------------------------------------------------------------
// Mem_Set_Xram_With_DMA0_MAC0
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    addr     start address of memory region in xram
//                len      bytes of memory region
//                value    each byte of the memory region will be set to this value
//
// The DMA will access 1 more byte if the DMA access 2 byte each time and length of
// memory region is odd.
//
// "addr + len" should be less than the max XRAM address
//-----------------------------------------------------------------------------
void Mem_Set_Xram_With_DMA0_MAC0(uint16_t addr, uint16_t len, uint8_t value)
{
	uint8_t SFRPAGE_save = SFRPAGE;
	// Memory block to initialize
	SFRPAGE  = DMA0_PAGE;                      // Change Page register to DMA0 Page
	DMA0SEL  = DMA0SEL_SELECT__CH0;            // Select DMA channel 0
    // Disable DMA channel 0
	DMA0EN  &= (DMA0EN & (~DMA0EN_CH0EN__BMASK)) | DMA0EN_CH0EN__DISABLED;
	DMA0INT &= ~DMA0INT_CH0I__BMASK;           // Clear interrupt bit for channel 0
	DMA0NCF = DMA0NCF_PERIPH__MAC_ACC_TO_XRAM; // Select MAC0 Accumulator to XRAM transfer
	DMA0NCF |= (LSB << 4);                     // Use LSB to spec endian bit for compiler independence
	DMA0NBA = addr;                            // XRAM base address
	DMA0NAO = 0;                               // XRAM offset
	DMA0NSZ = len;                             // Transfer len bytes
	DMA0EN  = DMA0EN_CH0EN__ENABLED;           // Enable DMA channel 0
	// No need to change page register as MAC0 and DMA0 registers are in same page.
	// Clear accumulator, Multiply Only mode
	MAC0CF0 = MAC0CF0_CLRACC__START | MAC0CF0_ACCMD__MUL_MODE;
	// Select unsigned mode, constant A and constant B
	MAC0CF1 = MAC0CF1_MAC0BC__EXISTING | MAC0CF1_ADMASRC__EXISTING;
	// Disable rounding, saturation and alignment logic
	MAC0A = value; // Load MAC0A[0-7] with value
#if  MEM_SET_DMA_ACCESS_BYTES == 1
	// Bytes of DMA transaction from MAC0 Accumulator to XRAM
	MAC0CF2 = MAC0CF2_ACCDMAOUT__1_BYTE;
	MAC0B = 1; // Load MAC0B with 1 decimal
#elif MEM_SET_DMA_ACCESS_BYTES == 2
	// Bytes of DMA transaction from MAC0 Accumulator to XRAM
	MAC0CF2 = MAC0CF2_ACCDMAOUT__2_BYTES;
	MAC0B = 0x101;              // MAC0A*0X101 =  (MAC0A <<8) | MAC0A
#else
	#error "Wrong MAC0CF2 setting!"
#endif
	MAC0ITER = 1; // Copy ACC->xram every time MUL is done
	MAC0STA  = MAC0STA_BUSY__SET; // Set BUSY to start MAC operation
	while (!(DMA0INT & DMA0INT_CH0I__BMASK)); // Poll for DMA Channel 0 completion interrupt
	// All len bytes will be initialized to value
	SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer2 overflows.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
    TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag
    LED5 = !LED5;                       // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
