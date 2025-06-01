sbit LCD_RS at RD7_bit;
sbit LCD_EN at RD6_bit;
sbit LCD_D4 at RD5_bit;
sbit LCD_D5 at RD4_bit;
sbit LCD_D6 at RD3_bit;
sbit LCD_D7 at RD2_bit;

sbit LCD_RS_Direction at TRISD7_bit;
sbit LCD_EN_Direction at TRISD6_bit;
sbit LCD_D4_Direction at TRISD5_bit;
sbit LCD_D5_Direction at TRISD4_bit;
sbit LCD_D6_Direction at TRISD3_bit;
sbit LCD_D7_Direction at TRISD2_bit;
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
    Delay_ms(200);
    

    ADCON1 = 0x0E;         // AN0 analog, rest digital
    TRISA.F0 = 1;          // RA0 as input (AN0)
    TRISA.F4 = 1;          // RA4/T0CKI as input
    PORTA = 0x00;

    TRISD = 0x00;          // PORTB for LCD

    // Timer0 config: 8-bit counter mode, rising edge, prescaler 1:2
    T0CON = 0b00100000;
    TMR0L = 0;
    T0CON.TMR0ON = 1;

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);

    Lcd_Out(1, 1, "Initializing...");

    Delay_ms(500);
    Lcd_Cmd(_LCD_CLEAR);

    while(1) {
        TMR0L = 0;
        Delay_ms(1000);             // 1 second window
        T0CON.TMR0ON = 0;

        Lcd_Out(2, 1, "FREQUENCY:");
        Display_Freq(TMR0L);    // Convert pulses to Hz
        TMR0L = 0;
        T0CON.TMR0ON = 1;

        adc_val = Read_ADC();       // Read amplitude
        Display_Amplitude(adc_val);
    }
}