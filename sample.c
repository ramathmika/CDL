class B{
	s:String<-"Hello";
	g(y:String):Int{
		y.concat(s)
	};
	--Any char bw "--" and new line
	--is treated as comments
	f(x:Int):Int{
		x+1
	};
};