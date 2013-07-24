## Module Julia

module Julia
  def Julia.eval(s,opts={})
    opts={:show=>nil,:simplify=>true,:init=>true,:safe=>true}.merge(opts)
    #p opts
    Julia.init if opts[:init] #just in case
    res=[]
    # if opts[:safe]
    #   evalLine <<-JLEND
    #     module RubySandbox
    #       # Replace OUTPUT_STREAM references so we can capture output.
    #       OUTPUT_STREAM = IOBuffer()
    #       print(x) = Base.print(OUTPUT_STREAM, x)
    #       println(x) = Base.println(OUTPUT_STREAM, x)
    #     end
    #   JLEND
    # end
    input,output="",""
    s=[s] unless s.is_a? Array
    s.each_with_index do |line,i|
      input << line
      # if !opts[:safe] || input[0...6]=="using "
        output=evalLine input
      # else
      #   output=evalLine "eval(RubySandbox,begin\n"+input+"\nend)"
      # end
      if output == "__incomplete__"
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
        print"in > ";puts cmd[:in]
        begin print "out> ";puts cmd[:out];end unless  cmd[:out]=="__incomplete__"
      end
    else
      res=res[0][:out] if res.length==1 and opts[:simplify]
      return res
    end
  end

  def Julia.<<(s)
    Julia.eval(s)
  end

end
 