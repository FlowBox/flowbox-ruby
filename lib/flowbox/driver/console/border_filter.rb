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
      class BorderFilter
        StatHeader = "# time_s, duration_s, flows, other, border_in, border_out, "
        def initialize()
          @generic_driver = FlowBox::Driver::Generic.new()
          @generic_driver.configure_core(FlowBox::Core::BorderFilter.new())
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
            ratio = 0
            if object[:flows] > 0
              border = (object[:border_in] + object[:border_out])
              ratio = border.to_f/object[:flows] 
            end

            msg  = "BorderFilter STAT "
            msg += "#{object[:time_s]}, "
            msg += "#{object[:duration_s]}, "
            msg += "#{object[:flows]}, "
            msg += "#{object[:other]}, "
            msg += "#{object[:border_in]}, "
            msg += "#{object[:border_out]}, "
            msg += "#{'% 5.4f'%ratio}, "
            @console.write msg
          elsif object[:msg_type] == 'observation'
            # just use the default output
            msg  = "BorderFilter OBS #{object[:time_s]}, #{object[:msg]}"
            @console.write msg
          end
        end
        def finished()
          @generic_driver.finished
        end
      end # BorderFilter
    end # Console
  end # Driver
end # FlowBox

