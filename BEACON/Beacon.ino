

#define RX_PIN 4
#define TX_PIN 5
#define PA_PIN 6
#define DO_PIN 11
#define CK_PIN 12
#define LE_PIN 10

#define MOD_PIN 3

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
uint8_t datatx[6] = {0x20, 0x44, 0x00, 0x32, 0x1C, 0x24}; // TX 432.450  
//uint8_t datatx[6] = {0x20, 0x44, 0x00, 0x32, 0x2d, 0x23}; // TX 446.037.5 KHz


#define TIME 100

//char *callsign = "..- .-. ....- ..- .--. ...-";
char *callsign = "  ..- .-. ----- ..- -... -...           -.- --- ..... ----- ..-. -.-";

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

int ledPin = 13;                 // LED pin 7
int testPin = 7;
int t2Pin = 6;
byte bb;

double dfreq;
// const double refclk=31372.549;  // =16MHz / 510
const double refclk=31376.6;      // measured

// variables used inside interrupt service declared as voilatile
volatile byte icnt;              // var inside interrupt
volatile byte icnt1;             // var inside interrupt
volatile byte c4ms;              // counter incremented all 4ms
volatile unsigned long phaccu;   // pahse accumulator
volatile unsigned long tword_m;  // dds tuning word m

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
  digitalWrite(MOD_PIN, LOW);
  pinMode(MOD_PIN, OUTPUT);
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
  delay(1);
    
  digitalWrite(LE_PIN, LOW);
  shift_byte(datarx[0]);
  digitalWrite(LE_PIN, HIGH);
  delay(1);

  digitalWrite(LE_PIN, LOW);
  shift_byte(datarx[1]);
  shift_byte(datarx[2]);
  digitalWrite(LE_PIN, HIGH);
  delay(1);

  digitalWrite(LE_PIN, LOW);
  shift_byte(datatx[3]);
  shift_byte(datatx[4]);
  shift_byte(datatx[5]);
  digitalWrite(LE_PIN, HIGH);

  digitalWrite(TX_PIN, LOW);

  digitalWrite(PA_PIN, HIGH);

  delay(10);
}

void setup()
{
  init_pll();

  Serial.begin (9600);
  Serial.println("DDS Test");
}

void loop() {

     if (0 == digitalRead(BTN_PIN))
     {
          digitalWrite(LED_BUILTIN, HIGH);
          digitalWrite(MOD_PIN, LOW);
          while(1) {}
     }

  
  // put your main code here, to run repeatedly:
  char *p = callsign;
  while(*p)
//  while(0)
  {
    switch (*p++)
    {
        case '.':
          digitalWrite(LED_BUILTIN, HIGH);
          digitalWrite(MOD_PIN, HIGH);
          delay(1*TIME);
          digitalWrite(LED_BUILTIN, LOW); 
          digitalWrite(MOD_PIN, LOW);
          delay(1*TIME);
      break;
      case '-':
          digitalWrite(LED_BUILTIN, HIGH);
          digitalWrite(MOD_PIN, HIGH);
          delay(3*TIME);
          digitalWrite(LED_BUILTIN, LOW); 
          digitalWrite(MOD_PIN, LOW);
          delay(1*TIME);
      break;
      default:
          delay(2*TIME);
      break;
    }
  }
//  delay(1000);
//  digitalWrite(LED_BUILTIN, HIGH);
//  digitalWrite(MOD_PIN, HIGH);
  delay(30000);
//  digitalWrite(LED_BUILTIN, LOW); 
//  digitalWrite(MOD_PIN, LOW);
//  delay(1000);

  init_pll();
}
