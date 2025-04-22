// LCD module connections
sbit LCD_RS at RB2_bit;
sbit LCD_EN at RB3_bit;
sbit LCD_D4 at RB4_bit;
sbit LCD_D5 at RB5_bit;
sbit LCD_D6 at RB6_bit;
sbit LCD_D7 at RB7_bit;

sbit LCD_RS_Direction at TRISB2_bit;
sbit LCD_EN_Direction at TRISB3_bit;
sbit LCD_D4_Direction at TRISB4_bit;
sbit LCD_D5_Direction at TRISB5_bit;
sbit LCD_D6_Direction at TRISB6_bit;
sbit LCD_D7_Direction at TRISB7_bit;
// End LCD module connections

char freq[4] = "000";
char voltStr[16];  // Enough space for formatted string

unsigned int Read_ADC() {
    return ADC_Read(0); // Read from AN0 (RA0)
}

void Display_Freq(unsigned int freq2write) {
    freq[0] = (freq2write / 100) + 48;
    freq[1] = ((freq2write / 10) % 10) + 48;
    freq[2] = (freq2write % 10) + 48;
    freq[3] = '\0';

    Lcd_Out(2, 11, freq);
    Lcd_Out(2, 14, "Hz");

    // Optional: LED indicators based on frequency
    if(freq2write == 50) {
        PORTA.F0 = 1;
        PORTA.F1 = 0;
        PORTA.F2 = 0;
    }
    else if(freq2write > 50) {
        PORTA.F0 = 0;
        PORTA.F1 = 1;
        PORTA.F2 = 0;
    }
    else {
        PORTA.F0 = 0;
        PORTA.F1 = 0;
        PORTA.F2 = 1;
    }
}

void Display_Amplitude(unsigned int adc_val) {
    float voltage = adc_val * 5.0 / 1023.0; // 10-bit ADC, 5V reference
    FloatToStr(voltage, voltStr);          // Convert to string
    voltStr[4] = 0;                         // Limit after 1 decimal place (e.g. "3.9")

    Lcd_Out(1, 1, "Amp: ");
    Lcd_Out_CP(voltStr);
    Lcd_Out_CP(" V");
}

void main() {
    unsigned int adc_val;

    ADCON1 = 0x0E;         // AN0 analog, rest digital
    TRISA.F0 = 1;          // RA0 as input (AN0)
    TRISA.F1 = 0;          // Output (optional LED)
    TRISA.F2 = 0;
    TRISA.F4 = 1;          // RA4/T0CKI as input
    PORTA = 0x00;

    TRISB = 0x00;          // PORTB for LCD

    // Timer0 config: 8-bit counter mode, rising edge, prescaler 1:2
    T0CON = 0b00100000;
    TMR0L = 0;
    T0CON.TMR0ON = 1;

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);

    Lcd_Out(1, 1, "Initializing...");

    Delay_ms(1000);
    Lcd_Cmd(_LCD_CLEAR);

    while(1) {
        TMR0L = 0;
        Delay_ms(1000);             // 1 second window
        T0CON.TMR0ON = 0;

        Lcd_Out(2, 1, "FREQUENCY:");
        Display_Freq(TMR0L / 2);    // Convert pulses to Hz
        TMR0L = 0;
        T0CON.TMR0ON = 1;

        adc_val = Read_ADC();       // Read amplitude
        Display_Amplitude(adc_val);
    }
}
