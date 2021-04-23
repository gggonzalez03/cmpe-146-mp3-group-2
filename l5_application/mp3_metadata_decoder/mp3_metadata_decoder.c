#include <string.h>

#include "mp3_metadata_decoder.h"

static uint32_t get_id3_header_tag(FIL *file, mp3_s *mp3) {

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

  printf("Frame ID: %s\n", frame_id);
  printf("Size: %ld\n", frame_size);
  printf("Flags: %d, %d\n", frame_flags[0], frame_flags[1]);
  printf("Encoding: %d\n", frame_encoding);
  printf("Endianness: %d, %d\n", frame_endianness[0], frame_endianness[1]);
  printf("Value: %s\n", frame_value);

  return 0;
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
uint32_t get_mp3_metadata_from_id3_header(FIL *file, mp3_s *mp3) {

  UINT bytes_read;

  if (file->obj.fs == 0) {
    return 0;
  }

  uint32_t id3_size_in_big_endian_synchsafe;
  uint32_t id3_size_in_little_endian_synchsafe;

  f_read(file, (void *)mp3->tag, 3, &bytes_read);
  f_read(file, (void *)mp3->id3_version, 2, &bytes_read);
  f_read(file, (void *)&mp3->id3_flags, 1, &bytes_read);
  f_read(file, (void *)&id3_size_in_big_endian_synchsafe, 4, &bytes_read);

  get_id3_header_tag(file, mp3);

  id3_size_in_little_endian_synchsafe = big_endian_to_little_endian(id3_size_in_big_endian_synchsafe);
  mp3->id3_size_in_bytes = decode_synchsafe_integer(id3_size_in_little_endian_synchsafe) + 10;

  f_lseek(file, mp3->id3_size_in_bytes);

  return mp3->id3_size_in_bytes;
}