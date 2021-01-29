//
//  leds.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef leds_h
#define leds_h

#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>


enum led_brightness {
    LED_OFF        = 0,
    LED_ON        = 1,
    LED_HALF    = 127,
    LED_FULL    = 255,
};

struct led_classdev {
    const char        *name;
    enum led_brightness     brightness;
    enum led_brightness     max_brightness;
    int             flags;

    /* Lower 16 bits reflect status */
#define LED_SUSPENDED        BIT(0)
#define LED_UNREGISTERING    BIT(1)
    /* Upper 16 bits reflect control information */
#define LED_CORE_SUSPENDRESUME    BIT(16)
#define LED_SYSFS_DISABLE    BIT(17)
#define LED_DEV_CAP_FLASH    BIT(18)
#define LED_HW_PLUGGABLE    BIT(19)
#define LED_PANIC_INDICATOR    BIT(20)
#define LED_BRIGHT_HW_CHANGED    BIT(21)
#define LED_RETAIN_AT_SHUTDOWN    BIT(22)
#define LED_INIT_DEFAULT_TRIGGER BIT(23)

    /* set_brightness_work / blink_timer flags, atomic, private. */
    unsigned long        work_flags;

#define LED_BLINK_SW            0
#define LED_BLINK_ONESHOT        1
#define LED_BLINK_ONESHOT_STOP        2
#define LED_BLINK_INVERT        3
#define LED_BLINK_BRIGHTNESS_CHANGE     4
#define LED_BLINK_DISABLE        5

    /* Set LED brightness level
     * Must not sleep. Use brightness_set_blocking for drivers
     * that can sleep while setting brightness.
     */
    void        (*brightness_set)(struct led_classdev *led_cdev,
                      enum led_brightness brightness);
    /*
     * Set LED brightness level immediately - it can block the caller for
     * the time required for accessing a LED device register.
     */
    int (*brightness_set_blocking)(struct led_classdev *led_cdev,
                       enum led_brightness brightness);
    /* Get LED brightness level */
    enum led_brightness (*brightness_get)(struct led_classdev *led_cdev);

    /*
     * Activate hardware accelerated blink, delays are in milliseconds
     * and if both are zero then a sensible default should be chosen.
     * The call should adjust the timings in that case and if it can't
     * match the values specified exactly.
     * Deactivate blinking again when the brightness is set to LED_OFF
     * via the brightness_set() callback.
     */
    int        (*blink_set)(struct led_classdev *led_cdev,
                     unsigned long *delay_on,
                     unsigned long *delay_off);

    int (*pattern_set)(struct led_classdev *led_cdev,
               struct led_pattern *pattern, u32 len, int repeat);
    int (*pattern_clear)(struct led_classdev *led_cdev);

    struct device        *dev;
    const struct attribute_group    **groups;

    struct list_head     node;            /* LED Device list */
    const char        *default_trigger;    /* Trigger to use */

    unsigned long         blink_delay_on, blink_delay_off;
    struct timer_list     blink_timer;
    int             blink_brightness;
    int             new_blink_brightness;
    void            (*flash_resume)(struct led_classdev *led_cdev);

    struct work_struct    set_brightness_work;
    int            delayed_set_value;

#ifdef CONFIG_LEDS_TRIGGERS
    /* Protects the trigger data below */
    struct rw_semaphore     trigger_lock;

    struct led_trigger    *trigger;
    struct list_head     trig_list;
    void            *trigger_data;
    /* true if activated - deactivate routine uses it to do cleanup */
    bool            activated;
#endif

#ifdef CONFIG_LEDS_BRIGHTNESS_HW_CHANGED
    int             brightness_hw_changed;
    struct kernfs_node    *brightness_hw_changed_kn;
#endif

    /* Ensures consistent access to the LED Flash Class device */
    struct mutex        led_access;
};

static inline int led_classdev_register(struct device *parent,
                    struct led_classdev *led_cdev)
{
    return 0;
}

static void led_classdev_unregister(struct led_classdev *led_cdev)
{
    
}

#endif /* leds_h */
