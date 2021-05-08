#include "cursor.h"

void cursor_update(volatile cursor_t* cursor) {
  if (++cursor->index == cursor->length) {
    cursor->index = 0;
    cursor->idle = CURSOR_IDLE;
  }
}
