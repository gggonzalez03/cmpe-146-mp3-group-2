#pragma once

#include <stddef.h>

typedef char song_memory_t[128];

void mp3_song_list__populate(void);
size_t mp3_song_list__get_item_count(void);
const char *mp3_song_list__get_name_for_item(size_t item_number);