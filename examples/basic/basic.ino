#include "Arduino.h"
#include "htcw_pool.h"
using namespace htcw;

using default_pool = pool<0>;
using psram_pool = pool<1,ps_malloc>;

void setup() {
    Serial.begin(115200);
    // create a 8192KB block
    if(!default_pool::initialize(8192)) {
        Serial.println("Unable to initialize pool");
        while(1);
    }
    Serial.print("Capacity: ");
    Serial.print(default_pool::capacity()/1024.f);
    Serial.println("KB");
    Serial.println();

    Serial.print("Free: ");
    Serial.print(default_pool::bytes_free()/1024.f);
    Serial.println("KB");
    Serial.println();

    Serial.println("Allocate string");
    char* str = (char*)default_pool::allocate(512);
    strcpy(str,"hello world!");
    Serial.println();

    Serial.print("Free: ");
    Serial.print(default_pool::bytes_free()/1024.f);
    Serial.println("KB");
    Serial.println();
    
    Serial.println(str);

    Serial.println("Allocate array");
    int* int_array = (int*)default_pool::allocate(sizeof(int)*10);
    Serial.println();

    Serial.print("Free: ");
    Serial.print(default_pool::bytes_free()/1024.f);
    Serial.println("KB");
    Serial.println();
    
    Serial.println("Reallocate array");
    int_array = (int*)default_pool::reallocate(int_array,sizeof(int)*20);
    Serial.println();

    Serial.print("Free: ");
    Serial.print(default_pool::bytes_free()/1024.f);
    Serial.println("KB");
    Serial.println();

    Serial.println("Reallocate string");
    str = (char*)default_pool::reallocate(str,1024);
    Serial.println();
    Serial.println(str);
    Serial.println();
    
    Serial.print("Free: ");
    Serial.print(default_pool::bytes_free()/1024.f);
    Serial.println("KB");
    Serial.println();
    
    Serial.println("Free string");
    default_pool::deallocate(str);
    Serial.println();
    
    Serial.print("Free: ");
    Serial.print(default_pool::bytes_free()/1024.f);
    Serial.println("KB");
    Serial.println();

    Serial.println("Reallocate array");
    int_array = (int*)default_pool::reallocate(int_array,sizeof(int)*40);
    Serial.println();

    Serial.print("Free: ");
    Serial.print(default_pool::bytes_free()/1024.f);
    Serial.println("KB");
    Serial.println();
    
    Serial.println("Dellocate all");
    default_pool::deallocate_all();
    Serial.println();

    Serial.print("Free: ");
    Serial.print(default_pool::bytes_free()/1024.f);
    Serial.println("KB");
    Serial.println();
    
}

void loop() {
}
