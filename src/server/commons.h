/**************************************************\
 * @file commons.h                         
 *
 * @brief Header for common stuff
\**************************************************/
#pragma once
/**************************************************\
 * @brief The index of the right wheel
\**************************************************/
#define RIGHT_IDX 0
/**************************************************\
 * @brief The index of the left wheel
\**************************************************/
#define LEFT_IDX 1
/**************************************************\
 * @brief Total number of encoders
\**************************************************/
#define TOTAL_ENCODERS 2
/**************************************************\
 * @brief Total number of wheels
\**************************************************/
#define TOTAL_WHEELS 2

/**************************************************\
 * @brief Represents the status of an encoder
 * 
 * @param counter The counter of the encoder
 * @param prev_value The previous value read from pins
 * @param curr_value The current value read from pins
 * @param msk_0 The mask for the first pin
 * @param msk_1 The mask for the second pin
\**************************************************/
typedef struct enc_s {
  int counter;

  // val for index 0-4
  uint8_t prev_value;
  uint8_t curr_value;

  // index of {53-50}
  uint8_t msk_0;
  uint8_t msk_1;
} enc_t;

/**************************************************\
 * @brief Represents the state of a wheel
 * 
 * @param enc The encoder of the wheel
 * @param old_cnt The old counter of the encoder
 * @param delta_enc The change in encoder counter
 * @param target_delta_enc The target change in encoder counter
 * @param integral_err The integral error of the PID
 * @param output_pid The output of the PID
\**************************************************/
typedef struct state_s
{
    enc_t enc;
    int old_cnt;
    int delta_enc;
    int target_delta_enc;
    int integral_err;
    int output_pid;
} state_t;