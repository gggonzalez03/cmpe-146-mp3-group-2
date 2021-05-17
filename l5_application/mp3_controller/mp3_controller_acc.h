#pragma once

void mp3_controller_acc__treble_task(void *parameter);
void mp3_controller_acc__bass_task(void *parameter);
void mp3_controller_acc__treble_task_suspender(void *parameter);
void mp3_controller_acc__bass_task_suspender(void *parameter);

void mp3_controller_acc__create_all_tasks(void);
void mp3_controller_acc__suspend_treble_task(void);
void mp3_controller_acc__suspend_bass_task(void);
void mp3_controller_acc__resume_treble_task(void);
void mp3_controller_acc__resume_bass_task(void);