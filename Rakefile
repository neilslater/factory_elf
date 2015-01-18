require "rspec/core/rake_task"
require 'rake/extensiontask'

# Insert project local lib folder into library search path
$: << File.join( File.dirname( __FILE__ ), 'lib' )

require 'factory_elf'

desc "Unit tests"
RSpec::Core::RakeTask.new(:test) do |t|
  t.pattern = "spec/*_spec.rb"
  t.verbose = true
end

Rake::ExtensionTask.new do |ext|
  ext.name = 'factory_elf'
  ext.source_pattern = "*.{c,h}"
  ext.ext_dir = 'ext/factory_elf'
  ext.lib_dir = 'lib/factory_elf'
end

desc "Import CSV"
task :import_csv do
  if FactoryElf.ready_to_run?
    puts "CSV data already imported."
  elsif FactoryElf.ready_to_import?
    FactoryElf.import_toys_from_csv
  else
    puts ''
    puts '**********************************************************'
    puts ' Place Kaggle file toys_rev2.csv in data directory'
    puts '**********************************************************'
    puts ''
    raise "Data file missing: #{FactoryElf::CSV_PATH}"
  end
end

desc "Run greedy scheduler and save submission"
task :solve do
  unless FactoryElf.ready_to_run?
    raise "Not ready, import CSV data first."
  end
  scheduler = FactoryElf::GreedyScheduler.new
  scheduler.run
  submission_file = File.join( FactoryElf::DATA_PATH, 'submission.csv' )
  puts "Writing submission file: #{submission_file}"
  scheduler.submission.write_csv( submission_file )
end

task :default => [:compile, :import_csv, :test, :solve]
