require 'date'

class DateTime
  def to_elf_clock
    FactoryElf::Clock.from_yymmdd_hhmm( year, month, day, hour, minute )
  end

  def elf_clock_sanctioned?
    FactoryElf::Clock.sanctioned_minute?( to_elf_clock )
  end

  def elf_clock_next_sanctioned_start
    DateTime.from_elf_clock( FactoryElf::Clock.next_sanctioned_start( to_elf_clock ) )
  end

  def elf_clock_next_sanctioned_end
    DateTime.from_elf_clock( FactoryElf::Clock.next_sanctioned_end( to_elf_clock ) )
  end

  def elf_clock_sanctioned_minutes_up_to next_datetime
    FactoryElf::Clock.sanctioned_minutes_between( self.to_elf_clock, next_datetime.to_elf_clock )
  end

  def elf_clock_non_sanctioned_minutes_up_to next_datetime
    FactoryElf::Clock.non_sanctioned_minutes_between( self.to_elf_clock, next_datetime.to_elf_clock )
  end

  def elf_clock_add_work_minutes duration
    DateTime.from_elf_clock( FactoryElf::Clock.add_work_minutes( to_elf_clock, duration.ceil ) )
  end

  def self.from_elf_clock abs_mins
    self.new( *FactoryElf::Clock.mins_to_yymmdd_hhmm( abs_mins ) ).new_offset( 0 )
  end
end
