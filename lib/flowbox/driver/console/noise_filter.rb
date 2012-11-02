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
    module Console
      class NoiseFilter
        def initialize()
          @generic_driver = FlowBox::Driver::Generic.new()
          @generic_driver.configure_core(FlowBox::Core::NoiseFilter.new())
          @console = FlowBox::Driver::Console::Console.instance()
          @generic_driver.register_obs_observer(self) # default output
          @generic_driver.register_stat_observer(self) # special format
        end
        def configure(configure_h)
          @generic_driver.configure(configure_h)
        end
        def start()
          @generic_driver.start()
        end
        def process(object)
          if object[:msg_type] == 'statistic'
            # we would like to have a specific format
            msg  = "NoiseFilter STAT "
            msg += "#{object[:time_s]}, "
            msg += "#{object[:duration_s]}, "
            msg += "#{object[:flows_in]}, "
            msg += "#{object[:flows_valid]}, "
            msg += "#{object[:flows_kept]}, "
            msg += "#{object[:flows_filtered]} "
            @console.write msg
          elsif object[:msg_type] == 'observation'
            # just use the default output
            msg  = "NoiseFilter OBS #{object[:time_s]}, #{object[:msg]}"
            @console.write msg
          end
        end
        def finished()
          @generic_driver.finished
        end
      end # NoiseFilter
    end # Console
  end # Driver
end # FlowBox

