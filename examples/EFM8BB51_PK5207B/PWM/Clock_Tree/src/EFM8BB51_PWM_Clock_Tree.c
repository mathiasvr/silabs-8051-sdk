//-----------------------------------------------------------------------------
// EFM8BB51_PWM_Clock_Tree.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the PWM clock independence from the SYSCLK
// prescaler. PWM and the divided SYSCLK are both output to GPIO pins, and the
// SYSCLK divider value can be changed by pressing BTN0 or BTN1.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 8
//   P0.6   - SYSCLK output (EXP Header pin 8)
//   P0.7   - PWM CH0X output (EXP Header pin 6)
//   P1.0   - PWM CH1X output (EXP Header pin 4)
//   P2.0   - Display enable, shared by C2D pin
//            See AN127 for C2D pin sharing information
//
//   BTN0 - P0.2, decrease SYSCLK divider by one
//   BTN1 - P0.3, increase SYSCLK divider by one
//
//   PWM0-CH0X - P0.7
//   PWM0-CH1X - P1.0
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB51 PK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB51 PK board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB51 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Connect oscilloscope probes to P0.6, P0.7, and P1.0 to view the SYSCLK 
//    and PWM waveforms on the scope.
// 6) Observe that PWM CH0 and CH1 output a 1MHz waveform with 50% and 99%
//    duty cycle respectively
// 7) Press BTN0 and BTN1 to change the SYSCLK divider
// 8) Observe the changes to the SYSCLK output while the PWM remains stable and
//    unaffected.
//
// Target:         EFM8BB51
// Tool chain:     Generic
//
// Release 1.0 (DS)
//    - Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB51_Register_Enums.h>
#include <InitDevice.h>

//-----------------------------------------------------------------------------
// Pin Definitions and General Defines
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable, shared by C2D pin
                                       // See AN127 for C2D pin sharing info
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (BTN0, SFR_P0, 2);          // BTN0
SI_SBIT (BTN1, SFR_P0, 3);          // BTN1

#define CH0                   0
#define CH1                   1

/* Configurable defines below */

/* Change SYSCLK_PREDIV_FREQ if you change the system clock source */
#define SYSCLK_PREDIV_FREQ    24500000UL   // in Hz

/* PWMCLKDIVval specifies the PWM clock divider
 * The PWM clock freq equals SYSCLK_PREDIV_FREQ / (2 ^ PWMCKDIVval)
 * */
#define PWMCKDIVval           2

/* Use PWM_FREQ_DESIRED to specify the desired EDGE aligned frequency
 * NOTE: If using center-alignment, the frequency will be approximately
 * half that of the edge-aligned frequency.
 * The relationship of center-aligned period (Pc) to edge-aligned period (Pe)
 * when using the same PWMLIM and PWMCPx values is given by:
 * Pc = (2 * Pe) - 2
 *  */
#define PWM_FREQ_DESIRED      1000000UL        // edge aligned frequency, in Hz

/* PWMLIMval is used to set the PWM counter overflow value, and is
 * calculated from the configurable constants above */
#define PWMLIMval             ((SYSCLK_PREDIV_FREQ/(1<<PWMCKDIVval))/PWM_FREQ_DESIRED)

/* PWMUD_STEP configures the step value used when adjusting the PWM
 * duty cycle - initial step is 5% */
#define PWMUD_STEP            PWMLIMval / 20UL//PWMLIMval / 100UL

/* Change PWMALIGN to configure the alignment of the PWM signal.
 * Choose one of the following values:
 * PWMCFG0_PWMMODE__EDGE
 * PWMCFG0_PWMMODE__CENTER
 * */
#define PWMALIGN              PWMCFG0_PWMMODE__EDGE

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
volatile uint8_t updateDiv;

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
  // Disable the watchdog here
}

//-----------------------------------------------------------------------------
// initPWM
//-----------------------------------------------------------------------------
//
// This routine initializes the PWM for single ended output on channels 0 and 1.
//
//-----------------------------------------------------------------------------
void initPWM(void)
{
  uint8_t SFRPAGE_save = SFRPAGE;
  SFRPAGE = PG2_PAGE;

  /* Configure PWMCFG0:
   * 1) Set compile-time configurable alignment (PWMALIGN)
   * 2) Update CH0 and CH1 from update register on OF
   * 5) Enable PWM0 clock
   */
  PWMCFG0 = PWMALIGN | PWMCFG0_SYNCUPD__CH0CH1 | PWMCFG0_PWMCLKEN__ENABLE;

  /* Configure PWMCFG1:
   * 1) Use single ended mode on CH0 and CH1
   * 2) Configure for f_PWM >= f_SYSCLK
   */
  PWMCFG1 = PWMCFG1_DIFFMODE0__DISABLE | PWMCFG1_DIFFMODE1__DISABLE
      | PWMCFG1_PWMLTSYS__GTSYSCK;

  /* Configure PWMCFG2:
   * 1) Clear the counter
   * 2) Enable CH0 and CH1
   */
  PWMCFG2 = PWMCFG2_CNTRZERO__ZERO | PWMCFG2_CH1EN__ENABLE
      | PWMCFG2_CH0EN__ENABLE;

  /* Configure PWM clock frequency based on calculated PWMCKDIVval */
  PWMCKDIV = PWMCKDIVval;

  /* Configure PWM frequency based on calculated PWMLIMval definition */
  PWMLIM = PWMLIMval;

  /* Set duty cycle, CH0 = 50% */
  PWMCP0 = PWMLIMval >> 1;
  PWMCPUD0 = PWMLIMval >> 1;

  /* Set duty cycle, CH1 = 99% */
  PWMCP1 = PWMLIMval - 1;
  PWMCPUD1 = PWMLIMval - 1;

  /* Enable PWM, do not overwrite previous PWMCFG1 settings */
  PWMCFG1 |= PWMCFG1_PWMEN__ENABLE;

  SFRPAGE = SFRPAGE_save;
}

void UpdateSysClkDiv() {
  uint8_t sysDiv = (CLKSEL & CLKSEL_CLKDIV__FMASK) >> CLKSEL_CLKDIV__SHIFT;
  uint8_t pwmDiv =
      (PWMCFG1 & PWMCFG1_PWMLTSYS__BMASK) >> PWMCFG1_PWMLTSYS__SHIFT;

  if (!BTN0) {
    // Decrease SYSCLK divider, don't underflow
    sysDiv = sysDiv - (sysDiv > 0);
  }
  if (!BTN1) {
    // Increase SYSCLK divider, don't overflow
    sysDiv = sysDiv + (sysDiv < 7);
  }

  /* Apply updates to the PWM Clock Setting register immediately before changing
   * the system clock divider. This will prevent synchronization issues with the
   * PWM registers.
   */
  PWMCFG1 = (PWMCFG1 & ~PWMCFG1_PWMLTSYS__BMASK)
      | (sysDiv <= pwmDiv) << PWMCFG1_PWMLTSYS__SHIFT;

  /* Update the sys clock divider
   * Wait for the change to the divider to become ready
   */
  CLKSEL = (CLKSEL & ~CLKSEL_CLKDIV__FMASK) | sysDiv << CLKSEL_CLKDIV__SHIFT;
  while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY);
}

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{
  // initialize the flag that triggers a PWM duty cycle update
  updateDiv = 0;

  enter_DefaultMode_from_RESET();

  DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8

  /* Initialize the PWM for single ended output on CH0 and CH1 */
  initPWM();

  IE_EA = 1;                          // Enable global interrupts

  while (1) {
    /* The variable 'updateDiv' is set by INT0 or INT1 edge interrupts
     * corresponding to BTN0 or BTN1 presses
     */
    if (updateDiv == 1) {
      UpdateSysClkDiv();
      updateDiv = 0;
    }
  }
}
