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
 * @file   rb_packet.cc
 * @author Dominik Schatzmann <schadomi@gmail.com>
 * @date   October, 2012
 * @brief  Ruby C++ Wrapper for Packet class.
 *
 * Ruby C++ Wrapper for Packet class.
 *
 */

#include "rb_packet.h"

void rb_packet_free(void *p) {
  delete (Packet*) p;
  p = NULL;
}

VALUE rb_packet_alloc(VALUE klass) {
  Packet* p = NULL;
  VALUE obj;
  p = new Packet();
  if (p == NULL) {
    rb_raise(rb_path2class("FlowBox::Core::PacketError"), "Out of Memory");
  }
  obj = Data_Wrap_Struct(klass, 0, rb_packet_free, p);
  return (obj);
}

VALUE rb_packet_get_length(VALUE self) {
  RB_PACKET_UNWRAP
  return (INT2FIX(packet->buffer_length_));
}

VALUE rb_packet_get_addr(VALUE self) {
  RB_PACKET_UNWRAP

  char tmp_ip[INET6_ADDRSTRLEN];
  if (packet->addr_length_ == 4)
    inet_ntop(AF_INET, &(((struct sockaddr_in*) (&(packet->addr_)))->sin_addr),
              tmp_ip, INET6_ADDRSTRLEN);
  else if (packet->addr_length_ == 16)
    inet_ntop(AF_INET6, &(((struct sockaddr_in*) (&(packet->addr_)))->sin_addr),
              tmp_ip, INET6_ADDRSTRLEN);
  else
    sprintf(tmp_ip, "NA");

  return (rb_str_new2(tmp_ip));
}

VALUE rb_packet_get_payload(VALUE self) {
  RB_PACKET_UNWRAP
  return (rb_str_new(packet->buffer_, packet->buffer_length_));
}

