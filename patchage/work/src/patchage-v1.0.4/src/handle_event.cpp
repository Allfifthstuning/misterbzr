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

#include "handle_event.hpp"

#include "Canvas.hpp"
#include "CanvasPort.hpp"
#include "Event.hpp"
#include "ILog.hpp"
#include "Metadata.hpp"
#include "Patchage.hpp"
#include "PortID.hpp"
#include "warnings.hpp"

PATCHAGE_DISABLE_FMT_WARNINGS
#include <fmt/core.h>
#include <fmt/ostream.h>
PATCHAGE_RESTORE_WARNINGS

#include <boost/variant/apply_visitor.hpp>

#include <iosfwd>
#include <memory>

namespace patchage {

namespace {

class EventHandler
{
public:
  using result_type = void; ///< For boost::apply_visitor

  explicit EventHandler(Patchage& patchage)
    : _patchage{patchage}
  {}

  void operator()(const DriverAttachmentEvent& event)
  {
    _patchage.driver_attached(event.type);
  }

  void operator()(const DriverDetachmentEvent& event)
  {
    _patchage.driver_detached(event.type);
  }

  void operator()(const ClientCreationEvent& event)
  {
    // Don't create empty modules, they will be created when ports are added
    _patchage.metadata().set_client(event.id, event.info);
  }

  void operator()(const ClientDestructionEvent& event)
  {
    _patchage.canvas()->remove_module(event.id);
    _patchage.metadata().erase_client(event.id);
  }

  void operator()(const PortCreationEvent& event)
  {
    _patchage.metadata().set_port(event.id, event.info);

    auto* const port =
      _patchage.canvas()->create_port(_patchage, event.id, event.info);

    if (!port) {
      _patchage.log().error(
        fmt::format("Unable to create view for port \"{}\"", event.id));
    }
  }

  void operator()(const PortDestructionEvent& event)
  {
    _patchage.canvas()->remove_port(event.id);
    _patchage.metadata().erase_port(event.id);
  }

  void operator()(const ConnectionEvent& event)
  {
    CanvasPort* port_1 = _patchage.canvas()->find_port(event.tail);
    CanvasPort* port_2 = _patchage.canvas()->find_port(event.head);

    if (!port_1) {
      _patchage.log().error(
        fmt::format("Unable to find port \"{}\" to connect", event.tail));
    } else if (!port_2) {
      _patchage.log().error(
        fmt::format("Unable to find port \"{}\" to connect", event.head));
    } else {
      _patchage.canvas()->make_connection(port_1, port_2);
    }
  }

  void operator()(const DisconnectionEvent& event)
  {
    CanvasPort* port_1 = _patchage.canvas()->find_port(event.tail);
    CanvasPort* port_2 = _patchage.canvas()->find_port(event.head);

    if (!port_1) {
      _patchage.log().error(
        fmt::format("Unable to find port \"{}\" to disconnect", event.tail));
    } else if (!port_2) {
      _patchage.log().error(
        fmt::format("Unable to find port \"{}\" to disconnect", event.head));
    } else {
      _patchage.canvas()->remove_edge_between(port_1, port_2);
    }
  }

private:
  Patchage& _patchage;
};

} // namespace

void
handle_event(Patchage& patchage, const Event& event)
{
  EventHandler handler{patchage};
  boost::apply_visitor(handler, event);
}

} // namespace patchage
