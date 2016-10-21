//////////////////////////////////////////////////////////////////////////////////////////
//
//   SPO2_cal:
//      - This Class calculates the SpO2 value According to the PPG data received
//
//   Created : Balasundari, Aug 2016
//   
/////////////////////////////////////////////////////////////////////////////////////////

public class SPO2_cal
{
  float Vac = 0;                                  //  Sum of Voltage values stored
  float spo2_cal_array[] = new float[pSize];      //  To store the ppg values in buffer
  float SPO2 = 0;                                 //  Final spo2 value is stored

  ////////////////////////////////////////////////////////////////////////////////////////////
  //  SPo2 Value is calculated by:
  //    * Calculate the square of the spo2 values and store it in the buffer
  //    * Sum of the values in the squared buffer is calculated.
  //    * This sum is sent to the main function
  //   
  ////////////////////////////////////////////////////////////////////////////////////////////

  public float SPO2_Value(float spo2_array[])
  {
    SPO2 = 0;
    int k = 0;
    for (int i = 50; i < spo2_array.length; i++)
    {
      spo2_cal_array[k++] = spo2_array[i] * spo2_array[i];
    }
    SPO2 = sum(spo2_cal_array, k);
    return (SPO2);
  }


  public float sum(float array[], int len)
  {
    float spo2 = 0;
    for (int p = 0; p < len; p++)
    {
      spo2 = spo2 + array[p];
    }
    Vac = (float)Math.sqrt(spo2/len);
    return Vac;
  }
}