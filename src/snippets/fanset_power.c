#include <math.h>
#include <stdio.h>

int calc_fanset(float setpoint, float sensor_max) {
  float setpoint_percent = setpoint / sensor_max;
  return (int)floor(setpoint_percent * 255);
}

int main(void) {
  float current_setpoint = 1.5;
  float sensor_max = 5;
  printf("Full-speed power: %.1fW\n", current_setpoint * 5);
  printf("Fan setpoint: %d\n", calc_fanset(current_setpoint, sensor_max));

  return 0;
}
