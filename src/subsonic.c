#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include "magic.h"

uint32_t fread_uint32(FILE *res) {
  uint32_t data;

  fread(&data, sizeof(uint32_t), 1, res);

  if (ferror(res)) {
    printf("resource is corrupt\n");
  }

  data = le32toh(data);

  return data;
}

int fread_varint(FILE *res) {
  int d = fgetc(res);

  if (ferror(res)) {
    printf("resource is corrupt\n");
  }

  if (d < 128) {
    return d;
  }
  else {
    return ((d - 128) << 7) + fread_varint(res);
  }
}

void unpack_resource(FILE *res) {
  uint32_t fmagic;
  uint32_t fsize;
  uint32_t ftype;

  uint32_t smagic;
  int ssize;
  uint8_t *sdata;

  fmagic = fread_uint32(res);
  fsize = fread_uint32(res);
  ftype = fread_uint32(res);

  if (fmagic != RIFX_FILE) {
    perror("Bad file");
    exit(1);
  }

  if (fmagic != RIFX_FILE) {
    perror("bad resource file");
    exit(1);
  }

  switch (ftype) {
  case FTYPE_DCR:
    printf("file [magic=RIFX, size=%d, type=DCR]\n", fsize);        
    break;
  case FTYPE_CCT:
    printf("file [magic=RIFX, size=%d, type=CCT]\n", fsize);        
    break;
  case FTYPE_CST:
    printf("file [magic=RIFX, size=%d, type=CST]\n", fsize);        
    break;
  default:
    perror("unknown resource type");
    exit(1);
  }

  for (int i = 0; i < 5; i++) {
    smagic = fread_uint32(res);
    ssize = fread_varint(res);
    
    if (ssize == 0) {
      break;
    }

    sdata = malloc(ssize);

    if (sdata == NULL) {
      printf("could not allocate memory for section\n");
    }
    else {
      fread(sdata, ssize, 1, res);

      if (ferror(res)) {
	printf("resource is corrupt\n");
      }

      switch (smagic) {
      case STYPE_FVER:
	printf("sect [magic=Fver, size=%d]\n", ssize);
	break;
      case STYPE_FCDR:
	printf("sect [magic=Fcdr, size=%d]\n", ssize);
	break;
      case STYPE_FGEI:
	printf("sect [magic=FGEI, size=%d]\n", ssize);
	break;
      case STYPE_ABMP:
	printf("sect [magic=ABMP, size=%d]\n", ssize);
	break;
      default:
	printf("sect [magic=0x%x, size=%d]\n", smagic, ssize);
      }

      free(sdata);
      sdata = NULL;
    }
  }
}

int main(int argc, char **argv) {
  /* reopen stdin in binary mode */
  freopen(NULL, "rb", stdin);

  unpack_resource(stdin);

  return 0;
}
