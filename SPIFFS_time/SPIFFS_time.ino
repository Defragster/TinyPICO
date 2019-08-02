#include "FS.h"
#include "SPIFFS.h"
#include <time.h>
#include <WiFi.h>
#include <TinyPICO.h>

// Initialise the TinyPICO library
TinyPICO tp = TinyPICO();
#define DOTSTAR_CYCLE 33 // change this to cyle dotstar rate of change

const char* ssid     = "ssid";
const char* password = "PW";

long timezone = -8;
byte daysavetime = 1;

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED  true // false

// Interval between internal temperature reads
unsigned long next_temp_read = 0;   // Next time step in milliseconds
uint32_t temp_read_interval = 10000;  // This is in milliseconds


void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.print (file.name());
      time_t t = file.getLastWrite();
      struct tm * tmstruct = localtime(&t);
      Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, ( tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.print(file.size());
      time_t t = file.getLastWrite();
      struct tm * tmstruct = localtime(&t);
      Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, ( tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char * path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void setup() {
  Serial.begin(115200);
  //Serial.begin(2000000);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Contacting Time Server");
  configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
  struct tm tmstruct ;
  delay(2000);
  tmstruct.tm_year = 0;
  getLocalTime(&tmstruct, 5000);
  Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct.tm_year) + 1900, ( tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour , tmstruct.tm_min, tmstruct.tm_sec);
  Serial.println("");

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    if (!SPIFFS.begin()) {
      Serial.println("SPIFFS Mount Failed :: Needs formatting?");
      return;
    }
    if ( FORMAT_SPIFFS_IF_FAILED) {
      Serial.println("SPIFFS Mount Failed :: FORMATTING");
    }
  }

  listDir(SPIFFS, "/", 0);
  removeDir(SPIFFS, "/mydir");
  createDir(SPIFFS, "/mydir");
  deleteFile(SPIFFS, "/hello.txt");
  writeFile(SPIFFS, "/hello.txt", "Hello ");
  appendFile(SPIFFS, "/hello.txt", "World!\n");
  listDir(SPIFFS, "/", 0);
  Serial.printf("\nSpiffs Space Total=%u and Used=%u\n", SPIFFS.totalBytes(), SPIFFS.usedBytes() );
}

void loop()
{
  // Cycle the DotStar colour every 25 miliseconds
  tp.DotStar_CycleColor(DOTSTAR_CYCLE);

  // You can set the DotStar colour directly using r,g,b values
  // tp.DotStar_SetPixelColor( 255, 128, 0 );

  // You can set the DotStar colour directly using a uint32_t value
  // tp.DotStar_SetPixelColor( 0xFFC900 );

  // You can aclear the DotStar too
  // tp.DotStar_Clear();

  // To power down the DotStar for deep sleep you call this
  // tp.DotStar_SetPower( false );

  // Print the internal temperature of the PICO-D4
  print_temp();
}

void print_temp()
{
  // We only run the contents of this function every *temp_read_interval* step
  if ( millis() > next_temp_read + temp_read_interval )
  {
    next_temp_read = millis();

    // Grab the temperature in Farenheit
    uint8_t temp_farenheit = tp.Get_Internal_Temp_F();

    // Grab the temperature in Celcius
    float temp_celsius = tp.Get_Internal_Temp_C();

    // Print the temperatures to the output console
    Serial.print("Temp In PICO-D4 ");
    Serial.print(temp_farenheit);
    Serial.print("°F ");
    Serial.print(temp_celsius);
    Serial.println("°C");

    if (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".!WiFi");
    }
    else   Serial.print("WiFi connected");
    Serial.print("\tIP address: ");
    Serial.print(WiFi.localIP());
    Serial.println("   Contacting Time Server");
    configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
    struct tm tmstruct ;
    tmstruct.tm_year = 0;
    getLocalTime(&tmstruct, 5000);
    Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct.tm_year) + 1900, ( tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour , tmstruct.tm_min, tmstruct.tm_sec);

  }
}
