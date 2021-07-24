#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
SoftwareSerial mySerial(2, 3);   // (receiver pin, transfer pin)
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#include <Servo.h>
Servo ServoMotor;
const int buttonPin = 7;
int buttonState = 0;
void setup()
{
    Serial.begin(115200); //Sets the data rate in bits per second 
    finger.begin(57600);  // data rate
    ServoMotor.attach(6);  // used to attach servo motor on which pin
    pinMode(buttonPin, INPUT);
    lcd.begin(16, 2);        // 16 characters in each line and 2 lines are there on lcd display
    lcd.setCursor(0, 0);      //  (col,row)
    lcd.print("  Finger Based  ");
    lcd.setCursor(0, 1);
    lcd.print("Security System ");
    delay(3000);
}
//// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez()
{
    uint8_t p = finger.getImage();  // getImage (void) Ask the sensor to take an image of the finger pressed on surface
    if (p != FINGERPRINT_OK)
    {
        return -1;
    }

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
    {
        return -1;
    }

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
    {

        return -1;
    }

    //  // found a match!
    Serial.print("Found ID #");
    Serial.print(finger.fingerID);
    Serial.print(" with confidence of ");
    Serial.println(finger.confidence);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Finger Matched ");
    lcd.setCursor(0, 1);
    lcd.print("  Open WELCOME  ");
    ServoMotor.write(0);
    delay(3000);
    ServoMotor.write(90);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" THANK YOU ");
    lcd.setCursor(0, 1);
    lcd.print("     Close     ");
    delay(1000);
    return 1;
}
void loop()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please Show your ");
    lcd.setCursor(0, 1);
    lcd.print("      Face       ");
    delay(1000);
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" FACE DETECTED  ");
        delay(2000);
        if (finger.verifyPassword())
        {
            Serial.println("Found fingerprint sensor!");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Please Insert  ");
            lcd.setCursor(0, 1);
            lcd.print("  Valid Finger  ");
            delay(3000);
            int ret = -1;
            int count = 0;
            while (ret == -1)
            {
                Serial.println("inside while loop");
                ret = getFingerprintIDez();
                count++;
                Serial.print("Count:");
                Serial.println(count);
                delay(500);
                if (count > 20)
                    break;
            }
        }
    }
}