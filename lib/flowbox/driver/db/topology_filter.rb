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
  module Driver
    module DB
      class TopologyFilter < Base
        def initialize()
          super()
          @generic_driver.configure_core(FlowBox::Core::TopologyFilter.new())
          @generic_driver.register_obs_observer(self)
          @generic_driver.register_stat_observer(self)
        end
        def configure()
          instance = ::TopologyFilter.find(@db_object_id) 
          throw "DB object not found (ID #{db_object_id})" if instance == nil

          config = ::TopologyFilterConfiguration.find(instance.topology_filter_configuration_id)
          throw "TopologyFilterConfiguration not found (ID #{instance.topology_filter_configuration_id})" if config == nil

          config_h = active_record_to_hash(config)
          config_h[:input] = @input
          config_h[:output] = @output

          @network_setup = get_current_network_setup()
          @next_hop_devices_a = get_next_hop_devices(@network_setup)
          @next_hop_device_parsed_a = parsed_next_hop_devices(@next_hop_devices_a)

          config_h[:interfaces] = []
          interfaces_a = get_interfaces(@network_setup)
          interfaces_a.each do |interface|
            config_h[:interfaces] << active_record_to_hash(interface)
          end
          @generic_driver.configure(config_h)
        end
        def parsed_next_hop_devices(next_hop_devices_a)

          next_hop_device_parsed_a = []
          next_hop_devices_a.each do |next_hop_device|

            range = next_hop_device[:address]
            purpose = next_hop_device[:purpose]

            # parse input
            addr_s, addr_n, addr_length, network = FlowBox::Core::TopologyFilter.get_addr_from_range(range)
            purpose = purpose.to_i
            FlowBox::Core::TopologyFilter.check_purpose(purpose)

            # add range
            data = { :range => range,
              :addr_s => addr_s,
              :addr_n => addr_n,
              :addr_length => addr_length,
              :network => network,
              :purpose => purpose
            }
            next_hop_device_parsed_a << data
          end
          return next_hop_device_parsed_a
        end

        def process(object)
          object[:topology_filter_id] = @db_object_id
          if object[:msg_type] == 'statistic'

            ratio = 0
            if object[:flows] > 0
              ratio = (object[:flows_filtered]).to_f/object[:flows] 
            end

            msg  = "TopologyFilter STAT "
            msg += "#{object[:time_s]}, "
            msg += "#{object[:duration_s]}, "
            msg += "#{object[:flows]}, "
            msg += "#{object[:flows_filtered]}, "
            msg += "#{'% 5.4f'%ratio}, "
            puts msg

            # log the statistic
            stat = ::TopologyFilterStatistic.new()
            object_to_active_record(object, stat)

          elsif object[:msg_type] == 'observation'

            msg  = "TopologyFilter OBS #{object[:time_s]}, #{object[:flow_exporter_id]}, #{object[:snmp_if_id]}, #{object[:state]},  #{object[:addr_observed][0, 10]}"
            puts msg

            # log the observation
            obs = ::TopologyFilterObservation.new()
            object_to_active_record(object, obs)

            # process the observation
            process_observation(object)
          end
          STDOUT.flush()
        end
        def process_observation(observation_h)
          flow_exporter_id = observation_h[:flow_exporter_id]
          snmp_if_id = observation_h[:snmp_if_id]
          addr_observed = observation_h[:addr_observed]
          state = observation_h[:state]
          FlowBox::Core::TopologyFilter.check_obs(flow_exporter_id, snmp_if_id, addr_observed)

          # get interface object
          interface = get_interface(flow_exporter_id, snmp_if_id, @network_setup)

          # update interface object
          update_interface(interface, addr_observed, state)

        end
        def addr_to_next_hop(addr_observed_h)
          purpose_a = []
          next_hop_a = []
          addr_observed_h.each do |addr_s, count|
            purpose, range = FlowBox::Core::TopologyFilter.predict_purpose(addr_s, @next_hop_device_parsed_a)
            purpose_a << purpose
            next_hop_a << range
          end         
          purpose_a.uniq!()
          next_hop_a.uniq!()
          return [purpose_a, next_hop_a]
        end
        def update_interface(interface, addr_observed, state)
          addr_observed_h = FlowBox::Core::TopologyFilter.obs_addr_to_h(addr_observed)
          purpose_a, next_hop_a = addr_to_next_hop(addr_observed_h)
          purpose, note = FlowBox::Core::TopologyFilter.merge_types(purpose_a)

          # set filter rule based on the purpose
          filter = FlowBox::Core::TopologyFilter::FilterNon
          if purpose == FlowBox::Core::TopologyFilter::PurposeUnknown
            filter = FlowBox::Core::TopologyFilter::FilterNon
          elsif purpose == FlowBox::Core::TopologyFilter::PurposeInternet
            filter = FlowBox::Core::TopologyFilter::FilterNon
          elsif purpose == FlowBox::Core::TopologyFilter::PurposeIntranet
            filter = FlowBox::Core::TopologyFilter::FilterAll
          elsif purpose == FlowBox::Core::TopologyFilter::PurposeMixed
            filter = FlowBox::Core::TopologyFilter::FilterNon
          else
            raise "WTF: purpose ?? unknown #{purpose}"            
          end

          # addr_expected
          addr_expected = next_hop_a*","

          interface[:note] = note
          interface[:purpose] = purpose
          interface[:addr_expected] = addr_expected
          interface[:filter] = filter
          interface[:addr_observed] = addr_observed
          interface[:state] = state
          save_interface(interface)
        end
      end # TopologyFilter
    end # DB
  end # Driver
end # FlowBox

