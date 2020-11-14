Pod::Spec.new do |s|
  s.name         = "MoltenVK"
  s.version      = "1.1.108"
  s.summary      = "MoltenVK is an implementation of the high-performance, industry-standard Vulkan graphics and compute API, that runs on Apple's Metal graphics framework, bringing Vulkan to iOS and macOS."
  s.description  = <<-DESC
Compiled from: https://github.com/KhronosGroup/MoltenVK
DESC

  s.homepage     = "https://github.com/KhronosGroup/MoltenVK"
  s.author       = { "wangyang" => "wangyang@kuaishou.com" }
  s.ios.deployment_target = "8.0"
  s.osx.deployment_target = "10.13"
  s.source       = { :git => "git@git.corp.kuaishou.com:facemagic/MoltenVK.git", :tag => "#{s.version}" }

  s.ios.public_header_files = "iOS/MoltenVK.framework/Headers/*.h"
  s.osx.public_header_files = "macOS/MoltenVK.framework/Headers/*.h"

  s.source_files = 'vulkan/*.{h,hpp}', 'vulkan-portability/*.{h,hpp}'
  s.header_mappings_dir = '.'

  s.osx.vendored_frameworks = 'macOS/MoltenVK.framework'
  s.ios.vendored_frameworks = 'iOS/MoltenVK.framework'
end
