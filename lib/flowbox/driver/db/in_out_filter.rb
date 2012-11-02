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
      class InOutFilter < Base

        def initialize()
          super()
          @generic_driver.configure_core(FlowBox::Core::InOutFilter.new())
          @generic_driver.register_obs_observer(self)
          @generic_driver.register_stat_observer(self)
        end

        def configure()
          instance = ::InOutFilter.find(@db_object_id) 
          throw "DB object not found (ID #{db_object_id})" if instance == nil

          config = ::InOutFilterConfiguration.find(instance.in_out_filter_configuration_id)
          throw "InOutFilterConfiguration not found (ID #{instance.in_out_filter_configuration_id})" if config == nil

          config_h = active_record_to_hash(config)
          config_h[:input] = @input
          config_h[:output] = @output

          @network_setup = get_current_network_setup()
          internal_prefixes_a = get_internal_prefixes(@network_setup)
          config_h[:prefixes] = []
          internal_prefixes_a.each do |prefix|
            if prefix[:internal]
              config_h[:prefixes] << "#{prefix[:prefix]}@1"
            else
              config_h[:prefixes] << "#{prefix[:prefix]}@0"
            end
          end
          @generic_driver.configure(config_h)
        end

        def process(object)
          object[:in_out_filter_id] = @db_object_id
          if object[:msg_type] == 'statistic'

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

            puts msg

            stat = ::InOutFilterStatistic.new()
            object_to_active_record(object, stat)

          elsif object[:msg_type] == 'observation'
            msg  = "InOutFilter OBS #{object[:time_s]}, #{object[:msg]}"

            puts msg

            obs = ::InOutFilterObservation.new()
            object_to_active_record(object, obs)

          end
          STDOUT.flush()
        end
      end # InOutFilter
    end # DB
  end # Driver
end # FlowBox

