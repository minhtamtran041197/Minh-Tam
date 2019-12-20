// Khai bao thu vien
#include <16f887.h>
// nowdt = no watch dog, put = power up timer, hs = high speed, noprotect = no protect code , nolvp = no low power
#fuses nowdt, put, hs, noprotect, nolvp 
// Tan so thach anh 20MHz
#use delay ( clock=20m)
#use rs232(baud=9600,xmit=pin_c6,rcv=pin_c7)
#INT_RDA
// Dinh nghia port B 
#define S1  pin_b1
#define S2  pin_b2
#define S3  pin_b3
#define S4  pin_b4
#define S5  pin_b5
// Dinh nghia port C
#define ENA  pin_c1 //ccp2 -> trai
#define ENB  pin_c2 //ccp1 -> phai
#define TX  pin_c6
#define RX  pin_c7
// Dinh nghia port D
#define IN1  pin_d0 // trai
#define IN2  pin_d1 // trai
#define IN3  pin_d2 // phai
#define IN4  pin_d3 // phai
// Variable
int t,t1,t2,t3,t4,t5,stt,sttc,c;
int mode = 0 ;
char b;
void Init();
void Sensor();
void PWM_Motor();
void Control();
#INT_RDA
void interrupt_receive()
{
   
      if (kbhit())
      {
         b=getc(); // nhan ky tu goi
         c=b; // gan ky tu nhan duoc vao bien luu
      }
 }
#INT_EXT
 void interrupt_ext()
 {
   mode ++;
   if( mode >1 )
   mode=0;
 }
 
/*---------------------------------------------------*/
// Chuong trinh chinh
void main()
{
   while (true)
   {
       Init();
       if (mode==0)
       {
         Sensor();
         PWM_Motor();
       }
       else
       {
         Control();
       }
       
      //Sensor();
      //PWM_Motor();
   }
}
void Init()
{
// ENA,ENB output
   set_tris_c(0x00);
// Port D output
   set_tris_d(0x00);
// Cho port d = 0
   output_d(0x00);
// PWM
// Chon che do pwm ccp1
   setup_ccp1(ccp_pwm);
// Chon che do PWM ccp2   
   setup_ccp2(ccp_pwm);
// T2 chia truoc 16, Tpwm = 0.8ms, chia sau 1 
   setup_timer_2(t2_div_by_16,249,1);
// Cho DC1 DC phai = 0
   set_pwm1_duty(0);
// Cho DC trai = 0
   set_pwm2_duty(0);
   enable_interrupts(global);
   enable_interrupts(int_ext);
   
   
}
void Sensor()
{
// Cho port b = output
   set_tris_b(0x01);
// Dua port b len High
   output_b(0xfe);
   delay_us(10);
// Cho port b lam input
   set_tris_b(0xff);
// Bien ung voi tung chan Sensor   
   t=0;t1=0;t2=0;t3=0;t4=0;t5=0;
   while(t<1000)
   {
      delay_us(1);
      t++; // Tang bien dem
      if( (input(S1)==0 && t1==0)) // Neu b0=Low  
      t1=t; // Gan t1 = bien dem
      // Tuong tu nhu tren
      if( (input(S2)==0 && t2==0))
      t2=t;
      // Tuong tu nhu tren
      if( (input(S3)==0 && t3==0))
      t3=t;
      // Tuong tu nhu tren
      if( (input(S4)==0 && t4==0))
      t4=t;
      // Tuong tu nhu tren
      if( (input(S5)==0 && t5==0))
      t5=t;  
   }
      if((t1>250)&&( t2<250)&&(t3<250)&&(t4<250)&&(t5>250))
      {
         stt=1; // Giua line
      }
      if((t1<250)&&(t2<250)&&(t3<250)&&(t4>250)&&(t5>250))
      {
         stt=2; // Phai line
      }
      if((t1>250)&&( t2>250)&&(t3<250)&&(t4<250)&&(t5<250))
      {
         stt=3; // Trai line
      }
}
void PWM_Motor()
{
   switch(stt)
   {
      case 1 :  // di thang 
      // phai
      
         delay_us(1);
         output_high(IN1);
         output_low (IN2);
         set_pwm1_duty(50);
      // trai
         output_high(IN3);
         output_low (IN4);
         set_pwm2_duty(50);
      
         break;
      case 2 :  // phai line 
      
      // phai
         delay_us(1);
         output_high(IN1);
         output_low (IN2);
         set_pwm1_duty(80);
      // trai
         output_high(IN3);
         output_low (IN4);
         set_pwm2_duty(30);
     
         break;
      case 3 :  // trai line
      
      // phai
         delay_us(1);
         output_high(IN1);
         output_low (IN2);
         set_pwm1_duty(30);
      // trai
         output_high(IN3);
         output_low (IN4);
         set_pwm2_duty(80);
         break;
}
}

 void Control()
{
   enable_interrupts(global);
   ext_int_edge(H_TO_L);
   enable_interrupts(int_rda);
   if ( c== '1')
   {
      sttc= 1; // toi
   }
   if ( c== '2')
   {
      sttc= 2; // lui
   }
   if ( c== '3')
   {
      sttc= 3; // trai
   }
   if ( c== '4')
   {
      sttc= 4; // phai
   }
   switch (sttc)
   {
      case 1 :
      delay_us(1);
         output_high(IN1);
         output_low (IN2);
         set_pwm1_duty(50);
      // trai
         output_high(IN3);
         output_low (IN4);
         set_pwm2_duty(50);
         break;
      case 2 :
      delay_us(1);
         output_high(IN2);
         output_low (IN1);
         set_pwm1_duty(50);
      // trai
         output_high(IN4);
         output_low (IN3);
         set_pwm2_duty(50);
         break;
       case 3 :
       delay_us(1);
         output_high(IN1);
         output_low (IN2);
         set_pwm1_duty(80);
      // trai
         output_high(IN3);
         output_low (IN4);
         set_pwm2_duty(30);
         break;
         case 4 :
       delay_us(1);
         output_high(IN1);
         output_low (IN2);
         set_pwm1_duty(30);
      // trai
         output_high(IN3);
         output_low (IN4);
         set_pwm2_duty(80);
         break;
   }
}


      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
     
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
 
     
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      


