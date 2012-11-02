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
require 'fileutils'
module FlowBox
  module Driver
    module Console
      class TopologyFilter

#### FUNCTIONS #################################################################
        def initialize()
          @generic_driver = FlowBox::Driver::Generic.new()
          @generic_driver.configure_core(FlowBox::Core::TopologyFilter.new())
          @console = FlowBox::Driver::Console::Console.instance()
          @generic_driver.register_obs_observer(self)
          @generic_driver.register_stat_observer(self)
        end
        def configure(configure_h)
          # cache some values
          @configuration_input = configure_h[:input]
          @configuration_output = configure_h[:output]
          @configuration_stat_interval = configure_h[:stat_interval]
          @interfaces_h ||= Hash.new
          configure_h[:interfaces].each do |interface|
            flow_exporter_id = interface[:flow_exporter_id]
            snmp_if_id = interface[:snmp_if_id]
            tag = "#{flow_exporter_id}-#{snmp_if_id}"
            @interfaces_h[tag] = interface
          end
          @generic_driver.configure(configure_h)
        end
        def start()
          @generic_driver.start()   
        end
        def finished()
          return @generic_driver.finished()
        end
        def process(object)
          if object[:msg_type] == 'statistic'
            # we would like to have a specific format
            ratio = 0.0
            ratio = object[:flows_filtered].to_f/object[:flows]
            @console.write "TopologyFilter STAT, #{object[:time_s]}, #{object[:duration_s]}, #{object[:flows]}, #{object[:flows_filtered]}, #{'% 5.4f'%ratio}"
          elsif object[:msg_type] == 'observation'
            # just use the default output
            addr_observed_short = object[:addr_observed][0,20]
            @console.write "TopologyFilter OBS, EXP-DEV-ID=#{object[:flow_exporter_id]}, SNMP-IF=#{object[:snmp_if_id]}, STATE=#{object[:state]} #{addr_observed_short}"
            process_observation(object)
            debug_save_mapping() if @debug == true
          end
        end

        def set_next_hop(range, purpose)
          # parse input
          addr_s, addr_n, addr_length, network = FlowBox::Core::TopologyFilter::get_addr_from_range(range)
          purpose = purpose.to_i
          FlowBox::Core::TopologyFilter::check_purpose(purpose)

          # add range
          data = { :range => range,
            :addr_s => addr_s,
            :addr_n => addr_n,
            :addr_length => addr_length,
            :network => network,
            :purpose => purpose
          }
          @next_hop_a ||= []
          @next_hop_a << data
        end

        def build_inc_configuration()

          if @configuration_input == nil
            raise "Need a valid config first"
          end

          config = {
            :input => @configuration_input,
            :output =>  @configuration_output,
            :stat_interval => @configuration_stat_interval,
            :interfaces => []
          }
          @interfaces_h.each do |key, interface|
            config[:interfaces] << interface
          end
          return config
        end

        def get_interface(export_id, snmp_id)
          FlowBox::Core::TopologyFilter::check_flow_exporter_id(export_id)
          FlowBox::Core::TopologyFilter::check_snmp_if_id(snmp_id)
          @interfaces_h ||= Hash.new
          tag = "#{export_id}-#{snmp_id}"
          interface = @interfaces_h[tag]
          if interface == nil
            interface = {
              :flow_exporter_id => export_id,
              :snmp_if_id => snmp_id,
              :addr_observed_h => {},
              :addr_expected_h => {}
            }
            @interfaces_h[tag] = interface
          end
          return(interface)
        end
        def self.interface_update_observed_addresses(
            interface,
            addr_obs
        )
          raise "wrong type for Interface " if interface.class != Hash
          raise "wrong type for addr_obs " if addr_obs.class != String
          interface[:addr_observed_h] ||= {}

          data = addr_obs.split(', ')
          data.each do |ip_count|
            if ip_count == nil or ip_count.size() < 3
              throw "'#{ip_count}' is not a valid IP Count TAG"  
            else
              data2 = ip_count.split('@')
              if data2.size() == 2
                interface[:addr_observed_h][data2[0]] = data2[1].to_i
              else
                throw "'#{ip_count}' is not a valid IP Count TAG -- split size != 2"  
              end
            end
          end
          # flat back
          obs = ""
          interface[:addr_observed_h].each do |ip, count|
            obs += "#{ip}@#{count}, "
          end
          interface[:addr_observed] = obs
        end 

        def interface_update_address_ranges_and_purpose(interface)

          purpose_a = []
          ranges_a = []
          interface[:addr_observed_h].each do |addr, count|
            purpose, range = FlowBox::Core::TopologyFilter::predict_purpose(addr, @next_hop_a)
            purpose_a << purpose
            ranges_a << range
          end

          purpose_a.uniq!()
          ranges_a.uniq!()

          purpose_interface = FlowBox::Core::TopologyFilter::merge_types(purpose_a)
          interface[:purpose_interface] = purpose_interface
          interface[:purpose] = purpose_interface[0]

          # set filter rule based on the purpose
          if purpose_interface[0] == FlowBox::Core::TopologyFilter::PurposeUnknown
            interface[:filter] = FlowBox::Core::TopologyFilter::FilterNon
          elsif purpose_interface[0] == FlowBox::Core::TopologyFilter::PurposeInternet
            interface[:filter] = FlowBox::Core::TopologyFilter::FilterNon
          elsif purpose_interface[0] == FlowBox::Core::TopologyFilter::PurposeIntranet
            interface[:filter] = FlowBox::Core::TopologyFilter::FilterAll
          elsif purpose_interface[0] == FlowBox::Core::TopologyFilter::PurposeMixed
            interface[:filter] = FlowBox::Core::TopologyFilter::FilterNon
          else
            raise "WTF: purpose_interface[0] ?? unknown #{purpose_interface[0]}"            
          end

          # update ranges
          interface[:addr_expected_h] ||= {}
          ranges_a.each do |range|
            interface[:addr_expected_h][range] = true
          end
          addr_ranges = nil
          interface[:addr_expected_h].each do |range, bla|
             if addr_ranges == nil
              addr_ranges = "#{range}"
             else
              addr_ranges += ",#{range}"
             end
          end
          interface[:addr_expected] = addr_ranges
        end

        def process_observation(observation_h)

          export_id = observation_h[:flow_exporter_id]
          snmp_if_id = observation_h[:snmp_if_id]
          addr_observed = observation_h[:addr_observed]
          state = observation_h[:state]

          FlowBox::Core::TopologyFilter.check_obs(export_id, snmp_if_id, addr_observed)
          FlowBox::Core::TopologyFilter.check_state(state)

          # get local interface data
          interface = get_interface(export_id, snmp_if_id)

          # update observed addresses
          TopologyFilter::interface_update_observed_addresses(
            interface, 
            addr_observed
          )
        
          # predict address range and its purpose
          purpose_a, ranges_a = interface_update_address_ranges_and_purpose(interface)

          # create configuration update
          config = build_inc_configuration()

          @generic_driver.configure(config)
        end

### DEBUG ONLY #################################################################
        def debug_enable(folder_p)
          @debug = true
          @debug_folder = folder_p
          FileUtils::mkdir_p(@debug_folder)
        end
        def debug_save_mapping()
          out = File.open(@debug_folder + '/mapping', 'w')
          out.puts "# flow_exporter_id, snmp_if_id, name, purpose, filter, addr_expected, addr_observed"
          flow_exporter_interface_h = {}          
          @interfaces_h.each do |key, ift|
            flow_exporter_interface_h[ift[:flow_exporter_id]] ||= {}
            flow_exporter_interface_h[ift[:flow_exporter_id]][ift[:snmp_if_id]] = ift
          end
          devices_a = flow_exporter_interface_h.sort()
          devices_a.each do |flow_exporter_id, interfaces_h|
            interfaces_a = interfaces_h.sort()
            interfaces_a.each do |interfaces_id, ifa|
              out.puts "#{ '%4d'%ifa[:flow_exporter_id]}, #{'%4d'%ifa[:snmp_if_id]}, '#{ifa[:name]}', #{ifa[:purpose]}, #{ifa[:filter]}, '#{ifa[:addr_expected]}', '#{ifa[:addr_observed]}'"
            end
          end
          out.close()
        end
      end # TopologyFilter
    end # Console
  end # Driver
end # FlowBox
