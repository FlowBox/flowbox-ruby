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

class TestDriverConsoleTopologyFilter < MiniTest::Unit::TestCase

=begin
  def setup
    @tf = FlowBox::Driver::Console::TopologyFilter.new()
  end

  def test_check_purpose
    assert_raises(RuntimeError) do 
      FlowBox::Driver::Console::TopologyFilter::check_purpose(-1)
    end
    assert_raises(RuntimeError) do 
      FlowBox::Driver::Console::TopologyFilter::check_purpose(100)
    end
  end

  def check_export_id
    assert_raises(RuntimeError) do 
      FlowBox::Driver::Console::TopologyFilter::check_export_id(-5)
    end
    assert_raises(RuntimeError) do 
      FlowBox::Driver::Console::TopologyFilter::check_export_id('adsf')
    end
  end

  def check_snmp_id
    assert_raises(RuntimeError) do 
      FlowBox::Driver::Console::TopologyFilter::check_snmp_id(-5)
    end
    assert_raises(RuntimeError) do 
      FlowBox::Driver::Console::TopologyFilter::check_snmp_id('adsf')
    end
  end

  def test_get_addr
    [
      ["0.0.0.0", ["0.0.0.0", 0, 4]],
      ["1.2.3.4", ["1.2.3.4", 16909060, 4]],
      ["0::0", ["0::0", 0, 16]]
     ].each do |addr_s, output|
      assert_equal output, FlowBox::Driver::Console::TopologyFilter::get_addr(addr_s)
    end
    assert_raises(ArgumentError) do 
      FlowBox::Driver::Console::TopologyFilter::get_addr("-1.1.1.1")
    end
    assert_raises(ArgumentError) do 
      FlowBox::Driver::Console::TopologyFilter::get_addr("asdf")
    end
  end

  def test_get_addr_from_range
    [
      ["0.0.0.0/32", ["0.0.0.0", 0, 4, 32]],
      ["1.2.3.4/16", ["1.2.3.4", 16909060, 4, 16]],
      ["0::0/32", ["0::0", 0, 16, 32]]
     ].each do |prefix, output|
      assert_equal output, FlowBox::Driver::Console::TopologyFilter::get_addr_from_range(prefix)
    end
    assert_raises(ArgumentError) do 
      FlowBox::Driver::Console::TopologyFilter::get_addr_from_range("-1.1.1.1/32")
    end
    assert_raises(RuntimeError) do 
      FlowBox::Driver::Console::TopologyFilter::get_addr_from_range("1.1.1.1/34")
    end
    assert_raises(RuntimeError) do 
      FlowBox::Driver::Console::TopologyFilter::get_addr_from_range("asdf")
    end
  end

  def test_set_next_hop
    @tf.set_next_hop("1.1.0.0/16", FlowBox::Driver::Console::TopologyFilter::PurposeUnknown)
  end

  def test_predict_purpose_no_prefixes
    [
      ["0.0.0.0/32", [FlowBox::Driver::Console::TopologyFilter::PurposeNextHopZero, "0.0.0.0/32"]],
      ["0::0/64", [FlowBox::Driver::Console::TopologyFilter::PurposeNextHopZero, "::/64"]],
      ["::/64", [FlowBox::Driver::Console::TopologyFilter::PurposeNextHopZero, "::/64"]],
      ["224.0.120.1", [FlowBox::Driver::Console::TopologyFilter::PurposeNextHopMulticast, "224.0.0.0/4"]],
      ["ff00:10::", [FlowBox::Driver::Console::TopologyFilter::PurposeNextHopMulticast, "ff00::/8"]],
      ["224.0.120.1", [FlowBox::Driver::Console::TopologyFilter::PurposeNextHopMulticast, "224.0.0.0/4"]],
      ["fe80:30::", [FlowBox::Driver::Console::TopologyFilter::PurposeNextHopLocal, "fe80::/10"]],
      ["192.168.0.1", [FlowBox::Driver::Console::TopologyFilter::PurposeNextHopUnknown, "192.168.0.1/32"]],
      ["faaa::224:d7ff:fe0a:c1cc", [FlowBox::Driver::Console::TopologyFilter::PurposeNextHopUnknown, "faaa::224:d7ff:fe0a:c1cc/128"]]
    ].each do |prefix, purpose|
      assert_equal purpose, @tf.predict_purpose(prefix)
    end
    assert_raises(ArgumentError) do 
      FlowBox::Driver::Console::TopologyFilter::get_addr_from_range("-1.1.1.1/32")
    end
  end

  def test_predict_purpose_registerd_prefixes  
    @tf.set_next_hop("192.168.0.0/16", FlowBox::Driver::Console::TopologyFilter::PurposeIntranet)
    assert_equal [FlowBox::Driver::Console::TopologyFilter::PurposeIntranet, "192.168.0.0/16"], @tf.predict_purpose("192.168.0.1")
    @tf.set_next_hop("192.168.0.0/24", FlowBox::Driver::Console::TopologyFilter::PurposeInternet)
    assert_equal [FlowBox::Driver::Console::TopologyFilter::PurposeInternet, "192.168.0.0/24"], @tf.predict_purpose("192.168.0.1")
    @tf.set_next_hop("192.168.0.0/28", FlowBox::Driver::Console::TopologyFilter::PurposeIntranet)
    assert_equal [FlowBox::Driver::Console::TopologyFilter::PurposeIntranet, "192.168.0.0/28"], @tf.predict_purpose("192.168.0.1")
  end

  def test_get_local_interface
    interface = @tf.get_interface(1,2)
    assert_equal 1, interface[:export_device_id]
    assert_equal 2, interface[:snmp_if_id]
    assert_equal 0, interface[:addr_observed_h].size()
    assert_equal 0, interface[:addr_ranges_h].size()
  end

  def test_interface_update_observed_addresses
    interface = @tf.get_interface(1,2)
    FlowBox::Driver::Console::TopologyFilter::interface_update_observed_addresses(interface,"192.168.0.1@10, 192.168.0.2@7, ")
    assert_equal 10, interface[:addr_observed_h]["192.168.0.1"]
    assert_equal 7, interface[:addr_observed_h]["192.168.0.2"]
  end

  def test_interface_update_address_ranges_and_purpose
    interface = @tf.get_interface(1,2)
    FlowBox::Driver::Console::TopologyFilter::interface_update_observed_addresses(interface,"192.168.0.1@10, 192.168.0.2@7, ")
    @tf.interface_update_address_ranges_and_purpose(interface)
  end
=end

end
