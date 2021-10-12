#include <stdlib.h>

#include "image.h"

void createImage(const char* name, u_int32_t ImageWidth, u_int32_t ImageHeight){
  // init structs
  BMPSignature BMP_S;
  BMPFileHeader BMP_FH;
  BMPImageHeader BMP_IH;

  BMP_S.Type[0] = 'B';
  BMP_S.Type[1] = 'M';

  // calculate padding
  u_int8_t padding = (ImageWidth * 3) % 4;

  BMP_FH.FileSize = (ImageWidth * 3 + padding) * ImageHeight + 54;
  BMP_FH.Reserved = 0;
  BMP_FH.OffBits = 54;

  BMP_IH.HeaderSize = 40;
  BMP_IH.Width = ImageWidth;
  BMP_IH.Height = ImageHeight;
  BMP_IH.Planes = 1;
  BMP_IH.BPP = 24;
  BMP_IH.Compression = 0;
  BMP_IH.ImageSize = 0;
  BMP_IH.XPPM = 0;
  BMP_IH.YPPM = 0;
  BMP_IH.ClrUsed = 0;
  BMP_IH.ClrImportant = 0;

  // create File
  FILE* nFile;
  nFile = fopen(name, "wb");

  fwrite(&BMP_S, sizeof(BMP_S), 1, nFile);
  fwrite(&BMP_FH, sizeof(BMP_FH), 1, nFile);
  fwrite(&BMP_IH, sizeof(BMP_IH), 1, nFile);

  // color values
  u_int8_t red;
  u_int8_t green;
  u_int8_t blue;

  // painting
  for(u_int32_t y = 0; y < ImageHeight; y++) {
    for(u_int32_t x = 0; x < ImageWidth; x++) {
      red = 255 * x / ImageWidth;
      blue = 255 * y / ImageHeight;
      green = 255 * x * y / (ImageHeight * ImageWidth);

      if(x < ImageWidth * 0.8 && x > ImageWidth * 0.2) {
        if(y < ImageHeight * 0.6 && y > ImageHeight * 0.4) {
          blue = 255 * x / ImageWidth;
          red = 255 * y / ImageHeight;
        }
      }

      fputc(blue, nFile);
      fputc(green, nFile);
      fputc(red, nFile);
    }
    if(padding) {
      for(u_int8_t i = 0; i < padding; i++) {
        fputc(0, nFile);
      }
    }
  }
  // close File
  fclose(nFile);
}
