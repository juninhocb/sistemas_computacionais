#include <Arduino_FreeRTOS.h>
TickType_t xLastWakeTime;
void vTask1( void * pvParameters ){
xLastWakeTime = xTaskGetTickCount();
for( ;; ){
Serial.println("*");
//vTaskDelay(100); 
vTaskDelayUntil(&xLastWakeTime, 100);
}

;}
void vTask2( void * pvParameters ){
xLastWakeTime = xTaskGetTickCount();
for( ;; ){
Serial.println("+");
//vTaskDelay(100) ;
vTaskDelayUntil(&xLastWakeTime, 100); 
}

}
void setup(){
Serial.begin(9600);
xTaskCreate( vTask1, "Periodic", 128, NULL, 1, NULL );
xTaskCreate( vTask2, "Periodic", 128, NULL, 2, NULL );
}
void loop(){
}
