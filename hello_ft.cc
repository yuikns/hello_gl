#include <math.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <vector>

#include "argcv/cxx/str/str.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define WIDTH 640
#define HEIGHT 480

static char colors[4] = {'.', ';', '&', '@'};

inline char i2c(unsigned char i) {
  if (i < 64) {
    return colors[0];
  } else if (i < 128) {
    return colors[1];
  } else if (i < 192) {
    return colors[2];
  } else {
    return colors[3];
  }
}

// Example:
// bazel run :hello_ft $(pwd)/fonts/FantasqueSansMono-Regular.ttf Hello
// utf8_split starting..n-bloocks: 5
//                    .@@@;    .@@@;
//  &@    @&          .;;@;    .;;@;
//  &@    @&             @;       @;
//  &@    &&  .&@@&.     @;       @;     .&@@&.
//  &@ ...@&  &@&;@@.    @;       @;     &@;;@@.
//  &@@@@@@& .@;  .@;    @;       @;    .@;  .@.
//  &@;;;;@& .@....@&    @;       @;    ;@.   @;
//  &@    && ;@@@@@@&    @;       @;    ;@    @;
//  &@    && .@;;;;;.    @;       @;    ;@    @;
//  &@    && .@;   .     @;       @;    .@.  .@.
//  &@    &&  &@;;&@;    @&;;&.   @&;;&. &@;;@&
//  &@    &&  .&@@@;     ;@@@&.   ;@@@&. .&@@&.

// Execute:
// bazel run :hello_ft  $(pwd)/fonts/SourceHanSerifCN-Regular.ttf '我爱你'
// Expected:
// utf8_split starting..n-bloocks: 3
//                                .       .
//        .&;.@.            ..;;&@@.      @; ;@.
//     .;&@&;.@.&.    .&&&&@@&&;;;..     .@. &&
//   ;;&;@   .@..@.     ;. .&.  .@&.     ;& .@.     ;.
//      .@    @. &;     .@. ;@  ;&.      @; .@@@@@@@@;
//      .@    @.  .;  .. &. .& .&. ..   .@. &;  @. .@.
//  .@@@@@@@@@@@@@@@; .@@@@@@@@@@@@@@.  &@..&   @. ;.
//      .@    &; ..  .@;  .@;     .&.  .&@ &... @...
//      .@   .;& ;@. .&&@@@@@@@@@@@;  .&.@.. @&.@..&
//      .@;&;.;@.@;      .@;     .    ...@  .@. @. &;
//  .;;&@@..  .@&&       ;@@@@@@@@;     .@  ;&  @. .@.
//  .@&;.@     &@.      .@&.   .@&      .@ .@.  @.  &&
//   .  .@    .@@. ..   ;;.&. .&&.      .@ ;.   @.  .@
//      .@   ;&.&@.;.  ;&. .&&&&.       .@..    @.  ..
//    ...@ .;;. .&@@. ;;.  .&@@@&;;...  .@   .;;@.
//    .;@@.;.    .&@;...;;&&.  .;&@@&.  .@    .@&
//      ..         ..  ...         ..

// origin is the upper left corner
static unsigned char image[HEIGHT][WIDTH];

// Replace this function with something useful.

void draw_bitmap(FT_Bitmap* bitmap, FT_Int x, FT_Int y) {
  FT_Int i, j, p, q;
  FT_Int x_max = x + bitmap->width;
  FT_Int y_max = y + bitmap->rows;

  for (i = x, p = 0; i < x_max; i++, p++) {
    for (j = y, q = 0; j < y_max; j++, q++) {
      if (i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT) continue;
      image[j][i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}

void show_image(void) {
  int i, j;
  int xlb = 0;
  int xub = WIDTH;
  int ylb = 0;
  int yub = HEIGHT;

  for (i = 0; i < WIDTH; i++) {
    int flg = false;
    for (j = 0; j < HEIGHT; j++) {
      // h - w
      if (image[j][i] != 0) flg = true;
    }
    if (flg == true) {
      break;
    } else {
      xlb = i;
    }
  }

  for (i = WIDTH; i > 0; i--) {
    int flg = false;
    for (j = 0; j < HEIGHT; j++) {
      if (image[j][i] != 0) flg = true;
    }
    if (flg == true) {
      break;
    } else {
      xub = i;
    }
  }

  for (i = 0; i < HEIGHT; i++) {
    int flg = false;
    for (j = 0; j < WIDTH; j++) {
      if (image[i][j] != 0) flg = true;
    }
    if (flg == true) {
      break;
    } else {
      ylb = i;
    }
  }

  for (i = HEIGHT; i > 0; i--) {
    int flg = false;
    for (j = 0; j < WIDTH; j++) {
      if (image[i][j] != 0) flg = true;
    }
    if (flg == true) {
      break;
    } else {
      yub = i;
    }
  }

  for (i = ylb; i < yub; i++) {
    for (j = xlb; j < xub; j++) {
      // h - w
      putchar(image[i][j] == 0 ? ' ' : i2c(image[i][j]));
    }
    putchar('\n');
  }
}

int main(int argc, char** argv) {
  FT_Library library;
  FT_Face face;

  FT_GlyphSlot slot;
  FT_Matrix matrix; /* transformation matrix */
  FT_Vector pen;    /* untransformed origin  */
  FT_Error error;

  char* filename;
  char* text;

  double angle;
  int target_height;

  if (argc < 3) {
    fprintf(stderr, "usage: %s font sample-text\n", argv[0]);
    fflush(nullptr);
    return -1;
  }

  filename = argv[1]; /* first argument     */
  text = argv[2];     /* second argument    */
  // angle = (25.0 / 360) * 3.14159 * 2; /* use 25 degrees     */
  // angle = (5.0 / 360) * 3.14159 * 2; /* use 25 degrees     */
  angle = (0 / 360) * 3.14159 * 2; /* use 0 degrees     */
  target_height = HEIGHT;

  error = FT_Init_FreeType(&library); /* initialize library */
  /* error handling omitted */

  error = FT_New_Face(library, filename, 0, &face); /* create face object */
  /* error handling omitted */

  /* use 50pt at 100dpi */
  // error = FT_Set_Char_Size(face, 50 * 64, 0, 100, 0); /* set character size
  // */
  error = FT_Set_Char_Size(face, 8 * 64, 0, 150, 0); /* set character size */
  /* error handling omitted */

  slot = face->glyph;

  /* set up matrix */
  matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
  matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
  matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
  matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
  pen.x = 300 * 64;
  pen.y = (target_height - 200) * 64;

  printf("utf8_split starting..");
  fflush(nullptr);
  std::vector<uint64_t> blocks = argcv::Utf8ToUnicode(text);
  printf("n-bloocks: %lu", blocks.size());
  for (FT_ULong block : blocks) {
    FT_Set_Transform(face, &matrix, &pen);

    /* load glyph image into the slot (erase previous one) */
    // error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
    error = FT_Load_Char(face, block, FT_LOAD_RENDER);

    if (error) continue; /* ignore errors */

    /* now, draw to our target surface (convert position) */
    draw_bitmap(&slot->bitmap, slot->bitmap_left,
                target_height - slot->bitmap_top);

    /* increment pen position */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }

  show_image();

  FT_Done_Face(face);
  FT_Done_FreeType(library);

  return 0;
}
