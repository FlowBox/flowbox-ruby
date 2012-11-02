#encoding: utf-8
########################################################################
# FLOWBOX
########################################################################
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

require 'rubygems'
require 'bundler'
require 'rake'
require 'jeweler'
require 'rake/testtask'
require 'rake/clean'
#require 'rcov/rcovtask'
require 'rdoc/task'

begin
  Bundler.setup(:default, :development)
rescue Bundler::BundlerError => e
  $stderr.puts e.message
  $stderr.puts "Run `bundle install` to install missing gems"
  exit e.status_code
end

Jeweler::Tasks.new do |gem|
  # gem is a Gem::Specification...
  # see http://docs.rubygems.org/read/chapter/20 for more options
  gem.name = "flowbox"
  gem.homepage = "http://github.com/FlowBox/flowbox-ruby"
  gem.license = "Gnu LGPL v3"
  gem.summary = %Q{Ruby wrapper for FlowBox libflowbox}
  gem.description = %Q{FlowBox: A ToolBox for online stream-processing of voluminous flow data}
  gem.email       = 'schadomi@gmail.com'
  gem.homepage    = 'http://flowbox.github.com'
  gem.authors = ["Dominik Schatzmann","Daniel Aschwanden"]
  gem.extensions << 'ext/flowbox/extconf.rb'
  gem.has_rdoc = false
  gem.required_ruby_version = '>= 1.9.2'
  gem.test_files = Dir.glob("test/test_*.rb")
  gem.executables = ['flow_to_txt.rb', 'demo_db.rb']
end
Jeweler::RubygemsDotOrgTasks.new


## BUILD the extensions ########################################################
# see 'Add Rake tasks' @ http://guides.rubygems.org/c-extensions/
# rule to build the extension: this says that the extension should be rebuilt
# after any change to the files in ext
file "lib/flowbox/flowbox.#{RbConfig::CONFIG['DLEXT']}" => Dir.glob("ext/flowbox/*{.rb,.c}") do
  Dir.chdir("ext/flowbox") do
    # this does essentially the same thing
    # as what RubyGems does
    ruby "extconf.rb"
    sh "make"
  end
  cp "ext/flowbox/flowbox.#{RbConfig::CONFIG['DLEXT']}", "lib/flowbox"
end
desc "Compile the extension"
task :compile => "lib/flowbox/flowbox.#{RbConfig::CONFIG['DLEXT']}"

## Run the TESTs  ##############################################################
# see "Recipe 19.1. Automatically Running Unit Tests"@ 'RubyCookbook'
require 'rake/testtask'
Rake::TestTask.new('test-datastructures') do |test|
  test.libs << 'test'
  test.test_files = ['test/test_datastructures.rb']
  test.warning = true
end
Rake::TestTask.new('test-modules') do |test|
  test.libs << 'test'
  test.test_files = ['test/test_modules.rb']
  test.warning = true
end
desc "Run tests"
task :test => "lib/flowbox/flowbox.#{RbConfig::CONFIG["DLEXT"]}"
task :test => ['test-datastructures', 'test-modules']
task :default => :test

#Rcov::RcovTask.new do |test|
#  test.libs << 'test'
#  test.files = ['test/test_datastructures.rb','test/test_modules.rb']
#  test.verbose = true
#  test.rcov_opts << '--exclude "gems/*"'
#end
#task :default => :test

Rake::RDocTask.new do |rdoc|
  version = File.exist?('VERSION') ? File.read('VERSION') : ""
  rdoc.rdoc_dir = 'rdoc'
  rdoc.title = "flowbox #{version}"
  rdoc.rdoc_files.include('README*')
  rdoc.rdoc_files.include('lib/**/*.rb')
end

## CLEAN UP  ###################################################################
# see "Recipe 19.3. Cleaning Up Generated Files"@ 'RubyCookbook'
# Include the "pkg" and "doc" directories and their contents.
CLOBBER.include('pkg', 'doc')

# Include InstalledFiles and .config: files created by setup.rb.
# Include temporary files created during test run.
CLEAN.include(
    FileList["ext/**/{Makefile,mkmf.log}"], 'build', 'dist', FileList['**/*.rbc'],
    FileList["{ext,lib}/**/*.{so,bundle,#{RbConfig::CONFIG['DLEXT']},o}"]
)
#CLEAN.include("ext/**/*{.o,.log,.so,.#{RbConfig::CONFIG['DLEXT']}")
#CLEAN.include("lib/**/*{.so,.bundle,.#{RbConfig::CONFIG['DLEXT']}")
CLEAN.include('InstalledFiles', '.config', 'test/**/*.tmp', 'flowbox.gemspec')
