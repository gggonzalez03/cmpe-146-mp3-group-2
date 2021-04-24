#include <stdio.h>
#include <string.h>

#include "mp3_metadata_decoder.h"

/**
 * ID3 Header Format:
 * ID3v2/file identifier   "ID3"          [3 bytes]
 * ID3v2 version           $03 00         [2 bytes]
 * ID3v2 flags             %abc00000      [1 byte]
 * ID3v2 size              4 * %0xxxxxxx  [4 bytes]
 **/
static uint32_t get_id3_header(FIL *file, mp3_s *mp3) {

  UINT bytes_read;

  uint32_t id3_size_in_big_endian_synchsafe;
  uint32_t id3_size_in_little_endian_synchsafe;

  f_read(file, (void *)mp3->tag, 3, &bytes_read);
  mp3->tag[3] = '\0';
  if (strcmp(mp3->tag, "ID3")) {
    return 0;
  }

  f_read(file, (void *)mp3->id3_version, 2, &bytes_read);
  f_read(file, (void *)&mp3->id3_flags, 1, &bytes_read);
  f_read(file, (void *)&id3_size_in_big_endian_synchsafe, 4, &bytes_read);

  id3_size_in_little_endian_synchsafe = big_endian_to_little_endian(id3_size_in_big_endian_synchsafe);
  mp3->id3_size_in_bytes = decode_synchsafe_integer(id3_size_in_little_endian_synchsafe) + 10;

  return mp3->id3_size_in_bytes;
}

/**
 * ID3 Frame Format:
 * Frame ID       $xx xx xx xx      [4 bytes]
 * Size           $xx xx xx xx      [4 bytes]
 * Flags          $xx xx            [2 bytes]
 **/
static uint32_t get_id3_header_frames(FIL *file, mp3_s *mp3) {

  char frame_id[5]; // the 5th char is for null-character
  uint32_t frame_size;
  uint8_t frame_flags[2];
  uint8_t frame_encoding;
  uint8_t frame_endianness[2];

  char frame_value[32];

  UINT bytes_read;

  f_read(file, (void *)frame_id, 4, &bytes_read);
  frame_id[4] = '\0';

  if (!strcmp(frame_id, "TALB")) {
    f_read(file, (void *)&frame_size, 4, &bytes_read);
    f_read(file, (void *)frame_flags, 2, &bytes_read);
    f_read(file, (void *)&frame_encoding, 1, &bytes_read);
    f_read(file, (void *)frame_endianness, 2, &bytes_read);

    frame_size = big_endian_to_little_endian(frame_size);

    f_read(file, (void *)frame_value, frame_size - 3, &bytes_read);
  }

  // printf("Frame ID: %s\n", frame_id);
  // printf("Size: %ld\n", frame_size);
  // printf("Flags: %d, %d\n", frame_flags[0], frame_flags[1]);
  // printf("Encoding: %d\n", frame_encoding);
  // printf("Endianness: %d, %d\n", frame_endianness[0], frame_endianness[1]);
  // printf("Value: %s\n", frame_value);

  return 0;
}

static void initialize_to_unknown_fields(mp3_s *mp3) {
  char unknown_string[] = "UNKW";
  strcpy(mp3->song_title, unknown_string);
  strcpy(mp3->artist, unknown_string);
  strcpy(mp3->album, unknown_string);
  strcpy(mp3->year, unknown_string);
}

static void insert_null_terminators(mp3_s *mp3) {
  mp3->song_title[30] = '\0';
  mp3->artist[30] = '\0';
  mp3->album[30] = '\0';
  mp3->year[4] = '\0';
}

uint32_t big_endian_to_little_endian(uint32_t big_endian) {
  uint32_t little_endian = 0UL;

  little_endian |= (0x000000FF & big_endian >> 24) | (0x0000FF00 & big_endian >> 8) | (0x00FF0000 & big_endian << 8) |
                   (0xFF000000 & big_endian << 24);

  return little_endian;
}

uint32_t decode_synchsafe_integer(uint32_t synchsafe_integer) {
  uint32_t result = 0UL;
  uint32_t mask = 0x7F000000;

  while (mask) {
    result >>= 1;
    result |= (synchsafe_integer & mask);
    mask >>= 8;
  }

  return result;
}

/**
 * ID3 Header Format:
 * ID3v2/file identifier   "ID3"          [3 bytes]
 * ID3v2 version           $03 00         [2 bytes]
 * ID3v2 flags             %abc00000      [1 byte]
 * ID3v2 size              4 * %0xxxxxxx  [4 bytes]
 *
 * ID3 Frame Format:
 * Frame ID       $xx xx xx xx      [4 bytes]
 * Size           $xx xx xx xx      [4 bytes]
 * Flags          $xx xx            [2 bytes]
 *
 * Reference: https://id3.org/id3v2.3.0
 **/
uint32_t get_mp3_metadata_from_id3_header(const char *filename, mp3_s *mp3) {

  FIL file;
  FRESULT result;

  uint32_t start_of_audio;

  result = f_open(&file, filename, (FA_READ));

  if (result != FR_OK) {
    return 0;
  }

  start_of_audio = get_id3_header(&file, mp3);

  /**
   * TODO: Read each ID3 frame that we're interested in
   * TALB = 0, // Album
   * TCON,     // Content Type/Genre
   * TIT2,     // Song Title
   * TPE1,     // Lead Performer
   * TYER,     // Year
   * TLEN,     // Length
   *
   * get_id3_header_frames()
   **/

  f_close(&file);

  return start_of_audio;
}

uint32_t get_mp3_metadata_from_id3v1_tag(const char *filename, mp3_s *mp3) {

  FIL file;
  FRESULT result;
  UINT bytes_read;
  uint32_t start_of_audio;

  result = f_open(&file, filename, (FA_READ));

  if (result != FR_OK) {
    return 0;
  }

  initialize_to_unknown_fields(mp3);

  // Get ID3 header data if it exists
  start_of_audio = get_id3_header(&file, mp3);

  // Start reading from the last 128 bytes
  f_lseek(&file, f_size(&file) - 128);

  f_read(&file, (void *)mp3->tag, 3, &bytes_read);
  mp3->tag[3] = '\0';
  if (strcmp(mp3->tag, "TAG")) {
    // Set the filename as the song name instead
    if (strlen(filename) <= 30) {
      strcpy(mp3->song_title, filename);
    }
    return 0;
  }

  f_read(&file, (void *)mp3->song_title, 30, &bytes_read);
  f_read(&file, (void *)mp3->artist, 30, &bytes_read);
  f_read(&file, (void *)mp3->album, 30, &bytes_read);
  f_read(&file, (void *)mp3->year, 4, &bytes_read);

  insert_null_terminators(mp3);

  f_close(&file);

  return start_of_audio;
}