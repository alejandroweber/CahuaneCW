void VFO_Button(void){
  byte tempo_pres = 0;
  while (digitalRead(BotonVFO) == LOW){
      tempo_pres++;
      delay(100);
      
      if(tempo_pres >= 10){
          ditdah(1);
          ditdah(1);          
          lcd.setCursor(0,0);
          lcd.print("                ");
          config_flag = true;
          delay(300); 
          break;
          }
  }
  if(tempo_pres < 10 && config_flag == false) Step_chg();
  if(tempo_pres < 10 && config_flag == true) config_flag = 0;
}

void Step_chg(void) {
//Serial.println("Boton Step Presionado");

delay(500);
while (digitalRead(BotonVFO) == HIGH) {
   
   if(cursor_pos == 5 && encoderPos == 1) cursor_pos = 7; //Salto el "."
   else if(cursor_pos == 7 && encoderPos == -1) cursor_pos = 5; //Salto el "."
   else cursor_pos = cursor_pos + encoderPos;
   
   encoderPos=0; //reset para que no acumule
   if(cursor_pos >= 8) cursor_pos = 8;
   if(cursor_pos <= 4) cursor_pos = 4;
   
   lcd.setCursor(cursor_pos,0);
   lcd.cursor();
}

switch(cursor_pos){
      case 4:
              step_size = 10000; //10KHz
              break;
      case 5:
              step_size = 1000; //1KHz
              break;
      case 7:
              step_size = 100; //100Hz
              break;
      case 8:
              step_size = 10; //10Hz
              break;
}

Serial.print("Step Seleccionado: ");
Serial.println(step_size);
lcd.noCursor();
delay(200);
}

