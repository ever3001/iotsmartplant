#ifndef _TASKS_H_
#define _TASKS_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct {
  TaskFunction_t pvTaskCode;
  const char* const pcName;
  const uint32_t usStackDepth;
  void* pvParameters;
  UBaseType_t uxPriority;
} Task_t;


esp_err_t setup_tasks(const Task_t* allTasks, size_t taskCount);

#endif // _TASKS_H_