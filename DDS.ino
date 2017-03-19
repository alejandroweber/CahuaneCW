void DDS_actualiza(unsigned long freq_dds){
  
 if(freq_dds != oldfreq_dds){
    oldfreq_dds = freq_dds;
    Serial.print("Frecuencia DDS: ");
    Serial.println(freq_dds);
  }
}

