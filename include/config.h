// config.h

// ============== COMMON ==============

// ============== SICABS_OUTDOOR ==============
#ifdef SICABS_OUTDOOR
#ifdef SICABS_INDOOR
#error "Only one SICABS_XXX can be defined"
#endif

// Camera Settings
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

#endif

// ============== SICABS_INDOOR ==============
#ifdef SICABS_INDOOR
#ifdef SICABS_OUTDOOR
#error "Only one SICABS_XXX can be defined"
#endif

#define LED_BUILTIN 2

#endif
