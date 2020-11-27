// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright 2006, Johannes Berg <johannes@sipsolutions.net>
 */

/* just for IFNAMSIZ */
#include <linux/if.h>
#include <linux/slab.h>
#include <linux/export.h>
#include "led.h"


static int ieee80211_tx_led_activate(struct led_classdev *led_cdev)
{
//    struct ieee80211_local *local = container_of(led_cdev->trigger,
//                             struct ieee80211_local,
//                             tx_led);
//
//    atomic_inc(&local->tx_led_active);

    return 0;
}

static void ieee80211_tx_led_deactivate(struct led_classdev *led_cdev)
{
//    struct ieee80211_local *local = container_of(led_cdev->trigger,
//                             struct ieee80211_local,
//                             tx_led);
//
//    atomic_dec(&local->tx_led_active);
}

static int ieee80211_rx_led_activate(struct led_classdev *led_cdev)
{
//    struct ieee80211_local *local = container_of(led_cdev->trigger,
//                             struct ieee80211_local,
//                             rx_led);
//
//    atomic_inc(&local->rx_led_active);

    return 0;
}

static void ieee80211_rx_led_deactivate(struct led_classdev *led_cdev)
{
//    struct ieee80211_local *local = container_of(led_cdev->trigger,
//                             struct ieee80211_local,
//                             rx_led);
//
//    atomic_dec(&local->rx_led_active);
}

static int ieee80211_assoc_led_activate(struct led_classdev *led_cdev)
{
//    struct ieee80211_local *local = container_of(led_cdev->trigger,
//                             struct ieee80211_local,
//                             assoc_led);
//
//    atomic_inc(&local->assoc_led_active);

    return 0;
}

static void ieee80211_assoc_led_deactivate(struct led_classdev *led_cdev)
{
//    struct ieee80211_local *local = container_of(led_cdev->trigger,
//                             struct ieee80211_local,
//                             assoc_led);
//
//    atomic_dec(&local->assoc_led_active);
}

static int ieee80211_radio_led_activate(struct led_classdev *led_cdev)
{
//    struct ieee80211_local *local = container_of(led_cdev->trigger,
//                             struct ieee80211_local,
//                             radio_led);
//
//    atomic_inc(&local->radio_led_active);

    return 0;
}

static void ieee80211_radio_led_deactivate(struct led_classdev *led_cdev)
{
//    struct ieee80211_local *local = container_of(led_cdev->trigger,
//                             struct ieee80211_local,
//                             radio_led);
//
//    atomic_dec(&local->radio_led_active);
}

static int ieee80211_tpt_led_activate(struct led_classdev *led_cdev)
{
//    struct ieee80211_local *local = container_of(led_cdev->trigger,
//                             struct ieee80211_local,
//                             tpt_led);
//
//    atomic_inc(&local->tpt_led_active);

    return 0;
}

static void ieee80211_tpt_led_deactivate(struct led_classdev *led_cdev)
{
//    struct ieee80211_local *local = container_of(led_cdev->trigger,
//                             struct ieee80211_local,
//                             tpt_led);
//
//    atomic_dec(&local->tpt_led_active);
}


const char *__ieee80211_get_radio_led_name(struct ieee80211_hw *hw)
{
//    struct ieee80211_local *local = hw_to_local(hw);
//
//    return local->radio_led.name;
    
    return NULL;
}
EXPORT_SYMBOL(__ieee80211_get_radio_led_name);

const char *__ieee80211_get_assoc_led_name(struct ieee80211_hw *hw)
{
//    struct ieee80211_local *local = hw_to_local(hw);
//
//    return local->assoc_led.name;
    return NULL;
}
EXPORT_SYMBOL(__ieee80211_get_assoc_led_name);

const char *__ieee80211_get_tx_led_name(struct ieee80211_hw *hw)
{
//    struct ieee80211_local *local = hw_to_local(hw);
//
//    return local->tx_led.name;
    return NULL;
}
EXPORT_SYMBOL(__ieee80211_get_tx_led_name);

const char *__ieee80211_get_rx_led_name(struct ieee80211_hw *hw)
{
//    struct ieee80211_local *local = hw_to_local(hw);
//
//    return local->rx_led.name;
    return NULL;
}
EXPORT_SYMBOL(__ieee80211_get_rx_led_name);

static unsigned long tpt_trig_traffic(struct ieee80211_local *local,
                      struct tpt_led_trigger *tpt_trig)
{
//    unsigned long traffic, delta;
//
//    traffic = tpt_trig->tx_bytes + tpt_trig->rx_bytes;
//
//    delta = traffic - tpt_trig->prev_traffic;
//    tpt_trig->prev_traffic = traffic;
//    return DIV_ROUND_UP(delta, 1024 / 8);
    return 0;
}

static void tpt_trig_timer(struct timer_list *t)
{
//    struct tpt_led_trigger *tpt_trig = from_timer(tpt_trig, t, timer);
//    struct ieee80211_local *local = tpt_trig->local;
//    struct led_classdev *led_cdev;
//    unsigned long on, off, tpt;
//    int i;
//
//    if (!tpt_trig->running)
//        return;
//
//    mod_timer(&tpt_trig->timer, round_jiffies(jiffies + HZ));
//
//    tpt = tpt_trig_traffic(local, tpt_trig);
//
//    /* default to just solid on */
//    on = 1;
//    off = 0;
//
//    for (i = tpt_trig->blink_table_len - 1; i >= 0; i--) {
//        if (tpt_trig->blink_table[i].throughput < 0 ||
//            tpt > tpt_trig->blink_table[i].throughput) {
//            off = tpt_trig->blink_table[i].blink_time / 2;
//            on = tpt_trig->blink_table[i].blink_time - off;
//            break;
//        }
//    }
//
//    read_lock(&local->tpt_led.leddev_list_lock);
//    list_for_each_entry(led_cdev, &local->tpt_led.led_cdevs, trig_list)
//        led_blink_set(led_cdev, &on, &off);
//    read_unlock(&local->tpt_led.leddev_list_lock);
}

const char *
__ieee80211_create_tpt_led_trigger(struct ieee80211_hw *hw,
                   unsigned int flags,
                   const struct ieee80211_tpt_blink *blink_table,
                   unsigned int blink_table_len)
{
//    struct ieee80211_local *local = hw_to_local(hw);
//    struct tpt_led_trigger *tpt_trig;
//
//    if (WARN_ON(local->tpt_led_trigger))
//        return NULL;
//
//    tpt_trig = kzalloc(sizeof(struct tpt_led_trigger), GFP_KERNEL);
//    if (!tpt_trig)
//        return NULL;
//
//    snprintf(tpt_trig->name, sizeof(tpt_trig->name),
//         "%stpt", wiphy_name(local->hw.wiphy));
//
//    local->tpt_led.name = tpt_trig->name;
//
//    tpt_trig->blink_table = blink_table;
//    tpt_trig->blink_table_len = blink_table_len;
//    tpt_trig->want = flags;
//    tpt_trig->local = local;
//
//    timer_setup(&tpt_trig->timer, tpt_trig_timer, 0);
//
//    local->tpt_led_trigger = tpt_trig;
//
//    return tpt_trig->name;
    return NULL;
}
EXPORT_SYMBOL(__ieee80211_create_tpt_led_trigger);

static void ieee80211_start_tpt_led_trig(struct ieee80211_local *local)
{
//    struct tpt_led_trigger *tpt_trig = local->tpt_led_trigger;
//
//    if (tpt_trig->running)
//        return;
//
//    /* reset traffic */
//    tpt_trig_traffic(local, tpt_trig);
//    tpt_trig->running = true;
//
//    tpt_trig_timer(&tpt_trig->timer);
//    mod_timer(&tpt_trig->timer, round_jiffies(jiffies + HZ));
}

static void ieee80211_stop_tpt_led_trig(struct ieee80211_local *local)
{
//    struct tpt_led_trigger *tpt_trig = local->tpt_led_trigger;
//    struct led_classdev *led_cdev;
//
//    if (!tpt_trig->running)
//        return;
//
//    tpt_trig->running = false;
//    del_timer_sync(&tpt_trig->timer);
//
//    read_lock(&local->tpt_led.leddev_list_lock);
//    list_for_each_entry(led_cdev, &local->tpt_led.led_cdevs, trig_list)
//        led_set_brightness(led_cdev, LED_OFF);
//    read_unlock(&local->tpt_led.leddev_list_lock);
}
