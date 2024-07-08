#pragma once
#define RIGHT_IDX 0
#define LEFT_IDX 1
#define TOTAL_ENCODERS 2
#define TOTAL_WHEELS 2

typedef struct enc_s {
  int counter;

  // val for index 0-4
  uint8_t prev_value;
  uint8_t curr_value;

  // index of {53-50}
  uint8_t msk_0;
  uint8_t msk_1;
} enc_t;

typedef struct state_s
{
    enc_t enc;
    int old_cnt;
    int delta_enc;
    int target_delta_enc;
    int integral_err;
    int output_pid;
} state_t;