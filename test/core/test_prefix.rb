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

class TestCorePrefix< MiniTest::Unit::TestCase
  def setup
    @prefix = FlowBox::Core::Prefix.new()
    @prefix_comp = FlowBox::Core::Prefix.new()
  end
  def test_new_prefix
    assert_equal false, @prefix.valid()
  end
  def test_from_txt_failures
    assert_raises(TypeError) do
      @prefix.from(10)
    end
    assert_raises(FlowBox::Core::PrefixError) do
      @prefix.from("asdlkj")
    end
    assert_raises(FlowBox::Core::PrefixError) do
      @prefix.from("192.168.0.1.1/32")
    end
    assert_raises(FlowBox::Core::PrefixError) do
      @prefix.from("192.168.0.1/33")
    end
    assert_raises(FlowBox::Core::PrefixError) do
      @prefix.from("192.168.0.1/-1")
    end
    assert_raises(FlowBox::Core::PrefixError) do
      @prefix.from("1::/-1")
    end
  end
  def test_from_text_cut_wrong_network_addr
      [
        ["255.255.255.255/32","255.255.255.255/32"],
        ["255.255.255.255/31","255.255.255.254/31"],
        ["255.255.255.255/30","255.255.255.252/30"],
        ["255.255.255.255/29","255.255.255.248/29"],
        ["255.255.255.255/28","255.255.255.240/28"],
        ["255.255.255.255/27","255.255.255.224/27"],
        ["255.255.255.255/26","255.255.255.192/26"],
        ["255.255.255.255/25","255.255.255.128/25"],
        ["255.255.255.255/24","255.255.255.0/24"],
        ["255.255.255.255/23","255.255.254.0/23"],
        ["255.255.255.255/22","255.255.252.0/22"],
        ["255.255.255.255/21","255.255.248.0/21"],
        ["255.255.255.255/20","255.255.240.0/20"],
        ["255.255.255.255/19","255.255.224.0/19"],
        ["255.255.255.255/18","255.255.192.0/18"],
        ["255.255.255.255/17","255.255.128.0/17"],
        ["255.255.255.255/16","255.255.0.0/16"],
        ["255.255.255.255/15","255.254.0.0/15"],
        ["255.255.255.255/14","255.252.0.0/14"],
        ["255.255.255.255/13","255.248.0.0/13"],
        ["255.255.255.255/12","255.240.0.0/12"],
        ["255.255.255.255/11","255.224.0.0/11"],
        ["255.255.255.255/10","255.192.0.0/10"],
        ["255.255.255.255/9","255.128.0.0/9"],
        ["255.255.255.255/8","255.0.0.0/8"],
        ["255.255.255.255/7","254.0.0.0/7"],
        ["255.255.255.255/6","252.0.0.0/6"],
        ["255.255.255.255/5","248.0.0.0/5"],
        ["255.255.255.255/4","240.0.0.0/4"],
        ["255.255.255.255/3","224.0.0.0/3"],
        ["255.255.255.255/2","192.0.0.0/2"],
        ["255.255.255.255/1","128.0.0.0/1"],
        ["255.255.255.255/0","0.0.0.0/0"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/127", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:fffe/127"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/126", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:fffc/126"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/125", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:fff8/125"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/124", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:fff0/124"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/123", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffe0/123"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/122", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffc0/122"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/121", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ff80/121"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/120", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ff00/120"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/119", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:fe00/119"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/118", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:fc00/118"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/117", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:f800/117"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/116", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:f000/116"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/115", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:e000/115"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/114", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:c000/114"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/113", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:8000/113"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/112", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:0/112"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/111", "ffff:ffff:ffff:ffff:ffff:ffff:fffe:0/111"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/110", "ffff:ffff:ffff:ffff:ffff:ffff:fffc:0/110"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/109", "ffff:ffff:ffff:ffff:ffff:ffff:fff8:0/109"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/108", "ffff:ffff:ffff:ffff:ffff:ffff:fff0:0/108"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/107", "ffff:ffff:ffff:ffff:ffff:ffff:ffe0:0/107"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/106", "ffff:ffff:ffff:ffff:ffff:ffff:ffc0:0/106"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/105", "ffff:ffff:ffff:ffff:ffff:ffff:ff80:0/105"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/104", "ffff:ffff:ffff:ffff:ffff:ffff:ff00:0/104"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/103", "ffff:ffff:ffff:ffff:ffff:ffff:fe00:0/103"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/102", "ffff:ffff:ffff:ffff:ffff:ffff:fc00:0/102"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/101", "ffff:ffff:ffff:ffff:ffff:ffff:f800:0/101"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/100", "ffff:ffff:ffff:ffff:ffff:ffff:f000:0/100"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/99",  "ffff:ffff:ffff:ffff:ffff:ffff:e000:0/99"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/98",  "ffff:ffff:ffff:ffff:ffff:ffff:c000:0/98"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/97",  "ffff:ffff:ffff:ffff:ffff:ffff:8000:0/97"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/96",  "ffff:ffff:ffff:ffff:ffff:ffff::/96"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/95",  "ffff:ffff:ffff:ffff:ffff:fffe::/95"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/94",  "ffff:ffff:ffff:ffff:ffff:fffc::/94"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/93",  "ffff:ffff:ffff:ffff:ffff:fff8::/93"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/92",  "ffff:ffff:ffff:ffff:ffff:fff0::/92"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/91",  "ffff:ffff:ffff:ffff:ffff:ffe0::/91"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/90",  "ffff:ffff:ffff:ffff:ffff:ffc0::/90"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/89",  "ffff:ffff:ffff:ffff:ffff:ff80::/89"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/88",  "ffff:ffff:ffff:ffff:ffff:ff00::/88"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/87",  "ffff:ffff:ffff:ffff:ffff:fe00::/87"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/86",  "ffff:ffff:ffff:ffff:ffff:fc00::/86"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/85",  "ffff:ffff:ffff:ffff:ffff:f800::/85"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/84",  "ffff:ffff:ffff:ffff:ffff:f000::/84"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/83",  "ffff:ffff:ffff:ffff:ffff:e000::/83"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/82",  "ffff:ffff:ffff:ffff:ffff:c000::/82"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/81",  "ffff:ffff:ffff:ffff:ffff:8000::/81"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/80",  "ffff:ffff:ffff:ffff:ffff::/80"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/79",  "ffff:ffff:ffff:ffff:fffe::/79"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/78",  "ffff:ffff:ffff:ffff:fffc::/78"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/77",  "ffff:ffff:ffff:ffff:fff8::/77"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/76",  "ffff:ffff:ffff:ffff:fff0::/76"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/75",  "ffff:ffff:ffff:ffff:ffe0::/75"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/74",  "ffff:ffff:ffff:ffff:ffc0::/74"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/73",  "ffff:ffff:ffff:ffff:ff80::/73"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/72",  "ffff:ffff:ffff:ffff:ff00::/72"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/71",  "ffff:ffff:ffff:ffff:fe00::/71"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/70",  "ffff:ffff:ffff:ffff:fc00::/70"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/69",  "ffff:ffff:ffff:ffff:f800::/69"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/68",  "ffff:ffff:ffff:ffff:f000::/68"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/67",  "ffff:ffff:ffff:ffff:e000::/67"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/66",  "ffff:ffff:ffff:ffff:c000::/66"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/65",  "ffff:ffff:ffff:ffff:8000::/65"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/64",  "ffff:ffff:ffff:ffff::/64"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/63",  "ffff:ffff:ffff:fffe::/63"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/62",  "ffff:ffff:ffff:fffc::/62"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/61",  "ffff:ffff:ffff:fff8::/61"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/60",  "ffff:ffff:ffff:fff0::/60"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/59",  "ffff:ffff:ffff:ffe0::/59"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/58",  "ffff:ffff:ffff:ffc0::/58"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/57",  "ffff:ffff:ffff:ff80::/57"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/56",  "ffff:ffff:ffff:ff00::/56"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/55",  "ffff:ffff:ffff:fe00::/55"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/54",  "ffff:ffff:ffff:fc00::/54"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/53",  "ffff:ffff:ffff:f800::/53"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/52",  "ffff:ffff:ffff:f000::/52"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/51",  "ffff:ffff:ffff:e000::/51"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/50",  "ffff:ffff:ffff:c000::/50"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/49",  "ffff:ffff:ffff:8000::/49"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/48",  "ffff:ffff:ffff::/48"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/47",  "ffff:ffff:fffe::/47"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/46",  "ffff:ffff:fffc::/46"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/45",  "ffff:ffff:fff8::/45"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/44",  "ffff:ffff:fff0::/44"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/43",  "ffff:ffff:ffe0::/43"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/42",  "ffff:ffff:ffc0::/42"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/41",  "ffff:ffff:ff80::/41"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/40",  "ffff:ffff:ff00::/40"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/39",  "ffff:ffff:fe00::/39"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/38",  "ffff:ffff:fc00::/38"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/37",  "ffff:ffff:f800::/37"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/36",  "ffff:ffff:f000::/36"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/35",  "ffff:ffff:e000::/35"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/34",  "ffff:ffff:c000::/34"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/33",  "ffff:ffff:8000::/33"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/32",  "ffff:ffff::/32"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/31",  "ffff:fffe::/31"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/30",  "ffff:fffc::/30"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/29",  "ffff:fff8::/29"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/28",  "ffff:fff0::/28"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/27",  "ffff:ffe0::/27"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/26",  "ffff:ffc0::/26"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/25",  "ffff:ff80::/25"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/24",  "ffff:ff00::/24"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/23",  "ffff:fe00::/23"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/22",  "ffff:fc00::/22"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/21",  "ffff:f800::/21"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/20",  "ffff:f000::/20"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/19",  "ffff:e000::/19"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/18",  "ffff:c000::/18"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/17",  "ffff:8000::/17"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/16",  "ffff::/16"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/15",  "fffe::/15"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/14",  "fffc::/14"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/13",  "fff8::/13"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/12",  "fff0::/12"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/11",  "ffe0::/11"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/10",  "ffc0::/10"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/9",  "ff80::/9"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/8",  "ff00::/8"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/7",  "fe00::/7"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/6",  "fc00::/6"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/5",  "f800::/5"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/4",  "f000::/4"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/3",  "e000::/3"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/2",  "c000::/2"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/1",  "8000::/1"],
        ["ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/0",  "::/0"]

      ].each do |prefix_in, prefix_out|
        @prefix.from(prefix_in)

        # Since not all systems are handling the ZEROing of IPv6 prefixes the sameway
        # let them to it by itself..
        # e.g ffff:ffff:ffff:ffff:ffff:ffff:ffff::/112 is the same as
        #     ffff:ffff:ffff:ffff:ffff:ffff:ffff:0/112
        @prefix_comp.from(prefix_out)
        # Check now if the cutting is done correctly
        assert_equal  @prefix_comp.to_s(), @prefix.to_s()
      end
  end
  def test_from_text_success
      [ "192.168.0.1/32",
        "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128"
      ].each do |prefix_txt|
        @prefix.from(prefix_txt)
        assert_equal prefix_txt, @prefix.to_s()
      end
  end
  def test_get_valid
    @prefix.clear()
    assert_equal false, @prefix.valid()
    @prefix.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/0")
    assert_equal true, @prefix.valid()
  end
  def test_get_family
    @prefix.clear()
    assert_equal FlowBox::Core::Prefix::FamilyUnkown, @prefix.family()

    @prefix.from("255.255.255.255/32")
    assert_equal FlowBox::Core::Prefix::FamilyIPv4, @prefix.family()

    @prefix.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128")
    assert_equal FlowBox::Core::Prefix::FamilyIPv6, @prefix.family()
  end

  def test_get_length
    @prefix.clear()
    assert_equal 0, @prefix.length()

    0.upto(32) do |i|
      @prefix.from("255.255.255.255/#{i}")
      assert_equal i, @prefix.length()
    end

    0.upto(128) do |i|
      @prefix.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/#{i}")
      assert_equal i, @prefix.length()
    end
  end

  def test_get_max_length
    @prefix.clear()
    assert_equal 0, @prefix.max_length()

    0.upto(32) do |i|
      @prefix.from("255.255.255.255/#{i}")
      assert_equal 32, @prefix.max_length()
    end

    0.upto(128) do |i|
      @prefix.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/#{i}")
      assert_equal 128, @prefix.max_length()
    end
  end

  def test_get_bit_at
    1.upto(32) do |i|
      @prefix.from("255.255.255.255/#{i}")
      assert_equal 1, @prefix.bit_at(i)
    end

    1.upto(128) do |i|
      @prefix.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/#{i}")
      assert_equal 1, @prefix.bit_at(i)
    end
  end

  def test_cut
    1.upto(32) do |i|
      @prefix.from("255.255.255.255/32")
      @prefix.cut(i)
      assert_equal 1, @prefix.bit_at(i-1) if i-1 > 0
      assert_equal i, @prefix.length()
    end

    1.upto(128) do |i|
      @prefix.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128")
       @prefix.cut(i)
      assert_equal 1, @prefix.bit_at(i-1) if i-1 > 0
      assert_equal i, @prefix.length()
    end
  end

  def test_common_bits
    # ipv4
    @prefix.from("255.255.255.255/32")
    other = FlowBox::Core::Prefix.new()
    0.upto(32) do |i|
      other.from("255.255.255.255/#{i}")
      assert_equal i, @prefix.common_bits(other)
      assert_equal i, other.common_bits(@prefix)
    end

    # ipv6
    @prefix.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128")
    other = FlowBox::Core::Prefix.new()
    0.upto(128) do |i|
      other.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/#{i}")
      assert_equal i, @prefix.common_bits(other)
      assert_equal i, other.common_bits(@prefix)
    end

    # mixed ipv6 <-> ipv4
    @prefix.from("255.255.255.255/32")
    other.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128")
    assert_raises(FlowBox::Core::PrefixError) do
      @prefix.common_bits(other)
    end
    assert_raises(FlowBox::Core::PrefixError) do
      other.common_bits(@prefix)
    end
  end
  def test_includes
    other = FlowBox::Core::Prefix.new()
    null = FlowBox::Core::Prefix.new()

    # ipv4
    @prefix.from("255.255.255.255/32")
    null.from("0.0.0.0/32")
    other = FlowBox::Core::Prefix.new()
    1.upto(32) do |i|
      other.from("255.255.255.255/#{i}")
      assert_equal true, @prefix.includes?(other), i
      assert_equal false, other.includes?(@prefix), i if i < 32
      assert_equal false, null.includes?(other), i
      assert_equal false, other.includes?(null), i
    end

    # ipv6
    @prefix.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128")
    null.from("::/128")
    other = FlowBox::Core::Prefix.new()
    1.upto(128) do |i|
      other.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/#{i}")
      assert_equal true, @prefix.includes?(other), i
      assert_equal false, other.includes?(@prefix), i if i < 128
      assert_equal false, null.includes?(other), i
      assert_equal false, other.includes?(null), i
    end

    # mixed ipv6 <-> ipv4
    @prefix.from("255.255.255.255/32")
    other.from("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128")
    assert_raises(FlowBox::Core::PrefixError) do
      @prefix.includes?(other)
    end
    assert_raises(FlowBox::Core::PrefixError) do
      other.includes?(@prefix)
    end
  end
  def test_equal
    other = FlowBox::Core::Prefix.new()
    1.upto(32) do |i|
      @prefix.from("255.255.255.255/#{i}")
      1.upto(32) do |j|
        other.from("255.255.255.255/#{j}")
        if i != j
          assert_equal false, @prefix.equal?(other), "lib claims: '#{@prefix}' == '#{other}' is true"
          assert_equal false, other.equal?(@prefix), "lib claims: '#{other}' == '#{@prefix}' is true"
        else
          assert_equal true, @prefix.equal?(other), "lib claims: '#{@prefix}' == '#{other}' is false"
          assert_equal true, other.equal?(@prefix), "lib claims: '#{other}' == '#{@prefix}' is false"
        end
      end
    end
  end
end
