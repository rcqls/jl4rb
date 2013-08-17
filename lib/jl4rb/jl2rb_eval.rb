## Module Julia

module Julia
  # Careful!, Julia.exec code, :get => nil does not fetch stdout
  def Julia.eval(s,opts={})
    opts={:print=>true,:show=>nil,:simplify=>true,:init=>true,:safe=>true}.merge(opts)
    #p opts
    Julia.init if opts[:init] #just in case
    res=[]
    input,output="",""
    s=[s] unless s.is_a? Array
    s.each_with_index do |line,i| #line here is a command!
      input << line        
      output=evalLine input, opts[:print]
      if output.is_a? String and output[0...12]=="__incomplete"
        if i==s.length - 1 #last!
          res << {:in => input, :out => output} unless input.empty?
        else
          input << "\n"
        end
      else
        res << {:in => input, :out => output}
        input=""
      end
    end
    if opts[:show]
      res.each do |cmd|
        print "in > ";puts cmd[:in]
        begin print "out> ";puts cmd[:out];end unless  cmd[:out]=="__incomplete__"
      end
    else
      res=res[0][:out] if res.length==1 and opts[:simplify]
      return res
    end
  end

  # Careful!, Julia.exec code, :get => nil does not fetch stdout
  def Julia.exec(code,opts={})
    opts={:get=>true}.merge(opts)
    execLine code, opts[:get]
  end

  def Julia.<(s)
    Julia.exec(s)
  end

  def Julia.<<(s)
    Julia.eval(s)
  end

end

module JL
  def JL.<<(s)
    Julia << s
  end
end

class String

  def jl4rb
    Julia << self
  end

  alias to_jl jl4rb
end
 