#This section works if no argument is entered
if [ "$@" == "" ];then 
    echo 'You should enter the string!!'
    echo "Press a key. . .";
    read
    exec ./Menu.sh
fi
  
#This section works if the number is entered else correct string is entered
if [[ $@ =~ ^[0-9]+$ ]];then
   echo "You should not enter the integer. You should enter the string."
else
   x=${@,,*}
   strlen=${#x}
   dec=`expr $strlen - 1`
   counter=0
   divlen=`expr $strlen / 2`
   for ((i=0; i<divlen; i++))
   do
   let1=${x:$i:1}
   let2=${x:$dec:1}
   if [ $let1 = $let2 ]
   then
   counter=`expr $counter + 1`
   dec=`expr $dec - 1`
   fi
   done
   if [ $counter -eq $divlen ]
   then
   echo "$@ is palindrome" 
   else
   echo "$@ is not palindrome"
   fi
fi

echo "Press a key. . .";
read
exec ./Menu.sh



