
#include <string.h>
#include <SPI.h>
#include <math.h>
#include <FIR.h>
//#include "Average.h"
#define BPM_WINDOW 300


#define CONTROL0		0x00
#define LED2STC			0x01
#define LED2ENDC		0x02
#define LED2LEDSTC		0x03
#define LED2LEDENDC		0x04
#define ALED2STC		0x05
#define ALED2ENDC		0x06
#define LED1STC			0x07
#define LED1ENDC		0x08
#define LED1LEDSTC		0x09
#define LED1LEDENDC		0x0a
#define ALED1STC		0x0b
#define ALED1ENDC		0x0c
#define LED2CONVST		0x0d
#define LED2CONVEND		0x0e
#define ALED2CONVST		0x0f
#define ALED2CONVEND	0x10
#define LED1CONVST		0x11
#define LED1CONVEND		0x12
#define ALED1CONVST		0x13
#define ALED1CONVEND	0x14
#define ADCRSTCNT0		0x15
#define ADCRSTENDCT0	0x16
#define ADCRSTCNT1		0x17
#define ADCRSTENDCT1	0x18
#define ADCRSTCNT2		0x19
#define ADCRSTENDCT2	0x1a
#define ADCRSTCNT3		0x1b
#define ADCRSTENDCT3	0x1c
#define PRPCOUNT		0x1d
#define CONTROL1		0x1e
#define SPARE1			0x1f
#define TIAGAIN			0x20
#define TIA_AMB_GAIN	0x21
#define LEDCNTRL		0x22
#define CONTROL2		0x23
#define SPARE2			0x24
#define SPARE3			0x25
#define SPARE4			0x26
#define SPARE4			0x26
#define RESERVED1		0x27
#define RESERVED2		0x28
#define ALARM			0x29
#define LED2VAL			0x2a
#define ALED2VAL		0x2b
#define LED1VAL			0x2c
#define ALED1VAL		0x2d
#define LED2ABSVAL		0x2e
#define LED1ABSVAL		0x2f
#define DIAG			0x30


#define count 60
int IRheartsignal[count];
int Redheartsignal[count];
int IRdc[count];
int Reddc[count];
double difIRheartsig_dc;
double difREDheartsig_dc;

FIR fir;

const int SOMI = 12; 
const int SIMO = 11; 
const int SCLK  = 13;
const int SPISTE = 7; 
const int SPIDRDY = 6;//wrong

int pin = 3;
int pin2 = 4;
volatile int state = LOW;


void AFE4490Init (void);
void AFE4490Write (uint8_t address, uint32_t data);
uint32_t AFE4490Read (uint8_t address);

void setup()
{
  Serial.begin(9600);

  SPI.begin(); 
  pinMode (SOMI,INPUT);
  pinMode (SPISTE,OUTPUT);
  pinMode (SCLK, OUTPUT);
  pinMode (SIMO, OUTPUT);
  pinMode (SPIDRDY,INPUT);

  pinMode(2, INPUT);
  pinMode(3, OUTPUT);

  //pinMode(pin, OUTPUT);
  attachInterrupt(0, blink, RISING );

  SPI.setClockDivider (SPI_CLOCK_DIV8);
  SPI.setDataMode (SPI_MODE0);
  SPI.setBitOrder (MSBFIRST);



  float coef[FILTERTAPS] = { 
    0.021, 0.096, 0.146, 0.096, 0.021  };
  fir.setCoefficients(coef);

  //declare gain coefficient to scale the output back to normal
  float gain = 1; // set to 1 and input unity to see what this needs to be
  fir.setGain(gain);



  AFE4490Init (); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
  float data = sp02();
  if(data)
  {

    if(data>=0.99 && data <= 1) Serial.println("Plug in the Probe!!!");
    else if(data<0.2 || data >1) Serial.println("Insert your finger!!!");
    else 
    {
        Serial.print("sp02:--> ");
        Serial.println(data);
    }
  }

}


void blink()
{
  state = HIGH;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AFE4490Init (void)
{ 
  Serial.println("AFE4490 Initialization Starts"); 
  AFE4490Write(CONTROL0,0x000000);

  AFE4490Write(TIAGAIN,0x000000);	// CF = 5pF, RF = 500kR
  AFE4490Write(TIA_AMB_GAIN,0x000005);	// Timers ON, average 3 samples 
  AFE4490Write(LEDCNTRL,0x0022020);	  
  AFE4490Write(CONTROL2,0x002000);	// LED_RANGE=100mA, LED=50mA 
  AFE4490Write(CONTROL1,0x010707);	// Timers ON, average 3 samples  

  AFE4490Write(PRPCOUNT, 0X001F3F);
  /*AFE4490Write(CONTROL1, 0x000101);
   AFE4490Write(CONTROL2, 0x000000);  
   AFE4490Write(PRPCOUNT, 0X001F3F);*/

  AFE4490Write(LED2STC, 0X001770); //timer control
  AFE4490Write(LED2ENDC,0X001F3E); //timer control
  AFE4490Write(LED2LEDSTC,0X001770); //timer control
  AFE4490Write(LED2LEDENDC,0X001F3F); //timer control
  AFE4490Write(ALED2STC, 0X000000); //timer control
  AFE4490Write(ALED2ENDC, 0X0007CE); //timer control
  AFE4490Write(LED2CONVST,0X000002); //timer control
  AFE4490Write(LED2CONVEND, 0X0007CF); //timer control
  AFE4490Write(ALED2CONVST, 0X0007D2); //timer control
  AFE4490Write(ALED2CONVEND,0X000F9F); //timer control

  AFE4490Write(LED1STC, 0X0007D0); //timer control
  AFE4490Write(LED1ENDC, 0X000F9E); //timer control
  AFE4490Write(LED1LEDSTC, 0X0007D0); //timer control
  AFE4490Write(LED1LEDENDC, 0X000F9F); //timer control
  AFE4490Write(ALED1STC, 0X000FA0); //timer control
  AFE4490Write(ALED1ENDC, 0X00176E); //timer control
  AFE4490Write(LED1CONVST, 0X000FA2); //timer control
  AFE4490Write(LED1CONVEND, 0X00176F); //timer control
  AFE4490Write(ALED1CONVST, 0X001772); //timer control
  AFE4490Write(ALED1CONVEND, 0X001F3F); //timer control

  AFE4490Write(ADCRSTCNT0, 0X000000); //timer control
  AFE4490Write(ADCRSTENDCT0,0X000000); //timer control
  AFE4490Write(ADCRSTCNT1, 0X0007D0); //timer control
  AFE4490Write(ADCRSTENDCT1, 0X0007D0); //timer control
  AFE4490Write(ADCRSTCNT2, 0X000FA0); //timer control
  AFE4490Write(ADCRSTENDCT2, 0X000FA0); //timer control
  AFE4490Write(ADCRSTCNT3, 0X001770); //timer control
  AFE4490Write(ADCRSTENDCT3, 0X001770);

  delay(1000);
  Serial.println("AFE4490 Initialization Done"); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AFE4490Write (uint8_t address, uint32_t data)
{
  digitalWrite (SPISTE, LOW); // enable device
  SPI.transfer (address); // send address to device
  SPI.transfer ((data >> 16) & 0xFF); // write top 8 bits
  SPI.transfer ((data >> 8) & 0xFF); // write middle 8 bits
  SPI.transfer (data & 0xFF); // write bottom 8 bits    
  digitalWrite (SPISTE, HIGH); // disable device
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t AFE4490Read (uint8_t address)
{       
  uint32_t data=0;
  digitalWrite (SPISTE, LOW); // enable device
  SPI.transfer (address); // send address to device
  //SPI.transfer (data);
  data |= (SPI.transfer (0)<<16); // read top 8 bits data
  data |= (SPI.transfer (0)<<8); // read middle 8 bits  data
  data |= SPI.transfer (0); // read bottom 8 bits data
  digitalWrite (SPISTE, HIGH); // disable device

  return data; // return with 24 bits of read data
}


void enableDRDY()
{
  state = LOW;
  attachInterrupt(0, blink, RISING );  //Ataching interupt handeler to interupt 0, ie pin 2
}

void disableDRDY()
{
  detachInterrupt(0);  
}

signed long AFERead (uint8_t address)
{       

  uint32_t data = AFE4490Read (address);

  unsigned long utemp = (unsigned long) (data<<10);            //to make it into 32 bit signed interger
  signed long stemp = (signed long) (utemp);
  stemp = (signed long) (stemp>>10);	


  return stemp; // return with 22 bits of read data
}


float sp02(void)
{
  //unsigned int 
  long Redvalue, IRvalue, Redhigh, Redlow, IRhigh, IRlow;
  long Redsum = 0, IRsum = 0;
  unsigned long Redac_sq = 0;
  unsigned long IRac_sq = 0;
  static int flag1 = 1;
  long Reddc, IRdc,Reddc_prev,IRdc_prev;
  int samples = 2000;
  long IRac; 
  long  Redac;


  AFE4490Write(CONTROL0,0x000001);  	
  Redhigh = Redlow = AFERead(LED2VAL);
  IRhigh = IRlow = AFERead(LED1VAL);



  for(int i=1;i<(samples+1);i++)
  {
    enableDRDY();
    while (state == LOW);        //this while wait for an interupt at pin 2

    Redvalue = AFERead(LED2VAL);
    IRvalue = AFERead(LED1VAL);
    disableDRDY();

    Redvalue = fir.process(Redvalue);  
    IRvalue = fir.process(IRvalue);  


    /* Serial.print(i); 
     Serial.print("\t");   
     Serial.print(Redvalue); 
     Serial.print("\t"); 
     Serial.print(IRvalue); 
     Serial.print("\t"); 
     	*/
    Redsum += Redvalue;
    IRsum += IRvalue;

    if(Redvalue > Redhigh)
      Redhigh = Redvalue;	
    if(Redvalue < Redlow)
      Redlow = Redvalue;	

    if(IRvalue > IRhigh)
      IRhigh = IRvalue;	
    if(IRvalue < IRlow)
      IRlow = IRvalue;	


    if (i<501)
    {
      continue;
    }
    Reddc = Redsum/i;
    IRdc = IRsum/i;


    Redac_sq += pow (((long)(Redvalue - Reddc)), 2.0);
    IRac_sq += pow (((long)(IRvalue - IRdc)), 2.0);

  }		

  /*
      if((Reddc < 0 && IRdc < 0) ||( Reddc>4000 && IRdc>4000))
   {
   Serial.println("Finger not detected.");
   return 0;
   }   
   */


  Redac = sqrt(Redac_sq/(samples-500));
  IRac = sqrt(IRac_sq/(samples-500));

  Serial.print("Reddc: "); 
  Serial.print(Reddc); 
  Serial.print("\t"); 
  Serial.print("Redhigh: ");                 
  Serial.print(Redhigh); 
  Serial.print("\t");
  Serial.print("Redlow: ");                 
  Serial.print(Redlow); 
  Serial.print("\t");
  Serial.print("Redac_sq: ");                 
  Serial.print(Redac_sq); 
  Serial.print("\t");
  Serial.print("Redac :"); 
  Serial.print(Redac); 
  Serial.print("\t");         


  Serial.print("IRdc: "); 
  Serial.print(IRdc); 
  Serial.print("\t");          
  Serial.print("IRhigh: ");                 
  Serial.print(IRhigh); 
  Serial.print("\t");
  Serial.print("IRlow: ");                 
  Serial.print(IRlow); 
  Serial.print("\t");     
  Serial.print("IRac_sq :")  ;
  Serial.print(IRac_sq); 
  Serial.print("\t");            
  Serial.print("IRac :"); 
  Serial.print(IRac); 
  Serial.print("\t"); 


  float spo2 = (float)((float)Redac/Reddc)/(float)((float)IRac/IRdc);

  return spo2;
}	



