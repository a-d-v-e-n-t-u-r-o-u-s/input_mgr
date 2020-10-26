/*!
 * \file
 * \brief Input manager header file
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

#ifndef INPUT_MGR_H
#define INPUT_MGR_H

#include <stdint.h>

typedef void (*INPUT_MGR_callback_t)(void);

typedef struct
{
    uint8_t gpio_config[2];
    INPUT_MGR_callback_t callback;
} INPUT_MGR_config_t;

int8_t INPUT_MGR_initialize(const INPUT_MGR_config_t *config, uint8_t size);
#endif
