# Copyright 2011,2012 Dominik Schatzmann (schadomi@gmail.com)
#
# This file is part of FlowBox. FlowBox is free software: you can redistribute
# it and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version 3
# of the License, or (at your option) any later version.
#
# FlowBox is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with FlowBox.  If not, see <http://www.gnu.org/licenses/>.

module FlowBox
  module Core
    class Flow
      # a string output which shows the values of one connection
      # returns invalid or returns
      # key_s | time_s | routing_s | network_s | content_s
      def to_s()
        return "invalid" if valid == false
        key_s = sprintf('%16s::%8d -> %16s::%8d %3d ', addr_src_s, port_src , addr_dst_s, port_dst, protocol)
        time_s = sprintf('%10d %10d', start_s, stop_s)
        routing_s = sprintf('%1d %2d %2d -> %16s', export_device_id, if_in, if_out, addr_next_s)
        network_s = sprintf('%1d %1d', direction, border)
        content_s = sprintf('P:%10d B:%10d', packets, bytes)
        return(key_s + ' | ' + time_s +  ' | ' + routing_s  + ' | ' + network_s + ' | ' + content_s)
      end # to_s
    end # Flow
  end # Core
end # FlowBox

