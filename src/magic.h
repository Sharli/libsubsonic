#ifndef _MAGIC_H
#define _MAGIC_H

/**
 * Adobe Shockwave resource files (movies, casts, etc)
 * always begin with the same magic number at the
 * beginning of the file.
 */

#define RIFX_FILE  0x52494658

/**
 * Resources have additional associated filetype magic
 * numbers that identify the type of resource
 * (e.g. DCR or CST).
 */

#define FTYPE_DCR  0x4647444d
#define FTYPE_CCT  0x46474443
#define FTYPE_CST  0x4d563933

/**
 * Each resource is divided up into sections, which are
 * prefixed by a magic number cooresponding with the
 * section type.
 *
 * Much of the following is guesswork, and could be wrong.
 */

#define STYPE_FVER 0x46766572 /* file version information */
#define STYPE_FCDR 0x46636472
#define STYPE_FGEI 0x46474549
#define STYPE_ABMP 0x41424d50

#endif
