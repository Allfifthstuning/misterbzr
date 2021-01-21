/* This file is part of Patchage.
 * Copyright 2007-2020 David Robillard <d@drobilla.net>
 *
 * Patchage is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Patchage is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Patchage.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PATCHAGE_MAKE_ALSA_DRIVER_HPP
#define PATCHAGE_MAKE_ALSA_DRIVER_HPP

#include "Driver.hpp"
#include "patchage_config.h"

#include <memory>

namespace patchage {

class ILog;

#if defined(HAVE_ALSA)

std::unique_ptr<Driver>
make_alsa_driver(ILog& log, Driver::EventSink emit_event);

#else

inline std::unique_ptr<Driver>
make_alsa_driver(ILog&, Driver::EventSink)
{
  return nullptr;
}

#endif

} // namespace patchage

#endif // PATCHAGE_MAKE_ALSA_DRIVER_HPP
