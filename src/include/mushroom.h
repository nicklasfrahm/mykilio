#ifndef MUSHROOM_H
#define MUSHROOM_H

#include <stddef.h>
#include <stdint.h>

typedef enum mushroom_register {
  REG_NONE = 0x00,
  // Metadata registers.
  REG_ADRPTR = 0x01,
  // Status registers.
  REG_BMCCPU = 0x10,
  REG_BMCFWN,
  REG_BMCFWV,
  REG_BMCSTA,
  REG_SBCSTA,
  REG_PSUSTA,
  REG_FANDCC,
  REG_FANPWM,
  REG_FANMIN,
  REG_FANMAX,
  REG_DUTMIN,
  REG_DUTMAX,
  // Specification registers.
  REG_SBCPON = 0x30,
  REG_SBCSON,
  REG_FANMOD,
  REG_FANFDB,
  REG_FANSET,
  REG_DUTSET,
  // Telemetry registers.
  REG_BMCVOL = 0x50,
  REG_BMCCUR,
  REG_SBCVOL,
  REG_SBCCUR,
  REG_TMPAMB,
  REG_TMPPSU,
  REG_FANSPD,
  REG_FANDUT,
  // Action registers.
  REG_ACTPCY = 0x70,
  REG_ACTREB,
  REG_ACTENU,
} mushroom_register_t;

// Metadata register.
#define REG_ADRPTR_LEN (size_t)1
// Status registers.
#define REG_BMCCPU_LEN (size_t)16
#define REG_BMCFWN_LEN (size_t)16
#define REG_BMCFWV_LEN (size_t)16
#define REG_BMCSTA_LEN (size_t)1
#define REG_SBCSTA_LEN (size_t)1
#define REG_PSUSTA_LEN (size_t)1
#define REG_FANDCC_LEN (size_t)1
#define REG_FANPWM_LEN (size_t)1
#define REG_FANMIN_LEN (size_t)1
#define REG_FANMAX_LEN (size_t)1
#define REG_DUTMIN_LEN (size_t)1
#define REG_DUTMAX_LEN (size_t)1
// Specification registers.
#define REG_SBCPON_LEN (size_t)1
#define REG_SBCSON_LEN (size_t)1
#define REG_FANMOD_LEN (size_t)1
#define REG_FANFDB_LEN (size_t)1
#define REG_FANSET_LEN (size_t)1
#define REG_DUTSET_LEN (size_t)1
// Telemetry registers.
#define REG_BMCVOL_LEN (size_t)4
#define REG_BMCCUR_LEN (size_t)4
#define REG_SBCVOL_LEN (size_t)4
#define REG_SBCCUR_LEN (size_t)4
#define REG_TMPAMB_LEN (size_t)4
#define REG_TMPPSU_LEN (size_t)4
#define REG_FANSPD_LEN (size_t)1
#define REG_FANDUT_LEN (size_t)1
// Action registers.
#define REG_ACTPCY_LEN (size_t)1
#define REG_ACTREB_LEN (size_t)1
#define REG_ACTENU_LEN (size_t)1

// Returns the
size_t mushroom_register_len(mushroom_register_t address);

// Fan mode for the FANMOD register.
typedef enum mushroom_fan_mode {
  FANMOD_OFF,
  FANMOD_MANUAL,
  FANMOD_LINEAR,
  FANMOD_SQUARE_ROOT,
  FANMOD_QUADRATIC,
  FANMOD_EXPONENTIAL,
} mushroom_fan_mode_t;

// Fan feedback source for the FANFDB register.
typedef enum mushroom_fan_feedback {
  FANFDB_TMPAMB,
  FANFDB_TMPPSU,
  FANFDB_BMCCUR,
  FANFDB_SBCCUR,
} mushroom_fan_feedback_t;

// A datatype to parse floats from several bytes.
union mushroom_float_t {
  uint8_t bytes[4];
  float value;
};

// A cursor to manage the location of read and write operations via TWI.
typedef struct mushroom_cursor {
  mushroom_register_t address;
  size_t length;
  uint8_t index;
} mushroom_cursor_t;

// A macro to initialize a cursor with the default values.
#define MUSHROOM_CURSOR_INITIALIZER \
  { 0x00, 1, 0 }

// A method to initializes a cursor with the default values.
void mushroom_cursor_init(mushroom_cursor_t* cursor);

#endif
