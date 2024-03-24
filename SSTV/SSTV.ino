/*
 *
 * DDS Sine Generator mit ATMEGS 168
 * Timer2 generates the  31250 KHz Clock Interrupt
 *
 * KHM 2009 /  Martin Nawrath
 * Kunsthochschule fuer Medien Koeln
 * Academy of Media Arts Cologne

 */


#define RX_PIN 4
#define TX_PIN 5
#define PA_PIN 6
#define DO_PIN 11
#define CK_PIN 12
#define LE_PIN 10

#define MOD_PIN 3 // Modulator output, series 5 kOhm to (47nf and 1 kOhm in parallel to the ground)
                  // Output is connected to pin 10 of TL064C on the bottom side of the PCB

#define BTN_PIN 2

//uint8_t data[6] = {0x20, 0x44, 0x00, 0x32, 0x06, 0x31}; // RX 460,112.5 KHz
uint8_t datarx[6] = {0x20, 0x44, 0x00, 0x31, 0xec, 0x36}; // RX 439,375 KHz
//uint8_t data[6] = {0x20, 0x44, 0x00, 0x31, 0xe1, 0x08}; // RX 430,000 KHz
//uint8_t datarx[6] = {0x20, 0x44, 0x00, 0x31, 0xeb, 0x38}; // RX 438,600 KHz
//uint8_t datarx[6] = {0x20, 0x44, 0x00, 0x31, 0xf5, 0x0b}; // RX 446,037.5 KHz

//uint8_t data[6] = {0x20, 0x44, 0x00, 0x32, 0x32, 0x29}; // TX 450,112.5 KHz
//uint8_t datatx[6] = {0x20, 0x44, 0x00, 0x32, 0x1b, 0x2e}; // TX 431.775 KHz
//uint8_t data[6] = {0x20, 0x44, 0x00, 0x32, 0x19, 0x20}; // TX 430,000 KHz
//uint8_t data[6] = {0x20, 0x44, 0x00, 0x32, 0x26, 0x00}; // TX 440,000 KHz
//uint8_t datatx[6] = {0x20, 0x44, 0x00, 0x32, 0x24, 0x10}; // TX 438,600 KHz
//uint8_t datatx[6] = {0x20, 0x44, 0x00, 0x32, 0x1C, 0x24}; // TX 432.450  
uint8_t datatx[6] = {0x20, 0x44, 0x00, 0x32, 0x1C, 0x28}; // TX 432.500  
//uint8_t datatx[6] = {0x20, 0x44, 0x00, 0x32, 0x2d, 0x23}; // TX 446.037.5 KHz


// table of 256 sine values / one sine period / stored in flash memory
const unsigned char sine256[] PROGMEM = {
  127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,223,225,227,229,231,233,234,236,238,239,240,
  242,243,244,245,247,248,249,249,250,251,252,252,253,253,253,254,254,254,254,254,254,254,253,253,253,252,252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,229,227,225,223,
  221,219,217,215,212,210,208,205,203,200,198,195,192,190,187,184,181,178,176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,
  76,73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,16,18,20,21,23,25,27,29,31,
  33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124

};
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

//FONTS
const uint8_t fonts[] PROGMEM = {
        0x00, 0x18, 0x24, 0x62, 0x62, 0x62, 0x7E, 0x62, 0x62, 0x62, 0x00, //00: A
        0x00, 0x7C, 0x32, 0x32, 0x32, 0x3C, 0x32, 0x32, 0x32, 0x7C, 0x00, //01: B
        0x00, 0x3C, 0x62, 0x62, 0x60, 0x60, 0x60, 0x62, 0x62, 0x3C, 0x00, //02: C
        0x00, 0x7C, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x7C, 0x00, //03: D
        0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x7E, 0x00, //04: E
        0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x00, //05: F
        0x00, 0x3C, 0x62, 0x62, 0x60, 0x60, 0x66, 0x62, 0x62, 0x3C, 0x00, //06: G
        0x00, 0x62, 0x62, 0x62, 0x62, 0x7E, 0x62, 0x62, 0x62, 0x62, 0x00, //07: H
        0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, //08: I
        0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x4C, 0x4C, 0x4C, 0x38, 0x00, //09: J
        0x00, 0x62, 0x64, 0x68, 0x70, 0x68, 0x64, 0x62, 0x62, 0x62, 0x00, //10: K
        0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00, //11: L
        0x00, 0x42, 0x62, 0x76, 0x6A, 0x62, 0x62, 0x62, 0x62, 0x62, 0x00, //12: M
        0x00, 0x42, 0x62, 0x72, 0x6A, 0x66, 0x62, 0x62, 0x62, 0x62, 0x00, //13: N
        0x00, 0x3C, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x3C, 0x00, //14: O
        0x00, 0x7C, 0x62, 0x62, 0x62, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x00, //15: P
        0x00, 0x3C, 0x62, 0x62, 0x62, 0x62, 0x62, 0x6A, 0x6A, 0x3C, 0x08, //16: Q
        0x00, 0x7C, 0x62, 0x62, 0x62, 0x7C, 0x68, 0x64, 0x62, 0x62, 0x00, //17: R
        0x00, 0x3C, 0x62, 0x60, 0x60, 0x3C, 0x06, 0x06, 0x46, 0x3C, 0x00, //18: S
        0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, //19: T
        0x00, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x3C, 0x00, //20: U
        0x00, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x22, 0x14, 0x08, 0x00, //21: V
        0x00, 0x62, 0x62, 0x62, 0x62, 0x62, 0x6A, 0x76, 0x62, 0x42, 0x00, //22: W
        0x00, 0x42, 0x62, 0x74, 0x38, 0x1C, 0x2E, 0x46, 0x42, 0x42, 0x00, //23: X
        0x00, 0x42, 0x62, 0x74, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, //24: Y
        0x00, 0x7E, 0x06, 0x0E, 0x0C, 0x18, 0x30, 0x70, 0x60, 0x7E, 0x00, //25: Z
        0x00, 0x3C, 0x62, 0x62, 0x66, 0x6A, 0x72, 0x62, 0x62, 0x3C, 0x00, //26: 0
        0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, //27: 1
        0x00, 0x3C, 0x46, 0x06, 0x06, 0x1C, 0x20, 0x60, 0x60, 0x7E, 0x00, //28: 2
        0x00, 0x3C, 0x46, 0x06, 0x06, 0x1C, 0x06, 0x06, 0x46, 0x3C, 0x00, //29: 3
        0x00, 0x0C, 0x1C, 0x2C, 0x4C, 0x4C, 0x7E, 0x0C, 0x0C, 0x0C, 0x00, //30: 4
        0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C, 0x06, 0x06, 0x46, 0x3C, 0x00, //31: 5
        0x00, 0x3C, 0x62, 0x60, 0x60, 0x7C, 0x62, 0x62, 0x62, 0x3C, 0x00, //32: 6
        0x00, 0x7E, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, //33: 7
        0x00, 0x3C, 0x62, 0x62, 0x62, 0x3C, 0x62, 0x62, 0x62, 0x3C, 0x00, //34: 8
        0x00, 0x3C, 0x46, 0x46, 0x46, 0x3E, 0x06, 0x06, 0x46, 0x3C, 0x00, //35: 9
        0x00, 0x00, 0x02, 0x06, 0x0E, 0x1C, 0x38, 0x70, 0x60, 0x40, 0x00, //36: /
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0x00, 0x00, //37: -
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, //38: .
        0x00, 0x3C, 0x46, 0x06, 0x06, 0x0C, 0x10, 0x00, 0x30, 0x30, 0x00, //39: ?
        0x00, 0x18, 0x18, 0x18, 0x18, 0x10, 0x10, 0x00, 0x18, 0x18, 0x00, //40: !
        0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, //41: :
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //42: space
};

// Buf 80x60
uint8_t frameBuf[600]; //160*120/8/2/2

void clear_frame_buf()
{
  for (int i=0; i<600; i++)
    frameBuf[i] = 0;
}

void set_pix(uint8_t x, uint8_t y)
{
  uint16_t pos = y*80 + x;
  frameBuf[pos>>3] |= (1<<(pos&7));
}

void put_char(uint8_t sx, uint8_t sy, char ch)
{
  byte fontNumber;

  if(ch >= 65 && ch <= 90) //A to Z
  {
          fontNumber = ch - 65;
  }
  else if(ch >= 48 && ch <= 57) //0 to 9
  {
          fontNumber = ch - 22;
  }
  else if(ch == '/'){fontNumber = 36;}
  else if(ch == '-'){fontNumber = 37;}
  else if(ch == '.'){fontNumber = 38;}
  else if(ch == '?'){fontNumber = 39;}
  else if(ch == '!'){fontNumber = 40;}
  else if(ch == ':'){fontNumber = 41;}
  else if(ch == ' '){fontNumber = 42;}
  else              {fontNumber = 42;}  

  for(int y=0; y<11; y++)
  {
    //char bb = pgm_read_byte(fonts+fontNumber*11+y);
    char bb = pgm_read_byte(fonts+fontNumber*11+y);
    //if((fonts[fontNumber][y] & mask) != 0)

     for(int x=0; x<8; x++)    
     {
        uint8_t mask;
        mask = 1<<(7 - x);

        if((bb & mask) != 0)
         set_pix(sx+x, sy+y);                               
     }
  }
  
}

void put_stringP(uint8_t sx, uint8_t sy, char* s)
{
  uint8_t x = sx;
  uint8_t y = sy;
  char c;
  for (char*p = s; c=pgm_read_byte(p); p++)
  {
    if ( c != '\n' )
    {
      put_char(x, y, c);
      x += 8;
    }
    else
    {
      x = sx;
      y += 12;
    }
  }
}

void put_string(uint8_t sx, uint8_t sy, char* s)
{
  uint8_t x = sx;
  uint8_t y = sy;
  char c;
  for (char*p = s; c=*p; p++)
  {
    if ( c != '\n' )
    {
      put_char(x, y, c);
      x += 8;
    }
    else
    {
      x = sx;
      y += 12;
    }
  }
}

unsigned long img_counter = 0;

//double dfreq;
// const double refclk=31372.549;  // =16MHz / 510
const double refclk=31376.6;      // measured

// variables used inside interrupt service declared as voilatile
volatile byte icnt;              // var inside interrupt
volatile byte icnt1;             // var inside interrupt
volatile byte c4ms;              // counter incremented all 4ms
volatile unsigned long phaccu;   // pahse accumulator
volatile unsigned long tword_m;  // dds tuning word m

void set_audio_fq(uint16_t af)
{
  tword_m=pow(2,32)*af/refclk;  // calulate DDS new tuning word 
}

volatile uint32_t wait_timer = 0;

// Wait functions are using PWM timer

void wait_ms(uint16_t ms)
{
  wait_timer = 31.37254901960784 * ms;
  while (wait_timer) 
  {
  }
}

void wait_us(uint16_t us)
{
  wait_timer = 0.03137254901960784 * us;
  while (wait_timer) 
  {
  }
}

void shift_byte(uint8_t data)
{
  int i;
  for (i = 0x80; i > 0; i >>= 1)
  {
    digitalWrite(CK_PIN, LOW);
    digitalWrite(DO_PIN, data & i ? HIGH : LOW);
    digitalWrite(CK_PIN, HIGH);
  }
}


void init_pll()
{
  digitalWrite(PA_PIN, LOW);
  pinMode(PA_PIN, OUTPUT);
  digitalWrite(RX_PIN, HIGH);
  pinMode(RX_PIN, OUTPUT);
  digitalWrite(TX_PIN, HIGH);
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(LE_PIN, HIGH);
  pinMode(LE_PIN, OUTPUT);
  digitalWrite(CK_PIN, LOW);
  pinMode(CK_PIN, OUTPUT);
  pinMode(DO_PIN, OUTPUT);

  pinMode(BTN_PIN, INPUT_PULLUP);
  digitalWrite(BTN_PIN, HIGH);


  digitalWrite(RX_PIN, HIGH);
  _delay_ms(1);
    
  digitalWrite(LE_PIN, LOW);
  shift_byte(datarx[0]);
  digitalWrite(LE_PIN, HIGH);
  _delay_ms(1);

  digitalWrite(LE_PIN, LOW);
  shift_byte(datarx[1]);
  shift_byte(datarx[2]);
  digitalWrite(LE_PIN, HIGH);
  _delay_ms(1);

  digitalWrite(LE_PIN, LOW);
  shift_byte(datatx[3]);
  shift_byte(datatx[4]);
  shift_byte(datatx[5]);
  digitalWrite(LE_PIN, HIGH);

  digitalWrite(TX_PIN, LOW);

  digitalWrite(PA_PIN, HIGH);

  _delay_ms(10);
}


void setup()
{
   digitalWrite(MOD_PIN, LOW);
   pinMode(MOD_PIN, OUTPUT);
  
    init_pll();

//  Serial.begin (9600);
//  Serial.println("DDS Test");
//  Serial.print(F_CPU);


  Setup_timer2();

  // disable interrupts to avoid timing distortion
  cbi (TIMSK0,TOIE0);              // disable Timer0 !!! delay() is now not available

  set_audio_fq(0);

  sbi (TIMSK2,TOIE2);              // enable Timer2 Interrupt
}

byte state = 0;

volatile uint8_t pic_line = 120;
volatile uint8_t pic_col = 0;

volatile uint8_t pixel_clk = 0;
volatile uint16_t line_clk = 0;


void loop()
{
     digitalWrite(LED_BUILTIN, HIGH);

      // If button in pressed during start-up, generate signal with 1 kHz FM
     if (0 == digitalRead(BTN_PIN))
     {
          set_audio_fq(1000);

          while(1) {}
     }

              // Silence
              set_audio_fq(0);
              wait_ms(2000);

                // Prepare text
                clear_frame_buf();
//                set_pix(10,10);
//                put_char(20,20,'A');
//                put_char(30,30,'B');
//                put_char(40,40,'C');

//                  put_string(10, 0, PSTR("UT4UWJ"));
//                  put_stringP(10, 7, PSTR("UT4UWJ\nKO50FK\n432.500\n"));
                  put_stringP(10, 7, PSTR("S51PV\nJN76EF\n432.500\n"));


                  img_counter ++;
                  char s[13] = "N:";
                  ultoa(img_counter, s+2, 9);
                  put_string(10, 7+12*3, s);
                  

// https://github.com/uzura-saan/arduino_sstv_bw8/blob/master/arduino_sk_19/arduino_sk_19.ino

                //--VIS CODE
                //VIS CODE for ROBOT B/W 8S is B0000010 (DECIMAL 2)
                set_audio_fq(1900);
                wait_ms(300);
                set_audio_fq(1200); //BREAK
                wait_ms(10);
                set_audio_fq(1900);
                wait_ms(300);
                set_audio_fq(1200); //START BIT
                wait_ms(30);
                set_audio_fq(1300); //BIT 0 (LSB FIRST)
                wait_ms(30);
                set_audio_fq(1100); //BIT 1
                wait_ms(30);
                set_audio_fq(1300); //BIT 2, 3, 4, 5, 6
                wait_ms(150);
                set_audio_fq(1100); //EVEN PARITY
                wait_ms(30);
                set_audio_fq(1200); //STOP BIT
                wait_ms(30);
                //--VIS DONE

               //--sync
                set_audio_fq(1200);
                wait_us(10000);
                //--

              // Start sending picture

              pixel_clk = 11;
              line_clk = 0;
              pic_col = 0;
              pic_line = 0; // This is a trigger

              while (pic_line < 120)
              {
                // Wait until the whole picture is transmitted
              }
//return;
              // Silence
              set_audio_fq(0);
              wait_ms(2000);

              // TX OFF
              //digitalWrite(PA_PIN, LOW);
              //digitalWrite(TX_PIN, HIGH);
              digitalWrite(PA_PIN, LOW);
              digitalWrite(LED_BUILTIN, LOW);

              // Pause
              for(int p=0; p<60*5; p++)
                wait_ms(1000);

//return;
              // Re-init PLL
              init_pll();

              
/*  
  while(1) {
     if ((0 == digitalRead(PTT_PIN)) && (0 == state))
     {
       state = 1;
       Serial.println("TX");
      digitalWrite(RX_PIN, HIGH);
      digitalWrite(LE_PIN, LOW);
      shift_byte(datatx[3]);
      shift_byte(datatx[4]);
      shift_byte(datatx[5]);
      digitalWrite(LE_PIN, HIGH);
      digitalWrite(TX_PIN, LOW);
      digitalWrite(PA_PIN, HIGH);
     }

     if ((1 == state) && (digitalRead(PTT_PIN)))
     {
      digitalWrite(PA_PIN, LOW);
      state = 0;
       Serial.println("RX");
      digitalWrite(TX_PIN, HIGH);
      digitalWrite(LE_PIN, LOW);
      shift_byte(datarx[3]);
      shift_byte(datarx[4]);
      shift_byte(datarx[5]);
      digitalWrite(LE_PIN, HIGH);
      digitalWrite(RX_PIN, LOW);
     
     }
    
     if (c4ms > 250) {                 // timer / wait fou a full second
      c4ms=0;
//      dfreq=67;
//
//      cbi (TIMSK2,TOIE2);              // disble Timer2 Interrupt
//      tword_m=pow(2,32)*dfreq/refclk;  // calulate DDS new tuning word
//      sbi (TIMSK2,TOIE2);              // enable Timer2 Interrupt 

      Serial.print(dfreq);
      Serial.print("  ");
      Serial.println(tword_m);
    }

    

//   sbi(PORTD,6); // Test / set PORTD,7 high to observe timing with a scope
//   cbi(PORTD,6); // Test /reset PORTD,7 high to observe timing with a scope
  }
 */ 
 }
//******************************************************************
// timer2 setup
// set prscaler to 1, PWM mode to phase correct PWM,  16000000/510 = 31372.55 Hz clock
void Setup_timer2() {

// Timer2 Clock Prescaler to : 1
  sbi (TCCR2B, CS20);
  cbi (TCCR2B, CS21);
  cbi (TCCR2B, CS22);

  // Timer2 PWM Mode set to Phase Correct PWM
  cbi (TCCR2A, COM2B0);  // clear Compare Match
  sbi (TCCR2A, COM2B1);

  sbi (TCCR2A, WGM20);  // Mode 1  / Phase Correct PWM
  cbi (TCCR2A, WGM21);
  cbi (TCCR2B, WGM22);
}

//******************************************************************
// Timer2 Interrupt Service at 31372,550 KHz = 32uSec
// this is the timebase REFCLOCK for the DDS generator
// FOUT = (M (REFCLK)) / (2 exp 32)
// runtime : 8 microseconds ( inclusive push and pop)
ISR(TIMER2_OVF_vect) {

//  sbi(PORTD,7);          // Test / set PORTD,7 high to observe timing with a oscope

  // Generate sine wave

  phaccu=phaccu+tword_m; // soft DDS, phase accu with 32 bits
  icnt=phaccu >> 24;     // use upper 8 bits for phase accu as frequency information
                         // read value fron ROM sine table and send to PWM DAC
  OCR2B=pgm_read_byte_near(sine256 + icnt);    

/*
  if(icnt1++ == 125) {  // increment variable c4ms all 4 milliseconds
    c4ms++;
    icnt1=0;    
   }   
*/

  // Advance wait timer

  if (wait_timer)
    wait_timer--;

  // Output the image

  if (pic_line < 120)
  {
      pixel_clk++;
      
      if (pixel_clk==12) // Next pixel
      {
          pixel_clk = 0;

          if (pic_col<160)
          {
            uint16_t pos = pic_line/2*80 + pic_col/2;
            uint8_t b = frameBuf[pos>>3] & (1<<(pos&7));
  
            if (b)
              set_audio_fq(1500); // Symbol, set pixel to Black
            else                
            {  
              // Set pixel to rainbow color
              
              //set_audio_fq(1700+(pic_col&0b11111000)*3);
              set_audio_fq(1700+(pic_col&0b11110000)*4);
              //set_audio_fq(2300-(pic_col&0b11110000)*3);
              //set_audio_fq(1700+pic_col*3);
            }

            pic_col++;
          }
          else
          if (pic_col==160)
          {
            set_audio_fq(1200); // Synch
            pic_col = 161;
            pixel_clk = 11;
            line_clk = 0;
          }
          else
          {
           pixel_clk = 11;
           //  line_clk++;
            // if (line_clk >=2091)        
            if (line_clk < 171)
              line_clk++;
            else        
            {
              pic_col = 0;
              pic_line++;
            }
               
          }
      }


  }



}
