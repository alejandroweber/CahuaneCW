void Banda_15m(void){
  unsigned long UpperLimit = 21450000;
  unsigned long LowerLimit = 21000000;

  if(flag_15m == false) {
    DDS_actualiza(rx15m + offset); //Actualiza DDS apenas cambio banda
  }

  //Set de flags
  flag_15m = true;
  flag_20m = false;
  flag_40m = false;
  
  //Incremento con el VFO
  freq = rx15m + (encoderPos * step_size);
  encoderPos = 0;
  
  //Verificacion de limites
  if(freq >= UpperLimit) freq = UpperLimit;
  if(freq <= LowerLimit) freq = LowerLimit;

  //Actualiza DDS en RX
  if(rx15m != freq){
    rx15m = freq;
    if(!rit_flag) tx15m = rx15m; //Mantengo TX Actualizado
    DDS_actualiza(rx15m + offset); //Envio la info a la fc DDS
    
    /*Serial.print("RX: ");
    Serial.println(rx15m + offset);
    Serial.print("TX: ");
    Serial.println(tx15m);
    Serial.println("--------------");*/
  }
}

void Banda_20m(void){
  unsigned long UpperLimit = 14350000;
  unsigned long LowerLimit = 14000000;

  if(flag_20m == false) {
    DDS_actualiza(rx20m + offset); //Actualiza DDS apenas cambio banda
  }

  //Set de flags
  flag_15m = false;
  flag_20m = true;
  flag_40m = false;
  
  //Incremento con el VFO
  freq = rx20m + (encoderPos * step_size);
  encoderPos = 0;
  
  //Verificacion de limites
  if(freq >= UpperLimit) freq = UpperLimit;
  if(freq <= LowerLimit) freq = LowerLimit;

  //Actualizacion de DDS
  if(rx20m != freq){
    rx20m = freq;
    if(!rit_flag) tx20m = rx20m; //Mantengo TX Actualizado
    DDS_actualiza(rx20m + offset); //Envio la info a la fc DDS
  }
}

void Banda_40m(void){
  unsigned long UpperLimit = 7300000;
  unsigned long LowerLimit = 7000000;
  
  if(flag_40m == false) {
    DDS_actualiza(rx40m + offset); //Actualiza DDS apenas cambio banda
  }

  //Set de flags
  flag_15m = false;
  flag_20m = false;
  flag_40m = true;
  
  //Incremento con el VFO
  freq = rx40m + (encoderPos * step_size);
  encoderPos = 0;

  //Verificacion de limites
  if(freq >= UpperLimit) freq = UpperLimit;
  if(freq <= LowerLimit) freq = LowerLimit;

  //Actualizacion de DDS
  if(rx40m != freq){
    rx40m = freq;
    if(!rit_flag) tx40m = rx40m; //Mantengo TX Actualizado
    DDS_actualiza(rx40m + offset); //Envio la info a la fc DDS
  }
}


