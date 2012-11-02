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

require 'ipaddr'
module FlowBox
  module Core
    class TopologyFilter

      PurposeUnknown  = 0  # we don't know the purpose (no data)
      PurposeInternet = 1  # connected toward the Internet
      PurposeIntranet = 2  # connected toward the Intranet
      PurposeMixed    = 3  # connected to a mixed network (Internet and Intranet)

      PurposeNextHopUnknown  = 0
      PurposeNextHopInternet = 1
      PurposeNextHopInternal = 2
      PurposeNextHopMulticast = 3
      PurposeNextHopZero = 4
      PurposeNextHopLocal = 5

## 
      def self.check_flow_exporter_id(flow_exporter_id)
        if flow_exporter_id == nil 
          raise('flow_exporter_id == nil')
        elsif flow_exporter_id.class != Fixnum
          raise('flow_exporter_id not Fixnum')
        elsif flow_exporter_id <= 0
          raise('flow_exporter_id <= 0')
        end
      end
      def self.check_snmp_if_id(snmp_if_id)
        if snmp_if_id == nil 
          raise('snmp_if_id == nil')
        elsif snmp_if_id.class != Fixnum
          raise('snmp_if_id not Fixnum')
        elsif snmp_if_id < 0
          raise('snmp_if_id < 0')
        end
      end
      def self.check_filter(filter)
        if filter == nil
         raise('filter not found')
        elsif filter.class != Fixnum
         raise('filter should be a Fixnum')
        elsif filter < TopologyFilter::FilterAll or filter > TopologyFilter::FilterNon
         raise(':filter out of range')
        end
      end
      def self.check_addr_expected(addr_expected)
        if addr_expected == nil
         raise('addr_expected not found')
        elsif addr_expected.class != String
         raise('addr_expected should be a String')
        end
      end
      def self.check_addr_observed(addr_observed)
        if addr_observed == nil
         raise('addr_observed not found')
        elsif addr_observed.class != String
         raise('addr_observed should be a String')
        end
      end
      def self.check_purpose(purpose)
        case purpose
          when PurposeUnknown
          when PurposeInternet
          when PurposeIntranet
          when PurposeMixed
          else
            raise "'#{purpose}' is not a valid purpose"
        end
      end
      def self.check_state(state)
        case state
          when StateNormal
          when StateAlarm
          else
            raise "'#{state}' is not a valid state"
        end
      end
      def self.check_obs(flow_exporter_id, snmp_if_id, addr_observed)
        check_flow_exporter_id(flow_exporter_id)
        check_snmp_if_id(snmp_if_id)
        check_addr_observed(addr_observed)
      end

###
      def self.get_addr(addr_s)
        m = IPAddr.new(addr_s)
        if m.ipv4?
          addr_n = m.to_i
          addr_length = 4
        elsif m.ipv6?
          addr_n = m.to_i
          addr_length = 16
        else
          raise "'#{addr_s}' is not a valid IPv4 or IPv6 Addr"
        end
        return [addr_s, addr_n, addr_length]
      end

      def self.get_addr_from_range(range)
        data = range.split('/')
        if data.size != 2
          raise "'#{range}' is not a valid format for IPv4 or IPv6 range"
        end
        addr_s = data[0]
        network = data[1].to_i
        addr_s, addr_n, addr_length = get_addr(addr_s)
        if network < 0 or (network > 32 and  addr_length == 4) or (network > 128 and  addr_length == 16)
          raise "'#{network}' is not a valid network size for an IPv4 or IPv6 Addr"
        end 
        return [addr_s, addr_n, addr_length, network]
      end

      def self.obs_addr_to_h(addr_observation)
        raise "wrong type for addr_observation " if addr_observation.class != String

        ob_a = addr_observation.split(', ')
        ob_h = {}
        ob_a.each do |ip_count|
          if ip_count == nil or ip_count.size() < 3
            throw "'#{ip_count}' is not a valid IP Count TAG"  
          else
            data2 = ip_count.split('@')
            if data2.size() == 2
              ob_h[data2[0]] = data2[1].to_i
            else
              throw "'#{ip_count}' is not a valid IP Count TAG -- split size != 2"  
            end
          end
        end
        return ob_h
      end
##
      def self.predict_purpose(addr_s, next_hop_a)
        # parse input
        addr_s, addr_n, addr_length = get_addr(addr_s)

        # check zero address
        if addr_n == 0 and addr_length == 4
          return [PurposeNextHopZero, "0.0.0.0/32"]
        elsif addr_n == 0 and addr_length == 16
          return [PurposeNextHopZero, "::/64"]	
        end
        #puts  "NOT ZERO"

        # check multicast
        if addr_length == 4 and (addr_n >> 28) == 14
           return [PurposeNextHopMulticast, "224.0.0.0/4"]
        elsif addr_length == 16 and  (addr_n >> 120) == 0xff 
           return [PurposeNextHopMulticast, "ff00::/8"]
        end
        #puts  "NOT MULTICAST"

        # link local addresses	
        if addr_length == 16 and (addr_n >> 118) == 1018
          return [PurposeNextHopLocal, "fe80::/10"]
        end
        #puts  "NOT Link Local"

        # get best match
        best = nil
        # find the next_hop network according to given prefixes in the cfg file
        next_hop_a ||= []
        if addr_length == 4
          addr_length_max = 32
        else
          addr_length_max = 128
        end
        next_hop_a.each do |data|
          if data[:addr_length] == addr_length and (best == nil or data[:network] > best[:network])         
            if( data[:addr_n] >> (addr_length_max - data[:network]) == (addr_n >> (addr_length_max - data[:network])) )
              best = data
            end
          end
        end
        #puts  "BEST MATCH #{best}"
        if best == nil
          if addr_length == 4
	          return([PurposeNextHopUnknown, "#{addr_s}/32"])
          else
	          return([PurposeNextHopUnknown, "#{addr_s}/128"])
          end
        else
	        return([best[:purpose], "#{best[:range]}"])
        end
      end

      def self.merge_types(types_a)
        note = ""
        purpose = PurposeUnknown

	      if types_a.include?(PurposeNextHopMulticast) == true
		      # note += "# WARNING: Multicast ignored\n"
		      types_a.delete(PurposeNextHopMulticast)
	      end

	      if types_a.include?(PurposeNextHopLocal) == true
		      # note += "# WARNING: Link Local ignored"
		      types_a.delete(PurposeNextHopLocal)
	      end

	      if types_a.include?(PurposeNextHopZero) == true
		      # note += "# WARNING: Zero Hop ignored"
		      types_a.delete(PurposeNextHopZero)
	      end


        if types_a.size == 0
          note += "# WARNING: NO NEXT HOP INFORMATION"
          purpose = PurposeUnknown
          return([purpose, note])
        elsif types_a.size() == 1  # normal case
          if types_a[0] == PurposeNextHopUnknown
            purpose = PurposeUnknown
          elsif types_a[0] == PurposeNextHopInternet
            purpose = PurposeInternet
          elsif types_a[0] == PurposeNextHopInternal
            purpose = PurposeIntranet
          elsif types_a[0] == PurposeNextHopZero
            purpose = PurposeUnknown
          elsif types_a[0] == PurposeNextHopLocal
            purpose = PurposeUnknown
          else
            purpose = PurposeUnknown
          end
          return([purpose, note])
        else # multiple types
          note += "# WARNING: Multiple Types #{types_a*","} "
          purpose = PurposeUnknown
          return([purpose, note])
        end # types_a.size == 0
      end


     # configuration
      def configure(configure_h)

        conf_check_input(configure_h)
        conf_check_output(configure_h)
        conf_check_stat_interval(configure_h)
        conf_check_interfaces(configure_h)

        conf_push(configure_h)
      end

      def conf_check_input(configure_h)
        input = configure_h[:input]
        case input
        when nil
          raise('no input buffer found tag found')
        when 'null'
          raise('we need an input (null) not accepted')
        end
      end

      def conf_check_output(configure_h)
        output = configure_h[:output]
        case output
        when nil
          raise('no output buffer found tag found')
        end
      end

      def conf_check_stat_interval(configure_h)
        stat_interval = configure_h[:stat_interval]
        case stat_interval
        when nil
          raise('no stat_interval found tag found')
        when stat_interval < 0
         raise('kidding? stat interval > 0')
        end
      end

      def conf_check_interfaces(configure_h)
        interfaces = configure_h[:interfaces]
        if interfaces == nil
          raise('no interfaces found tag found')
        elsif interfaces.class != Array
         raise(':interfaces should be a Array')
        end
        interfaces.each do |interface|
          # check each subelement
          if interface.class != Hash
           raise('interfaces should be a Hash')
          end

          # export device id
          FlowBox::Core::TopologyFilter.check_flow_exporter_id(interface[:flow_exporter_id])

          # snmp if id
          FlowBox::Core::TopologyFilter.check_snmp_if_id(interface[:snmp_if_id])

          # filter
          FlowBox::Core::TopologyFilter.check_filter(interface[:filter])

          # addr_ranges
          FlowBox::Core::TopologyFilter.check_addr_expected(interface[:addr_expected])
        end
      end


    end # TopologyFilter
  end # Core
end # RStreaMon
