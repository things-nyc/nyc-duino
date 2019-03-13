#include <TheThingsNetwork.h>
#include <CayenneLPP.h>
#include <DHT.h>

// Set your AppEUI and AppKey
const char *appEui = "70B3D57ED00117CD";
const char *appKey = "D43209B0B6D943006D9FD0A80FB343D7";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_US915

#define DHTPIN 4
#define LEDPIN 13

//Choose your DHT sensor moddel
//#define DHTTYPE DHT11
//#define DHTTYPE DHT21
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

CayenneLPP lpp(51);

static int vccRead()
{
  analogRead(0);                // achieves 90% of ADC setup
  ADMUX = 0b01011110;           // set mux to bandgap input, AVcc ref
  
  delayMicroseconds(250);       // give time for bandgap to settle
  
  ADCSRA |= (1 << ADSC);        // start conversion
  while (ADCSRA & (1 << ADSC)); // wait for conversion to finish

  int x = ADC;
  return (1100L * 1023) / x;    // bandgap is 1100mV nom.
}

void setup()
{
  // Initialize LED and turn it off
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  Serial.println("-- STATUS --");
  ttn.showStatus();

  Serial.println("-- JOIN --");
  ttn.join(appEui, appKey);

  dht.begin();
}

void loop()
{
  Serial.println("-- LOOP --");
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (Fahrenheit = true)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again)
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(1000);
    return;
  }

  // Read VCC of 32u4, output of 3.3V LDO but can drop when battery is < 3.4V
  int v = vccRead();

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print("VCC: ");
  Serial.print(v);
  Serial.println(" mV");

  // Prepare Cayenne LPP
  lpp.reset();
  lpp.addTemperature(1, t);
  lpp.addRelativeHumidity(2, h);
  lpp.addAnalogInput(3, float(v)/1000);

  // Send it off
  digitalWrite(LEDPIN, HIGH);
  ttn.sendBytes(lpp.getBuffer(), lpp.getSize());
  digitalWrite(LEDPIN, LOW);

  // Wait 20 seconds between transmissions
  delay(20000);
}
