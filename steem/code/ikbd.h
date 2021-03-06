#ifdef IN_EMU
#define EXT
#define INIT(s) =s
#else
#define EXT extern
#define INIT(s)
#endif

extern BYTE key_table[256];

#define IKBD_HBLS_FROM_COMMAND_WRITE_TO_PROCESS 5

#define IKBD_DEFAULT_MOUSE_MOVE_MAX 15 // 5 for X-Out
#define IKBD_RESET_MESSAGE 0xf1  // 0xf0 in docs

EXT void keyboard_buffer_write(BYTE);
EXT void keyboard_buffer_write_n_record(BYTE);
EXT void keyboard_buffer_write_string(int s1,...);
EXT bool ikbd_keys_disabled();
EXT void ikbd_mouse_move(int,int,int,int DEFVAL(IKBD_DEFAULT_MOUSE_MOVE_MAX));

EXT bool ST_Key_Down[128];
EXT int disable_input_vbl_count INIT(0);
EXT int ikbd_joy_poll_line INIT(0),ikbd_mouse_poll_line INIT(0),ikbd_key_poll_line INIT(0);

#define MAX_KEYBOARD_BUFFER_SIZE 1024
EXT BYTE keyboard_buffer[MAX_KEYBOARD_BUFFER_SIZE];
EXT int keyboard_buffer_length;

EXT int mouse_speed INIT(10);

#ifdef WIN32
// When task switching is turned off we have to manually update these keys at the VBL
extern BYTE TaskSwitchVKList[4];
extern bool CutTaskSwitchVKDown[4];
#endif

#define STKEY_PAD_DIVIDE 0x65
#define STKEY_PAD_ENTER 0x72

EXT int mouse_move_since_last_interrupt_x,mouse_move_since_last_interrupt_y;
EXT bool mouse_change_since_last_interrupt;
EXT int mousek;

#ifndef CYGWIN
EXT int no_set_cursor_pos INIT(0);
#else
EXT int no_set_cursor_pos INIT(true);
#endif

#ifdef IN_EMU

BYTE key_table[256]={
//         $.0  $.1  $.2  $.3  $.4  $.5  $.6  $.7  $.8  $.9  $.a  $.b  $.c  $.d  $.e  $.f
/* $0. */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x0f,0x00,0x00,0x00,0x1c,0x00,0x00,
/* $1. */ 0x2a,0x1d,0x38,0x00,0x3a,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
/* $2. */ 0x39,0x62,0x61,0x00,0x47,0x4b,0x48,0x4d,0x50,0x00,0x00,0x00,0x00,0x52,0x53,0x62,
//    PageUp=Help^    ^Page Down=Undo
/* $3. */ 0x0b,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,
/* $4. */ 0x00,0x1e,0x30,0x2e,0x20,0x12,0x21,0x22,0x23,0x17,0x24,0x25,0x26,0x32,0x31,0x18,
/* $5. */ 0x19,0x10,0x13,0x1f,0x14,0x16,0x2f,0x11,0x2d,0x15,0x2c,0x00,0x00,0x00,0x00,0x00,
/* $6. */ 0x70,0x6d,0x6e,0x6f,0x6a,0x6b,0x6c,0x67,0x68,0x69,0x66,0x4e,0x00,0x4a,0x71,0x65,
/* $7. */ 0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x63,0x64,0x00,0x00,0x00,0x00,
//                                                           ^F11 ^F12 (numpad brackets)
/* $8. */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* $9. */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* $a. */ 0x2a,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//        ^ Left/Right distinguishing constants (doesn't work for most things that take VK codes)
/* $b. */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* $c. */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* $d. */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* $e. */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* $f. */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
                    };

#ifdef WIN32
// When task switching is turned off we have to manually update these keys at the VBL
BYTE TaskSwitchVKList[4]={VK_ESCAPE,VK_TAB,VK_DELETE,0};
bool CutTaskSwitchVKDown[4]={0,0,0,0};
#endif

void IKBD_VBL();
void agenda_ikbd_process(int);  //intelligent keyboard handle byte
void ikbd_run_start(bool);
void ikbd_reset(bool);

void agenda_keyboard_reset(int);
void ikbd_report_abs_mouse(int);
void ikbd_send_joystick_message(int);

#define IKBD_MOUSE_MODE_ABSOLUTE 0x9
#define IKBD_MOUSE_MODE_RELATIVE 0x8
#define IKBD_MOUSE_MODE_CURSOR_KEYS 0xa
#define IKBD_MOUSE_MODE_OFF 0x12

#define IKBD_JOY_MODE_OFF 0x1a
#define IKBD_JOY_MODE_ASK 0x15
#define IKBD_JOY_MODE_AUTO_NOTIFY 0x14
#define IKBD_JOY_MODE_CURSOR_KEYS 0x19
#define IKBD_JOY_MODE_DURATION 100
#define IKBD_JOY_MODE_FIRE_BUTTON_DURATION 101

#define IKBD_SCANLINES_FROM_ABS_MOUSE_POLL_TO_SEND int((MONO) ? 50:30)
#define IKBD_SCANLINES_FROM_JOY_POLL_TO_SEND int((MONO) ? 32:20)

#define RMB_DOWN(mk) (mk & 1)
#define LMB_DOWN(mk) (mk & 2)

#define BIT_RMB BIT_0
#define BIT_LMB BIT_1

const int ikbd_clock_days_in_mon[13]={0,31,28,31,30,/*MAY!*/31,30,31,31,30,31,30,31};
const int ikbd_clock_max_val[6]={99,12,0,23,59,59};

struct _IKBD_STRUCT{
  int command_read_count,command_parameter_counter;
  BYTE command;
  BYTE command_param[8];
  BYTE mouse_button_press_what_message;
  int mouse_mode;
  int joy_mode;
  int abs_mouse_max_x,abs_mouse_max_y;
  int cursor_key_mouse_pulse_count_x,cursor_key_mouse_pulse_count_y;
  int relative_mouse_threshold_x,relative_mouse_threshold_y;
  int abs_mouse_scale_x,abs_mouse_scale_y;
  int abs_mouse_x,abs_mouse_y;
  bool mouse_upside_down;
  bool send_nothing;
  int duration;
  BYTE clock[6];
  DWORD cursor_key_joy_time[6];
  DWORD cursor_key_joy_ticks[4];
  bool port_0_joy;
  int abs_mousek_flags;
  bool resetting;
  int psyg_hack_stage;
  int clock_vbl_count;
  WORD load_memory_address;
  BYTE ram[128];
  int reset_121A_hack;
  int reset_0814_hack;
  int reset_1214_hack;
  int joy_packet_pos;
  int mouse_packet_pos;
}ikbd;

BYTE keyboard_buffer_read();

#endif

#undef EXT
#undef INIT

