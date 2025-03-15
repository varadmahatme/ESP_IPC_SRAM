#define PLACE_ON_SHARED_MEM __attribute__((section(".my_shared_section")))


typedef struct sharedMem {
  uint32_t core0Status;
  uint32_t core1Status;
} tSharedMem;


tSharedMem sharedMemory PLACE_ON_SHARED_MEM = {0};

void setupCore0() 
{
  sharedMemory.core0Status = 26;
  Serial.printf("Core 0: Set core0Status to %d\n", sharedMemory.core0Status);
}


void setupCore1() 
{
  Serial.printf("Core 1: Read core0Status as %d\n", sharedMemory.core0Status);
  
  sharedMemory.core1Status = 27;
  Serial.printf("Core 1: Set core1Status to %d\n", sharedMemory.core1Status);
}

void setup() {
  Serial.begin(115200);
  delay(2000); 
  
  Serial.println("Starting shared memory test");
  
  // Create tasks on specific cores
  xTaskCreatePinnedToCore([](void* param) {
      setupCore0();
      vTaskDelete(NULL);
    },
    "Core0Task", 2048, NULL, 1, NULL, 0
  );
  
  delay(1000);
  
  xTaskCreatePinnedToCore([](void* param) {
      setupCore1();
      vTaskDelete(NULL);
    },
    "Core1Task", 2048, NULL, 1, NULL, 1
  );
}

void loop() {
  Serial.printf("MAIN: core0Status=%d, core1Status=%d\n", sharedMemory.core0Status, sharedMemory.core1Status);
  delay(5000);
}