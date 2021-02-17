#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key; 

int code[] = {102,99,67,31,0};
int codeRead = 0;
String uidString;

void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(10,0); 
  display.print("RFID Lock");
  display.display();
  
}

void loop() 
{
  if( rfid.PICC_IsNewCardPresent() )
  {
    readRFID();
  }
  delay(100);
}

void readRFID()
{
  
  rfid.PICC_ReadCardSerial();
  Serial.print(F("\nPICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) 
    {
      Serial.println(F("Your tag is not of type MIFARE Classic."));
      return;
    }
    clearUID();

    Serial.println("Scanned PICC's UID:");
    printDec(rfid.uid.uidByte, rfid.uid.size);

    uidString = String(rfid.uid.uidByte[0])+" "+String(rfid.uid.uidByte[1])+" "+String(rfid.uid.uidByte[2])+ " "+String(rfid.uid.uidByte[3]);
    
    printUID();

    int i = 0;
    boolean match = true;
    while(i<rfid.uid.size)
    {
      if(!(rfid.uid.uidByte[i] == code[i]))
      {
           match = false;
      }
      i++;
    }

    if(match)
    {
      Serial.println("\nI know this card!");
      printUnlockMessage();
    }else
    {
      Serial.println("\nUnknown Card");
    }

  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

void printDec(byte *buffer, byte bufferSize) 
{
  for (byte i = 0; i < bufferSize; i++) 
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

  void clearUID()
  {
    display.setTextColor(BLACK);
    display.setTextSize(1);
    display.setCursor(30,20); 
    display.print(uidString);
    display.display();
  }

  void printUID()
  {
    display.setTextColor(WHITE);
    display.setCursor(0,20); 
    display.print("UID: ");
    display.setCursor(30,20); 
    display.print(uidString);
    display.display();
  }

  void printUnlockMessage()
  {
    display.display();
    display.setTextColor(BLACK);
    display.setTextSize(2);
    display.setCursor(10,0); 
    display.print("RFID Lock");
    display.display();
    
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(10,0); 
    display.print("Unlocked");
    display.display();
    
    delay(2000);
    
    display.setTextColor(BLACK);
    display.setTextSize(2);
    display.setCursor(10,0); 
    display.print("Unlocked");

    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(10,0); 
    display.print("RFID Lock");
    display.display();
  }
