#This section works if no argument is entered
if [ "$1" == "" ];then
    echo 'You should enter the two number!!'
    echo "Press a key. . .";
    read
    exec ./Menu.sh
fi

if [ "$2" == "" ];then
    echo 'You should enter the two number!!'
    echo "Press a key. . .";
    read
    exec ./Menu.sh
fi

#This section works the first argument must be greater than the second
if [ $2 -gt $1 ] ; then
    echo 'The first argument must be greater than the second one!!'
    echo "Press a key. . .";
    read
    exec ./Menu.sh
fi

#This section works the difference between the two arguments must be an even number
number=$1-$2
if [ $((number%2)) -ne 0 ] ; then
    echo 'The difference between the two arguments must be an even number!!'
    echo "Press a key. . .";
    read
    exec ./Menu.sh
fi

#This section prints hollowed square
a=($1-$2)/2
       for ((i=1; i<=$1; i++))
         do 
            for ((j=1; j<=$1; j++))
             do
                 if ((i==1 || i==$1 || j==1 || j==$1))
                 then
                    printf "*"
		 elif (($2==$1-2))
		 then
                    printf " ";
                 else
		    for ((k=2; k<=a; k++))
             	    do
	           	if ((i==k || j==k || i==$1-(k-1) || j==$1-(k-1)))
		   	then
                    	   printf "*";
			   break;
			elif((k == a))
			then
		    	   printf " ";
                   	fi
		    done
                 fi
             done
            echo;
        done

echo "Press a key. . .";
read
exec ./Menu.sh
