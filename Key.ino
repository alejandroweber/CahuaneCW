void keyfunc(boolean trx){
  if( trx==true && key_flag == false) {
    //Actualiza la frecuencia
    if(flag_15m == true) DDS_actualiza(tx15m);
    if(flag_20m == true) DDS_actualiza(tx20m);
    if(flag_40m == true) DDS_actualiza(tx40m);
    
    digitalWrite(Relay,HIGH);//Aca se pega el rele  
    key_flag = true;
  }
  else if(trx==false && key_flag == true && millis() - last_key >= DELAY_TRX){


    //Actualiza la frecuencia
    if(flag_15m == true) DDS_actualiza(rx15m + offset);
    if(flag_20m == true) DDS_actualiza(rx20m + offset);
    if(flag_40m == true) DDS_actualiza(rx40m + offset);
    
    digitalWrite(Relay,LOW); //Aca va se libera el rele       
    key_flag = false;
  }
}

