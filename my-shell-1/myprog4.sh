word=$1
path=$2

#I splitted the argument because I can't use the wildcard directly.
IFS='*' read -ra array <<< $word

#check the accuracy of the path
if [ ! -d $path ]; then
    echo Given path is not a directory or not found
    echo "Press a key. . .";
    read
    exec ./Menu.sh
fi

#check the accuracy of the argument
if [ ! $word ]; then
    echo Please type a word to first parameter.
    echo "Press a key. . .";
    read
    exec ./Menu.sh
fi

[[ $path ]] && files=$path/*.txt || files=*.txt

for file in $files; 
    do
        if [ ! -f "$file" ]; then
            echo " "
            echo "Cannot find any file that have txt extension"
            echo "Press a key. . .";
            read
    	    exec ./Menu.sh
        fi

        #words that will change
        matched_words=$(cat $file |  grep -e "^${array[0]}\w*${array[1]}\b")

        #making uppercase
        for word in $matched_words
        do
            uppercase_word=${word^^}
            echo The word \"$word\" inside $file is substituted with \"$uppercase_word\"
            echo " "
            sed -i "s/$word/$uppercase_word/g" $file
        done
done

echo "Press a key. . .";
read
exec ./Menu.sh