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

int vox_set_autostop(int slot, int stop) {
  return sv_set_autostop(slot, stop);
}

int vox_play_from_beginning(int slot) {
  return sv_play_from_beginning(slot);
}

int vox_stop(int slot) {
  return sv_stop(slot);
}

int vox_end_of_song(int slot) {
  return sv_end_of_song(slot);
}

int vox_rewind(int slot, int t) {
  return sv_rewind(slot, t);
}

const char* vox_get_song_name(int slot) {
  return sv_get_song_name(slot);
}

int vox_send_event(int slot, int channel_num, int note, int vel, int module, int ctl, int ctl_val ) {
  return sv_send_event(slot, channel_num, note, vel, module, ctl, ctl_val);
}

int vox_get_current_signal_level(int slot, int channel) {
  return sv_get_current_signal_level(slot, channel);
}

int vox_get_song_bpm(int slot) {
  return sv_get_song_bpm(slot);
}

int vox_get_song_tpl(int slot) {
  return sv_get_song_tpl(slot);
}

int vox_get_song_length_frames(int slot) {
  return sv_get_song_length_frames(slot);
}

int vox_get_song_length_lines(int slot) {
  return sv_get_song_length_lines(slot);
}
