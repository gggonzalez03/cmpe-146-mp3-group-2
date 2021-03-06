#pragma once

#include <stdint.h>

#include "ff.h"

typedef struct {
  // + 1 for the null terminator
  char filename[32 + 1];
  char tag[3 + 1];
  uint8_t id3_version[2];
  uint8_t id3_flags;
  uint32_t id3_size_in_synchsafe_integer;
  uint32_t id3_size_in_bytes;

  // + 1 for the null terminator
  char song_title[30 + 1];
  char artist[30 + 1];
  char album[30 + 1];
  char year[4 + 1];
} mp3_s;

// typedef enum {
//   TALB = 0, // Album
//   TCON,     // Content Type
//   TIT2,     // Song Title
//   TPE1,     // Lead Performer
//   TYER,     // Year
//   TLEN,     // Length
// } mp3_e;

/**
 * Convert 32-bit big endian into 32-bit little endian
 * @param big_endian is the 32-bit big endian to be converted to little endian
 * @return 32-bit little endian result
 **/
uint32_t big_endian_to_little_endian(uint32_t big_endian);

/**
 * Convert sychsafe integer to non-synchsafe integer
 * @param synchsafe_integer
 * @result non-synchsafe integer
 **/
uint32_t decode_synchsafe_integer(uint32_t synchsafe_integer);

/**
 * Get mp3 metadata from the id3 header. This will also call f_lseek() to change the
 * file pointer to the start of audio data.
 * @param file is the name of the mp3 file
 * @param mp3 is the struct where the metadata will be stored
 * @return file pointer to where the header ends (start of audio data)
 **/
uint32_t get_mp3_metadata_from_id3_header(const char *filename, mp3_s *mp3);

/**
 * Get mp3 metadata from the last 128 bytes of the file. This 128 bytes of data
 * are prefaced with a string "TAG". This function will also try to f_seek() to the
 * start of the audio file if an ID3 header exists.
 * @param file is the name of the mp3 file
 * @param mp3 is the struct where the metadata will be stored
 * @return file pointer to where the header ends (start of audio data)
 **/
uint32_t get_mp3_metadata_from_id3v1_tag(const char *filename, mp3_s *mp3);