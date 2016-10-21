//////////////////////////////////////////////////////////////////////////////////////////
//
//   GUI for controlling the AFE4400 Oximeter Shield
//
//   Created: Balasundari, Aug 2016
//
//   Requires g4p_control graphing library for processing.  Built on V4.1
//   Downloaded from Processing IDE Sketch->Import Library->Add Library->G4P Install
//
/////////////////////////////////////////////////////////////////////////////////////////

import g4p_controls.*;                       // Processing GUI Library to create buttons, dropdown,etc.,
import processing.serial.*;                  // Serial Library

// Java Swing Package For prompting message
import java.awt.*;
import static javax.swing.JOptionPane.*;

// File Packages to record the data into a text file
import javax.swing.JFileChooser;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.BufferedReader;

// Date Format
import java.util.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

// General Java Package
import java.math.*;

/************** Packet Validation  **********************/
private static final int CESState_Init = 0;
private static final int CESState_SOF1_Found = 1;
private static final int CESState_SOF2_Found = 2;
private static final int CESState_PktLen_Found = 3;

/*CES CMD IF Packet Format*/
private static final int CES_CMDIF_PKT_START_1 = 0x0A;
private static final int CES_CMDIF_PKT_START_2 = 0xFA;
private static final int CES_CMDIF_PKT_STOP = 0x0B;

/*CES CMD IF Packet Indices*/
private static final int CES_CMDIF_IND_LEN = 2;
private static final int CES_CMDIF_IND_LEN_MSB = 3;
private static final int CES_CMDIF_IND_PKTTYPE = 4;
private static int CES_CMDIF_PKT_OVERHEAD = 5;

/************** Packet Related Variables **********************/

int ecs_rx_state = 0;                                        // To check the state of the packet
int CES_Pkt_Len;                                             // To store the Packet Length Deatils
int CES_Pkt_Pos_Counter, CES_Pkt_Data_Counter1, CES_Pkt_Data_Counter2;
int CES_Pkt_PktType;                                         // To store the Packet Type
char DataRcvPacket1[] = new char[10];                        // Receive the 1st channel raw data from the packet.
char DataRcvPacket2[] = new char[10];                        // Recieve the 2nd channel raw data from the packet.
int pSize = 750;                                             // Total Size of the buffer
float time =0;                                               // X axis increment variable
int arrayIndex = 0;                                          // Increment Variable for the buffer
float[] xdata = new float[pSize];                            // Buffer for time (x axis)
float[] ydata = new float[pSize];                            // Buffer for IR (y axis)
float[] AvgYdata = new float[pSize];                         // Buffer for average of IR (y axis)
float[] zdata = new float[pSize];                            // Buffer for RED (y axis)
float[] AvgZdata = new float[pSize];                         // Buffer for average of RED (y axis)

/************** User Defined Class Object **********************/

HelpWidget helpWidget;
HeaderButton headerButton;
MessageBox msgBox;
Graph g, g1;
SPO2_cal s;

/************** Graph Related Variables **********************/

double maxAxis_ir, minAxis_ir, maxAxis_red, minAxis_red;         // To Calculate the Minimum and Maximum of the Buffer
double receivedVoltage_RED, receivedVoltage_IR;                  // To store the current RED and IR data
boolean startPlot = false;                             // Conditional Variable to start and stop the plot

/************** File Related Variables **********************/

boolean logging = false;                                // Variable to check whether to record the data or not
FileWriter output;                                      // In-built writer class object to write the data to file
JFileChooser jFileChooser;                              // Helps to choose particular folder to save the file
Date date, MPDate;                                      // Variables to record the date related values                              
BufferedWriter bufferedWriter;
DateFormat dateFormat;

/************** Port Related Variables **********************/

Serial port = null;                                     // Oject for communicating via serial port
String[] comList;                                       // Buffer that holds the serial ports that are paired to the laptop
boolean serialSet, portSelected;                        // Conditional Variable for connecting to the selected serial port
char inString = '\0';                                   // To receive the bytes from the packet
String selectedPort;                                    // Holds the selected port number

/************** Logo Related Variables **********************/

PImage logo;                                           // Variable to store the location of the image

// Variables to store the Minimum, Maximum, Average and RMS values
// with 2 decimal precision using BigDecimal

BigDecimal min, max;
BigDecimal avg, rms, a;

// Variables to store the digital values for calculation

double additionFactor_red, additionFactor_ir;
float value1, value2;
float RedAC = 0, RedDC = 0, IrAC = 0, IrDC = 0;

/*********************************************** Set Up Function *********************************************************/

////////////////////////////////////////////////////////////////////////////////
//
//  This Function is executed only once
//  The Objects for classes are initialized here
//
///////////////////////////////////////////////////////////////////////////////

/*************************************************************************************************************************/

public void setup() {
  size(1000, 700, JAVA2D);

  /* G4P created Methods */

  createGUI();
  customGUI();

  /* Object initialization for User-defined classes */

  headerButton = new HeaderButton(0, 0, width, 60);
  helpWidget = new HelpWidget(0, height - 30, width, 40); 
  msgBox = new MessageBox();
  s = new SPO2_cal();
  g = new Graph(100, 100, width-120, 200);
  g1 = new Graph(100, 350, width-120, 200);
  g.GraphColor = color(0, 255, 0);
  g.Title = "RED";
  g1.GraphColor = color( 0, 255, 0);
  g1.Title = "IR";

  setChartSettings();                                    // graph function to set minimum and maximum for axis

  /*******  Initializing zero for buffer ****************/

  for (int i=0; i<pSize; i++) 
  {
    time = time + 0.1;
    xdata[i]=time;
    ydata[i] = 0;
    zdata[i] = 0;
  }
  time = 0;

  date = new Date();                              // Current Date and Time
  logo = loadImage("logo.png");                   // Location of the Image
}

/*********************************************** Draw Function *********************************************************/

////////////////////////////////////////////////////////////////////////////////
//
//  This Function is executed repeatedly according the Frame Refresh Rate  
//
///////////////////////////////////////////////////////////////////////////////

/***********************************************************************************************************************/

public void draw() {
  background(0);

  // When Port is selected from the dropdown, 
  //    then this function is called to connect with the selected port.

  while (portSelected == true && serialSet == false)
  {
    startSerial(comList);
  }

  // When the port is connected and Start button is pressed, then this function is called
  // Method call to trace the graph

  if (startPlot)
  {
    g.LineGraph(xdata, ydata);
    g1.LineGraph(xdata, zdata);
  }

  // Helps to Draw the Axes Lines

  g.DrawAxis();
  g1.DrawAxis();

  /****** User Defined Class Call ********/

  msgBox.MessageBoxAxis(0, height - 100, width, 70);
  msgBox.draw();
  headerButton.draw();
  helpWidget.draw();
}

/*********** Method to connect with the selected or inputted port number  *************************/

void startSerial(String[] theport)
{
  try
  {
    port = new Serial(this, selectedPort, 57600);          // Used to connect with the port selected from the drop down
    port.clear();
    serialSet = true;
  }
  catch(Exception e)
  {
    showMessageDialog(null, "Port is busy", "Alert", ERROR_MESSAGE);
    System.exit (0);
  }
}

/*********************************************** Serial Port Event Function *********************************************************/

///////////////////////////////////////////////////////////////////
//
//  Event Handler To Read the packets received from the Device
//
//////////////////////////////////////////////////////////////////

void serialEvent (Serial blePort) 
{
  inString = blePort.readChar();
  ecsProcessData(inString);
}

/*********************************************** Getting Packet Data Function *********************************************************/

///////////////////////////////////////////////////////////////////////////
//  
//  The Logic for the below function :
//      //  The Packet recieved is separated into header, footer and the data
//      //  If Packet is not received fully, then that packet is dropped
//      //  The data separated from the packet is assigned to the buffer
//      //  If Record option is true, then the values are stored in the text file
//
//////////////////////////////////////////////////////////////////////////

void ecsProcessData(char rxch)
{
  switch(ecs_rx_state)
  {
  case CESState_Init:
    if (rxch==CES_CMDIF_PKT_START_1)
      ecs_rx_state=CESState_SOF1_Found;
    break;

  case CESState_SOF1_Found:
    if (rxch==CES_CMDIF_PKT_START_2)
      ecs_rx_state=CESState_SOF2_Found;
    else
      ecs_rx_state=CESState_Init;                    //Invalid Packet, reset state to init
    break;

  case CESState_SOF2_Found:
    ecs_rx_state = CESState_PktLen_Found;
    CES_Pkt_Len = (int) rxch;
    CES_Pkt_Pos_Counter = CES_CMDIF_IND_LEN;
    CES_Pkt_Data_Counter1 = 0;
    CES_Pkt_Data_Counter2 = 0;
    break;

  case CESState_PktLen_Found:
    CES_Pkt_Pos_Counter++;
    if (CES_Pkt_Pos_Counter < CES_CMDIF_PKT_OVERHEAD)                        //Read Header
    {
      if (CES_Pkt_Pos_Counter==CES_CMDIF_IND_LEN_MSB)
        CES_Pkt_Len = (int) ((rxch<<8)|CES_Pkt_Len);
      else if (CES_Pkt_Pos_Counter==CES_CMDIF_IND_PKTTYPE)
        CES_Pkt_PktType = (int) rxch;
    } else if ( (CES_Pkt_Pos_Counter >= CES_CMDIF_PKT_OVERHEAD) && (CES_Pkt_Pos_Counter < CES_CMDIF_PKT_OVERHEAD+CES_Pkt_Len+1) )  //Read Data
    {
      if (CES_Pkt_PktType == 2)
      {
        if (CES_Pkt_Data_Counter1 < 4)                           
        {
          DataRcvPacket1[CES_Pkt_Data_Counter1]= (char) (rxch);             // Buffer to receive Digital Value for RED Value of the Spo2
          CES_Pkt_Data_Counter1++;
        } else
        {
          DataRcvPacket2[CES_Pkt_Data_Counter2]= (char) (rxch);             // Buffer to receive Digital Value for IR Value of the Spo2
          CES_Pkt_Data_Counter2++;
        }
      }
    } else                                                                  //All header and data received
    {
      if (rxch==CES_CMDIF_PKT_STOP)
      {  
        // Converting the ADC read-out value to get PPG Wave

        int data1 = ecsParsePacket(DataRcvPacket1, DataRcvPacket1.length-1);
        int data2 = ecsParsePacket(DataRcvPacket2, DataRcvPacket2.length-1);
        receivedVoltage_RED = data1*0.0000057220458984375 ;
        receivedVoltage_IR = data2*0.0000057220458984375 ;

        // Assigning the PPG values to the respective buffer
        // Calculating the Average value of the buffer and storing it.

        time = time+0.1;
        xdata[arrayIndex] = time;
        AvgYdata[arrayIndex] = (float)receivedVoltage_RED;
        AvgZdata[arrayIndex] = (float)receivedVoltage_IR;
        value1 = (float)( AvgYdata[arrayIndex] - averageValue(AvgYdata));
        value2 = (float)( AvgZdata[arrayIndex] - averageValue(AvgZdata));
        ydata[arrayIndex] = value1;
        zdata[arrayIndex] = value2;
        arrayIndex++;
        float RedDC = (float) averageValue(AvgYdata);
        float IrDC = (float) averageValue(AvgZdata);

        // When the buffer memory is used fully, then we have to replace the data with the existing data
        // SpO2 is calculated by sending the buffer to SPO2_cal class.

        if (arrayIndex == pSize)
        {  
          arrayIndex = 0;
          time = 0;
          RedAC = s.SPO2_Value(ydata);
          IrAC = s.SPO2_Value(zdata);
          float value = (RedAC/abs(RedDC))/(IrAC/abs(IrDC));

          ///////////////////////////////////////////////////////////////////////////
          //  
          //  To Calulate the Spo2 value any one of the following Emprical Formal are used:
          //    1. float SpO2 = 10.0002*(value)-52.887*(value) + 26.817*(value) + 98.293;
          //    2. float SpO2 =((0.81-0.18*(value))/(0.73+0.11*(value)));
          //    3. float SpO2=110-25*(value);
          //  In this Program, the 3rd formulae is used
          //
          //////////////////////////////////////////////////////////////////////////

          float SpO2=110-25*(value);
          SpO2 = (int)(SpO2 * 100);
          SpO2 = SpO2/100;
          oxygenSaturation.setText(SpO2+"");
        }

        // Minimum, Maximum, Average & RMS values are used
        // These values are display in the message box class
        // The values are precisioned upto 2 decimal points using BigDecimal class

        a = new BigDecimal(averageValue(ydata));
        avg = a.setScale(5, BigDecimal.ROUND_HALF_EVEN); 
        a = new BigDecimal(RMSValue(ydata));
        rms = a.setScale(5, BigDecimal.ROUND_HALF_EVEN); 
        a = new BigDecimal(max(ydata));
        max = a.setScale(5, BigDecimal.ROUND_HALF_EVEN); 
        a = new BigDecimal(min(ydata));
        min = a.setScale(5, BigDecimal.ROUND_HALF_EVEN); 
        msgBox.msg(min, max, avg, rms);

        // Logging the data in a text file

        if (logging == true)
        {
          try {
            date = new Date();
            dateFormat = new SimpleDateFormat("HH:mm:ss");
            output = new FileWriter(jFileChooser.getSelectedFile(), true);
            bufferedWriter = new BufferedWriter(output);
            bufferedWriter.write(arrayIndex-1+" , "+value1 +" , "+value2);
            bufferedWriter.newLine();
            bufferedWriter.flush();
            bufferedWriter.close();
          }
          catch(IOException e) {
            println("It broke!!!");
            e.printStackTrace();
          }
        }

        // Calculating the minimum and maximum from the buffer and auto scaling the graph

        maxAxis_red = max(ydata);
        minAxis_red = min(ydata);
        maxAxis_ir = max(zdata);
        minAxis_ir = min(zdata);

        if (g.yMax != maxAxis_red)
          g.yMax = (float)(maxAxis_red);
        if (g.yMin != minAxis_red)
          g.yMin = (float)(minAxis_red);

        if (g1.yMax != maxAxis_ir)
          g1.yMax = (float)(maxAxis_ir);
        if (g1.yMin != minAxis_ir)
          g1.yMin = (float)(minAxis_ir);

        ecs_rx_state=CESState_Init;                                  // Setting to initial State
      } else                                                         // Packet end not found, drop packet
      {
        ecs_rx_state=CESState_Init;
      }
    }
    break;

  default:
    //Invalid state
    break;
  }
}

/*********************************************** Recursion Function To Reverse The data *********************************************************/

public int ecsParsePacket(char DataRcvPacket[], int n)
{
  if (n == 0)
    return (int) DataRcvPacket[n]<<(n*8);
  else
    return (DataRcvPacket[n]<<(n*8))| ecsParsePacket(DataRcvPacket, n-1);
}

/********************************************* User-defined Method for G4P Controls  **********************************************************/

///////////////////////////////////////////////////////////////////////////////
//
//  Customization of controls is done here
//  That includes : Font Size, Visibility, Enable/Disable, ColorScheme, etc.,
//
//////////////////////////////////////////////////////////////////////////////

public void customGUI() {
  comList = port.list();
  String comList1[] = new String[comList.length+1];
  comList1[0] = "SELECT THE PORT";
  for (int i = 1; i <= comList.length; i++)
  {
    comList1[i] = comList[i-1];
  }
  start.setEnabled(false);
  oxygenSaturation.setVisible(false);
  comList = comList1;
  portList.setItems(comList1, 0);

  oxygenSaturation.setFont(new Font("Arial", Font.PLAIN, 55));
  oxygenSaturation.setLocalColor(2, color(255, 255, 255));

  start.setLocalColorScheme(GCScheme.CYAN_SCHEME);
}

/**************** Setting the Limits for the graph **************/

void setChartSettings() {
  g.xDiv=10;  
  g.xMax=pSize; 
  g.xMin=0;  
  g.yMax=0.001; 
  g.yMin=0.005;

  g1.xDiv=10;  
  g1.xMax=pSize; 
  g1.xMin=0;  
  g1.yMax=0.001; 
  g1.yMin=0.005;
}

/*************** Function to Calculate Average & RMS *********************/

double averageValue(float dataArray[])
{

  float total = 0;
  for (int i=0; i<dataArray.length; i++)
  {
    total = total + dataArray[i];
  }

  return total/dataArray.length;
}

double RMSValue(float dataArray[])
{
  float total = 0;
  for (int i=0; i<dataArray.length; i++)
  {
    total = (float)(total + Math.pow(dataArray[i], 2));
  }
  total /= dataArray.length;
  return Math.sqrt(total);
}