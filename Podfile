# frozen_string_literal: true

source 'https://cdn.cocoapods.org/'

inhibit_all_warnings!
use_frameworks!

APP_MACOS_DEPLOYMENT_TARGET = Gem::Version.new('10.14')

platform :osx, APP_MACOS_DEPLOYMENT_TARGET
workspace 'Simplenote.xcworkspace'

## Tools
## ===================
##

def swiftlint_version
  require 'yaml'

  YAML.load_file('.swiftlint.yml')['swiftlint_version']
end

abstract_target 'Tools' do
  pod 'SwiftLint', swiftlint_version
end

# Main
#
abstract_target 'Automattic' do
  # Automattic Shared
  #
  pod 'Simperium-OSX', '1.9.0'

  # Main Target
  #
  target 'Simplenote'

  # Testing Target
  #
  target 'SimplenoteTests'
end

post_install do |installer|
  installer.pods_project.targets.each do |target|
    # Let Pods targets inherit deployment target from the app
    # See https://github.com/CocoaPods/CocoaPods/issues/4859
    target.build_configurations.each do |configuration|
      macos_deployment_key = 'MACOSX_DEPLOYMENT_TARGET'
      pod_macos_deployment_target = Gem::Version.new(configuration.build_settings[macos_deployment_key])
      configuration.build_settings.delete macos_deployment_key if pod_macos_deployment_target <= APP_MACOS_DEPLOYMENT_TARGET
    end
  end
end
