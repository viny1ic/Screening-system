#include <U8glib.h>
#include <Adafruit_MLX90614.h>
#include <Wire.h>

//U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9);	// SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SH1106_128X64 u8g(4, 5, 6, 7);	// SW SPI Com: SCK = 4, MOSI = 5, CS = 6, A0 = 7 (new blue HalTec OLED)
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_FAST);	// Dev 0, Fast I2C / TWI
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);	// Display which does not send ACK

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

const int trigPin = A2;
const int echoPin = A1;
long duration;
int distance;


void draw(int i) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  char buf[9];
  sprintf (buf, "%d", i);
  u8g.drawStr(40, 40, buf);
}


void setup(void) {
  // flip screen, if required
  // u8g.setRot180();
  pinMode(9,INPUT);
  pinMode(7,INPUT);
  pinMode(A0,OUTPUT);
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  Serial.println("Adafruit MLX90614 test");  

  mlx.begin();
  
  pinMode(8, OUTPUT);
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop(void) {
  u8g.firstPage();  
  do {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance= duration*0.034/2;
      if(distance<10){
        int temp=mlx.readObjectTempF();
        draw(temp);
        if(temp>90)
          while(1)
            digitalWrite(A0,HIGH);
      }
//      Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
        draw(-1);
      Serial.println();
  } while( u8g.nextPage() );
  
}
