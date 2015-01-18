require 'csv'

class FactoryElf::Submission
  CSV_HEADERS = %w(ToyId ElfId StartTime Duration)

  attr_reader :elf_toys, :schedule

  def initialize schedule
    raise ArgumentError, "Schedule must be an NArray" unless schedule.is_a?( NArray )
    raise ArgumentError, "Schedule must contain integers" unless schedule.typecode == 3
    raise ArgumentError, "Schedule must be 10M x 3" unless schedule.shape == [3,10_000_000]
    @schedule = schedule
  end

  def each_schedule_item
    date_idx = @schedule[1,0..9_999_999].sort_index
    (0..9_999_999).each do |i|
      toy_id = date_idx[i]
      yield( toy_id, @schedule[0,toy_id], @schedule[1,toy_id], @schedule[2,toy_id] )
    end
  end

  def verify
    elves = (0..899).map { |i| FactoryElf::Worker.new( i, 1.0 ) }
    @elf_toys = [0] * 900
    toys = FactoryElf.toys
    max_abs_mins = 0

    each_schedule_item do |toy_id,elf_id,start_min,duration|
      elf = elves[elf_id]
      @elf_toys[elf_id] += 1
      toy_order_time = toys[0,toy_id]
      toy_size = toys[1,toy_id]

      if start_min < toy_order_time
        raise "Toy order #{toy_id} not ready. Schedule entry #{[toy_id,elf_id,start_min,duration].inspect}. Toy available at #{toy_order_time}"
      end

      if start_min < elf.ready_abs_mins
        raise "Elf #{elf_id} not ready. Schedule entry #{[toy_id,elf_id,start_min,duration].inspect}. Elf available at #{elf.ready_abs_mins}"
      end

      unless FactoryElf::Clock.sanctioned_minute?( start_min )
        raise "Attempt to start work outside sanctioned hours. Schedule entry #{[toy_id,elf_id,start_min,duration].inspect}"
      end

      if duration < elf.calc_build_time_mins( toy_size )
        raise "Elf #{elf_id} not given enough time for toy. Schedule entry #{[toy_id,elf_id,start_min,duration].inspect}. Elf needs #{elf.calc_build_time_mins( toy_size )}"
      end

      if duration > elf.calc_build_time_mins( toy_size )
        raise "Elf #{elf_id} given too much time for toy. Schedule entry #{[toy_id,elf_id,start_min,duration].inspect}. Elf needs #{elf.calc_build_time_mins( toy_size )}"
      end

      elf.update_for_toy_build( start_min, toy_size  )

      if (start_min + duration) > max_abs_mins
        max_abs_mins = start_min + duration
      end
    end

    max_abs_mins * Math.log(901)
  end

  def analyse_elf elf_id
    elf = FactoryElf::Worker.new( elf_id, 1.0 )
    toys = FactoryElf.toys
    pre_chunks = 0
    current_chunk = nil
    chunks = []

    each_schedule_item do |toy_id,sch_elf_id,start_min,duration|
      next unless elf_id == sch_elf_id
      toy_size = toys[1,toy_id]

      pre_productivity = elf.productivity
      elf.update_for_toy_build( start_min, toy_size )
      record = Hash[
        :pre => pre_productivity, :post => elf.productivity, :start => start_min, :size => toy_size,
        :duration => duration, :toy_id => toy_id,
        :next_ready => elf.ready_abs_mins
      ]

      if pre_productivity == 0.25
        current_chunk = [ record ]
        chunks << current_chunk
      else
        # Continue existing chunk
        if current_chunk
          current_chunk << record
        else
          pre_chunks += 1
        end
      end
    end

    [pre_chunks, chunks]
  end

  def write_csv csv_filename
    csv = CSV.open( csv_filename, 'wb' )
    csv << CSV_HEADERS
    each_schedule_item do |toy_id,elf_id,start_min,duration|
      row = [
        toy_id + 1,
        elf_id + 1,
        (FactoryElf::Clock.mins_to_yymmdd_hhmm( start_min )).join(' '),
        duration
      ]
      csv << row
    end
    csv.close
  end
end
