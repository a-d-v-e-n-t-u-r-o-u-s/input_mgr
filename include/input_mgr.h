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

/*!
 *
 * \addtogroup input_mgr
 * \ingroup modules
 * \brief Input manager
 */

/*@{*/

/*!
 *
 * \addtogroup input_mgr_events
 * \ingroup input_mgs
 * \brief Input manager events
 */
/*@{*/
#define BUTTON_SHORT_PRESSED    (0U)
#define BUTTON_LONG_PRESSED     (1U)
#define BUTTON_RELEASED         (2U)
/*@}*/

typedef struct
{
    uint8_t id;
    uint8_t event;
} INPUT_MGR_event_t;

/*!
 * \brief Gets input manager event
 *
 * \param event storage for event
 *
 * \retval 0 success, event valid
 * \retval -1 failure, no valid event
 */
int8_t INPUT_MGR_get_event(INPUT_MGR_event_t *event);

/*!
 * \brief Initializes input manager
 */
void INPUT_MGR_initialize(void);

/*@}*/
#endif
