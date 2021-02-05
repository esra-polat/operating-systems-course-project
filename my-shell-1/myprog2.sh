path=$1
c_program_files=$path/cprogs

#check the accuracy of the path
if [ ! -d $path ]; then
    echo Given path is not a directory or not found
    echo "Press a key. . .";
    read
    exec ./Menu.sh
fi

#if there is argument of path
if [ $path ]; then
    mkdir -p $c_program_files
    mv *.c $c_program_files #moving the files into path+cprogs
    echo " "
    echo A directory named cprogs is created under $path
    echo " "
    echo All the files whose name ends with .c in $path are moved into cprogs directory
    echo " "
else
#if there is not argument
    mkdir -p cprogs
    mv *.c cprogs #maving the files into cprogs
    echo " "
    echo A directory named cprogs is created under current working directory.
    echo " "
    echo All the files whose name ends with .c in the current working directory are moved into cprogs directory.
    echo " "
fi


echo "Press a key. . .";
read
exec ./Menu.sh
