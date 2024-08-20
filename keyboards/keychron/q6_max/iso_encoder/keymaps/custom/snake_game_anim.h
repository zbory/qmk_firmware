#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
#   ifdef ENABLE_RGB_MATRIX_SNAKE_GAME
    RGB_MATRIX_EFFECT(SNAKE_GAME)
#       ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

    #define GAME_MAP_ROWS 6
    #define GAME_MAP_COLS 14
    #define SNAKE_MAX_LEN 15

    typedef struct {
        uint8_t row;
        uint8_t col;
    } snake_part_pos;

    enum direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    snake_part_pos snake[SNAKE_MAX_LEN];
    uint8_t direction;
    uint32_t lastGameTickTime;
    snake_part_pos head = {5,4}; // G
    snake_part_pos food = {4, 9};
    uint8_t length = 1;
    uint8_t death = 0;

    void init_game(void)
    {
        snake_part_pos temp = {5,4};
        snake[0] = temp;
        length = 1;
        direction = LEFT;
        death = 0;
    }

    bool collision(snake_part_pos pos)
    {
        for(uint8_t i = 1; i < length; i++)
        {
            if(snake[i].row == pos.row && snake[i].col == pos.col)
                return true;
        }

        return false;
    }

    void move_snake(void)
    {
        uint8_t mod_col = GAME_MAP_COLS;

        if(head.row == 0 || head.row == 4) mod_col = 13;
        if(head.row == 5) mod_col = 8;

        switch (direction)
        {
            case UP:
                head.row = (head.row - 1) % GAME_MAP_ROWS;
                break;
            case DOWN:
                head.row = (head.row + 1) % GAME_MAP_ROWS;
                break;
            case LEFT:
                head.col = (head.col - 1) % mod_col;
                break;
            case RIGHT:
                head.col = (head.col + 1) % mod_col;
                break;
        }

        if(head.row == food.row && head.col == food.col)
        {
            snake_part_pos new_food;
            do
            {
                uint8_t frow = random8_min_max(0,4);
                uint8_t fcol = random8_min_max(1,11);
                new_food.row = frow;
                new_food.col = fcol;

            } while(collision(new_food));

            food = new_food;

            if(length < SNAKE_MAX_LEN)
                length++;
        }

        for(int8_t i = length - 2; i >= 0; i--)
        {
            if(i < SNAKE_MAX_LEN - 2)
                snake[i + 1] = snake[i];
            else
                continue;
        }
        snake[0] = head;

        if(collision(head) && death == 0)
            death = 1;
    }

    void set_input_snake_game(uint8_t row, uint8_t col)
    {
        if(row == 4 && col == 15)
            direction = UP;

        if(row == 5 && col == 15)
            direction = DOWN;

        if(row == 5 && col == 14)
            direction = LEFT;

        if(row == 5 && col == 16)
            direction = RIGHT;
    }

    bool SNAKE_GAME(effect_params_t* params)
    {
        RGB_MATRIX_USE_LIMITS(led_min, led_max);
        RGB rgb = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);

        if (params->init) {
            rgb_matrix_set_color_all(0, 0, 0);
            init_game();
            lastGameTickTime = timer_read();
        }

        // Move snake
        uint16_t step_delay = 1000 - length * 20 - rgb_matrix_config.speed * 2;
        if(timer_elapsed(lastGameTickTime) >= ((step_delay > 0) ? step_delay : 50))
        {
            lastGameTickTime = timer_read();

            if(death > 0)
                death++;
            else
                move_snake();

            if(death == 3)
                init_game();
        }

        // Draw snake
        rgb_matrix_set_color_all(0, 0, 0);
        for (uint8_t s = 0; s < length; s++)
        {
            uint8_t row = snake[s].row;
            uint8_t col = snake[s].col;

            if (!HAS_ANY_FLAGS(g_led_config.flags[g_led_config.matrix_co[row][col]], params->flags)) continue;

            if(death > 0)
                rgb_matrix_set_color(g_led_config.matrix_co[row][col], 255, 0, 0);
            else
                rgb_matrix_set_color(g_led_config.matrix_co[row][col], rgb.r, rgb.g, rgb.b);
        }

        // Draw food
        rgb_matrix_set_color(g_led_config.matrix_co[food.row][food.col], 255 - rgb.r, 255 - rgb.g, 255 - rgb.b);

        return rgb_matrix_check_finished_leds(led_max);
    }

#       endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#   endif  // ENABLE_RGB_MATRIX_SNAKE_GAME
#endif  // RGB_MATRIX_KEYREACTIVE_ENABLED
