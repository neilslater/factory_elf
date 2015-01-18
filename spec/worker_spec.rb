require 'helpers'

describe FactoryElf::Worker do
  describe "class method" do
    describe "#new" do
      it "instantiates a new worker with supplied id and productivity" do
        w = FactoryElf::Worker.new(1, 1.5)
        expect( w ).to be_a FactoryElf::Worker
        expect( w.id ).to eql 1
        expect( w.productivity ).to eql 1.5
        expect( w.ready_abs_mins ).to eql 9 * 60
      end
    end
  end

  describe "instance method" do
    before :each do
      @w = FactoryElf::Worker.new( 7, 1.25 )
    end

    describe "#clone" do
      it "should copy a Worker object and its properties" do
        w_copy = @w.clone
        expect( w_copy ).to be_a FactoryElf::Worker
        expect( w_copy ).to_not be @w
        expect( w_copy.id ).to eql @w.id
        expect( w_copy.productivity ).to eql @w.productivity
      end
    end

    describe "#calc_new_productivity" do
      it "should calculate the worker's productivity after supplied times in minutes" do
        expect( @w.calc_new_productivity( 60, 0 ) ).to be_within( 1e-6 ).of 1.25 * 1.02
        expect( @w.calc_new_productivity( 0, 60 ) ).to be_within( 1e-6 ).of 1.25 * 0.9
        expect( @w.calc_new_productivity( 30, 20 ) ).to be_within( 1e-6 ).of 1.25 * ( 1.02 ** 0.5 ) * ( 0.9 ** 0.3333333 )
      end

      it "should have a maximum result of 4.0" do
        w = FactoryElf::Worker.new( 12, 3.9 )
        expect( w.calc_new_productivity( 800, 0 ) ).to be_within( 1e-6 ).of 4.0
        expect( w.calc_new_productivity( 2000, 120 ) ).to be_within( 1e-6 ).of 4.0
      end

      it "should have a minimum result of 0.25" do
        expect( @w.calc_new_productivity( 0, 1200 ) ).to be_within( 1e-6 ).of 0.25
        expect( @w.calc_new_productivity( 6000, 6000 ) ).to be_within( 1e-6 ).of 0.25
      end
    end

    describe "#skip_rest_of_day" do
      it "should keep existing time if on day start" do
        @w.skip_rest_of_day
        expect( @w.ready_abs_mins ).to eql 540
      end

      it "should move to start of next day, if day is partway done" do
        @w.update_for_toy_build(0,60)
        @w.skip_rest_of_day
        expect( @w.ready_abs_mins ).to eql 1980
      end
    end

    describe "#calc_build_time_mins" do
      it "should calculate the build time according to productivity" do
        expect( @w.calc_build_time_mins( 120 ) ).to eql 96
      end

      it "should round up" do
        expect( @w.calc_build_time_mins( 121 ) ).to eql 97
      end
    end

    describe "#predict_productivity" do
      it "should predict correct result for productivity at a particular time and duration" do
        test_data = [
           [ 540, 750, 1.25 * 1.02 ** 10 ],
           [ 1440, 75, 1.25 * 0.9 ],
        ]
        test_data.each do |start_mins, size, expected_productivity|
          expect( @w.predict_productivity( start_mins, size ) ).to be_within(1e-9).of expected_productivity
        end
      end
    end

    describe "#update_for_toy_build" do
      it "should take progressively less time to build same toy" do
        build_mins = [ @w.ready_abs_mins ] + (0..9).map {|i| @w.update_for_toy_build(0,60); @w.ready_abs_mins }
        build_sizes = build_mins.each_cons(2).map { |b1,b2| b2-b1 }
        expect( build_sizes ).to eql [48, 48, 47, 46, 46, 45, 44, 44, 43, 42]
      end

      it "should improve worker's productivity during sanctioned time" do
        10.times { @w.update_for_toy_build(0,60) }
        expect( @w.productivity ).to be_within(0.0001).of 1.4516
      end

      it "should improve worker's productivity the same for one large present" do
        @w.update_for_toy_build( 0,566 ) # This is sum of work times on 60-min presents above, times 1.25
        expect( @w.productivity ).to be_within(0.0001).of 1.4516
      end

      it "should add overtime penalties" do
        @w.update_for_toy_build( 0,1250 ) # This is sum of work times on 60-min presents above, times 1.25
        expect( @w.productivity ).to be_within(0.0001).of 1.25 * ( 1.02 ** 10 ) * ( 0.9 ** 6.66666667 )
        expect( @w.ready_abs_mins ).to eql 9 * 60 + 1440 + 400
      end

      it "should allow directly schedulung start of the build" do
        @w.update_for_toy_build( 18 * 60, 60 )
        expect( @w.productivity ).to be_within(0.0001).of 1.25 * ( 1.02 ** (48.0/60) )
        expect( @w.ready_abs_mins ).to eql 18 * 60 + 48
      end

      it "should raise an error if the worker is asked to schedule a build in its past" do
        expect {
          @w.update_for_toy_build( 60, 60 )
        }.to raise_error ArgumentError
      end
    end

    describe "#max_sanctioned_size" do
      it "should predict the maximum toy size that fits in available time" do
        expect( @w.max_sanctioned_size ).to eql 750
      end

      it "should update when time and productivity have updated" do
        @w.update_for_toy_build( 10 * 60, 60 )
        expect( @w.max_sanctioned_size ).to eql 624
      end
    end

    describe "#best_training_size" do
      it "should predict the toy size that results in a desired productivity" do
        w = FactoryElf::Worker.new( 0, 3.3 )
        expect( w.best_training_size( 3.5 ) ).to eql 589
      end

      it "should predict maximum use of the day if not possible to train to target in given time" do
        w = FactoryElf::Worker.new( 0, 3.1 )
        expect( w.best_training_size( 4.0 ) ).to eql (600 * 3.1).floor
      end
    end

    describe "#max_no_loss_size" do
      it "should predict a toy size that if scheduled" do
        prediction = @w.max_no_loss_size
        expect(prediction).to be > 750
        @w.update_for_toy_build( 0, prediction )
        expect( @w.productivity ).to be_within(0.01).of 1.25
        expect( @w.productivity ).to be > 1.25
      end

      it "should work when time and productivity have updated" do
        @w.update_for_toy_build( 10 * 60, 60 )

        prediction = @w.max_no_loss_size
        expect(prediction).to be > 624
        @w.update_for_toy_build( 0, prediction )
        expect( @w.productivity ).to be_within(0.01).of 1.25 * ( 1.02 ** (48.0/60) )
        expect( @w.productivity ).to be > 1.25 * ( 1.02 ** (48.0/60) )
      end

      it "should predict the boundary accurately for start of day" do
        prediction = @w.max_no_loss_size
        @w.update_for_toy_build( 0, prediction + 1 )
        expect( @w.productivity ).to be < 1.25
      end

      it "should predict the boundary accurately when time and productivity have updated" do
        @w.update_for_toy_build( 10 * 60, 60 )

        prediction = @w.max_no_loss_size
        @w.update_for_toy_build( 0, prediction + 1 )
        expect( @w.productivity ).to be < 1.25 * ( 1.02 ** (48.0/60) )
      end
    end
  end
end
