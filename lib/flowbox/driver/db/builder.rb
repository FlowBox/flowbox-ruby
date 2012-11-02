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
      class Builder
        @@sleep_interval_s = 60
        def initialize()
          @driver_a = [] # the driver object for the unit
          @buffer_a = [] # the buffers between the units
        end
        def build_chain()
          network_setup = get_current_network_setup()
          puts "Network Setup: '#{network_setup.name}' (#{network_setup.id})"

          unit_setup = get_current_unit_setup()
          puts "Unit Setup: '#{unit_setup.name}' (#{unit_setup.id})"

          unit_a = get_units(unit_setup.id()) # the units
          @driver_a = []
          @buffer_a = []

          # link chain
          driver_current = nil
          driver_last = nil
          buffer_current = nil
          unit_a.each do |unit|

            puts "#{unit.priority}, #{unit.species} with id = #{unit.species_id}"
            driver_current = get_driver(unit.species)
            driver_current.set_object_id(unit.species_id)
            @driver_a << driver_current

            if driver_last != nil
              buffer_current = FlowBox::Core::FlowContainerBuffer.new()
              driver_last.set_output(buffer_current)
              driver_current.set_input(buffer_current)
              @buffer_a << buffer_current
            end
            driver_last = driver_current
          end
          driver_last.set_output("NULL")

          # configure it
          @driver_a.each do |driver|
            puts "Configure #{driver}"
            driver.configure()
          end
        end
        def start_chain()
          @driver_a.each do |driver|
            driver.start()
          end
        end
        def wait_on_finish()
          @driver_a.each do |driver|
            while driver.finished() == false
              puts "wait_on_finish: #{Time.now}"
              sleep(@@sleep_interval_s)
            end
          end
        end
        def get_current_network_setup()
          network_setup_a = ::NetworkSetup.find_all_by_active(true)
          throw "Non active Network Setup found" if network_setup_a == nil
          throw "Multiple active Network Setup found" if network_setup_a.size() != 1 
          network_setup = network_setup_a[0]
          return network_setup
        end
        def get_current_unit_setup()
         unit_setup_a = ::UnitSetup.find_all_by_active(true)
          throw "Non active Unit Setup found" if unit_setup_a == nil
          throw "Multiple active Unit Setup found" if unit_setup_a.size() != 1 
          unit_setup = unit_setup_a[0]
          return unit_setup
        end
        def get_units(unit_setup_id)
          unit_a = ::Unit.find(:all, :conditions => ["unit_setup_id = ? AND active = ?", unit_setup_id , true], :order => "priority ASC")
        end
        def get_driver(species)
          raise "Unknown species: '#{species}'" if !(::Unit.valid_species?(species))
          driver = eval("FlowBox::Driver::DB::" + species.camelize() +".new()")
        end
      end # Builder
    end # DB
  end # Driver
end # FlowBox
