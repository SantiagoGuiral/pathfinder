#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdio.h>
#include <pthread.h>

typedef struct {
  char Type[2];
} BMPSignature;

typedef struct {
  u_int32_t FileSize;
  u_int32_t Reserved;
  u_int32_t OffBits;
} BMPFileHeader;

typedef struct {
  u_int32_t HeaderSize;
  u_int32_t Width;
  u_int32_t Height;
  u_int16_t Planes;
  u_int16_t BPP;
  u_int32_t Compression;
  u_int32_t ImageSize;
  u_int32_t XPPM;
  u_int32_t YPPM;
  u_int32_t ClrUsed;
  u_int32_t ClrImportant;
} BMPImageHeader;

void createImage(const char* name, u_int32_t ImageWidth, u_int32_t ImageHeight);

#endif /* IMAGE_H_ */

