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

#ifndef PATCHAGE_UIFILE_HPP
#define PATCHAGE_UIFILE_HPP

#include "patchage_config.h"

#ifdef PATCHAGE_BINLOC
#  include "binary_location.h"
#endif

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace patchage {

class UIFile
{
public:
  inline static bool is_readable(const std::string& filename)
  {
    std::ifstream fs(filename.c_str());
    const bool    fail = fs.fail();
    fs.close();
    return !fail;
  }

  static Glib::RefPtr<Gtk::Builder> open(const std::string& base_name)
  {
    std::string ui_filename;
#ifdef PATCHAGE_BINLOC
    const std::string bundle = bundle_location();
    if (!bundle.empty()) {
      ui_filename = bundle + "/" + base_name + ".ui";
      if (is_readable(ui_filename)) {
        std::cout << "Loading UI file " << ui_filename << std::endl;
        return Gtk::Builder::create_from_file(ui_filename);
      }
    }
#endif
    ui_filename = std::string(PATCHAGE_DATA_DIR) + "/" + base_name + ".ui";
    if (is_readable(ui_filename)) {
      std::cout << "Loading UI file " << ui_filename << std::endl;
      return Gtk::Builder::create_from_file(ui_filename);
    }

    std::stringstream ss;
    ss << "Unable to find " << base_name << std::endl;
    throw std::runtime_error(ss.str());
    return Glib::RefPtr<Gtk::Builder>();
  }
};

} // namespace patchage

#endif // PATCHAGE_UIFILE_HPP
