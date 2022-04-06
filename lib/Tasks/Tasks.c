#include "Tasks.h"

#include "esp_log.h"

esp_err_t setup_tasks(const Task_t* allTasks, size_t taskCount)
{
  esp_err_t esp_error = ESP_FAIL;
  ESP_LOGI("[TASKS]", "Setting up tasks..\r\n");
  for(int i = 0; i < taskCount; ++i) {
    Task_t* task = &allTasks[i];
    BaseType_t xReturned =
        xTaskCreate(task->pvTaskCode, task->pcName, task->usStackDepth, task->pvParameters, task->uxPriority, NULL);
    if(xReturned != pdPASS) {
      ESP_LOGE("[TASKS]", "Error Creating the task %s", task->pcName);
      goto error;
    }
    ESP_LOGI("[TASKS]", "Task %s created", task->pcName);
  }
  ESP_LOGI("[TASKS]", "tasks setup complete\r\n");
  esp_error = ESP_OK;
error:
  return esp_error;
}