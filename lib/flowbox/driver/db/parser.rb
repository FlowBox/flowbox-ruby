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
      class Parser < Base
        def initialize()
          super()
          @generic_driver.register_obs_observer(self)
          @generic_driver.register_stat_observer(self)
        end
        def configure()
          parser_object = ::Parser.find(@db_object_id) 
          throw "DB object not found (ID #{db_object_id})" if parser_object == nil

          parser_config = ParserConfiguration.find(parser_object.parser_configuration_id)
          throw "ParserConfig not found (ID #{parser_object.parser_configuration_id})" if parser_config == nil

          network_setup = NetworkSetup.find_by_active(true)
          throw "Network Setup not found" if network_setup == nil

          flow_exporter_a = FlowExporter.find_all_by_network_setup_id(network_setup.id)

          # build config hash
          config_h = active_record_to_hash(parser_config)
      
          config_h[:output] = @output
          config_h[:flow_exporters] = []
          flow_exporter_a.each do |exporter|
            config_h[:flow_exporters] << active_record_to_hash(exporter)
          end

          case parser_config.source
          when ParserConfiguration::SourceCSGFile
            @generic_driver.configure_core(FlowBox::Core::ParserUnitCSGFileNetflow.new())
          else
            throw "Unknown parser source #{parser_config.source}"
          end
          @generic_driver.configure(config_h)
        end

        def process(object)
          # p object
          # STDOUT.flush()
          object[:parser_id] = @db_object_id
          if object[:msg_type] == 'statistic'

            msg  = "Parser STAT "
            msg += "#{object[:time_s]}, "
            msg += "#{object[:duration_s]}, "
            msg += "#{object[:real_time_s]}, "
            msg += "#{object[:real_duration_s]}, "
            msg += "#{object[:packets]}, "
            msg += "#{object[:iv_min_size]}, "
            msg += "#{object[:iv_format]}, "
            msg += "#{object[:iv_exporter]}, "
            msg += "#{object[:iv_export_time_too_small]}, "
            msg += "#{object[:iv_export_time_too_big]} "
            puts msg

            stat = ParserStatistic.new()
            object_to_active_record(object, stat)

          elsif object[:msg_type] == 'observation'
            msg  = "Parser OBS #{object[:time_s]}, #{object[:msg]}"
            puts msg

            obs = ParserObservation.new()
            object_to_active_record(object, obs)

 #           msg = "'#{object[:msg]}'"
 #           obs = ::ParserObservation.create(
 #             :parser_id => @db_object_id,
 #             :time_s => object[:time_s],
 #             :msg => msg
 #           )
 #           if obs.valid? == false
 #              puts "Can't Save ParserObservation -- check format"
 #           end
          end
          STDOUT.flush()
        end

      end
    end # DB
  end # Driver
end # FlowBox

