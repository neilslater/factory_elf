require 'helpers'

describe FactoryElf::Clock do
  describe "#from_yymmdd_hhmm" do
    it "should return 0 for midnight on 1 Jan 2014" do
      expect( FactoryElf::Clock.from_yymmdd_hhmm( 2014, 1, 1, 0, 0 ) ).to be 0
    end

    it "should return 20 for 00:20 on 1 Jan 2014" do
      expect( FactoryElf::Clock.from_yymmdd_hhmm( 2014, 1, 1, 0, 20 ) ).to be 20
    end

    it "should return 140 for 02:20 on 1 Jan 2014" do
      expect( FactoryElf::Clock.from_yymmdd_hhmm( 2014, 1, 1, 2, 20 ) ).to be 140
    end

    it "should return 6347 for 09:47 on 5 Jan 2014" do
      expect( FactoryElf::Clock.from_yymmdd_hhmm( 2014, 1, 5, 9, 47 ) ).to be 6347
    end

    it "should return number of minutes since 2014-01-01T00:00:00" do
      test_data = [
        [ [ 2014, 1, 15, 9, 47 ], 6347 + 10 * 1440 ],
        [ [ 2014, 1,  1, 23, 59 ], 1439 ],
        [ [ 2014, 1,  2, 0, 0 ], 1440 ],
        [ [ 2014, 1,  31, 23, 59 ], 44639 ],
        [ [ 2014, 2,  1, 0, 0 ], 44640 ],
        [ [ 2014, 3,  1, 0, 0 ], 84960 ],
        [ [ 2014, 4,  1, 0, 0 ], 129600 ],
        [ [ 2014, 5,  1, 0, 0 ], 172800 ],
        [ [ 2014, 6,  1, 0, 0 ], 217440 ],
        [ [ 2014, 7,  1, 0, 0 ], 260640 ],
        [ [ 2014, 8,  1, 0, 0 ], 305280 ],
        [ [ 2014, 9,  1, 0, 0 ], 349920 ],
        [ [ 2014, 10,  1, 0, 0 ], 393120 ],
        [ [ 2014, 11,  1, 0, 0 ], 437760 ],
        [ [ 2014, 12,  1, 0, 0 ], 480960 ],

        [ [ 2015,  1,  1, 0, 0 ], 525600 ],
        [ [ 2015,  2,  1, 0, 0 ], 570240 ],
        [ [ 2015,  2,  28, 0, 0 ], 609120 ],
        [ [ 2015,  3,  1, 0, 0 ], 610560 ],

        [ [ 2016,  1,  1, 0, 0 ], 1051200 ],
        [ [ 2016,  2,  1, 0, 0 ], 1095840 ],
        [ [ 2016,  2,  29, 0, 0 ], 1136160 ],
        [ [ 2016,  3,  1, 0, 0 ], 1137600 ],
      ]

      test_data.each do |ymd_hm, expected_mins|
        expect( FactoryElf::Clock.from_yymmdd_hhmm( *ymd_hm ) ).to eql expected_mins
      end
    end
  end

  describe "#mins_to_yymmdd_hhmm" do
    it "should convert 0 to midnight on 1 Jan 2014" do
      expect( FactoryElf::Clock.mins_to_yymmdd_hhmm( 0 ) ).to eql [2014, 1, 1, 0, 0]
    end

    it "should convert 20 to 00:20 on 1 Jan 2014" do
      expect( FactoryElf::Clock.mins_to_yymmdd_hhmm( 20 ) ).to eql [2014, 1, 1, 0, 20]
    end

    it "should convert 140 to 02:20 on 1 Jan 2014" do
      expect( FactoryElf::Clock.mins_to_yymmdd_hhmm( 140 ) ).to eql [2014, 1, 1, 2, 20]
    end

    it "should convert 6347 to 09:47 on 5 Jan 2014" do
      expect( FactoryElf::Clock.mins_to_yymmdd_hhmm( 6347 ) ).to eql [2014, 1, 5, 9, 47]
    end

    it "should convert number of minutes since 2014-01-01T00:00:00 to correct dates" do
      test_data = [
        [ [ 2014, 1, 15, 9, 47 ], 6347 + 10 * 1440 ],
        [ [ 2014, 1,  1, 23, 59 ], 1439 ],
        [ [ 2014, 1,  2, 0, 0 ], 1440 ],
        [ [ 2014, 1,  31, 23, 59 ], 44639 ],
        [ [ 2014, 2,  1, 0, 0 ], 44640 ],
        [ [ 2014, 3,  1, 0, 0 ], 84960 ],
        [ [ 2014, 4,  1, 0, 0 ], 129600 ],
        [ [ 2014, 5,  1, 0, 0 ], 172800 ],
        [ [ 2014, 6,  1, 0, 0 ], 217440 ],
        [ [ 2014, 7,  1, 0, 0 ], 260640 ],
        [ [ 2014, 8,  1, 0, 0 ], 305280 ],
        [ [ 2014, 9,  1, 0, 0 ], 349920 ],
        [ [ 2014, 10,  1, 0, 0 ], 393120 ],
        [ [ 2014, 11,  1, 0, 0 ], 437760 ],
        [ [ 2014, 12,  1, 0, 0 ], 480960 ],

        [ [ 2015,  1,  1, 0, 0 ], 525600 ],
        [ [ 2015,  2,  1, 0, 0 ], 570240 ],
        [ [ 2015,  2,  28, 0, 0 ], 609120 ],
        [ [ 2015,  3,  1, 0, 0 ], 610560 ],

        [ [ 2016,  1,  1, 0, 0 ], 1051200 ],
        [ [ 2016,  2,  1, 0, 0 ], 1095840 ],
        [ [ 2016,  2,  29, 0, 0 ], 1136160 ],
        [ [ 2016,  3,  1, 0, 0 ], 1137600 ],
      ]

      test_data.each do |expected_ymd_hm, abs_mins|
        expect( FactoryElf::Clock.mins_to_yymmdd_hhmm( abs_mins ) ).to eql expected_ymd_hm
      end
    end

    it "should round trip same integer via from_yymmdd_hhmm" do
      # Bunch of standard minutes, including end of Feb 2014
      (50000..150000).each do |abs_mins|
        ymd_hm = FactoryElf::Clock.mins_to_yymmdd_hhmm( abs_mins )
        expect( FactoryElf::Clock.from_yymmdd_hhmm( *ymd_hm ) ).to eql abs_mins
      end

      # Year boundary 2014/2015
      (490000..530000).each do |abs_mins|
        ymd_hm = FactoryElf::Clock.mins_to_yymmdd_hhmm( abs_mins )
        expect( FactoryElf::Clock.from_yymmdd_hhmm( *ymd_hm ) ).to eql abs_mins
      end

      # End Feb, start March 2016
      (1120000..1140000).each do |abs_mins|
        ymd_hm = FactoryElf::Clock.mins_to_yymmdd_hhmm( abs_mins )
        expect( FactoryElf::Clock.from_yymmdd_hhmm( *ymd_hm ) ).to eql abs_mins
      end
    end
  end

  describe "#sanctioned_minute?" do
    it "should be false at midnight on 1 Jan 2014" do
      expect( FactoryElf::Clock.sanctioned_minute?( 0 ) ).to be false
    end

    it "should be false at 08:59 on 1 Jan 2014" do
      expect( FactoryElf::Clock.sanctioned_minute?( 539 ) ).to be false
    end

    it "should be true at 09:00 on 1 Jan 2014" do
      expect( FactoryElf::Clock.sanctioned_minute?( 540 ) ).to be true
    end

    it "should be true at 10:00 on 1 Jan 2014" do
      expect( FactoryElf::Clock.sanctioned_minute?( 600 ) ).to be true
    end

    it "should be true at 15:00 on 1 Jan 2014" do
      expect( FactoryElf::Clock.sanctioned_minute?( 900 ) ).to be true
    end

    it "should be true at 18:59 on 1 Jan 2014" do
      expect( FactoryElf::Clock.sanctioned_minute?( 1139 ) ).to be true
    end

    it "should be false at 19:00 on 1 Jan 2014" do
      expect( FactoryElf::Clock.sanctioned_minute?( 1140 ) ).to be false
    end

    it "should be false at 20:00 on 1 Jan 2014" do
      expect( FactoryElf::Clock.sanctioned_minute?( 1200 ) ).to be false
    end
  end

  describe "#next_sanctioned_start" do
    it "should always forward to next 09:00" do
      test_data = [
        [1049760, 1050300],
        [1050299, 1050300],
        [1050300, 1050300],
        [1050301, 1051740],
        [1050301, 1051740],
        [1050585, 1051740],
        [1050899, 1051740],
        [1050900, 1051740],
        [1050901, 1051740],
        [1051199, 1051740],
        [1051439, 1051740],
        [1135979, 1136700],
        [1136820, 1138140],
      ]
      test_data.each do |input,expected_output|
        expect( FactoryElf::Clock.next_sanctioned_start(input) ).to eql expected_output
      end
    end
  end

  describe "#next_sanctioned_end" do
    it "should always forward to next 19:00" do
      test_data = [
        [1049760, 1050900],
        [1050299, 1050900],
        [1050300, 1050900],
        [1050301, 1050900],
        [1050301, 1050900],
        [1050585, 1050900],
        [1050899, 1050900],
        [1050900, 1050900],
        [1050901, 1052340],
        [1051199, 1052340],
        [1051439, 1052340],
        [1135979, 1137300],
        [1136820, 1137300],
      ]
      test_data.each do |input,expected_output|
        expect( FactoryElf::Clock.next_sanctioned_end(input) ).to eql expected_output
      end
    end
  end

  describe "#sanctioned_minutes_between" do
    it "should return the number of sanctioned minutes between two times (inclusive)" do
      test_data = [
        [524160, 524759, 60],
        [524730, 524730, 1],
        [524745, 525165, 421],
        [525285, 525299, 15],
        [525285, 525300, 15],
        [525285, 525690, 15],
        [525285, 526139, 15],
        [525285, 526140, 16],
        [525285, 526170, 46],
        [525285, 531930, 2446],
      ]
      test_data.each do | from_abs_mins, to_abs_mins, expected_mins |
        expect( FactoryElf::Clock.sanctioned_minutes_between( from_abs_mins, to_abs_mins ) ).to eql expected_mins
      end
    end
  end

  describe "#non_sanctioned_minutes_between" do
    it "should return the number of non-sanctioned minutes between two times (inclusive)" do
      test_data = [
        [524160, 524759, 540],
        [524730, 524730, 0],
        [524745, 525165, 0],
        [525285, 525299, 0],
        [525285, 525300, 1],
        [525303, 525303, 1],
        [525303, 525312, 10],
        [525285, 525690, 391],
        [525285, 526139, 840],
        [525285, 526140, 840],
        [525285, 526170, 840],
        [525285, 531930, 4200],
      ]
      test_data.each do | from_abs_mins, to_abs_mins, expected_mins |
        expect( FactoryElf::Clock.non_sanctioned_minutes_between( from_abs_mins, to_abs_mins ) ).to eql expected_mins
      end
    end
  end

  describe "#add_work_minutes" do
    it "should always end up with correct next schedule time" do
      test_data = [
        [524160, 1, 524701],
        [524160, 0, 524700],
        [524700, 0, 524700],
        [524700, 1, 524701],
        [524700, 65, 524765],
        [524700, 605, 526145],
        [84060, 605, 85505],
        [1135260, 605, 1136705],
        [1136700, 605, 1138145],
        [1136700, 1440, 1139820],
      ]
      test_data.each do | from_abs_mins, work_mins, expected_to_abs_mins |
        expect( FactoryElf::Clock.add_work_minutes( from_abs_mins, work_mins ) ).to eql expected_to_abs_mins
      end
    end
  end

  describe "#sanctioned_minutes_remaining" do
    it "should be 0 for any non-sanctioned minute" do
      expect( FactoryElf::Clock.sanctioned_minutes_remaining( 0 ) ).to eql 0
      expect( FactoryElf::Clock.sanctioned_minutes_remaining( 539 ) ).to eql 0
      expect( FactoryElf::Clock.sanctioned_minutes_remaining( 1140 ) ).to eql 0
      expect( FactoryElf::Clock.sanctioned_minutes_remaining( 1439 ) ).to eql 0
    end

    it "should equal remaining minutes in current sanctioned time window" do
      expect( FactoryElf::Clock.sanctioned_minutes_remaining( 540 ) ).to eql 600
      expect( FactoryElf::Clock.sanctioned_minutes_remaining( 600 ) ).to eql 540
      expect( FactoryElf::Clock.sanctioned_minutes_remaining( 1139 ) ).to eql 1
    end
  end
end

###################################################################################################

describe DateTime do
  describe "#from_elf_clock" do
    it "should convert 0 to 2014-01-01T00:00:00" do
      expect( DateTime.from_elf_clock(0) ).to eql DateTime.parse('2014-01-01T00:00:00')
    end

    it "should convert 6347 to 2014-01-05T09:47:00" do
      expect( DateTime.from_elf_clock(6347) ).to eql DateTime.parse('2014-01-05T09:47:00')
    end
  end

  describe "#elf_clock_sanctioned?" do
    it "should be true between 09:00 and 18:59 every day" do
      dts = ['2014-01-05T09:47:00','2014-02-28T10:17:00','2014-04-11T12:01:00',
             '2014-05-03T13:13:00','2015-07-30T16:27:00','2014-12-11T18:47:00' ]
      dts.each do |dt|
        expect( DateTime.parse( dt ).elf_clock_sanctioned? ).to be true
      end
    end

    it "should be false before 09:00 every day" do
      dts = ['2014-01-01T08:59:00','2014-12-31T07:19:00','2015-04-12T07:59:00',
             '2014-11-23T08:30:00','2014-09-18T08:59:00','2014-05-30T00:00:00',]
      dts.each do |dt|
        expect( DateTime.parse( dt ).elf_clock_sanctioned? ).to be false
      end
    end

    it "should be false at or after 19:00 every day" do
      dts = ['2014-01-11T20:59:00','2014-12-31T19:19:00','2015-04-12T23:59:00',
             '2015-01-23T21:30:00','2014-10-18T20:59:00','2014-07-30T19:00:00']
      dts.each do |dt|
        expect( DateTime.parse( dt ).elf_clock_sanctioned? ).to be false
      end
    end
  end

  describe "#to_elf_clock" do
    it "should cope with leap years" do
      end_feb = DateTime.parse( '2016-02-29T23:59:00' )
      start_mar = DateTime.parse( '2016-03-01T00:00:00' )
      expect( start_mar.to_elf_clock - end_feb.to_elf_clock ).to eql 1

      end_feb = DateTime.parse( '2015-02-28T23:59:00' )
      start_mar = DateTime.parse( '2015-03-01T00:00:00' )
      expect( start_mar.to_elf_clock - end_feb.to_elf_clock ).to eql 1
    end
  end

  describe "converting to/from elf-clock" do
    it "should round trip same integer via DateTime.from_elf_clock and DateTime#to_elf_clock" do
      # Bunch of standard minutes, including end of Feb 2014 and start of daylight savings
      (50000..150000).each do |abs_mins|
        dt = DateTime.from_elf_clock( abs_mins )
        expect( dt.to_elf_clock ).to eql abs_mins
      end

      # Year boundary 2014/2015
      (490000..530000).each do |abs_mins|
        dt = DateTime.from_elf_clock( abs_mins )
        expect( dt.to_elf_clock ).to eql abs_mins
      end

      # End Feb, start March 2016
      (1120000..1140000).each do |abs_mins|
        dt = DateTime.from_elf_clock( abs_mins )
        expect( dt.to_elf_clock ).to eql abs_mins
      end
    end

    it "should round trip same date/time value via DateTime#to_elf_clock DateTime.from_elf_clock" do
      test_dates = [
        '2016-12-31T00:00', '2016-12-31T09:00', '2020-01-11T12:12', '2020-12-31T19:00',
        '2015-12-31T00:00', '2015-12-31T09:00', '2014-12-31T18:45', '2014-12-31T19:00',
      ].map { |dt_str| DateTime.parse( dt_str ) }

      test_dates.each do |dt|
        abs_mins = dt.to_elf_clock
        expect( DateTime.from_elf_clock( abs_mins ) ).to eql dt
      end
    end
  end

  describe "#elf_clock_next_sanctioned_start" do
    it "should always forward to next 09:00" do
      test_data = [
        [ '2015-12-31T00:00', '2015-12-31T09:00' ],
        [ '2015-12-31T08:59', '2015-12-31T09:00' ],
        [ '2015-12-31T09:00', '2015-12-31T09:00' ],
        [ '2015-12-31T09:01', '2016-01-01T09:00' ],
        [ '2015-12-31T09:01', '2016-01-01T09:00' ],
        [ '2015-12-31T13:45', '2016-01-01T09:00' ],
        [ '2015-12-31T18:59', '2016-01-01T09:00' ],
        [ '2015-12-31T19:00', '2016-01-01T09:00' ],
        [ '2015-12-31T19:01', '2016-01-01T09:00' ],
        [ '2015-12-31T23:59', '2016-01-01T09:00' ],
        [ '2016-01-01T03:59', '2016-01-01T09:00' ],
        [ '2016-02-28T20:59', '2016-02-29T09:00' ],
        [ '2016-02-29T11:00', '2016-03-01T09:00' ],
      ]

      test_data.each do |input,expected_output|
        in_dt = DateTime.parse(input)
        out_expect_dt = DateTime.parse(expected_output)
        expect( in_dt.elf_clock_next_sanctioned_start ).to eql out_expect_dt
      end
    end
  end

  describe "#elf_clock_next_sanctioned_end" do
    it "should always forward to next 19:00" do
      test_data = [
        [ '2015-12-31T00:00', '2015-12-31T19:00' ],
        [ '2015-12-31T08:59', '2015-12-31T19:00' ],
        [ '2015-12-31T09:00', '2015-12-31T19:00' ],
        [ '2015-12-31T09:01', '2015-12-31T19:00' ],
        [ '2015-12-31T09:01', '2015-12-31T19:00' ],
        [ '2015-12-31T13:45', '2015-12-31T19:00' ],
        [ '2015-12-31T18:59', '2015-12-31T19:00' ],
        [ '2015-12-31T19:00', '2015-12-31T19:00' ],
        [ '2015-12-31T19:01', '2016-01-01T19:00' ],
        [ '2015-12-31T23:59', '2016-01-01T19:00' ],
        [ '2016-01-01T03:59', '2016-01-01T19:00' ],
        [ '2016-02-28T20:59', '2016-02-29T19:00' ],
        [ '2016-02-29T11:00', '2016-02-29T19:00' ],
      ]

      test_data.each do |input,expected_output|
        in_dt = DateTime.parse(input)
        out_expect_dt = DateTime.parse(expected_output)
        expect( in_dt.elf_clock_next_sanctioned_end ).to eql out_expect_dt
      end
    end
  end

  describe "#elf_clock_sanctioned_minutes_up_to" do
    it "should return the number of sanctioned minutes between two times (inclusive)" do
      test_data = [
        [ '2014-12-31T00:00', '2014-12-31T09:59',  60 ],
        [ '2014-12-31T09:30', '2014-12-31T09:30',   1 ],
        [ '2014-12-31T09:45', '2014-12-31T16:45', 421 ],
        [ '2014-12-31T18:45', '2014-12-31T18:59',  15 ],
        [ '2014-12-31T18:45', '2014-12-31T19:00',  15 ],
        [ '2014-12-31T18:45', '2015-01-01T01:30',  15 ],
        [ '2014-12-31T18:45', '2015-01-01T08:59',  15 ],
        [ '2014-12-31T18:45', '2015-01-01T09:00',  16 ],
        [ '2014-12-31T18:45', '2015-01-01T09:30',  46 ],
        [ '2014-12-31T18:45', '2015-01-05T09:30', 2446 ],
      ]
      test_data.each do | from_str, to_str, expected_mins |
        from_dt = DateTime.parse( from_str )
        to_dt = DateTime.parse( to_str )
        expect( from_dt.elf_clock_sanctioned_minutes_up_to( to_dt ) ).to eql expected_mins
      end
    end
  end

  describe "#elf_clock_non_sanctioned_minutes_up_to" do
    it "should return the number of non-sanctioned minutes between two times (inclusive)" do
      test_data = [
        [ '2014-12-31T00:00', '2014-12-31T09:59', 540 ],
        [ '2014-12-31T09:30', '2014-12-31T09:30',   0 ],
        [ '2014-12-31T09:45', '2014-12-31T16:45',   0 ],
        [ '2014-12-31T18:45', '2014-12-31T18:59',   0 ],
        [ '2014-12-31T18:45', '2014-12-31T19:00',   1 ],
        [ '2014-12-31T19:03', '2014-12-31T19:03',   1 ],
        [ '2014-12-31T19:03', '2014-12-31T19:12',  10 ],
        [ '2014-12-31T18:45', '2015-01-01T01:30', 391 ],
        [ '2014-12-31T18:45', '2015-01-01T08:59', 840 ],
        [ '2014-12-31T18:45', '2015-01-01T09:00', 840 ],
        [ '2014-12-31T18:45', '2015-01-01T09:30', 840 ],
        [ '2014-12-31T18:45', '2015-01-05T09:30', 4200 ],
      ]
      test_data.each do | from_str, to_str, expected_mins |
        from_dt = DateTime.parse( from_str )
        to_dt = DateTime.parse( to_str )
        expect( from_dt.elf_clock_non_sanctioned_minutes_up_to( to_dt ) ).to eql expected_mins
      end
    end
  end

  describe "#elf_clock_add_work_minutes" do
    it "should always end up with correct next schedule time" do
      test_data = [
        [ '2014-12-31T00:00', 1, '2014-12-31T09:01' ],
        [ '2014-12-31T00:00', 0, '2014-12-31T09:00' ],
        [ '2014-12-31T09:00', 0, '2014-12-31T09:00' ],
        [ '2014-12-31T09:00', 1, '2014-12-31T09:01' ],
        [ '2014-12-31T09:00', 65, '2014-12-31T10:05' ],
        [ '2014-12-31T09:00', 605, '2015-01-01T09:05' ],
        [ '2014-02-28T09:00', 605, '2014-03-01T09:05' ],
        [ '2016-02-28T09:00', 605, '2016-02-29T09:05' ],
        [ '2016-02-29T09:00', 605, '2016-03-01T09:05' ],
        [ '2016-02-29T09:00', 1440, '2016-03-02T13:00' ],
      ]
      test_data.each do | from_str, work_mins, expected_str |
        from_dt = DateTime.parse( from_str )
        expected_dt = DateTime.parse( expected_str )
        expect( from_dt.elf_clock_add_work_minutes( work_mins ) ).to eql expected_dt
      end
    end
  end
end
