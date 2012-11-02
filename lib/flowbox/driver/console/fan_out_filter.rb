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
      class FanOutFilter
        def initialize()
          @generic_driver = FlowBox::Driver::Generic.new()
          @generic_driver.configure_core(FlowBox::Core::FanOutFilter.new())
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
            msg  = "FanOutFilter STAT "
            msg += "#{object[:time_s]}, "
            msg += "#{object[:duration_s]}, "
            msg += "#{object[:duration_all_build]}, "
            msg += "#{object[:duration_selection_build]}, "
            msg += "#{object[:biflows_in]}, "
            msg += "#{object[:biflows_kept]}, "
            msg += "#{object[:biflows_filtered]}, "
            msg += "#{object[:all_in]}, "
            msg += "#{object[:all_out]}, "
            msg += "#{object[:selection_in]}, "
            msg += "#{object[:selection_out]}, "
            msg += "#{object[:buckets_all_in]}, "
            msg += "#{object[:buckets_all_out]}, "
            msg += "#{object[:buckets_selection_in]}, "
            msg += "#{object[:buckets_selection_in]} "
            @console.write msg
          elsif object[:msg_type] == 'observation'
            # just use the default output
            msg  = "FanOutFilter OBS #{object[:time_s]}, #{object[:msg]}"
            @console.write msg
          end
        end
        def finished()
          @generic_driver.finished
        end
      end # FanOutFilter
    end # Console
  end # Driver
end # FlowBox

