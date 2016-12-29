#include <string.h>
#include <SPI.h>
#include <math.h>

#define CONTROL0    0x00
#define LED2STC     0x01
#define LED2ENDC    0x02
#define LED2LEDSTC    0x03
#define LED2LEDENDC   0x04
#define ALED2STC    0x05
#define ALED2ENDC   0x06
#define LED1STC     0x07
#define LED1ENDC    0x08
#define LED1LEDSTC    0x09
#define LED1LEDENDC   0x0a
#define ALED1STC    0x0b
#define ALED1ENDC   0x0c
#define LED2CONVST    0x0d
#define LED2CONVEND   0x0e
#define ALED2CONVST   0x0f
#define ALED2CONVEND  0x10
#define LED1CONVST    0x11
#define LED1CONVEND   0x12
#define ALED1CONVST   0x13
#define ALED1CONVEND  0x14
#define ADCRSTCNT0    0x15
#define ADCRSTENDCT0  0x16
#define ADCRSTCNT1    0x17
#define ADCRSTENDCT1  0x18
#define ADCRSTCNT2    0x19
#define ADCRSTENDCT2  0x1a
#define ADCRSTCNT3    0x1b
#define ADCRSTENDCT3  0x1c
#define PRPCOUNT    0x1d
#define CONTROL1    0x1e
#define SPARE1      0x1f
#define TIAGAIN     0x20
#define TIA_AMB_GAIN  0x21
#define LEDCNTRL    0x22
#define CONTROL2    0x23
#define SPARE2      0x24
#define SPARE3      0x25
#define SPARE4      0x26
#define SPARE4      0x26
#define RESERVED1   0x27
#define RESERVED2   0x28
#define ALARM     0x29
#define LED2VAL     0x2a
#define ALED2VAL    0x2b
#define LED1VAL     0x2c
#define ALED1VAL    0x2d
#define LED2ABSVAL    0x2e
#define LED1ABSVAL    0x2f
#define DIAG      0x30


#define BPM_WINDOW 300
#define count 60
int IRheartsignal[count];
int Redheartsignal[count];
int IRdc[count];
int Reddc[count];
double difIRheartsig_dc;
double difREDheartsig_dc;
double powdifIR;
double powdifRed;
double IRac; 
double Redac;
double SpOpercentage;
double Ratio;

const int SOMI = 12; 
const int SIMO = 11; 
const int SCLK  = 13;
const int SPISTE = 7; 
const int SPIDRDY = 6;

const int AFERESET = 5;

int pin = 3;
int pin2 = 4;
volatile int state = LOW;


void AFE4490Init (void);
void AFE4490Write (uint8_t address, uint32_t data);
uint32_t AFE4490Read (uint8_t address);
signed long average_BPM( signed long );
//--------------------------------------------------------------------------------------------------------------------

#define CES_CMDIF_PKT_START_1   0x0A
#define CES_CMDIF_PKT_START_2   0xFA
#define CES_CMDIF_TYPE_DATA   0x02
#define CES_CMDIF_PKT_STOP    0x0B

volatile char DataPacketHeader[16];
volatile char DataPacketFooter[2];
volatile int datalen = 135;
unsigned long time;

volatile byte SPI_RX_Buff[150] ;
volatile static int SPI_RX_Buff_Count = 0;
volatile char *SPI_RX_Buff_Ptr;
volatile int Responsebyte = false;
volatile unsigned int pckt =0 , buff=0,t=0 , j1=0,j2=0;
volatile unsigned long int EEG_Ch1_Data[150],EEG_Ch2_Data[150];
volatile unsigned char datac[150];
unsigned long ueegtemp = 0, ueegtemp2 = 0, Pkt_Counter=0;
unsigned long IRtemp,REDtemp;
signed long seegtemp=0, seegtemp2=0;
volatile int i;

void setup()
{
   Serial.begin(57600);
    
   SPI.begin(); 
   pinMode (SOMI,INPUT);
   pinMode (SPISTE,OUTPUT);
   pinMode (SCLK, OUTPUT);
   pinMode (SIMO, OUTPUT);
   pinMode (SPIDRDY,INPUT);
   
   pinMode(2, INPUT);
   pinMode(3, OUTPUT);

   attachInterrupt(0, blink, RISING );
   
   SPI.setClockDivider (SPI_CLOCK_DIV8);
   SPI.setDataMode (SPI_MODE0);
   SPI.setBitOrder (MSBFIRST);
   
//-------------------------------------------
  DataPacketHeader[0] = CES_CMDIF_PKT_START_1;
  DataPacketHeader[1] = CES_CMDIF_PKT_START_2;
  DataPacketHeader[2] = (datalen);
  DataPacketHeader[3] = (datalen >> 8);
  DataPacketHeader[4] = CES_CMDIF_TYPE_DATA;
  
  DataPacketFooter[0] = 0x00;
  DataPacketFooter[1] = CES_CMDIF_PKT_STOP;
//------------------------------------------------- 
   
   AFE4490Init (); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{    
     if (state == HIGH)
     {
                        //noInterrupts();
        detachInterrupt(0);
        digitalWrite(3, 1);        //test pin 3
        digitalWrite(3, 0); 
        AFE4490Write(CONTROL0,0x000001);  
        IRtemp = AFE4490Read(LED1VAL);
        AFE4490Write(CONTROL0,0x000001);  
        REDtemp = AFE4490Read(LED2VAL);   
     //   REDtemp = fir.process(REDtemp);   // Filter 
     //   IRtemp = fir.process( IRtemp);    // 
        Responsebyte = true;

     }  
                    
  if(Responsebyte == true)
  {
    { 
      buff = 0;
      Pkt_Counter++; 
                      
        
      IRtemp = (unsigned long) (IRtemp<<10);
      seegtemp = (signed long) (IRtemp);
      seegtemp = (signed long) (seegtemp>>10);  
//     seegtemp = seegtemp*100;
                        
      REDtemp = (unsigned long) (REDtemp<<10);
      seegtemp2 = (signed long) (REDtemp);
      seegtemp2 = (signed long) (seegtemp2>>10);

    DataPacketHeader[0] = 0x0A;
    DataPacketHeader[1] = 0xFA;
    DataPacketHeader[2] = 0x08;
    DataPacketHeader[3] = 0;
    DataPacketHeader[4] = 0x02;
    

    DataPacketHeader[5] = seegtemp;
    DataPacketHeader[6] = seegtemp>>8;
    DataPacketHeader[7] = seegtemp>>16;
    DataPacketHeader[8] = seegtemp>>24; 
 
    DataPacketHeader[9] = seegtemp2;
    DataPacketHeader[10] = seegtemp2>>8;
    DataPacketHeader[11] = seegtemp2>>16;
    DataPacketHeader[12] = seegtemp2>>24; 

    DataPacketHeader[13] = 0x00;
    DataPacketHeader[14] = 0x0b;

    for(i=0; i<15; i++) // transmit the data
    {
        Serial.write(DataPacketHeader[i]);

     }                 Responsebyte = false;
                
                 state = LOW;
                       attachInterrupt(0, blink, RISING );      
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
  //  Serial.println("AFE4490 Initialization Starts"); 
     AFE4490Write(CONTROL0,0x000000);

     AFE4490Write(CONTROL0,0x000008);  

     AFE4490Write(TIAGAIN,0x000000); // CF = 5pF, RF = 500kR
     AFE4490Write(TIA_AMB_GAIN,0x000001); 
     
     AFE4490Write(LEDCNTRL,0x001414);    
     AFE4490Write(CONTROL2,0x000000); // LED_RANGE=100mA, LED=50mA 
     AFE4490Write(CONTROL1,0x010707); // Timers ON, average 3 samples  
    
     AFE4490Write(PRPCOUNT, 0X001F3F);

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
   // Serial.println("AFE4490 Initialization Done"); 
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
unsigned long AFE4490Read (uint8_t address)
{       
    unsigned long data=0;
    digitalWrite (SPISTE, LOW); // enable device
    SPI.transfer (address); // send address to device
    //SPI.transfer (data);
    data |= (SPI.transfer (0)<<16); // read top 8 bits data
    data |= (SPI.transfer (0)<<8); // read middle 8 bits  data
    data |= SPI.transfer (0); // read bottom 8 bits data
    digitalWrite (SPISTE, HIGH); // disable device
    

    return data; // return with 24 bits of read data
}

