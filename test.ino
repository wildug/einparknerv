int trig = 7; 
int echo = 8; 
int sound = 13;
int beepPeriod = 2;
long lecture_echo;
long beepTime = 500; //length of beep
long cm;
long Dist;
long tolb; //time of last beep
long tslb; //time since last beep
const int smoothCount = 16;
long smoothArr[smoothCount];
const int halfMedCount = 2;
long medianArr[halfMedCount*2];
long medianArrSorted[halfMedCount*2];

void setup() {
  // put your setup code here, to run once:
  pinMode(trig, OUTPUT); 
  digitalWrite(trig, LOW); 
  pinMode(echo, INPUT); 
  Serial.begin(9600);
  tolb = 0;
  tslb = 0;
  for (int i = 0; i<=(smoothCount-1); i++){
    smoothArr[i]=0;
  }
}
int calcdist()
{
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  lecture_echo = pulseIn(echo, HIGH); 
  cm = lecture_echo / 58; 
  return cm; 
}
void beep(){
  tslb = millis() - tolb;
  if(tslb > beepTime){  //um nicht unnötig zweimal tone() zu callen, keine Ahnung ob das eine unterbrechung im ton hervorruft
    tone(sound, 1000);
  }
  tolb = millis();
  tslb = 0;
}
long smoothFilter(long newVal){
  long newArr[smoothCount];
  long sum = 0;
  for (int i = 0; i<=(smoothCount-2); i++){
    newArr[i] = smoothArr[i+1];
    smoothArr[i] = smoothArr[i+1];
    sum+=newArr[i];
  }
  newArr[smoothCount-1] = newVal;
  smoothArr[smoothCount-1] = newVal;
  sum+=newVal;  
  return(sum/smoothCount);
}
long medianFilter(long newVal){
  for (int i = 0; i<=(halfMedCount*2)-2; i++){ //Array Shift
    medianArr[i]=medianArr[i+1];
  }
  medianArr[(halfMedCount*2)-1] = newVal;  //rechts neuen Value insertieren
  arrSort(medianArr);
  return (medianArrSorted[halfMedCount] + medianArrSorted[halfMedCount+1])/2; //Median returnen
}
void arrSort(long inArr[halfMedCount*2]){ //TODO
  // das sortierte Array in medianArrSorted[] schreiben
}
void loop() {
  Dist = calcdist()*beepPeriod * 10;
  Serial.print("Abstand-Roh: ");
  Serial.println(Dist);
  Dist = smoothFilter(Dist);
  if(Dist<=tslb){
    beep();
  } else{
    tslb = millis() - tolb;
  }  
  if(tslb >= beepTime){
    noTone(sound);
  }
  // put your main code here, to run repeatedly:
  Serial.print("Abstand Filter: ");
  Serial.println(Dist);
  Serial.print("Zeit:");
  Serial.println(tslb);
}