#include <Arduino_FreeRTOS.h>
#include <queue.h>

 /*define as variáveis.
  * -------------------------------------------------------*/
  
QueueHandle_t xQueue;
static void Produtor( void *pvParameters );
static void Consumidor( void *pvParameters );
 
#define QUEUE_ITEM_SIZE 4
#define QUEUE_LENGTH    20
 
void setup()
{
  Serial.begin(9600);
  xQueue = xQueueCreate(QUEUE_LENGTH,QUEUE_ITEM_SIZE);

  /*Cria as tarefas
   * -----------------------------------------------------------------------------------------------------*/
  xTaskCreate( Produtor,( const char * ) "Produtor", 128, NULL, 1, NULL );                   
  
  xTaskCreate( Consumidor,( const char * ) "Consumidor", 128, NULL, 1, NULL );                 
  
  /* Inicialização do escalonador. */
  
  vTaskStartScheduler();
  return ;
}

void loop() {


}

/* Cria o produtor, o produtor gera 400 números aleatórios.
 *  --------------------------------------------------- */
static void Produtor( void *pvParameters )
{  
  int randNumber;
  while(1)
  {
    if (uxQueueSpacesAvailable > 0){
      randNumber = random(400);
      xQueueSend(xQueue,&randNumber,1);
    }
  }
}

/* Cria o consumidor, o consumidor recebe o valor que o produtor gerou
 * e verifica se ele é número primo.
 ------------------------------------------------------------------*/
static void Consumidor( void *pvParameters )
{
  int n;
 
  while(1)
  {
    int resultado = 0;
    if (uxQueueSpacesAvailable >= 0){
    xQueueReceive(xQueue,&n,10);
    //Serial.println(n);
    }

  for (int i = 2; i <= n; i++) {
    if (n %i == 0) { 
      resultado = resultado+1;
      }

     if (resultado == 0){
        Serial.print(n);
        Serial.println("-é primo\n");
        break;
     }else{
        Serial.println(n);
        Serial.println("-Não é um número primo\n");
        break;
           }
     }
  
}
}
