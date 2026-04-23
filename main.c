
//Joystick Setip
#include <msp430fr6989.h>
#define redLED BIT0
#define greenLED BIT7
#define BUT BIT2
#define BUZZER BIT7
#define FLAGS UCA1IFG // Contains the transmit & receive flags
#define RXFLAG UCRXIFG // Receive flag
#define TXFLAG UCTXIFG // Transmit flag
#define TXBUFFER UCA1TXBUF // Transmit buffer
#define RXBUFFER UCA1RXBUF // Receive buffer
unsigned char LF = 10;
unsigned char CR = 13;
void Initialize_ADC(void);
void Play_Buzzer(void);
void Initialize_UART(void);
void uart_write_char(unsigned char ch);
void uart_write_uint16(unsigned int n);
void joystick_mode(void);
void accel_mode(void);
void show_screen(unsigned int x, unsigned int y);

int altitude = 0;

void main(void){
    WDTCTL = WDTPW | WDTHOLD;   //Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       //Enable the GPIO pins

    // Configure LEDs
    P1DIR |= redLED;            // Set P1.0 (red LED) to output
    P1OUT &= ~redLED;           // Start with LED off
    P9DIR |= greenLED;           // Start with red off
    P9OUT &= ~greenLED;          // Start with green off

    P3DIR &= ~BUT;              //Set up button
    P3REN |= BUT;
    P3OUT |= BUT;
    P3IFG &= ~BUT;
    

    Initialize_ADC();
    Initialize_UART();
    //Play_Buzzer();


    //Infinite loop
    for(;;){
        accel_mode();
        joystick_mode();


    }

}

void accel_mode(void){

    for(;;){
            // --- STEP 1: READ X-AXIS (A7) ---
        ADC12CTL0 &= ~ADC12ENC;             // Disable to change channel
        ADC12MCTL0 = ADC12VRSEL_0 | ADC12INCH_7; // Set to A7
        ADC12CTL0 |= ADC12ENC;              // Re-enable
        
        ADC12CTL0 |= ADC12SC;               // Start conversion
        while(ADC12CTL1 & ADC12BUSY);       // Wait for it to finish
        unsigned int xValue = ADC12MEM0;    // Read X result

        // --- STEP 2: READ Y-AXIS (A6) ---
        ADC12CTL0 &= ~ADC12ENC;             // Disable to change channel
        ADC12MCTL0 = ADC12VRSEL_0 | ADC12INCH_6; // Set to A6
        ADC12CTL0 |= ADC12ENC;              // Re-enable
        
        ADC12CTL0 |= ADC12SC;               // Start conversion
        while(ADC12CTL1 & ADC12BUSY);       // Wait for it to finish
        unsigned int yValue = ADC12MEM0;    // Read Y result

        // X-axis altitude
        if (xValue > 2100) {
            altitude++;
        }
        if (xValue < 1900 && altitude > 0) {
            altitude--;
        }


        // X-axis Logic (Red LED + Sound)
        if ((xValue > 2500) | (xValue < 1500)){
            P1OUT |= redLED;
            Play_Buzzer();
        }
        else {
            P1OUT &= ~redLED;
        }

        // Y-axis Logic (Green LED + Sound)
        if ((yValue > 2500) | (yValue < 1500)){      
            P9OUT |= greenLED;
            Play_Buzzer();
        }
        else {
            P9OUT &= ~greenLED;
        }

        //Center is about 2000; left and down are 1000, up and right are 3000
        uart_write_uint16(xValue);
        uart_write_uint16(yValue);
        

        if((P3IFG & BUT) != 0){
            P3IFG &= ~BUT;
            __delay_cycles(250000);
            return;
        }


        __delay_cycles(50000);
    }
}


void joystick_mode(void){

    for(;;){
            // --- STEP 1: READ X-AXIS (A10) ---
        ADC12CTL0 &= ~ADC12ENC;             // Disable to change channel
        ADC12MCTL0 = ADC12VRSEL_0 | ADC12INCH_10; // Set to A10
        ADC12CTL0 |= ADC12ENC;              // Re-enable
        
        ADC12CTL0 |= ADC12SC;               // Start conversion
        while(ADC12CTL1 & ADC12BUSY);       // Wait for it to finish
        unsigned int xValue = ADC12MEM0;    // Read X result

        // --- STEP 2: READ Y-AXIS (A4) ---
        ADC12CTL0 &= ~ADC12ENC;             // Disable to change channel
        ADC12MCTL0 = ADC12VRSEL_0 | ADC12INCH_4; // Set to A4
        ADC12CTL0 |= ADC12ENC;              // Re-enable
        
        ADC12CTL0 |= ADC12SC;               // Start conversion
        while(ADC12CTL1 & ADC12BUSY);       // Wait for it to finish
        unsigned int yValue = ADC12MEM0;    // Read Y result

        // X-axis altitude
        if (xValue > 2300) {
            altitude++;
        }
        if (xValue < 1700 && altitude > 0) {
            altitude--;
        }

        // X-axis Logic (Red LED + Sound)
        if ((xValue > 3000) | (xValue < 1000)){
            P1OUT |= redLED;
            Play_Buzzer();
        }
        else {
            P1OUT &= ~redLED;
        }

        // Y-axis Logic (Green LED + Sound)
        if ((yValue > 3000) | (yValue < 1000)){      
            P9OUT |= greenLED;  
            Play_Buzzer();
        }
        else {
            P9OUT &= ~greenLED;
        }

        if((P3IFG & BUT) != 0){
            P3IFG &= ~BUT;
            __delay_cycles(250000);
            return;
        }

        //Center is about 2000; left and down are 1, up and right are 4000
        uart_write_uint16(xValue);
        uart_write_uint16(yValue);
        __delay_cycles(50000);
    }

}


void uart_write_char(unsigned char ch){
    // Wait for any ongoing transmission to complete
    while ( (FLAGS & TXFLAG)==0 ) {}
    // Copy the byte to the transmit buffer
    TXBUFFER = ch; // Tx flag goes to 0 and Tx begins!
    return;
}


//Write 5 digit integer
void uart_write_uint16 (unsigned int n){
    //n is a 16-bit value, that means that it can go up to 65,535
    //Have to write from right to left to show digits in order
    int j;
    if(n >= 10000){
        int j = (n/10000);
        uart_write_char('0'+ j);        
    }
    if(n >= 1000){
        j = (n/1000)%10;
        uart_write_char('0'+ j);
    }
    if(n >= 100){
        j = (n/100)%10;
        uart_write_char('0'+ j);
    }
    if(n >= 10){
        j = (n/10)%10;
        uart_write_char('0'+ j);
    }
    if(n >= 0){
        j = n%10;
        uart_write_char('0'+ j);
    }

    uart_write_char(LF);
    uart_write_char(CR);
}

// Configure UART to the popular configuration
// 9600 baud, 8-bit data, LSB first, no parity bits, 1 stop bit
// no flow control, oversampling reception
// Clock: SMCLK @ 1 MHz (1,000,000 Hz)
void Initialize_UART(void){
// Configure pins to UART functionality
    P3SEL1 &= ~(BIT4|BIT5);
    P3SEL0 |= (BIT4|BIT5);
    // Main configuration register
    UCA1CTLW0 = UCSWRST; // Engage reset; change all the fields to zero
    // Most fields in this register, when set to zero, correspond to the
    // popular configuration
    UCA1CTLW0 |= UCSSEL_2; // Set clock to SMCLK
    // Configure the clock dividers and modulators (and enable oversampling)
    UCA1BRW = 6; // divider
    // Modulators: UCBRF = 8 = 1000 --> UCBRF3 (bit #3)
    // UCBRS = 0x20 = 0010 0000 = UCBRS5 (bit #5)
    UCA1MCTLW = UCBRF3 | UCBRS5 | UCOS16;
    // Exit the reset state
    UCA1CTLW0 &= ~UCSWRST;
}


void Play_Buzzer(void) {
    
    P2DIR |= BIT7;
    P2SEL0 &= ~BIT7;
    P2SEL1 &= ~BIT7;   // Plain GPIO, no timer
    int i;
    for(i = 0; i < 100; i++){
        P2OUT ^= BIT7;
        __delay_cycles(500);
    }
}

void Initialize_ADC() {
    // Configure the pins to analog functionality
    // X-axis: A10/P9.2, for A10 (P9DIR=x, P9SEL1=1, P9SEL0=1)
    P9SEL1 |= BIT2;
    P9SEL0 |= BIT2;

    // Y-axis: A4/P8.7, for A4 (P9DIR=x, P8SEL1=1, P8SEL0=1)
    P8SEL1 |= BIT7;
    P8SEL0 |= BIT7;

    // Turn on the ADC module
    ADC12CTL0 |= ADC12ON;

    // Turn off ENC (Enable Conversion) bit while modifying the configuration
    ADC12CTL0 &= ~ADC12ENC;

    //*************** ADC12CTL0 ***************
    // Set ADC12SHT0 (select the number of cycles that you computed)
    ADC12CTL0 |= ADC12SHT0_2; 
    
    //*************** ADC12CTL1 ***************
    // Set ADC12SHS (select ADC12SC bit as the trigger)
    ADC12CTL1 |= ADC12SHS_0;

    // Set ADC12SHP bit
    ADC12CTL1 |= ADC12SHP;
    
    // Set ADC12DIV (select the divider you determined)
    ADC12CTL1 |= ADC12DIV_0;

    // Set ADC12SSEL (select MODOSC)
    ADC12CTL1 |= ADC12SSEL_0;

    //*************** ADC12CTL2 ***************
    // Set ADC12RES (select 12-bit resolution)
    ADC12CTL2 |= ADC12RES_2;

    // Set ADC12DF (select unsigned binary format)
    ADC12CTL2 &= ~ADC12DF;

    //*************** ADC12CTL3 ***************
    // Leave all fields at default values
    //*************** ADC12MCTL0 ***************
    // Set ADC12VRSEL (select VR+=AVCC, VR-=AVSS)
    ADC12MCTL0 |= ADC12VRSEL_0;

    // Set ADC12INCH (select channel A10)
    ADC12MCTL0 |= ADC12INCH_10;
    
    // Turn on ENC (Enable Conversion) bit at the end of the configuration
    ADC12CTL0 |= ADC12ENC;  
    return;
}



