/* CPU speed benchmark which measures the time to digitally sign
   a string using SHA256 digest and RSA2048.
*/

#include "local_rsa_sign.h"

extern const uint8_t known_correct_signature[];

#define USE_TASK0 // Comment this to not start TASK0
#define USE_TASK1 // Comment this to not start TASK1

TaskHandle_t Task0;
TaskHandle_t Task1;

uint32_t t0C = 0;
uint32_t t1C = 0;

void setup() {
  Serial.begin(115200);

  rsa_init();

  //create a task that will be executed in the Task0code() function, with priority 1 and executed on core 0
#ifdef USE_TASK0
  xTaskCreatePinnedToCore(
    Task0code,   /* Task function. */
    "Task0",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task0,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
#endif

#ifdef USE_TASK1
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
#endif

  Serial.print(__func__);
  Serial.print(": running on core ");
  Serial.println(xPortGetCoreID());
}

uint8_t signature[256];
uint8_t signature1[256];

void Task0code( void * pvParameters ) {
  delay(1);
  vTaskDelay(200 / portTICK_PERIOD_MS); // Delay for 200 msecs as a workaround to an apparent Arduino environment issue.
  Serial.print(__func__);
  Serial.print(": running on core ");
  Serial.println(xPortGetCoreID());
  vTaskDelay(200 / portTICK_PERIOD_MS); // Delay for 200 msecs as a workaround to an apparent Arduino environment issue.
  Serial.print("RSA Signature Speed Test: Task");
  Serial.println(xPortGetCoreID());
  Serial.println();

  uint32_t begin_usec;
  do {
    begin_usec = micros();
    vTaskDelay(2 / portTICK_PERIOD_MS); // Delay for 200 msecs as a workaround to an apparent Arduino environment issue.
    for (int ii = 0; ii < 6; ii++) {
      RunTest0();
    }
    begin_usec = micros() - begin_usec;
    vTaskDelay(2 / portTICK_PERIOD_MS); // Delay for 200 msecs as a workaround to an apparent Arduino environment issue.
    Serial.printf( "Task%d: 6 iterations in us=%u", xPortGetCoreID() , begin_usec );

    // Print results
    Serial.print("\tSignature computation took ");
    Serial.print((float)(begin_usec) * 1.0e-6 / 6, 3);
    Serial.println(" seconds");
    // Verify the computed result
    if (memcmp(signature, known_correct_signature, 256) == 0) {
      Serial.println("Signature is good :-)");
    } else {
      Serial.println("ERROR: Signature is not correct!");
    }
    Serial.println();
  } while (1);
}

void Task1code( void * pvParameters ) {
  delay(1);
  vTaskDelay(200 / portTICK_PERIOD_MS); // Delay for 200 msecs as a workaround to an apparent Arduino environment issue.
  t1C++;
  Serial.print(__func__);
  Serial.print(": running on core ");
  Serial.println(xPortGetCoreID());
  //vTaskDelay(200 / portTICK_PERIOD_MS); // Delay for 200 msecs as a workaround to an apparent Arduino environment issue.
  Serial.print("RSA Signature Speed Test: Task");
  Serial.println(xPortGetCoreID());
  Serial.println();

  uint32_t begin_usec;
  do {
    begin_usec = micros();
    //  vTaskDelay(2 / portTICK_PERIOD_MS); // Delay for 200 msecs as a workaround to an apparent Arduino environment issue.
    for (int ii = 0; ii < 16; ii++) {
      RunTest1();
    }
    begin_usec = micros() - begin_usec;
    //    vTaskDelay(2 / portTICK_PERIOD_MS); // Delay for 200 msecs as a workaround to an apparent Arduino environment issue.
    Serial.printf( "Task%d: 16 iterations in us=%u", xPortGetCoreID() , begin_usec );

    // Print results
    Serial.print("\tSignature computation took ");
    Serial.print((float)(begin_usec) * 1.0e-6 / 16, 3);
    Serial.println(" seconds");
    // Verify the computed result
    if (memcmp(signature1, known_correct_signature, 256) == 0) {
      Serial.println("Signature is good :-)");
    } else {
      Serial.println("ERROR: Signature is not correct!");
    }
    Serial.println();
  } while (1);
}


// Quick dupe of call with unique signature{}, could pass *ptr ...
void RunTest0()
{
  // Run the speed benchmark
  rsa_sign_string("Sample plaintext string to be signed", signature);
}
void RunTest1()
{
  // Run the speed benchmark
  rsa_sign_string("Sample plaintext string to be signed", signature1);
}

#if 0
void foo() { //NOT USED
  // Print results
  Serial.print("Signature computation took ");
  //Serial.print((float)(end_usec - begin_usec) * 1.0e-6, 3);
  Serial.println(" seconds");
  Serial.println();
  for (int i = 0; i < 256; i++) {
    if (signature[i] < 16) Serial.print('0');
    Serial.print(signature[i], HEX);
    if ((i + 1) % 16 == 0) {
      Serial.println();
    } else {
      Serial.print(' ');
    }
  }

  // Verify the computed result
  if (memcmp(signature, known_correct_signature, 256) == 0) {
    Serial.println("Signature is good :-)");
  } else {
    Serial.println("ERROR: Signature is not correct!");
  }
}
#endif

void loop()
{
  vTaskDelay(200 / portTICK_PERIOD_MS);
}

// Known good result was obtained with this command:
//  echo -n "Sample plaintext string to be signed" | openssl dgst -sha256 -sign mykey.key | hd -v
// See comments in local_rsa_sign.c for generating the RSA key.

const uint8_t known_correct_signature[256] = {
  0x22, 0x14, 0xf7, 0x95, 0xb6, 0x7b, 0x7e, 0xfd, 0xc5, 0x9e, 0x6c, 0x3d, 0xbd, 0x28, 0xcb, 0x93,
  0xaf, 0x2c, 0x94, 0x21, 0xc7, 0x88, 0xa3, 0xd3, 0x6a, 0x37, 0xdd, 0x26, 0xd9, 0x9f, 0xfc, 0x5c,
  0xe7, 0xdc, 0xab, 0x51, 0xdb, 0xdf, 0x3e, 0x54, 0xfa, 0x4b, 0xfc, 0x8d, 0x8a, 0x09, 0x51, 0x58,
  0x48, 0xab, 0x3a, 0xb8, 0x1c, 0xef, 0xd3, 0x4f, 0x51, 0xd8, 0x3f, 0x3d, 0xba, 0xaa, 0xd6, 0x75,
  0x29, 0x91, 0x2c, 0x05, 0xbd, 0x90, 0x32, 0x4a, 0x61, 0x66, 0xe7, 0x82, 0xc0, 0x99, 0xeb, 0x8c,
  0x8f, 0x03, 0x1c, 0x4b, 0x29, 0x73, 0xd6, 0x6c, 0xf0, 0xc7, 0x4d, 0xdd, 0xad, 0x29, 0xc9, 0xec,
  0xa9, 0xd7, 0x65, 0x06, 0x2c, 0xf8, 0x74, 0x27, 0x02, 0xcd, 0xca, 0xf1, 0xa6, 0xc9, 0xf7, 0x82,
  0xf4, 0x3a, 0x6f, 0xcc, 0x42, 0x76, 0x9c, 0x37, 0x21, 0x67, 0x2d, 0xbe, 0xb1, 0xbb, 0x67, 0x3a,
  0xf1, 0xcf, 0xeb, 0x3d, 0x39, 0xf9, 0xc1, 0x24, 0x5a, 0xff, 0x33, 0xf7, 0x56, 0x8d, 0xfb, 0x6d,
  0x8f, 0x95, 0x08, 0x6c, 0xb4, 0x6f, 0xb9, 0x2c, 0xc3, 0xb3, 0xe9, 0x7a, 0x42, 0x24, 0x4d, 0x6e,
  0x48, 0x57, 0xed, 0xcb, 0x00, 0x10, 0x96, 0xf1, 0xb4, 0xd6, 0x19, 0x38, 0x57, 0xc5, 0x21, 0x27,
  0xa0, 0xb2, 0x36, 0x3b, 0x91, 0xed, 0x89, 0xfd, 0xa9, 0x60, 0xd0, 0x7f, 0xea, 0x83, 0x18, 0x11,
  0x16, 0x55, 0x51, 0x28, 0x29, 0xb4, 0x0c, 0x49, 0xe3, 0x81, 0x0a, 0xc4, 0x3f, 0x47, 0x2a, 0x61,
  0xea, 0x3f, 0x70, 0xba, 0xa4, 0x47, 0x2c, 0xd4, 0x27, 0x55, 0xc1, 0xd4, 0x06, 0x6f, 0x73, 0x1e,
  0x50, 0x09, 0x21, 0xb9, 0xc7, 0x42, 0xd6, 0x0e, 0x54, 0x9c, 0xf3, 0x34, 0x1a, 0xf6, 0x6e, 0x25,
  0x0c, 0xaf, 0xfe, 0x1d, 0xe1, 0x6b, 0x34, 0x1b, 0x36, 0x0f, 0x6c, 0x62, 0x0f, 0xcd, 0x11, 0x48
};
