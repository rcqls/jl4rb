module Julia

  def Julia.init
    unless Julia.alive?
      @@initJL=Julia.initJL
    end
  end

  def Julia.alive?
    defined? @@initJL
  end

  def Julia.exit()
    Julia.exitJL(0)
  end

end
