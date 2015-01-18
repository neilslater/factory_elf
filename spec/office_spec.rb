require 'helpers'

describe FactoryElf::Office do
  describe "class method" do
    describe "#new" do
      it "instantiates a new office" do
        o = FactoryElf::Office.new
        expect( o ).to be_a FactoryElf::Office
        expect( o.toys ).to be FactoryElf::TOYS
        expect( o.toys_size_idx ).to be FactoryElf::TOYS_SIZE_IDX
      end
    end
  end

  describe "instance method" do
    before :each do
      @o = FactoryElf::Office.new
      @s = FactoryElf::Schedule.new
    end
    describe "#find_toy" do
      it "can find a toy with a common size" do
        common_sizes = (1..600).to_a
        common_sizes.each do |max_size|
          t = @o.find_toy( @s, 0, 527040, max_size, 0 )
          expect( t.size ).to eql max_size
        end
      end

      it "can find the biggest toy" do
        t = @o.find_toy( @s, 0, 527040, 50000, 0 )
        expect( t.size ).to eql 47473
      end

      it "can find the closest size below max_size" do
        t = @o.find_toy( @s, 0, 527040, 40000, 0 )
        expect( t.size ).to eql 38684
      end

      it "returns next lower size if first option is already scheduled" do
        t1 = @o.find_toy( @s, 0, 527040, 40000, 0 )
        si = FactoryElf::ScheduleItem.new( t1.toy_id, 0, 10000, t1.size )
        @s.store_item( si )
        t2 = @o.find_toy( @s, 0, 527040, 40000, 0 )
        expect( t2.toy_id ).to_not eql t1.toy_id
        expect( t2.size ).to eql 36989
        si = FactoryElf::ScheduleItem.new( t2.toy_id, 1, 10000, t2.size )
        @s.store_item( si )
        t3 = @o.find_toy( @s, 0, 527040, 40000, 0 )
        expect( t3.toy_id ).to_not eql t1.toy_id
        expect( t3.toy_id ).to_not eql t2.toy_id
        expect( t3.size ).to eql 32332
      end

      it "returns next lower size if first option is already scheduled (with compressed size index too!)" do
        t1 = @o.find_toy( @s, 0, 527040, 40000, 0 )
        si = FactoryElf::ScheduleItem.new( t1.toy_id, 0, 10000, t1.size )
        @s.store_item( si )
        @o.compress_size_index( @s )
        t2 = @o.find_toy( @s, 0, 527040, 40000, 0 )
        expect( t2.toy_id ).to_not eql t1.toy_id
        expect( t2.size ).to eql 36989
        si = FactoryElf::ScheduleItem.new( t2.toy_id, 1, 10000, t2.size )
        @s.store_item( si )
        @o.compress_size_index( @s )
        t3 = @o.find_toy( @s, 0, 527040, 40000, 0 )
        expect( t3.toy_id ).to_not eql t1.toy_id
        expect( t3.toy_id ).to_not eql t2.toy_id
        expect( t3.size ).to eql 32332
      end

      it "meets constraints on schedule time" do
        common_sizes = (1..600).to_a
        common_sizes.each do |max_size|
          t = @o.find_toy( @s, 0, 10000, max_size, 0 )
          expect( t.size ).to be <= max_size
          expect( t.abs_mins ).to be <= 10000
        end
      end

      it "meets constraints on id" do
        common_sizes = (1..600).to_a
        common_sizes.each do |max_size|
          t = @o.find_toy( @s, 5000000, 527040, max_size, 0 )
          expect( t.size ).to be <= max_size
          expect( t.toy_id ).to be >= 5000000
        end
      end

      it "returns nothing if size constraints cannot be met" do
        expect( @o.find_toy( @s, 0, 527040, 40000, 39000 ) ).to be_nil
      end

      it "returns nothing if id limits cannot be met" do
        expect( @o.find_toy( @s, 9999000, 527040, 1, 0 ) ).to be_nil
      end

      it "returns nothing if schedule limits cannot be met" do
        expect( @o.find_toy( @s, 0, 5000, 50000, 20000 ) ).to be_nil
      end

      it "eventually returns nothing if all possible matches have been scheduled" do
        while ( t = @o.find_toy( @s, 0, 527040, 1000, 1000 ) ) do
          si = FactoryElf::ScheduleItem.new( t.toy_id, rand(900), 540 + rand(1000000), t.size )
          @s.store_item( si )
        end
        expect(  @o.find_toy( @s, 0, 527040, 1000, 1000 ) ).to be_nil

        # Just to show other toys can still be found . . .
        expect(  @o.find_toy( @s, 0, 527040, 1001, 1001 ) ).to_not be_nil
        expect(  @o.find_toy( @s, 0, 527040, 999, 999 ) ).to_not be_nil

        t = @o.find_toy( @s, 0, 527040, 1000, 0 )
        expect( t.size ).to eql 999
      end

      it "eventually returns nothing if all possible matches have been scheduled (with compressed size index)" do
        while ( t = @o.find_toy( @s, 0, 527040, 1000, 1000 ) ) do
          si = FactoryElf::ScheduleItem.new( t.toy_id, rand(900), 540 + rand(1000000), t.size )
          @s.store_item( si )
        end
        @o.compress_size_index( @s )
        expect(  @o.find_toy( @s, 0, 527040, 1000, 1000 ) ).to be_nil

        # Just to show other toys can still be found . . .
        expect(  @o.find_toy( @s, 0, 527040, 1001, 1001 ) ).to_not be_nil
        expect(  @o.find_toy( @s, 0, 527040, 999, 999 ) ).to_not be_nil

        t = @o.find_toy( @s, 0, 527040, 1000, 0 )
        expect( t.size ).to eql 999
      end
    end

    describe "#count_toys_between" do
      it "gets correct count for size range from complete data set" do
        FactoryElf.cached_toys_size_histogram.each do |min_size,max_size,toy_count,toy_size_total|
          expect( @o.count_toys_between( @s, min_size, max_size ) ).to eql toy_count
        end
      end

      it "gets correct count for size range after an item has been used" do
        num_thousands = @o.count_toys_between( @s, 1000, 1000 )
        while ( t = @o.find_toy( @s, 0, 527040, 1000, 1000 ) ) do
          si = FactoryElf::ScheduleItem.new( t.toy_id, rand(900), 540 + rand(1000000), t.size )
          @s.store_item( si )
        end
        expect( @o.count_toys_between( @s, 1000, 1000 ) ).to eql 0
        expect( @o.count_toys_between( @s, 988,  1597) ).to eql 111128 - num_thousands

        @o.compress_size_index( @s )

        expect( @o.count_toys_between( @s, 1000, 1000 ) ).to eql 0
        expect( @o.count_toys_between( @s, 988,  1597 ) ).to eql 111128 - num_thousands
      end
    end

    describe "#sum_toy_sizes_between" do
      it "gets correct count for size range from complete data set" do
        FactoryElf.cached_toys_size_histogram.each do |min_size,max_size,toy_count,toy_size_total|
          expect( @o.sum_toy_sizes_between( @s, min_size, max_size ) ).to eql toy_size_total
        end
      end

      it "gets correct count for size range after an item has been used" do
        sum_eighties = @o.sum_toy_sizes_between( @s, 80, 80 )
        while ( t = @o.find_toy( @s, 0, 527040, 80, 80 ) ) do
          si = FactoryElf::ScheduleItem.new( t.toy_id, rand(900), 540 + rand(1000000), t.size )
          @s.store_item( si )
        end
        expect( @o.sum_toy_sizes_between( @s, 80, 80 ) ).to eql 0
        expect( @o.sum_toy_sizes_between( @s, 56, 89) ).to eql 69512410 - sum_eighties

        @o.compress_size_index( @s )

        expect( @o.sum_toy_sizes_between( @s, 80, 80 ) ).to eql 0
        expect( @o.sum_toy_sizes_between( @s, 56, 89) ).to eql 69512410 - sum_eighties
      end
    end
  end
end
