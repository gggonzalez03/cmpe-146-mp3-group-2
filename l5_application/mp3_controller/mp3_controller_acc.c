#include <stdbool.h>
#include <stdlib.h>

#include "mp3_controller_acc.h"

#include "FreeRTOS.h"
#include "task.h"

#include "acceleration.h"
#include "vs1053b_mp3_decoder.h"

static volatile bool treble_task_suspend_allowed = false;
static volatile bool bass_task_suspend_allowed = false;

TaskHandle_t treble_task;
TaskHandle_t bass_task;
TaskHandle_t treble_task_suspender;
TaskHandle_t bass_task_suspender;

void mp3_controller_acc__create_all_tasks(void) {
  xTaskCreate(mp3_controller_acc__treble_task, "treble task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &treble_task);
  xTaskCreate(mp3_controller_acc__bass_task, "bass task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, &bass_task);
  xTaskCreate(mp3_controller_acc__treble_task_suspender, "treble task suspend", 4096 / sizeof(void *), NULL,
              PRIORITY_MEDIUM, &treble_task_suspender);
  xTaskCreate(mp3_controller_acc__bass_task_suspender, "bass task suspend", 4096 / sizeof(void *), NULL,
              PRIORITY_MEDIUM, &bass_task_suspender);

  vTaskSuspend(treble_task);
  vTaskSuspend(bass_task);
  vTaskSuspend(treble_task_suspender);
  vTaskSuspend(bass_task_suspender);
}

void mp3_controller_acc__treble_task(void *parameter) {

  static int x;
  static uint8_t frq;

  while (true) {
    treble_task_suspend_allowed = false;

    x = (int)acceleration__get_acceleration_x();
    frq = 10 - (uint8_t)abs(x);

    vs1053b__set_treble_frequency(frq);

    treble_task_suspend_allowed = true;
    vTaskDelay(250);
  }
}

void mp3_controller_acc__bass_task(void *parameter) {
  static int x;
  static uint8_t frq;

  while (true) {
    bass_task_suspend_allowed = false;

    x = (int)acceleration__get_acceleration_x();
    frq = (uint8_t)abs(x) * 2;

    vs1053b__set_bass_frequency(frq);

    bass_task_suspend_allowed = true;
    vTaskDelay(250);
  }
}

void mp3_controller_acc__treble_task_suspender(void *parameter) {
  while (true) {
    while (!treble_task_suspend_allowed) {
      ;
    }
    vTaskSuspend(treble_task);
    vTaskSuspend(NULL);
  }
}

void mp3_controller_acc__bass_task_suspender(void *parameter) {
  while (true) {
    while (!bass_task_suspend_allowed) {
    }
    vTaskSuspend(bass_task);
    vTaskSuspend(NULL);
  }
}

void mp3_controller_acc__suspend_treble_task(void) {
  TaskStatus_t status;

  vTaskGetInfo(treble_task, &status, pdFALSE, eInvalid);

  if (status.eCurrentState != eSuspended) {
    vTaskResume(treble_task_suspender);
    vs1053b__set_treble_frequency(15); // back to normal treble
  }
}

void mp3_controller_acc__suspend_bass_task(void) {

  TaskStatus_t status;

  vTaskGetInfo(bass_task, &status, pdFALSE, eInvalid);

  if (status.eCurrentState != eSuspended) {
    vTaskResume(bass_task_suspender);
    vs1053b__set_bass_frequency(2); // back to normal bass
  }
}

void mp3_controller_acc__resume_treble_task(void) { vTaskResume(treble_task); }

void mp3_controller_acc__resume_bass_task(void) { vTaskResume(bass_task); }