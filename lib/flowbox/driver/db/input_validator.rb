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
      class InputValidator < Base

        def initialize()
          super()
          @generic_driver.configure_core(FlowBox::Core::InputValidator.new())
          @generic_driver.register_obs_observer(self)
          @generic_driver.register_stat_observer(self)
        end
        def configure()
          instance = ::InputValidator.find(@db_object_id) 
          throw "DB object not found (ID #{db_object_id})" if instance == nil

          config = ::InputValidatorConfiguration.find(instance.input_validator_configuration_id)
          throw "InputValidatorConfiguration not found (ID #{instance.input_validator_configuration_id})" if config == nil

          config_h = active_record_to_hash(config)
          config_h[:input] = @input
          config_h[:output] = @output

          @generic_driver.configure(config_h)
        end

        def process(object)
          object[:input_validator_id] = @db_object_id
          if object[:msg_type] == 'statistic'

            msg  = "InputValidator STAT "
            msg += "#{object[:time_s]}, "
            msg += "#{object[:duration_s]}, "
            msg += "#{object[:flows]}, "
            msg += "#{object[:iv_time_freshness]}, "
            msg += "#{object[:iv_time_causality]}, "
            msg += "#{object[:iv_bytes]}, "
            msg += "#{object[:iv_packets]}, "
            msg += "#{object[:iv_mtu]}"
            puts msg

            stat = ::InputValidatorStatistic.new()
            object_to_active_record(object, stat)

          elsif object[:msg_type] == 'observation'
            msg  = "InputValidator OBS #{object[:time_s]}, #{object[:msg]}"
            puts msg

            obs = ::InputValidatorObservation.new()
            object_to_active_record(object, obs)

          end
          STDOUT.flush()
        end
      end # InputValidator
    end # DB
  end # Driver
end # FlowBox

