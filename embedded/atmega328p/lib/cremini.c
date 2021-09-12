#include "cremini.h"

inline size_t cremini_register_len(cremini_register_t address) {
  switch (address) {
    case REG_BMCCPU:
    case REG_BMCFWN:
    case REG_BMCFWV: {
      return (size_t)16;
    }
    case REG_BMCVOL:
    case REG_BMCCUR:
    case REG_SBCVOL:
    case REG_SBCCUR:
    case REG_TMPAMB:
    case REG_TMPPSU: {
      return (size_t)4;
    }
    default: {
      return (size_t)1;
    }
  }
}
