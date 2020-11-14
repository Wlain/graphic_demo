
Pod::Spec.new do |s|

  s.name         = "stb"
  s.version      = "1.0.0"
  s.summary      = "stb image"
  s.description  = <<-DESC
  stb image
  DESC

  s.homepage     = "git@git.corp.kuaishou.com:facemagic/stb_image.git"

  s.license      = "MIT"
  s.author       = { "wangyang" => "wangyang@kuaishou.com" }
  # s.platform     = :ios, "7.0"
  # s.platform     = :osx, "10.13"
  s.ios.deployment_target = '8.0'
  s.osx.deployment_target = '10.13'

  s.source       = { :git => "git@git.corp.kuaishou.com:facemagic/stb_image.git", :tag => "#{s.version}" }
  
  s.source_files = '*.h'

end
