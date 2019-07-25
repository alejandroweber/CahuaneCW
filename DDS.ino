void DDS_actualiza(unsigned long freq_dds){
 
 if(freq_dds != oldfreq_dds){
    //int32_t freq = freq_dds * 4294967295/125000000;
    int32_t freq = freq_dds * 4294967295/125000050; //Ajustar poco!!
    
    for (int b=0; b<4; b++, freq>>=8) {
    tfr_byte(freq & 0xFF);
    }
    tfr_byte(0x000);   // Final control byte, all 0 for 9850 chip
    pulseHigh(AD9850_FQ);  // Done!  Should see output 
    
    oldfreq_dds = freq_dds;
    Serial.print("Frecuencia DDS: ");
    Serial.println(freq_dds);
  }
}

// transfers a byte, a bit at a time, LSB first to the 9850 via serial AD9850_DATA line
void tfr_byte(byte data)
{
  for (int i=0; i<8; i++, data>>=1) {
    digitalWrite(AD9850_DATA, data & 0x01);
    pulseHigh(AD9850_CLK);   //after each bit sent, CLK is pulsed high
  }
}

