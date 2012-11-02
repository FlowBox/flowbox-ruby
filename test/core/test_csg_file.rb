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

require 'minitest/autorun'
class TestCoreCSGFile < MiniTest::Unit::TestCase
  def setup
    @csg_file = FlowBox::Core::CSGFile.new()
    @packet =  FlowBox::Core::Packet.new()
    @local_path = File.dirname(__FILE__) + "/"
    @csg_file_stat_ok =  @local_path + "csg_file_ok.stat.bz2"
    @csg_file_dat_ok  =  @local_path + "csg_file_ok.dat.bz2"
  end
  #def test_process_file
    #puts "TestCoreCSGFile::test_process_file"
    #@csg_file.process_file(@csg_file_dat_ok, @csg_file_stat_ok)
    #packets_c = 0
    #while @csg_file.get_error() == 0
      #@csg_file.get_packet(@packet)
      #puts @packet.get_addr()
      #puts @packet.get_length()
      #puts @packet.get_payload().size()
      #packets_c += 1;
      #p packets_c if packets_c%10000 == 0
    #end
    #puts "END: #{packets_c}"
  #end
end
