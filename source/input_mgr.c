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
#include "input_mgr.h"
#include <stddef.h>
#include "system.h"
#include "gpio.h"

static const INPUT_MGR_config_t *input_mgr;
static uint8_t input_mgr_size;
static uint8_t input_mgr_cnt[2];
static bool input_mgr_flags[2];

static void input_mgr_main(void)
{
    for(uint8_t i = 0U; i < input_mgr_size; i++)
    {
        const uint8_t port = input_mgr[i].gpio_config[0];
        const uint8_t pin = input_mgr[i].gpio_config[1];

        if(!GPIO_read_pin(port, pin))
        {
            input_mgr_cnt[i]++;

            if(input_mgr_cnt > 3U && !input_mgr_flags[i])
            {
                if(input_mgr[i].callback != NULL)
                {
                    input_mgr[i].callback();
                    input_mgr_flags[i] = true;
                }
            }
        }
        else
        {
            input_mgr_cnt[i] = 0u;
            input_mgr_flags[i] = false;
        }
    }
}

int8_t INPUT_MGR_initialize(const INPUT_MGR_config_t *config, uint8_t size)
{
    if(config == NULL)
    {
        return -1;
    }

    if(SYSTEM_register_task(input_mgr_main, 10) != 0)
    {
        return -1;
    }

    for(uint8_t i = 0U; i < size; i++)
    {
        const uint8_t port = config[i].gpio_config[0];
        const uint8_t pin = config[i].gpio_config[1];

        GPIO_config_pin(port, pin, GPIO_INPUT_FLOATING);
    }

    input_mgr = config;
    input_mgr_size = size;

    return 0;
}
