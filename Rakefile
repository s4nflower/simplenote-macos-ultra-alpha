# frozen_string_literal: true

require 'English'
require 'fileutils'
require 'tmpdir'
require 'rake/clean'
require 'yaml'
require 'digest'

# Constants
XCODE_WORKSPACE = 'Simplenote.xcworkspace'
XCODE_SCHEME = 'Simplenote'
XCODE_CONFIGURATION = 'Debug'
PROJECT_DIR = __dir__
LOCAL_PATH = 'vendor/bundle'

task default: %w[test]

desc 'Install required dependencies'
task dependencies: %w[dependencies:check]

namespace :dependencies do
  task check: %w[bundler:check bundle:check pod:check]

  namespace :bundler do
    task :check do
      Rake::Task['dependencies:bundler:install'].invoke unless command?('bundler')
    end

    task :install do
      puts 'Bundler not found in PATH, installing to vendor'
      ENV['GEM_HOME'] = File.join(PROJECT_DIR, 'vendor', 'gems')
      ENV['PATH'] = File.join(PROJECT_DIR, 'vendor', 'gems', 'bin') + ":#{ENV.fetch('PATH', nil)}"
      sh 'gem install bundler' unless command?('bundler')
    end
    CLOBBER << 'vendor/gems'
  end

  namespace :bundle do
    task :check do
      sh "bundle config set --local path #{LOCAL_PATH} > /dev/null", verbose: false
      sh 'bundle check > /dev/null', verbose: false do |ok, _res|
        next if ok

        # bundle check exits with a non zero code if install is needed
        dependency_failed('Bundler')
        Rake::Task['dependencies:bundle:install'].invoke
      end
    end

    task :install do
      fold('install.bundler') do
        sh 'bundle install --jobs=3 --retry=3'
      end
    end
    CLOBBER << 'vendor/bundle'
    CLOBBER << '.bundle'
  end

  namespace :pod do
    task :check do
      unless podfile_locked? && lockfiles_match?
        dependency_failed('CocoaPods')
        Rake::Task['dependencies:pod:install'].invoke
      end
    end

    task :install do
      fold('install.cocoapds') do
        pod %w[install]
      end
    end

    task :clean do
      fold('clean.cocoapds') do
        FileUtils.rm_rf('Pods')
      end
    end
    CLOBBER << 'Pods'
  end
end

CLOBBER << 'vendor'

desc "Build #{XCODE_SCHEME}"
task build: [:dependencies] do
  xcodebuild(:build)
end

desc "Profile build #{XCODE_SCHEME}"
task buildprofile: [:dependencies] do
  ENV['verbose'] = '1'
  xcodebuild(:build, "OTHER_SWIFT_FLAGS='-Xfrontend -debug-time-compilation -Xfrontend -debug-time-expression-type-checking'")
end

task timed_build: [:clean] do
  require 'benchmark'
  time = Benchmark.measure do
    Rake::Task['build'].invoke
  end
  puts "CPU Time: #{time.total}"
  puts "Wall Time: #{time.real}"
end

desc 'Run test suite'
task test: [:dependencies] do
  xcodebuild(:build, :test)
end

desc 'Remove any temporary products'
task :clean do
  xcodebuild(:clean)
end

namespace :git do
  hooks = %w[post-checkout post-merge]

  desc 'Install git hooks'
  task :install_hooks do
    hooks.each do |hook|
      target = hook_target(hook)
      source = hook_source(hook)
      backup = hook_backup(hook)

      next if File.symlink?(target) && (File.readlink(target) == source)
      next if File.file?(target) && File.identical?(target, source)

      if File.exist?(target)
        puts "Existing hook for #{hook}. Creating backup at #{target} -> #{backup}"
        FileUtils.mv(target, backup, force: true)
      end
      FileUtils.ln_s(source, target)
      puts "Installed #{hook} hook"
    end
  end

  desc 'Uninstall git hooks'
  task :uninstall_hooks do
    hooks.each do |hook|
      target = hook_target(hook)
      source = hook_source(hook)
      backup = hook_backup(hook)

      next unless File.symlink?(target) && (File.readlink(target) == source)

      puts "Removing hook for #{hook}"
      File.unlink(target)
      if File.exist?(backup)
        puts "Restoring hook for #{hook} from backup"
        FileUtils.mv(backup, target)
      end
    end
  end

  def hook_target(hook)
    ".git/hooks/#{hook}"
  end

  def hook_source(hook)
    "../../Scripts/hooks/#{hook}"
  end

  def hook_backup(hook)
    "#{hook_target(hook)}.bak"
  end
end

namespace :git do
  task :post_merge do
    check_dependencies_hook
  end

  task :post_checkout do
    check_dependencies_hook
  end
end

desc 'Open the project in Xcode'
task xcode: [:dependencies] do
  sh "open #{XCODE_WORKSPACE}"
end

def fold(label)
  puts "--- #{label}" if ENV['BUILDKITE']
  yield
end

def pod(args)
  args = %w[bundle exec pod] + args
  sh(*args)
end

def lockfiles_match?
  File.file?('Pods/Manifest.lock') && FileUtils.compare_file('Podfile.lock', 'Pods/Manifest.lock')
end

def podfile_locked?
  podfile_checksum = Digest::SHA1.file('Podfile')
  lockfile_checksum = YAML.load_file('Podfile.lock')['PODFILE CHECKSUM']

  podfile_checksum == lockfile_checksum
end

def xcodebuild(*build_cmds)
  cmd = 'xcodebuild'
  cmd += " -destination 'platform=iOS Simulator,name=iPhone 6s'"
  cmd += ' -sdk iphonesimulator'
  cmd += " -workspace #{XCODE_WORKSPACE}"
  cmd += " -scheme #{XCODE_SCHEME}"
  cmd += " -configuration #{xcode_configuration}"
  cmd += ' '
  cmd += build_cmds.map(&:to_s).join(' ')
  cmd += ' | bundle exec xcpretty -f `bundle exec xcpretty-travis-formatter` && exit ${PIPESTATUS[0]}' unless ENV['verbose']
  sh(cmd)
end

def xcode_configuration
  ENV['XCODE_CONFIGURATION'] || XCODE_CONFIGURATION
end

def command?(command)
  system("which #{command} > /dev/null 2>&1")
end

def dependency_failed(component)
  msg = "#{component} dependencies missing or outdated. "
  if ENV['DRY_RUN']
    msg += 'Run rake dependencies to install them.'
    raise msg
  else
    msg += 'Installing...'
    puts msg
  end
end

def check_dependencies_hook
  ENV['DRY_RUN'] = '1'
  begin
    Rake::Task['dependencies'].invoke
  rescue StandardError => e
    puts e.message
    exit 1
  end
end
