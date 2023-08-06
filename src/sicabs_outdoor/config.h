// config.h

// Camera Model
#define CAMERA_MODEL_AI_THINKER

#ifdef CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#define LED_FLASH_GPIO_NUM 4
#define LED_BUILTIN 33
#else
#error "Camera model not selected"
#endif

// Keypad
#define KEYPAD_ADDR 0x38

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 3
char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte rowPins[KEYPAD_ROWS] = {2, 7, 6, 4};  // TODO: Verify these pins and index order
byte colPins[KEYPAD_COLS] = {3, 1, 5};

#define KEYPAD_DEBOUNCE_TIME_MS 10  // Default by library is 10
#define KEYPAD_HOLD_TIME_MS 500     // Default by library is 500
