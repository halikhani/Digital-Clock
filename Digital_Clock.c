/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : test2
Version : 
Date    : 4/14/2020
Author  : halikhani
Company : 
Comments: 


Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega16.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <delay.h>
// Alphanumeric LCD functions
#include <alcd.h>

void showtime();
void show_swtime();
void set_time();
void set_hour();
void set_min();
void stopwatch();
void sw_init();
void save_sw_time();
void resume_sw_init();

// Declare your global variables here
int milisec_count = 0;
int sec = 0;
int min = 0;
int hour = 0;

int sw_milisec = 0;
int sw_sec = 0;
int sw_min = 0;
int sw_hour = 0;
int tmp_sw_milisec = 0;
int tmp_sw_sec = 0;
int tmp_sw_min = 0;
int tmp_sw_hour = 0;



int alarm_min = 0;
int alarm_hour = 0;
char buffer[10];
char tmp[10];
char sw_buffer[10];

bool checkset_flag = false;
bool showtime_flag = false;
bool alarm_flag = false;
bool alarmset_flag = false;
//bool stopwatch_flag = false;
bool sw_start_flag = false;



// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
// Reinitialize Timer 0 value
TCNT0=0x83;
// Place your code here
    if(++milisec_count == 1000){ 
        milisec_count = 0;  
        if(++sec == 60){
            sec = 0;
            if(++min == 60){
                min = 0;
                if(++hour == 24){
                    hour = 0;
                };
            };
        };      
        if(alarmset_flag == false){PORTB.0 = 0;};  
        showtime();  
        
    };

}

// Timer2 overflow interrupt service routine
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
// Reinitialize Timer2 value
TCNT2=0x06;
// Place your code here
//TIFR=0x0;               //check this line.
// Place your code here
    if(++sw_milisec == 1000){ 
        sw_milisec = 0;  
        if(++sw_sec == 60){
            sw_sec = 0;
            if(++sw_min == 60){
                sw_min = 0;
                if(++sw_hour == 24){
                    sw_hour = 0;
                };
            };
        };          
        show_swtime();
    }; 
     


}

void show_swtime(){
    if (sw_start_flag){
        lcd_gotoxy(0,0);
        lcd_puts("                ");
        lcd_gotoxy(0,0); 
        sprintf(sw_buffer, "%d", sw_hour);
        lcd_gotoxy(0,0);
        if (sw_hour/10 == 0){
            lcd_puts("0"); 
            lcd_puts(sw_buffer);
        }
        else{
            lcd_puts(sw_buffer);
        };  
        lcd_gotoxy(2,0);  
        lcd_puts(":");
        sprintf(sw_buffer, "%d", sw_min);
        if (sw_min/10 == 0){
            lcd_puts("0"); 
            lcd_puts(sw_buffer);
        }
        else{
            lcd_puts(sw_buffer);
        };   
        lcd_gotoxy(5,0);
        lcd_puts(":");  
        sprintf(sw_buffer, "%d", sw_sec);
        if (sw_sec/10 == 0){
            lcd_puts("0"); 
            lcd_puts(sw_buffer);
        }
        else{
            lcd_puts(sw_buffer);
        }; 
        
        
        
        
        
    };
    

    
}
                    


void showtime(){
    if (showtime_flag){
        lcd_clear(); 
        sprintf(buffer, "%d", hour);
        lcd_gotoxy(0,0);
        if (hour/10 == 0){
            lcd_puts("0"); 
            lcd_puts(buffer);
        }
        else{
            lcd_puts(buffer);
        };  
        lcd_gotoxy(2,0);  
        lcd_puts(":");
        sprintf(buffer, "%d", min);
        if (min/10 == 0){
            lcd_puts("0"); 
            lcd_puts(buffer);
        }
        else{
            lcd_puts(buffer);
        };   
        lcd_gotoxy(5,0);
        lcd_puts(":");  
        sprintf(buffer, "%d", sec);
        if (sec/10 == 0){
            lcd_puts("0"); 
            lcd_puts(buffer);
        }
        else{
            lcd_puts(buffer);
        }; 
        
        lcd_gotoxy(9,0);
        lcd_puts("alarm:1");
        lcd_gotoxy(5,1);
        lcd_puts("stopwatch:2"); 
    };
    if ((alarmset_flag == true) && (alarm_hour == hour) && (alarm_min == min) ){  
        alarm_flag = false;  
        alarmset_flag = false;
        lcd_clear();
        lcd_puts("alarm time!");
        PORTB.0 = ~ PORTB.0;
        
    }; 
    
}; 









void main(void)
{
// Declare your local variables here

DDRB = 0xff;
PORTB = 0x00;


DDRA = 0x00;
PORTA = 0xff;       // for mode 

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 125.000 kHz
// Mode: Normal top=0xFF
// OC0 output: Disconnected
// Timer Period: 1 ms
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
TCNT0=0x83;
OCR0=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 250.000 kHz
// Mode: Normal top=0xFF
// OC2 output: Disconnected
// Timer Period: 1 ms
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (1<<CS21) | (1<<CS20);
TCNT2=0x06;
OCR2=0x00;





// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);


// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTC Bit 0
// RD - PORTC Bit 1
// EN - PORTC Bit 2
// D4 - PORTC Bit 3
// D5 - PORTC Bit 4
// D6 - PORTC Bit 5
// D7 - PORTC Bit 6
// Characters/line: 16
// Global enable interrupts
#asm("sei")
lcd_init(16);
lcd_puts("press 1 to set");
lcd_gotoxy(0,1);
lcd_puts("the clock.");
while (1)
      {    
        
        if (! showtime_flag && PINA.0 == 0){
            set_time();                          
        };  
        
        if (showtime_flag && PINA.0 == 0){   
            showtime_flag = false;
            alarm_flag = true;
            set_time();        
            alarmset_flag = true;
            
        };       
        
        if (showtime_flag && PINA.1 == 0){   
            showtime_flag = false;  
            lcd_clear();
            lcd_puts("stopwatch mode");            
            delay_ms(500); 
            stopwatch(); 
            sw_start_flag = false;
            
        };
  

      }
}


void set_time(){
       lcd_clear(); 
       lcd_puts("set hour:");
       lcd_gotoxy(0,1);
       lcd_puts("set:1    inc:2");  
       set_hour(); 
       delay_ms(100);    
       lcd_clear(); 
       lcd_puts("set min:");
       lcd_gotoxy(0,1);
       lcd_puts("set:1    inc:2");  
       set_min();
       showtime_flag = true;  
       delay_ms(1000);
}


void set_hour(){
    int tmp_hour = 0;
    lcd_gotoxy(9,0);
    lcd_puts("00");
    delay_ms(500);
    while(!checkset_flag){
        while(PINA.1 == 1){
            if (PINA.0 == 0){
            checkset_flag = true; 
            break;
            };         
            
        };  
        if (checkset_flag){break;};
        if(++tmp_hour == 24){tmp_hour = 0;};  
        sprintf(tmp, "%d", tmp_hour); 
        lcd_gotoxy(9,0);
        if (tmp_hour/10 == 0){
            lcd_puts("0"); 
            lcd_puts(tmp);
        }
        else{
            lcd_puts(tmp);
        };  
        if (alarm_flag == true){alarm_hour = tmp_hour;}
        else {hour = tmp_hour; };
        delay_ms(500);  
  
    };   
    checkset_flag = false;
    

}

void set_min(){
    int tmp_min = 0;
    lcd_gotoxy(9,0);
    lcd_puts("00");
    delay_ms(500);
    while(1){
        while(PINA.1 == 1){
            if (PINA.0 == 0){
            checkset_flag = true; 
            break;
            };         
            
        };  
        if (checkset_flag){break;};
        if(++tmp_min == 60){tmp_min = 0;};  
        sprintf(tmp, "%d", tmp_min); 
        lcd_gotoxy(9,0);
        if (min/10 == 0){
            lcd_puts("0"); 
            lcd_puts(tmp);
        }
        else{
            lcd_puts(tmp);
        };  
        if (alarm_flag == true){
            alarm_min = tmp_min; 
        }
        else {
            min = tmp_min;
            
        };
        
        delay_ms(500); 
        
  
        
    }; 
    if (alarm_flag == false){sec = -1;};
    checkset_flag = false;
    

}


void stopwatch(){
    
    bool init_flag = true;
    bool start_flag = false; 
    bool stopped_flag = false;   
    bool reset_flag = false; 
    bool return_flag = false;
    lcd_clear();  
    //showtime_flag = false;
    
    //lcd_puts("stop:1");  
    
    
    while(1){
        if (init_flag){
            init_flag = false; 
            lcd_gotoxy(0,0);
            lcd_puts("00:00:00     ");
            lcd_gotoxy(0,1);
            lcd_puts("start:1 back:2  ");
            while(PINA.0 == 1){
                if (PINA.1 == 0){ 
                    return_flag = true; 
                    lcd_clear();
                    lcd_puts("back to clock");
                    break;
                //return to idle state and clear the flags for stopwatch        
                };
            };
            delay_ms(500);
            
            if (!return_flag){   
                
                sw_init();
                lcd_clear();
                delay_ms(300);
                //lcd_gotoxy(0,1); 
                sw_start_flag = true;  
                start_flag = true;
            };     
        };
        
        if (start_flag){
        start_flag = false;
        lcd_gotoxy(0,1);
        lcd_puts("stop:1          ");  
        while(PINA.0 == 1);
        save_sw_time();
          
        sw_start_flag = false;
        stopped_flag = true; 
        delay_ms(500);       
        };
        
        if (stopped_flag){
            stopped_flag = false;      
            lcd_gotoxy(0,1);
            lcd_puts("resume:1 reset:2"); 
            while(PINA.0 == 1){
                if (PINA.1 == 0){
                    //sw_init(); 
                    lcd_clear();
                    reset_flag = true;
                    sw_start_flag = false;
                    lcd_puts("00:00:00"); 
                    
                    init_flag = true; 
                    break; 
                    //return to idle state and clear the flags for stopwatch
                };
            };
            delay_ms(500);
            
            if (!reset_flag){
                  
                resume_sw_init();
                start_flag = true; 
                sw_start_flag = true;
            };
            
                
            
        };
        
        
                                                
        if (return_flag){
            return_flag = false; 
            showtime_flag = true;
            break;
        };        
    };

}


void sw_init(){
    sw_milisec = 0;
    sw_sec = 0;
    sw_min = 0;
    sw_hour = 0;
}

void save_sw_time(){
    tmp_sw_milisec = sw_milisec;
    tmp_sw_sec = sw_sec;
    tmp_sw_min = sw_min;
    tmp_sw_hour = sw_hour;
    
}

void resume_sw_init(){
    sw_milisec = tmp_sw_milisec;
    sw_sec = tmp_sw_sec;
    sw_min = tmp_sw_min;
    sw_hour = tmp_sw_hour;
    
}