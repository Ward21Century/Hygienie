
#ifdef CONFIG_MAX_OFFLINE_READINGS
#define MAX_OFFLINE_READINGS CONFIG_MAX_OFFLINE_READINGS
#else
#define MAX_OFFLINE_READINGS 10
#endif

#ifdef CONFIG_DEVICE_NAME
#define DEVICE_NAME CONFIG_DEVICE_NAME
#else
#define DEVICE_NAME "hospital"
#endif

#ifdef CONFIG_DEVICE_ID
#define DEVICE_ID CONFIG_DEVICE_ID
#else
#define DEVICE_ID "0001"
#endif
