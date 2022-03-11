#include <cstdio>

#include "pico/stdlib.h"

#include "tusb.h"

#include "usb_descriptors.h"

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len)
{
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
}

static void init_button(int gpio)
{
  gpio_set_function(gpio, GPIO_FUNC_SIO);
  gpio_set_dir(gpio, GPIO_IN);
  gpio_pull_up(gpio);
}

int main()
{
    stdio_init_all();

    tusb_init();

    init_button(PICOSYSTEM_SW_A_PIN);
    init_button(PICOSYSTEM_SW_B_PIN);
    init_button(PICOSYSTEM_SW_X_PIN);
    init_button(PICOSYSTEM_SW_Y_PIN);

    init_button(PICOSYSTEM_SW_UP_PIN);
    init_button(PICOSYSTEM_SW_RIGHT_PIN);
    init_button(PICOSYSTEM_SW_DOWN_PIN);
    init_button(PICOSYSTEM_SW_LEFT_PIN);

    auto last_update = get_absolute_time();

    while(true)
    {
        tud_task();

        auto now = get_absolute_time();

        if(tud_hid_ready() && absolute_time_diff_us(last_update, now) >= 10000)
        {
            hid_gamepad_report_t report{};

            if(!gpio_get(PICOSYSTEM_SW_A_PIN))
                report.buttons |= GAMEPAD_BUTTON_0;
            if(!gpio_get(PICOSYSTEM_SW_B_PIN))
                report.buttons |= GAMEPAD_BUTTON_1;
            if(!gpio_get(PICOSYSTEM_SW_X_PIN))
                report.buttons |= GAMEPAD_BUTTON_2;
            if(!gpio_get(PICOSYSTEM_SW_Y_PIN))
                report.buttons |= GAMEPAD_BUTTON_3;

            if(!gpio_get(PICOSYSTEM_SW_UP_PIN))
            {
                if(!gpio_get(PICOSYSTEM_SW_LEFT_PIN))
                    report.hat = GAMEPAD_HAT_UP_LEFT;
                else if(!gpio_get(PICOSYSTEM_SW_RIGHT_PIN))
                    report.hat = GAMEPAD_HAT_UP_RIGHT;
                else
                    report.hat = GAMEPAD_HAT_UP;
            }
            else if(!gpio_get(PICOSYSTEM_SW_DOWN_PIN))
            {
                if(!gpio_get(PICOSYSTEM_SW_LEFT_PIN))
                    report.hat = GAMEPAD_HAT_DOWN_LEFT;
                else if(!gpio_get(PICOSYSTEM_SW_RIGHT_PIN))
                    report.hat = GAMEPAD_HAT_DOWN_RIGHT;
                else
                    report.hat = GAMEPAD_HAT_DOWN;
            }
            else if(!gpio_get(PICOSYSTEM_SW_LEFT_PIN))
                report.hat = GAMEPAD_HAT_LEFT;
            else if(!gpio_get(PICOSYSTEM_SW_RIGHT_PIN))
                report.hat = GAMEPAD_HAT_RIGHT;

            tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

            last_update = now;
        }
        else
            sleep_ms(1);
    }

    return 0;
}
