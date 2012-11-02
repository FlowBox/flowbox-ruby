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
      class InOutFilter
        StatHeader = "# time_s, duration_s, flows, in_in, in_out, out_in, out_out"
        def initialize()
          @generic_driver = FlowBox::Driver::Generic.new()
          @generic_driver.configure_core(FlowBox::Core::InOutFilter.new())
          @console = FlowBox::Driver::Console::Console.instance()
          @generic_driver.register_obs_observer(self)
          @generic_driver.register_stat_observer(self)
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
              ratio = (object[:in_in] + object[:out_out]).to_f/object[:flows] 
            end

            msg  = "InOutFilter STAT "
            msg += "#{object[:time_s]}, "
            msg += "#{object[:duration_s]}, "
            msg += "#{object[:flows]}, "
            msg += "#{object[:in_in]}, "
            msg += "#{object[:in_out]}, "
            msg += "#{object[:out_in]}, "
            msg += "#{object[:out_out]}, "
            msg += "#{'% 5.4f'%ratio}, "
            @console.write msg
          elsif object[:msg_type] == 'observation'
            # just use the default output
            msg  = "InOutFilter OBS #{object[:time_s]}, #{object[:msg]}"
            @console.write msg
          end
        end
        def finished()
          @generic_driver.finished
        end
      end # InOutFilter
    end # Console
  end # Driver
end # FlowBox

