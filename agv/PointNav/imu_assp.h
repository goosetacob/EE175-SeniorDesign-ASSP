
float printHeading(float hx, float hy)
{
  float heading;
//  Serial.print("hy :");
//  Serial.print(hy);
//  Serial.print("hx :");
//  Serial.print(hx);
  if (hy > 0)
  {
    heading = 90 - (atan(hx / hy) * (180 / PI));
//    Serial.print("      hy > 0  ");
//    Serial.print("hy: ");
//    Serial.print(hy);
//    Serial.print(" hx: ");
//    Serial.println(hx);
  }
  else if (hy < 0)
  {
    heading = - (atan(hx / hy) * (180 / PI)) - 90 + 180 + 180;
//    Serial.print("      hy < 0  ");
//    Serial.print("hy: ");
//    Serial.print(hy);
//    Serial.print(" hx: ");
//    Serial.println(hx);
  }
  else // hy = 0
  {
    if (hx < 0) heading = 180;
    else heading = 0;
    Serial.print("hy: ");
    Serial.print(hy);
    Serial.print(" hx: ");
    Serial.println(hx);
  }
  return heading;
}

void printOrientation(float x, float y, float z)
{
  float pitch, roll;

  pitch = atan2(x, sqrt(y * y) + (z * z));
  roll = atan2(y, sqrt(x * x) + (z * z));
  pitch *= 180.0 / PI;
  roll *= 180.0 / PI;
//
//  Serial.print("Pitch, Roll: ");
//  Serial.print(pitch, 2);
//  Serial.print(", ");
//  Serial.println(roll, 2);
}

void printMag()
{
  // To read from the magnetometer, you must first call the
  // readMag() function. When this exits, it'll update the
  // mx, my, and mz variables with the most current data.
  dof.readMag();

  // Now we can use the mx, my, and mz variables as we please.
  // Either print them as raw ADC values, or calculated in Gauss.
//  Serial.print("M: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcMag helper function to convert a raw ADC value to
  // Gauss. Give the function the value that you want to convert.
//  Serial.print(dof.calcMag(dof.mx), 2);
//  Serial.print(", ");
//  Serial.print(dof.calcMag(dof.my), 2);
//  Serial.print(", ");
//  Serial.println(dof.calcMag(dof.mz), 2);
#elif defined PRINT_RAW
//  Serial.print(dof.mx);
//  Serial.print(", ");
//  Serial.print(dof.my);
//  Serial.print(", ");
//  Serial.println(dof.mz);
#endif
}

void printAccel()
{
  // To read from the accelerometer, you must first call the
  // readAccel() function. When this exits, it'll update the
  // ax, ay, and az variables with the most current data.
  dof.readAccel();

  // Now we can use the ax, ay, and az variables as we please.
  // Either print them as raw ADC values, or calculated in g's.
//  Serial.print("A: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcAccel helper function to convert a raw ADC value to
  // g's. Give the function the value that you want to convert.
//  Serial.print(dof.calcAccel(dof.ax), 2);
//  Serial.print(", ");
//  Serial.print(dof.calcAccel(dof.ay), 2);
//  Serial.print(", ");
//  Serial.println(dof.calcAccel(dof.az), 2);
#elif defined PRINT_RAW
//  Serial.print(dof.ax);
//  Serial.print(", ");
//  Serial.print(dof.ay);
//  Serial.print(", ");
//  Serial.println(dof.az);
#endif

}

void printGyro()
{
  // To read from the gyroscope, you must first call the
  // readGyro() function. When this exits, it'll update the
  // gx, gy, and gz variables with the most current data.
  dof.readGyro();

  // Now we can use the gx, gy, and gz variables as we please.
  // Either print them as raw ADC values, or calculated in DPS.
//  Serial.print("G: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcGyro helper function to convert a raw ADC value to
  // DPS. Give the function the value that you want to convert.
//  Serial.print(dof.calcGyro(dof.gx), 2);
//  Serial.print(", ");
//  Serial.print(dof.calcGyro(dof.gy), 2);
//  Serial.print(", ");
//  Serial.println(dof.calcGyro(dof.gz), 2);
#elif defined PRINT_RAW
//  Serial.print(dof.gx);
//  Serial.print(", ");
//  Serial.print(dof.gy);
//  Serial.print(", ");
//  Serial.println(dof.gz);
#endif
}


float get_heading(){
  
  printGyro();  // Print "G: gx, gy, gz"
  printAccel(); // Print "A: ax, ay, az"
  printMag();   // Print "M: mx, my, mz"
  
  
  printOrientation(dof.calcAccel(dof.ax), dof.calcAccel(dof.ay), 
                   dof.calcAccel(dof.az));
  return printHeading((float) dof.mx, (float) dof.my);
}

double sample_heading(double pre_heading){
  int counter = 0;
  double heading_sum = 0;
  for(int i = 0 ; i <= 20 ; i++)
  { 
    heading = get_heading();
    
          if( heading != 0)
          { 
            if(pre_heading - heading < 30 ){
              heading_sum += heading;
              counter++;
          }
        }
   
  }
  if ( counter == 0){
    return pre_heading;
  
  }
  else{
    heading_sum = heading_sum/counter;  
    return heading_sum;
  }
}
  
  
