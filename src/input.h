unsigned int input_read_keys_2player(void);
unsigned int input_read_keys_1player(void);

typedef struct CPUPlayer
{
  int target_angle;
  int target_power;
} CPUPlayer;

extern CPUPlayer cpu_player;
