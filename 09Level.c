/* 
#include "msp430fr6989.h"
#include "Grlib/grlib/grlib.h"          // Graphics library (grlib)
#include "LcdDriver/lcd_driver.h"       // LCD driver
#include <stdio.h>

#define redLED BIT0
#define greenLED BIT7
#define S1 BIT1
#define S2 BIT2

extern const Graphics_Image LevelImage;
extern const Graphics_Image StraightDescentImage;
extern const Graphics_Image StraightClimbImage;
extern const Graphics_Image LevelRightBankImage;
extern const Graphics_Image LevelLeftBankImage;
extern const Graphics_Image DescendingRightBankImage;
extern const Graphics_Image DescendingLeftBankImage;
extern const Graphics_Image ClimbingRightBankImage;
extern const Graphics_Image ClimbingLeftBankImage;

// ****************************************************************************
void main(void) {

    // Configure WDT & GPIO
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LEDs
    P1DIR |= redLED;                P9DIR |= greenLED;
    P1OUT &= ~redLED;               P9OUT &= ~greenLED;

    // Configure buttons
    P1DIR &= ~(S1|S2);
    P1REN |= (S1|S2);
    P1OUT |= (S1|S2);
    P1IFG &= ~(S1|S2);          // Flags are used for latched polling


    // Configure clock system
    Initialize_Clock_System();

    // Graphics functions
    Graphics_Context g_sContext;        // Declare a graphic library context
    Crystalfontz128x128_Init();         // Initialize the display

    // Set the screen orientation
    Crystalfontz128x128_SetOrientation(0);

    // Initialize the context
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    // Set the default font for strings
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    //Clear the screen
    Graphics_clearDisplay(&g_sContext);
    
    //Graphics_drawImage(&g_sContext, &LevelImage, 0, 0);
    //Graphics_drawImage(&g_sContext, &StraightDescentImage, 0, 0);
    Graphics_drawImage(&g_sContext, &StraightClimbImage, 0, 0);
    //Graphics_drawImage(&g_sContext, &LevelRightBankImage, 0, 0);
    //Graphics_drawImage(&g_sContext, &LevelLeftBankImage, 0, 0);
    //Graphics_drawImage(&g_sContext, &DescendingRightBankImage, 0, 0);
    //Graphics_drawImage(&g_sContext, &DescendingLeftBankImage, 0, 0);
    //Graphics_drawImage(&g_sContext, &ClimbingRightBankImage, 0, 0);
    //Graphics_drawImage(&g_sContext, &ClimbingLeftBankImage, 0, 0);


  for(;;){
    __delay_cycles(50000000);
    Graphics_drawStringCentered(&g_sContext, "Altitude: xxxxx", AUTO_STRING_LENGTH, 64, 5, OPAQUE_TEXT);
    __delay_cycles(50000000);
    Graphics_drawStringCentered(&g_sContext, "Altitude: yyyyy", AUTO_STRING_LENGTH, 64, 5, OPAQUE_TEXT);

  }

}


void Initialize_Clock_System() {
  // DCO frequency = 16 MHz
  // MCLK = fDCO/1 = 16 MHz
  // SMCLK = fDCO/1 = 16 MHz

  // Activate memory wait state
  FRCTL0 = FRCTLPW | NWAITS_1;    // Wait state=1
  CSCTL0 = CSKEY;
  // Set DCOFSEL to 4 (3-bit field)
  CSCTL1 &= ~DCOFSEL_7;
  CSCTL1 |= DCOFSEL_4;
  // Set DCORSEL to 1 (1-bit field)
  CSCTL1 |= DCORSEL;
  // Change the dividers to 0 (div by 1)
  CSCTL3 &= ~(DIVS2|DIVS1|DIVS0);    // DIVS=0 (3-bit)
  CSCTL3 &= ~(DIVM2|DIVM1|DIVM0);    // DIVM=0 (3-bit)
  CSCTL0_H = 0;

  return;
}













 */