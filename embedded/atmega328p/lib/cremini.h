#ifndef CREMINI_H
#define CREMINI_H

#include <stddef.h>
#include <stdint.h>

// The address of the first metadata register.
#define REG_METADATA 0x00
// The address of the first status register.
#define REG_STATUS 0x10
// The address of the first specification register.
#define REG_SPECIFICATION 0x30
// The address of the first telemetry register.
#define REG_TELEMETRY 0x50
// The address of the first action register.
#define REG_ACTION 0x70

typedef enum cremini_register {
  // Metadata registers.
  REG_ADRPTR = REG_METADATA,
  // Status registers.
  REG_BMCSTA = REG_STATUS,
  REG_SBCSTA,
  REG_PSUSTA,
  REG_FANDCC,
  REG_FANPWM,
  REG_FANMIN,
  REG_FANMAX,
  REG_DUTMIN,
  REG_DUTMAX,
  REG_BMCCPU,
  REG_BMCFWN,
  REG_BMCFWV,
  // Specification registers.
  REG_SBCPON = REG_SPECIFICATION,
  REG_SBCSON,
  REG_FANMOD,
  REG_FANFDB,
  REG_FANSET,
  REG_FANMAN,
  REG_LEDMOD,
  REG_LEDFDB,
  REG_LEDSET,
  REG_LEDMAN,
  // Telemetry registers.
  REG_FANSPD = REG_TELEMETRY,
  REG_FANDUT,
  REG_BMCVOL,
  REG_BMCCUR,
  REG_SBCVOL,
  REG_SBCCUR,
  REG_TMPAMB,
  REG_TMPPSU,
  // Action registers.
  REG_ACTPCY = REG_ACTION,
  REG_ACTREB,
  REG_ACTENU,
} cremini_register_t;

// IMPORTANT: If new registers are added,
// the values below need to be updated as well.
// These registers are important for the range
// checks to prevent memory leaks and segmentation
// faults.

#define REG_METADATA_LAST REG_ADRPTR
#define REG_STATUS_LAST REG_DUTMAX
#define REG_STATUS_STRING REG_BMCCPU
#define REG_STATUS_STRING_LAST REG_BMCFWV
#define REG_SPECIFICATION_LAST REG_LEDMAN
#define REG_TELEMETRY_LAST REG_FANDUT
#define REG_TELEMETRY_FLOAT REG_BMCVOL
#define REG_TELEMETRY_FLOAT_LAST REG_TMPPSU
#define REG_ACTION_LAST REG_ACTENU

#define IS_METADATA(address) \
  (address >= REG_METADATA && address <= REG_METADATA_LAST)
#define IS_STATUS(address) (address >= REG_STATUS && address <= REG_STATUS_LAST)
#define IS_STATUS_STRING(address) \
  (address >= REG_STATUS_STRING && address <= REG_STATUS_STRING_LAST)
#define IS_SPECIFICATION(address) \
  (address >= REG_SPECIFICATION && address <= REG_SPECIFICATION_LAST)
#define IS_TELEMETRY(address) \
  (address >= REG_TELEMETRY && address <= REG_TELEMETRY_LAST)
#define IS_TELEMETRY_FLOAT(address) \
  (address >= REG_TELEMETRY_FLOAT && address <= REG_TELEMETRY_FLOAT_LAST)
#define IS_ACTION(address) (address >= REG_ACTION && address <= REG_ACTION_LAST)

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

// Returns the length of a given register.
size_t cremini_register_len(cremini_register_t address);

// The control mode of a fan or an LED.
typedef enum cremini_control_mode {
  MOD_OFF,
  MOD_MANUAL,
  MOD_LINEAR,
  MOD_SQUARE_ROOT,
  MOD_QUADRATIC,
  MOD_EXPONENTIAL,
} cremini_control_mode_t;

// The feedback source for automatic control of a fan or an LED.
typedef enum cremini_feedback_source {
  FDB_TMPAMB,
  FDB_TMPPSU,
  FDB_BMCCUR,
  FDB_SBCCUR,
} cremini_feedback_source_t;

// A datatype to translate a uint8_t to bytes and vice-versa.
typedef union cremini_uint8 {
  uint8_t bytes[1];
  uint8_t value;
} cremini_uint8_t;

// A datatype to translate a 16 character char array to bytes and vice-versa.
typedef union cremini_string {
  uint8_t bytes[16];
  char value[16];
} cremini_string_t;

// A datatype to translate a 32-bit float to bytes and vice-versa.
typedef union cremini_float {
  uint8_t bytes[4];
  float value;
} cremini_float_t;

#endif
