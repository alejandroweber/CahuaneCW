void ditdah(int largo){
  switch(largo){
    case 1: //Dit
      tone(SideTone,abs(offset));
      delay(50);
      noTone(SideTone);
      delay(50);
      break;
    case 2: //Dah
      tone(SideTone,abs(offset));
      delay(150);
      noTone(SideTone);
      delay(50);
      break;
    case 3: //Espera
      delay(150);
      break;
  }
}

