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

require 'thread'
module FlowBox
  module Driver
    module DB
      class Base
        @@mutex = Mutex.new
        def initialize()
          @generic_driver = FlowBox::Driver::Generic.new()
          @input = nil
          @output = nil
          @db_object_id = nil
        end
        def set_object_id(db_object_id)
          @db_object_id = db_object_id
        end
        def set_input(buffer)
          @input = buffer
        end
        def set_output(buffer)
          @output = buffer
        end
        def start()
          @generic_driver.start()
        end
        def finished()
          return @generic_driver.finished()
        end
        def object_to_active_record(ob, ar)
          ar.attributes.each do |attr_name, attr_value|
            if ob[attr_name.to_sym] != nil
              ar[attr_name.to_sym] = ob[attr_name.to_sym]
            end
          end

          if ar.valid? == false
            throw "illigal format for #{ar.class}"
          end
          saved_err = nil

          # global mutex ... sucks ... but sqlite crashes otherwise
          if ActiveRecord::Base.connection.adapter_name == 'SQLite'
            @@mutex.synchronize do
              saved_err = ar.save()
            end
          else
            raise RuntimeError, 'Code it first'
          end

          if saved_err  == false
            saved_err = throw "cant save for #{ar.class}"
          end
        end
        def active_record_to_hash(ar)
          obj = {}
          ar.attributes.each do |attr_name, attr_value|
            obj[attr_name.to_sym] = attr_value
          end
          return obj
        end
        def get_current_network_setup()
          network_setup_a = nil
          if ActiveRecord::Base.connection.adapter_name == 'SQLite'
            @@mutex.synchronize do
              network_setup_a = ::NetworkSetup.find_all_by_active(true)
            end
          else
            raise RuntimeError, 'Code it first'
          end
          throw "Non active Network Setup found" if network_setup_a == nil
          throw "Multiple active Network Setup found" if network_setup_a.size() != 1 
          network_setup = network_setup_a[0]
          return network_setup
        end
        def get_next_hop_devices(network_setup)
          next_hop_devices_a = nil
          if ActiveRecord::Base.connection.adapter_name == 'SQLite'
            @@mutex.synchronize do
              next_hop_devices_a = ::NextHopDevice.find_all_by_network_setup_id(network_setup.id())
            end
          else
            raise RuntimeError, 'Code it first'
          end
          return next_hop_devices_a
        end
        def get_interfaces(network_setup)
          interfaces_a = nil
          if ActiveRecord::Base.connection.adapter_name == 'SQLite'
            @@mutex.synchronize do
              interfaces_a = ::Interface.find_all_by_network_setup_id(network_setup.id())
            end
          else
            raise RuntimeError, 'Code it first'
          end
          return(interfaces_a)
        end
        def get_interface(flow_exporter_id, snmp_if_id, network_setup)
          interface = nil
          if ActiveRecord::Base.connection.adapter_name == 'SQLite'
            @@mutex.synchronize do
              interface = ::Interface.find_or_create_by_flow_exporter_id_and_snmp_if_id_and_network_setup_id(flow_exporter_id, snmp_if_id, network_setup.id())
            end
          else
            raise RuntimeError, 'Code it first'
          end
          return(interface)
        end
        def save_interface(interface)
          if ActiveRecord::Base.connection.adapter_name == 'SQLite'
            @@mutex.synchronize do
              interface.save()
            end
          else
            raise RuntimeError, 'Code it first'
          end
        end
        def get_internal_prefixes(network_setup)
          internal_prefixe_a = nil
          if ActiveRecord::Base.connection.adapter_name == 'SQLite'
            @@mutex.synchronize do
              internal_prefixe_a = ::InternalPrefix.find_all_by_network_setup_id(network_setup.id())
            end
          else
            raise RuntimeError, 'Code it first'
          end
          return(internal_prefixe_a)
        end
      end # Base
    end # DB
  end # Driver
end # FlowBox
