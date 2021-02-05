argument=$1

#delete operations
delete_function(){

    for line in `cat empty_files.sh`
    do
        echo " " 
        read -p "Do you want to delete the $line file? (Y/N)": answer

        if [ "$answer" = "y" ] || [ "$answer" = "Y" ]  ; then
            rm -f $line
            echo "1 file deleted!"
        elif [ "$answer" = "n" ] || [ "$answer" = "N" ] ; then
            echo "Skipped!"
        else echo "Sorry, wrong selection!"
        fi
    done

    #the most recent temporary file is also deleted.
    rm -f empty_files.sh
}

#if there is argument
if [ $argument ]; then
    #with -R argument
    if [ "$argument" = "r" ] || [ "$argument" = "R" ] || [ "$argument" = "-r" ] || [ "$argument" = "-R" ] ; then
        ls -l $argument;
        find . -name "*.txt" -size 0k > empty_files.sh #creating temporary file
        delete_function
    
    #with path argument
    elif [ -d $argument ]; then
        ls -l $argument;
        find $argument -type f -name "*.txt" -size 0k > empty_files.sh #creating temporary file
        delete_function
    else echo It is not valid argument.
    fi
#if there is not argument
else
    ls -l .
    find . -type f -name "*.txt" -size 0k > empty_files.sh #creating temporary file
    delete_function
fi

echo "Press a key. . .";
read
exec ./Menu.sh
