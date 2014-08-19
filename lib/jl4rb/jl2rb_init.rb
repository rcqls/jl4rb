module Julia

  def Julia.init(args={})
    unless Julia.alive?
      args={:img=>File.join(ENV["JULIA_DIR"],"lib")}.merge(args) if ENV["JULIA_DIR"]
      @@initJL=Julia.initJL([args[:img]])
    end 
  end

  def Julia.alive?
    defined? @@initJL
  end

end