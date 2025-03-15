#define PLACE_ON_SHARED_MEM __attribute__((section(".my_shared_section")))

// Define the shared memory structure
typedef struct sharedMem {
  uint32_t core0Status;
  uint32_t core1Status;
} tSharedMem;

// Declare the variable with the attribute in the correct position
PLACE_ON_SHARED_MEM tSharedMem sharedMemory = {0};

// Example of using the shared buffer from Core 0
void setupCore0() {
  sharedMemory.core0Status = 26;
  Serial.printf("Core 0: Set core0Status to %d\n", sharedMemory.core0Status);
}

// Example of using the shared buffer from Core 1
void setupCore1() {
  // First read the value set by Core 0
  Serial.printf("Core 1: Read core0Status as %d\n", sharedMemory.core0Status);
  
  // Then set core1Status
  sharedMemory.core1Status = 27;
  Serial.printf("Core 1: Set core1Status to %d\n", sharedMemory.core1Status);
}

void setup() {
  Serial.begin(115200);
  delay(2000); // Give time for serial to initialize
  
  Serial.println("Starting shared memory test");
  
  // Create tasks on specific cores
  xTaskCreatePinnedToCore(
    [](void* param) {
      setupCore0();
      vTaskDelete(NULL);
    },
    "Core0Task", 2048, NULL, 1, NULL, 0
  );
  
  delay(1000); // Give Core 0 task time to complete
  
  xTaskCreatePinnedToCore(
    [](void* param) {
      setupCore1();
      vTaskDelete(NULL);
    },
    "Core1Task", 2048, NULL, 1, NULL, 1
  );
}

void loop() {
  // Print the values periodically to verify they're stable
  Serial.printf("MAIN: core0Status=%d, core1Status=%d\n", 
                sharedMemory.core0Status, sharedMemory.core1Status);
  delay(5000);
}