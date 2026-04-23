/* #include <msp430fr6989.h>
#define BUZZER BIT7

void main(void){
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    // Configure clock - use ACLK from LFXT or VLO
    CSCTL0_H = CSKEY >> 8;              // Unlock clock registers
    CSCTL1 = DCOFSEL_6;                 // DCO = 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL0_H = 0;                       // Lock clock registers

    // Configure P2.7 for Timer output
    P2DIR |= BUZZER;
    P2SEL1 &= ~BUZZER;
    P2SEL0 |= BUZZER;                   // Select TA0.1

    // Use ACLK (VLO ~10kHz)
    // 10000 / 10 = 1000Hz = 1kHz tone
    TA0CTL = TASSEL_1 | MC_1 | TACLR;  // ACLK, Up mode
    TA0CCR0 = 9;                        // Period
    TA0CCR1 = 5;                        // 50% duty cycle
    TA0CCTL1 = OUTMOD_7;                // Reset/Set

    for(;;){}
} */