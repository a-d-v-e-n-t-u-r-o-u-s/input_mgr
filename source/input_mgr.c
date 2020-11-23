/*!
 * \file
 * \brief Input manager implementation file
 * \author Dawid Babula
 * \email dbabula@adventurous.pl
 *
 * \par Copyright (C) Dawid Babula, 2020
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#define DEBUG_APP_ID "INPM"

#include "input_mgr.h"
#include <stddef.h>
#include "system.h"
#include "gpio.h"
#include "fifo.h"
#include "debug.h"

#define MAIN_CYCLE          (10U)
#define SHORT_PRESS_TIME    (20U)
#define SHORT_PRESS_CNT     (SHORT_PRESS_TIME / MAIN_CYCLE)
#define LONG_PRESS_TIME     (2000U)
#define LONG_PRESS_CNT      (LONG_PRESS_TIME / MAIN_CYCLE)
#define RELEASE_TIME        (20U)
#define RELEASE_CNT         (RELEASE_TIME / MAIN_CYCLE)

static const INPUT_MGR_config_t *input_mgr;
static uint8_t input_mgr_size;
static uint8_t press_cnt[2];
static uint8_t release_cnt[2];
static bool short_presses[2];
static bool long_presses[2];
static bool releases[2];
static FIFO_t fifo;
uint8_t buffer[64];


static void input_mgr_main(void)
{
    for(uint8_t i = 0U; i < input_mgr_size; i++)
    {
        const uint8_t port = input_mgr[i].gpio_config[0];
        const uint8_t pin = input_mgr[i].gpio_config[1];

        INPUT_MGR_event_t event =
        {
            .id = i,
            .event = BUTTON_SHORT_PRESSED
        };

        if(!GPIO_read_pin(port, pin))
        {
            press_cnt[i]++;
            release_cnt[i] = 0u;
            releases[i] = false;

            bool const is_short_press = (press_cnt[i] >= SHORT_PRESS_CNT);
            bool const is_long_press = (press_cnt[i] >= LONG_PRESS_CNT);

            if(is_short_press && !short_presses[i])
            {

                FIFO_enqueue(&fifo, (const void *) &event);
                short_presses[i] = true;
            }

            if(is_long_press && !long_presses[i])
            {
                event.event = BUTTON_LONG_PRESSED;
                FIFO_enqueue(&fifo, (const void *) &event);
                long_presses[i] = true;
            }
        }
        else
        {
            if(short_presses[i] || long_presses[i])
            {
                release_cnt[i]++;

                bool const is_release = (release_cnt[i] >= RELEASE_CNT);

                if(is_release && !releases[i])
                {
                    event.event = BUTTON_RELEASED;
                    FIFO_enqueue(&fifo, (const void *) &event);
                    releases[i] = true;
                    press_cnt[i] = 0u;
                    short_presses[i] = false;
                    long_presses[i] = false;
                }
            }
        }
    }
}

int8_t INPUT_MGR_get_event(INPUT_MGR_event_t *event)
{
    if(event == NULL)
    {
        return -1;
    }

    return FIFO_dequeue(&fifo, event);
}

void INPUT_MGR_initialize(const INPUT_MGR_config_t *config, uint8_t size)
{
    ASSERT(config != NULL);

    int8_t ret = SYSTEM_register_task(input_mgr_main, MAIN_CYCLE);

    (void) ret;
    ASSERT(ret == 0);

    for(uint8_t i = 0U; i < size; i++)
    {
        const uint8_t port = config[i].gpio_config[0];
        const uint8_t pin = config[i].gpio_config[1];

        GPIO_config_pin(port, pin, GPIO_INPUT_FLOATING);
    }

    FIFO_config_t fifo_config =
    {
        .elements_no = sizeof(buffer)/2,
        .elements_size = 2U,
        .buffer = buffer
    };

    FIFO_configure(&fifo, &fifo_config);

    input_mgr = config;
    input_mgr_size = size;
}
