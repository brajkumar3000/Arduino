

//  AD9850 Variables

#define  CLOCK A2            // Pin connections for AD9850 Module.
#define  LOAD  A3
#define  DATA  A5            // Note that A6 and A7 are ANALOG ONLY.
#define  RESET A4
#define  DDS_CLOCK 125000000  // Timebase of oscillator on AD9850 module.
#define  pulsedelay 1   // Used for DDS module serial timing.

unsigned long init_f=1000; // Initial Set Frequency


void setup()
{
  
  pinMode (DATA,  OUTPUT); // Initialize control pins to AD9850 module as output.
  pinMode (CLOCK, OUTPUT); 
  pinMode (LOAD,  OUTPUT); 
  pinMode (RESET, OUTPUT); 
  
  AD9850_init();           // Initialize the AD9850 module.
  AD9850_reset();          // Reset the module.
  SetFrequency(init_f);   // Output the initial frequency.
  Serial.begin(115200);
  Serial.print("Frequency initialized to: ");
  Serial.println(init_f); 
}


void loop()
{
  String s;
  float freq;
  
  if(Serial.available()>0) /* If data available at serial port, enter if loop */
  {
    s = Serial.readString(); 
    freq=s.toFloat();
    SetFrequency(freq);
    Serial.print("Frequency set to: "); 
    Serial.println(s); /* Print data received */
  }  
  
}

//  SetFrequency
//  Serially sends the "frequency" to the DDS module.
void SetFrequency(unsigned long frequency)
{
  unsigned long tuning_word = (frequency * pow(2, 32)) / DDS_CLOCK;
  digitalWrite (LOAD, LOW); 

  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 8);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 16);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 24);
  shiftOut(DATA, CLOCK, LSBFIRST, 0x0);
  digitalWrite (LOAD, HIGH); 
}


void AD9850_init()
{
  digitalWrite(RESET, LOW);  // Initialize DDS module serial pins.
  digitalWrite(CLOCK, LOW);
  digitalWrite(LOAD, LOW);
  digitalWrite(DATA, LOW);
}


void AD9850_reset()
{
  //  Reset the DDS Module.
  //  Reset sequence is:
  //    CLOCK & LOAD = LOW
  //    Pulse RESET high for a few uS (use 5 uS here)
  //    Pulse CLOCK high for a few uS (use 5 uS here)
  //    Set DATA to ZERO and pulse LOAD for a few uS (use 5 uS here)

  // The data sheet diagrams show only RESET and CLOCK being used to reset the device,
  // but I see no output unless I also toggle the LOAD line here.

  digitalWrite(CLOCK, LOW);
  digitalWrite(LOAD, LOW);

  digitalWrite(RESET, LOW);
  delay(pulsedelay);
  digitalWrite(RESET, HIGH);  //pulse RESET
  delay(pulsedelay);
  digitalWrite(RESET, LOW);
  delay(pulsedelay);

  digitalWrite(CLOCK, LOW);
  delay(pulsedelay);
  digitalWrite(CLOCK, HIGH);  //pulse CLOCK
  delay(pulsedelay);
  digitalWrite(CLOCK, LOW);
  delay(pulsedelay);
  digitalWrite(DATA, LOW);    //make sure DATA pin is LOW

  digitalWrite(LOAD, LOW);
  delay(pulsedelay);
  digitalWrite(LOAD, HIGH);  //pulse LOAD
  delay(pulsedelay);
  digitalWrite(LOAD, LOW);
  // Chip is RESET now
}
