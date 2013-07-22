module Julia

  def Julia.init(args={})
    unless Julia.alive?
      args={:img=>File.join(ENV["JL4RB_HOME"],"lib")}.merge(args)
      @@initJL=Julia.initJL([args[:img]])
    end 
  end

  def Julia.alive?
    defined? @@initJL
  end

end