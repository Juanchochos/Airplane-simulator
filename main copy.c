/* #include <msp430fr6989.h>
#define BUZZER BIT7

void main(void){
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P2DIR |= BUZZER;
    P2SEL1 &= ~BUZZER;
    P2SEL0 &= ~BUZZER;      // Plain GPIO, no timer

    // Manually check if TA0R is counting
    TA0CTL = TASSEL_1 | MC_1 | TACLR | ID_0;
    TA0CCR0 = 999;

    for(;;){
        // If TA0R is incrementing, toggle the LED so we can see something
        if(TA0R == 0){
            P2OUT ^= BUZZER;    // Toggle pin every time timer resets
        }
    }
}
 */