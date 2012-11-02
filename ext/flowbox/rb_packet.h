// Copyright (C) 2011-12 Dominik Schatzmann <schadomi@gmail.com>
// This file is part of FlowBox. FlowBox is free software: you can redistribute
// it and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// FlowBox is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with FlowBox. If not, see <http://www.gnu.org/licenses/>.

/**
 * @file   rb_packet.h
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for Packet class.
 *
 * Ruby C++ Wrapper for Packet class.
 *
 */

#ifndef FLOWBOXRUBY_EXT_RB_PACKET_H
#define FLOWBOXRUBY_EXT_RB_PACKET_H

#include <arpa/inet.h>
#include <string>

#include "ruby.h"
#include <flowbox/packet.h>

#define RB_PACKET_UNWRAP Packet* packet; Data_Get_Struct(self, Packet, packet);

void rb_packet_free(void *p);
VALUE rb_packet_alloc(VALUE klass);

VALUE rb_packet_get_length(VALUE self);
VALUE rb_packet_get_addr(VALUE self);
VALUE rb_packet_get_payload(VALUE self);

#endif  // FLOWBOXRUBY_EXT_RB_PACKET_H
