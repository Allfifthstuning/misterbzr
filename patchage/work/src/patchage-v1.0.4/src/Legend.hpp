/* This file is part of Patchage.
 * Copyright 2014-2020 David Robillard <d@drobilla.net>
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

#ifndef PATCHAGE_LEGEND_HPP
#define PATCHAGE_LEGEND_HPP

#include "PortType.hpp"

#include <gtkmm/box.h>
#include <sigc++/signal.h>

#include <cstdint>
#include <string>

namespace Gtk {
class ColorButton;
} // namespace Gtk

namespace patchage {

class Configuration;

class Legend : public Gtk::HBox
{
public:
  explicit Legend(const Configuration& configuration);

  sigc::signal<void, PortType, std::string, uint32_t> signal_color_changed;

private:
  void add_button(PortType id, const std::string& label, uint32_t rgba);

  void on_color_set(PortType                id,
                    const std::string&      label,
                    const Gtk::ColorButton* but);
};

} // namespace patchage

#endif // PATCHAGE_LEGEND_HPP
