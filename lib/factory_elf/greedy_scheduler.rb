# This loops through first-available elf, and tries to find a good choice of toy, until it can find no more.
# Elves use search params for the best toy based on current productivity rating, and a target productivity rating that
# decays sharply over time.

class FactoryElf::GreedyScheduler
  attr_reader :team, :schedule, :office, :allocated, :submission, :opts

  def initialize opts = {}
    set_opts( opts )
    @team = FactoryElf::Team.new( DateTime.parse("#{@opts[:start_date]}T09:00:00").to_elf_clock )
    @schedule = FactoryElf::Schedule.new
    @office = FactoryElf::Office.new
    @allocated = 0
  end

  def run
    while (@allocated < 10_000_000) do
      elf = @team.first_worker
      targ = adaptive_training_target
      allocations = find_best_toy_for( elf, targ )

      allocations.each do |toy, start_time|
        elf = @team.first_worker
        prod_tag = sprintf( '%0.3f/%0.3f', elf.productivity, targ)
        si = @office.schedule_toy_build( @schedule, @team, toy, start_time )
        @allocated += 1

        if ( @allocated % @opts[:reindex_every] == 0 )
          @office.compress_size_index( @schedule )
        end

        if ( @allocated % @opts[:report_every] == 0 )
          report( si, prod_tag )
        end
      end

      @team.sort_first_worker
    end

    @submission = FactoryElf::Submission.new( @schedule.data )
    score = @submission.verify
    puts "Score: #{score}"
    score
  end

  def self.default_opts
    Hash[
      :reindex_every => 1000,
      :report_every => 10000,
      :start_date => "2014-11-11",
      :tail_target_alpha => 55150,
      :tail_target_beta => 33750,
      :head_training_size => 525.0,
      :train_min_size_day_fraction => 0.252,
      :train_min_size_offset => -40.3,
      :burn_factor => 0.750,
      :double_day_min => 1763.0,
      :double_day_max=> 1502.0,
      :grind_min_size_day_fraction => 0.984,
    ]
  end

  private

  def set_opts opts
    @opts = self.class.default_opts.merge( opts )
  end

  def report si, prod_tag
    puts [ sprintf('%2.2f%%',@allocated/100000.0), ' - ',
      DateTime.from_elf_clock( si.abs_mins ).strftime('%F %H:%M'),
      sprintf('toy:%7d',si.toy_id),
      sprintf('elf:%3d',si.elf_id),
      "rate: #{prod_tag}",
      sprintf('build:%5d',si.duration ),
      ( @tail_mode ? 'T' : '' )
      ].join('  ')
  end

  def find_best_toy_for elf, targ
    next_day_elf = elf.clone
    next_day_elf.skip_rest_of_day

    # Elves prefer to work sanctioned hours until they are trained
    if elf.productivity < targ
      toy = find_training_toy_for( elf )
      return [ [toy, elf.ready_abs_mins] ] if toy
      toy = find_training_toy_for( next_day_elf )
      return [ [toy, next_day_elf.ready_abs_mins] ] if toy
    end

    unless @tail_mode
      if allocations = non_training_toys_today( elf )
        return allocations
      end

      if allocations = non_training_toys_tomorrow( elf, next_day_elf )
        return allocations
      end
    end

    # Nothing better to do? Just go for it . . .
    toy = find_burnout_toy_for( elf )
    return [ [toy, elf.ready_abs_mins] ] if toy
    raise "Ran out of toys"
  end

  def non_training_toys_today elf
    toy = find_grind_toy_for( elf )
    return [ [toy, elf.ready_abs_mins] ] if toy

    if elf.productivity > 3.0 && elf.max_sanctioned_size/elf.productivity > 400
      toy = find_double_day_toy_for( elf )
      return [ [toy, elf.ready_abs_mins] ] if toy
    end

    toy = find_minor_burn_toy_for( elf )
    return [ [toy, elf.ready_abs_mins] ] if toy

    return nil
  end

  def non_training_toys_tomorrow elf, next_day_elf
    nd_toy = find_grind_toy_for( next_day_elf )
    if nd_toy
      toy = find_training_toy_for( elf )
      return [ [toy, elf.ready_abs_mins], [nd_toy, next_day_elf.ready_abs_mins] ] if toy
      return [ [nd_toy, next_day_elf.ready_abs_mins] ]
    end

    if elf.productivity > 3.0
      nd_toy = find_double_day_toy_for( next_day_elf )
      if nd_toy
        toy = find_training_toy_for( elf )
        return [ [toy, elf.ready_abs_mins], [nd_toy, next_day_elf.ready_abs_mins] ] if toy
        return [ [nd_toy, next_day_elf.ready_abs_mins] ]
      end
    end

    nd_toy = find_minor_burn_toy_for( next_day_elf )
    if nd_toy
      toy = find_training_toy_for( elf )
      return [ [toy, elf.ready_abs_mins], [nd_toy, next_day_elf.ready_abs_mins] ] if toy
      return [ [nd_toy, next_day_elf.ready_abs_mins] ]
    end

    return nil
  end

  def find_training_toy_for elf
    # Prevent over-training when in tail mode.
    if @tail_mode
      max_toy_size = elf.best_training_size( @training_target )
      min_toy_size = 0
    else
      max_toy_size = elf.max_sanctioned_size
      min_toy_size = min_size_for_prod( elf.productivity )
    end

    toy = find_toy_for_ready_elf( elf, max_toy_size, min_toy_size )

    toy
  end

  def find_double_day_toy_for elf, over_run = @opts[:double_day_max]
    max_toy_size = elf.max_sanctioned_size + over_run * elf.productivity
    return nil unless max_toy_size > @opts[:double_day_min]  * elf.productivity
    find_toy_for_ready_elf( elf, max_toy_size, (@opts[:double_day_min] * elf.productivity).ceil )
  end

  def min_size_for_prod x
    return 0 if @tail_mode
    max_day_size = 600 * x * @opts[:train_min_size_day_fraction]
    [ ( max_day_size + @opts[:train_min_size_offset] ).floor, 0].max
  end

  def min_grind_size_for_prod x
    max_day_size = 600 * x * @opts[:grind_min_size_day_fraction]
    [ max_day_size.floor, 600 ].max
  end

  def find_grind_toy_for elf
    find_toy_for_ready_elf( elf, elf.max_no_loss_size, min_grind_size_for_prod( elf.productivity ) )
  end

  def find_minor_burn_toy_for elf
    minor_burn_target = ( @opts[:burn_factor] * elf.productivity )
    minor_burn_factor = minor_burn_target / elf.productivity
    minor_burn_size = elf.max_no_loss_size + ( (60.0/elf.productivity) * Math.log(minor_burn_factor)/(-0.10536051565782628)).floor
    find_toy_for_ready_elf( elf, minor_burn_size, min_grind_size_for_prod( elf.productivity ) )
  end

  def find_burnout_toy_for elf
    find_toy_for_ready_elf( elf, 50000 )
  end

  def find_toy_for_ready_elf elf, max_size, min_size = 0
    @office.find_toy( @schedule, 0, elf.ready_abs_mins, max_size, min_size )
  end

  def adaptive_training_target
    @small_toy = @office.find_toy( @schedule, 0, 1000000, 2400, 0 )
    @big_toy = @office.find_toy( @schedule, 0, 1000000, 50000, 0 )
    tt_ratio = @allocated.to_f/10_000_000

    unless @small_toy
      @tail_mode = true
      @training_target = @big_toy.size / ( (1.0 - tt_ratio) * @opts[:tail_target_alpha] + tt_ratio * @opts[:tail_target_beta] )
      return 0.25
    end

    @head_target = @small_toy.size / @opts[:head_training_size]
    @tail_target = @big_toy.size / ( (1.0 - tt_ratio) * @opts[:tail_target_alpha] + tt_ratio * @opts[:tail_target_beta] )

    @tail_mode = ( @tail_target > @head_target )
    @training_target = [ @head_target, @tail_target, 0.263 ].max
    @training_target = [ @training_target, 4.0 ].min
  end
end

class FactoryElf::GreedySchedulerSearch
  START_OPTS = FactoryElf::GreedyScheduler.default_opts

  def self.perturb x, r
    if x.abs > 1.0
      x * ( 1.0 - r + 2 * r * rand() )
    else
      x - r + 2 * r * rand()
    end
  end

  def self.random
    best_score = 1_300_000_000.to_f
    best_hash = START_OPTS.clone

    loop do
      this_hash = START_OPTS.clone.merge( :report_every => 1_000_000 )

      pkeys = [ :train_min_size_day_fraction, :grind_min_size_day_fraction,
                :train_min_size_offset,
                :burn_factor, :double_day_max, :double_day_min,
                :tail_target_alpha, :tail_target_beta,
                :head_training_size
                ]

      pkeys.each do |pk|
        this_hash[pk] = perturb( this_hash[pk], 0.015 )
      end

      solver = FactoryElf::GreedyScheduler.new( this_hash )

      this_score = solver.run
      p [ this_score, this_hash ]

      if this_score < best_score
        best_score = this_score
        best_hash = this_hash
      end

      p [ "BEST", best_score, best_hash ]
    end
  end
end
