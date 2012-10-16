#include "./data/headers/sunvox.h"

int vox_init(char* dev, int freq, int channels, int flags) {
  return sv_init(dev, freq, channels, flags);
}

int vox_deinit() {
  return sv_deinit();
}

int vox_get_sample_type() {
  return sv_get_sample_type();
}

int vox_open_slot(int slot) {
  return sv_open_slot(slot);
}

int vox_close_slot(int slot) {
  return sv_close_slot(slot);
}

int vox_load(int slot, char* name) {
 return sv_load(slot, name);
}

int vox_volume(int slot, int vol) {
  return sv_volume(slot, vol);
}

int vox_play(int slot) {
  return sv_play(slot);
}

int vox_get_current_line(int slot) {
  return sv_get_current_line(slot);
}

