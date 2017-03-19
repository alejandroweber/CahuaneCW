void RIT_Button(void){
  byte tempo_pres = 0;
  while (digitalRead(BotonRIT) == LOW){
      tempo_pres++;
      delay(100);
            
      if(tempo_pres == 10){  //Toggle control          
          lcd.setCursor(10,0);
          
          if(offset_flag == true){
            offset_flag = false;
            offset = offset - (2*offset);
            ditdah(2);
            lcd.print("-");
          }
          else {
            offset_flag = true;
            offset = offset + (-2*offset);
            ditdah(2);
            lcd.print("+");
          }
          DDS_actualiza(freq + offset);
          delay(500); 
          break;
          }
  }
  if(tempo_pres < 10) RIT_chg();
}

void RIT_chg(void) {

delay(200);

//toogle flag
if(rit_flag == true) {
  rit_flag=false;
  ditdah(1); // Solo un punto
  //Actualizo DDS
  if(tx15m != rx15m) DDS_actualiza(tx15m + offset);
  if(tx20m != rx20m) DDS_actualiza(tx20m + offset);
  if(tx40m != rx40m) DDS_actualiza(tx40m + offset);
  
  //Devuelvo los valores guardados
  rx15m = tx15m;
  rx20m = tx20m;
  rx40m = tx40m;
  step_size = step_rit;
  
  }
else {
  rit_flag = true;
  
  ditdah(1); //Envio una R
  ditdah(2);
  ditdah(1);
  
  step_rit = step_size; //Guardo el viejo step  
  step_size = 10; //Lo cambio por uno para RIT
  }
delay (200);
}

