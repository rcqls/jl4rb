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

  class Vector

    def <<(name)
      if name.is_a? Symbol
        @name=name.to_s
        @type="var"
      else
        @name=name
        @type="expr"
      end
      return self
    end

    def arg=(arg)
      @arg=arg
    end

    #this method is the same as the previous one but return self! Let us notice that even by adding return self in the previous one
    # I could not manage to execute (rvect.arg="[2]").value_with_arg but fortunately rvect.set_arg("[2]").value_with_arg is working!
    def set_arg(arg)
      @arg=arg
      return self
    end

    def [](key)
      set_arg("["+(key+1).to_s+"]")
      get_with_arg
    end

    def []=(key,val)
      set_arg("["+(key+1).to_s+"]")
      set_with_arg(val)
    end
   
    def >(arr)
      res=self.get
#puts "res";p @name;p res
      if res
#puts "arr.class:";p arr.class
#puts "res.class";p res.class
        res=[res] unless res.is_a? Array
        arr.replace(res)
      else
        arr.clear
      end
      return self
    end

=begin #Done directly inside R4rb.c
    def value_with_arg(arg)
      old_name,old_type=@name.dup,@type.dup
      @name,@type=@name+arg,"expr"
      value
      @name,@type=old_name,old_type
    end
=end

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
 