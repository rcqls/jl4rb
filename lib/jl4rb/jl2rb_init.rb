module Julia

  def Julia.init(args={})
    unless Julia.alive?
      args={:img=>File.join(ENV["JULIA_DIR"],"lib")}.merge(args) if ENV["JULIA_DIR"]
      args={:img=>ENV["JULIA_IMG_DIR"]}.merge(args) if ENV["JULIA_IMG_DIR"]
      #p [:julia_init,args,ENV]
      @@initJL=Julia.initJL([args[:img]])
    end
  end

  def Julia.alive?
    defined? @@initJL
  end

  def Julia.exit()
    Julia.exitJL(0)
  end

end
