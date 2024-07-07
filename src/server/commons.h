#pragma once
#define TOTAL_ENCODERS 2

typedef struct pid_s
{
    int speed;
    int old_cnt;
    int target_speed;
    int integral_err;
    int output;
} pid_t;

typedef struct state_s {
  pid_t pid;
  int counter;

  // val for index 0-4
  uint8_t prev_value;
  uint8_t curr_value;

  // index of {53-50}
  uint8_t msk_0;
  uint8_t msk_1;
} state_t;



// typedef struct enc_s {
//   int counter;

//   // val for index 0-4
//   uint8_t prev_value;
//   uint8_t curr_value;

//   // index of {53-50}
//   uint8_t msk_0;
//   uint8_t msk_1;
// } enc_t;

// typedef struct state_s
// {
//     enc_t enc;
//     int old_cnt;
//     int speed;
//     int target_speed;
//     int integral_err;
//     int output_pid;
// } state_t;