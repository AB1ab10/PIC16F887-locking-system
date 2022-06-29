#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)



#include <xc.h>
#define _XTAL_FREQ 8000000

#define  C1 PORTDbits.RD0
#define  C2 PORTDbits.RD1
#define  C3 PORTDbits.RD2
#define  C4 PORTDbits.RD3
#define  RowA PORTDbits.RD4
#define  RowB PORTDbits.RD5
#define  RowC PORTDbits.RD6
#define  RowD PORTDbits.RD7



unsigned char password[4] = {'1','2','0','8'};
unsigned char input[5] = {'n' , 'n' , 'n' , 'n'};
unsigned char disp[4] = {0xBF , 0xBF , 0xBF, 0xBF};



char READ_SWITCHES(void)  
{  
  
  
  C1 = 1; C2 = 0; C3 = 0; C4 = 0;   //Test Column 1
  if (RowA == 1) { __delay_ms(250); while (RowA==1);  return '1';  }
  if (RowB == 1) { __delay_ms(250); while (RowB==1); return '4'; }
  if (RowC == 1) { __delay_ms(250); while (RowC==1); return '7'; }
  if (RowD == 1) { __delay_ms(250); while (RowD==1); return '*'; }
 
  C1 = 0; C2 = 1; C3 = 0; C4 = 0;   //Test Column 2
  if (RowA == 1) { __delay_ms(250); while (RowA==1); return '2'; }
  if (RowB == 1) { __delay_ms(250); while (RowB==1); return '5'; }
  if (RowC == 1) { __delay_ms(250); while (RowC==1); return '8'; }
  if (RowD == 1) { __delay_ms(250); while (RowD==1); return '0'; }
  
  C1 = 0; C2 = 0; C3 = 1; C4 = 0;   //Test Column 3
  if (RowA == 1) { __delay_ms(250); while (RowA==1); return '3'; }
  if (RowB == 1) { __delay_ms(250); while (RowB==1); return '6'; }
  if (RowC == 1) { __delay_ms(250); while (RowC==1); return '9'; }
  if (RowD == 1) { __delay_ms(250); while (RowD==1); return 'H'; }
  
  C1 = 0; C2 = 0; C3 = 0; C4 = 1;   //Test Column 4
  if (RowA == 1) { __delay_ms(250); while (RowA==1); return 'A'; }
  if (RowB == 1) { __delay_ms(250); while (RowB==1); return 'B'; }
  if (RowC == 1) { __delay_ms(250); while (RowC==1); return 'C'; }
  if (RowD == 1) { __delay_ms(250); while (RowD==1); return 'D'; }
  return 'n';             // Means no key has been pressed
}

char GetKey(void)              // Get key from user
{
    char key = 'n';              // Assume no key pressed
    while(key=='n')              // Wait until a key is pressed
        key = READ_SWITCHES();   // Scan the keys again and again
    return key;                  //when key pressed then return its value
}


void store(){
    int enter = 0;
    int cnt = 0;
    while(!enter){
        input[cnt] = GetKey();
        if(input[cnt] == 'A') enter = 1;
        else cnt += 1;
    }

}

int checkPassword(){
    int flag = 1;
    for(int i = 0; i < 4; i++){
        if (input[i] != password[i])
            flag = 0;
      
    }
    return flag;
}




unsigned char sevenSegmentDecoder(unsigned char a)
{
 switch(a)
 {
   case '0': return 0xC0;
   case '1': return 0xF9;
   case '2': return 0xA4;
   case '3': return 0xA8;
   case '4': return 0x99;
   case '5': return 0x8A;
   case '6': return 0x82;
   case '7': return 0xF8;
   case '8': return 0x80;
   case '9': return 0x88;
   case 'A': return 0x90;
   case 'B': return 0x83;
   case 'C': return 0xC6;
   case 'D': return 0xA1;
   case '-': return 0xBF;
    
   default : return 0xBF;
 }
}



void display(){
    PORTA = 0b00000001;
    PORTC = disp[0];
    __delay_ms(1);
    
    PORTA = 0b00000010;
    PORTC = disp[1];
    __delay_ms(1);
    
    PORTA = 0b00000100;
    PORTC = disp[2];
    __delay_ms(1);
    
    PORTA = 0b00001000;
    PORTC = disp[3];
    __delay_ms(1);
}

void displayBuild(int i){
        disp[i] = sevenSegmentDecoder(input[i]);
        
    }




void main(void) {
    TRISD = 0xf0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISE = 0;
    
    int flag;
    PORTE = 0b11111111;
    
    instr:
    display();
    store();
    for(int i = 0; i < 4; i++){
        display();
        displayBuild(i);
    }
    flag = checkPassword();
        
        if(flag) PORTE = 0b11111110;
        else PORTE = 0b11111011;

    while(1){
        
        
        display();
        
        
        
        
        
    }
    return;
}
