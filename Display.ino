void show_freq(void){
  
  if (byte(freq/1000000) > 9) lcd.setCursor(0,0);
  else {
    lcd.setCursor(0,0);
    lcd.print(" ");   
    lcd.setCursor(1,0);
  }
    
  lcd.print(byte(freq/1000000));
  lcd.print(".");
  lcd.print((freq/100000)%10);
  lcd.print((freq/10000)%10);
  lcd.print((freq/1000)%10);
  lcd.print(".");
  lcd.print((freq/100)%10);
  lcd.print((freq/10)%10);
  lcd.print((freq)%10);

  if(offset_flag) lcd.print("+"); //Significa que se suman los 750Hz
  else lcd.print("-"); 
  
  if(rit_flag==true) lcd.print("R");
  else lcd.print(" ");
  lcd.print("12.8");
}

void s_meter(void){
  unsigned int AudioAF = analogRead(Audio);

  meter_counter = meter_counter * 100;
  if(meter_counter >= 1100) meter_counter = 0;
  lcd.setCursor(meter_counter/100,1);
  if(AudioAF > meter_counter) lcd.print((char)255);
  else {
    lcd.print(" ");
  }
  meter_counter = meter_counter/100 + 1;
  
  lcd.setCursor(12,1);
  if(AudioAF >= 0 && AudioAF <=10) lcd.print("S-0");
  if(AudioAF >= 11 && AudioAF <= 200) lcd.print("S-1");
  if(AudioAF >= 201 && AudioAF <= 400) lcd.print("S-3");
  if(AudioAF >= 401 && AudioAF <= 600) lcd.print("S-5");
  if(AudioAF >= 601 && AudioAF <= 800) lcd.print("S-7");
  if(AudioAF >= 801 && AudioAF <= 1023) lcd.print("S-9");
}

void show_config(void){
  byte opcion = 1;

  //Primera impresion
  lcd.setCursor(0,0);
  lcd.print("Offset  :");
  if(offset >= 1000) lcd.setCursor(10,0);
  else lcd.setCursor(11,0);
  lcd.print(abs(offset));
  lcd.print("Hz");
  opcion = 1;
  
  while(digitalRead(BotonVFO) == HIGH){
    lcd.setCursor(0,0);    
    if(encoderPos == -1) {
      lcd.print("Offset  :");
      if(offset >= 1000) lcd.setCursor(10,0);
      else lcd.setCursor(11,0);
      lcd.print(abs(offset));
      lcd.print("Hz");
      opcion = 1;
    }
    if(encoderPos ==  1) {
      lcd.print("Potencia:");
      lcd.setCursor(11,0);
      if(offset >= 1000) {
         lcd.setCursor(10,0);
         lcd.print("  ");
      }
      lcd.print(potencia);
      lcd.print("W  ");
      opcion = 2;
    }
  } 
  
  delay(300);
  
  while (digitalRead(BotonVFO) == HIGH) {
    lcd.setCursor(10,0);
    
    //Opcion de Offset
    if(opcion == 1){
      offset = offset + (encoderPos * 10);
      encoderPos = 0; //Reset para que no acumule
      if(offset >= 1200) offset = 1200;
      if(offset <= 600) offset = 600;
      if(offset < 1000) lcd.print(" ");
      lcd.print(offset);
      lcd.print("Hz");
    }

    //Opcion para potencia
    if(opcion == 2){
      if(potencia <= 9) potencia = potencia + encoderPos;
      else potencia = potencia + (encoderPos * 10);
      encoderPos = 0; //Reset para que no acumule
      if(potencia >= 120) potencia = 120;
      if(potencia <= 1) potencia = 1;
      if(potencia < 100) lcd.print(" ");
      if(potencia < 10) lcd.print(" ");
      
      lcd.print(potencia);
      lcd.print("W  ");
    }
  }

  delay(300);
  config_flag = false;
  
}

void txmeter(bool meter_flag){
  if(meter_flag == true){
    lcd.setCursor(0,1);
    lcd.print((char)255);
    lcd.print((char)255);
    lcd.print((char)255);
    lcd.print((char)255);
    lcd.print((char)255);
    lcd.print((char)255);
    lcd.print((char)255);
    lcd.print((char)255);
    lcd.print((char)255);
    lcd.print((char)255);
    lcd.print((char)255);
  }
  else {
    lcd.setCursor(0,1);
    lcd.print("           ");
  }
  
  lcd.setCursor(12,1);
  lcd.print(" TX");
}

