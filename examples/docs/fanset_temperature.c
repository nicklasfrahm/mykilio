#include <math.h>
#include <stdio.h>

int calc_fanset(float setpoint, float sensor_max) {
  float setpoint_percent = setpoint / sensor_max;
  return (int)floor(setpoint_percent * 255);
}

int main(void) {
  float temperature_setpoint = 40;
  float sensor_max = 100;
  printf("Full-speed temperature: %.0f°C\n", temperature_setpoint);
  printf("Fan setpoint: %d\n", calc_fanset(temperature_setpoint, sensor_max));

  return 0;
}
