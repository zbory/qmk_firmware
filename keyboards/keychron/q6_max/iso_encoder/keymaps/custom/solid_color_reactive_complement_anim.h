#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
#   ifdef ENABLE_RGB_MATRIX_SOLID_COLOR_REACTIVE_COMPLEMENT
    RGB_MATRIX_EFFECT(SOLID_COLOR_REACTIVE_COMPLEMENT)
#       ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

    bool SOLID_COLOR_REACTIVE_COMPLEMENT(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    RGB rgb = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);

    uint16_t max_tick = 65535 / qadd8(rgb_matrix_config.speed, 1);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        bool led_processed = false;
        uint16_t tick = max_tick;
        // Reverse search to find most recent key hit
        for (int8_t j = g_last_hit_tracker.count - 1; j >= 0; j--) {
            if (g_last_hit_tracker.index[j] == i && g_last_hit_tracker.tick[j] < tick)
            {
                tick = g_last_hit_tracker.tick[j];
                uint16_t offset = scale16by8(tick, qadd8(rgb_matrix_config.speed, 1));

                uint8_t r_comp = 255 - rgb.r;
                uint8_t g_comp = 255 - rgb.g;
                uint8_t b_comp = 255 - rgb.b;
                fract8 scale = scale8(qsub8((255 - offset), 0), 255);

                rgb_matrix_set_color(i, lerp8by8(rgb.r, r_comp, scale), lerp8by8(rgb.g, g_comp, scale), lerp8by8(rgb.b, b_comp, scale));
                led_processed = true;
                break;
            }
        }

        if(led_processed)
            continue;

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#       endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#   endif  // ENABLE_RGB_MATRIX_SOLID_COLOR_REACTIVE_COMPLEMENT
#endif  // RGB_MATRIX_KEYREACTIVE_ENABLED
