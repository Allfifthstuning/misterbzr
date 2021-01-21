/* This file is part of Patchage.
 * Copyright 2020 David Robillard <d@drobilla.net>
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

#ifndef PATCHAGE_CLIENTTYPE_HPP
#define PATCHAGE_CLIENTTYPE_HPP

namespace patchage {

/// A type of client (program) with supported ports
enum class ClientType {
  jack,
  alsa,
};

} // namespace patchage

#endif // PATCHAGE_CLIENTTYPE_HPP
