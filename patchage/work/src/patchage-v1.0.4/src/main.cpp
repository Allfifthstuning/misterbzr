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

#ifdef __APPLE__
#  include "binary_location.h"

#  include <gtk/gtkrc.h>

#  include <unistd.h>

#  include <cstdlib>
#  include <string>
#endif

#include "Options.hpp"
#include "Patchage.hpp"
#include "patchage_config.h"

#include <glibmm/exception.h>
#include <glibmm/thread.h>
#include <glibmm/ustring.h>
#include <gtkmm/main.h>

#include <cstring>
#include <exception>
#include <iostream>

namespace {

#ifdef __APPLE__
void
set_bundle_environment()
{
  const std::string bundle   = patchage::bundle_location();
  const std::string lib_path = bundle + "/lib";
  if (!Glib::file_test(lib_path, Glib::FILE_TEST_EXISTS)) {
    // If lib does not exist, we have not been bundleified, do nothing
    return;
  }

  setenv("GTK_PATH", lib_path.c_str(), 1);
  setenv("DYLD_LIBRARY_PATH", lib_path.c_str(), 1);

  const std::string pangorc_path(bundle + "/Resources/pangorc");
  if (Glib::file_test(pangorc_path, Glib::FILE_TEST_EXISTS)) {
    setenv("PANGO_RC_FILE", pangorc_path.c_str(), 1);
  }

  const std::string fonts_conf_path(bundle + "/Resources/fonts.conf");
  if (Glib::file_test(fonts_conf_path, Glib::FILE_TEST_EXISTS)) {
    setenv("FONTCONFIG_FILE", fonts_conf_path.c_str(), 1);
  }

  const std::string loaders_cache_path(bundle + "/Resources/loaders.cache");
  if (Glib::file_test(loaders_cache_path, Glib::FILE_TEST_EXISTS)) {
    setenv("GDK_PIXBUF_MODULE_FILE", loaders_cache_path.c_str(), 1);
  }

  const std::string gtkrc_path(bundle + "/Resources/gtkrc");
  if (Glib::file_test(gtkrc_path, Glib::FILE_TEST_EXISTS)) {
    gtk_rc_parse(gtkrc_path.c_str());
  }
}
#endif

void
print_usage()
{
  std::cout << "Usage: patchage [OPTION]...\n";
  std::cout << "Visually connect JACK and ALSA Audio and MIDI ports.\n\n";
  std::cout << "Options:\n";
  std::cout << "  -h, --help     Display this help and exit.\n";
  std::cout << "  -A, --no-alsa  Do not automatically attach to ALSA.\n";
  std::cout << "  -J, --no-jack  Do not automatically attack to JACK.\n";
}

void
print_version()
{
  std::cout << "Patchage " PATCHAGE_VERSION << R"(
Copyright 2007-2020 David Robillard <d@drobilla.net>.
License GPLv3+: <http://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
)";
}

} // namespace

int
main(int argc, char** argv)
{
#ifdef __APPLE__
  set_bundle_environment();
#endif

  try {
    Glib::thread_init();

    Gtk::Main app(argc, argv);
    ++argv;
    --argc;

    // Parse command line options
    patchage::Options options;
    while (argc > 0) {
      if (!strcmp(*argv, "-h") || !strcmp(*argv, "--help")) {
        print_usage();
        return 0;
      }

      if (!strcmp(*argv, "-A") || !strcmp(*argv, "--no-alsa")) {
        options.alsa_driver_autoattach = false;
      } else if (!strcmp(*argv, "-J") || !strcmp(*argv, "--no-jack")) {
        options.jack_driver_autoattach = false;
      } else if (!strcmp(*argv, "-V") || !strcmp(*argv, "--version")) {
        print_version();
        return 0;
      } else {
        std::cerr << "patchage: invalid option -- '" << *argv << "'\n";
        print_usage();
        return 1;
      }

      ++argv;
      --argc;
    }

    // Run until main loop is finished
    patchage::Patchage patchage(options);
    Gtk::Main::run(*patchage.window());
    patchage.save();

  } catch (std::exception& e) {
    std::cerr << "patchage: error: " << e.what() << std::endl;
    return 1;
  } catch (Glib::Exception& e) {
    std::cerr << "patchage: error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
