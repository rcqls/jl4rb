module Julia

  def Julia.init(args={})
    unless Julia.alive?
      args={:img=>File.join(ENV["JLAPI_HOME"],"lib"),:mode=>"rcqls"}.merge(args)
      @@initJL=Julia.initJL([args[:img],args[:mode]])
    end 
  end

  def Julia.alive?
    defined? @@initJL
  end

end