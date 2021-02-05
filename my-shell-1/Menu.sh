 #!/bin/bash
 # Script to create simple menus and take action according to that selected
 # menu item
 #
 while :
  do
     clear
     echo "-------------------------------------"
     echo " Main Menu "
     echo "-------------------------------------"
     echo "[1] Check for palindromes"
     echo "[2] Move .c files"
     echo "[3] Draw hollowed square"
     echo "[4] Uppercase conversion"
     echo "[5] Delete files"
     echo "[6] Exit"
     echo "======================="
     echo -n "Enter your menu choice [1-6]: "
     read yourch
     case $yourch in

        1) echo Please enter the argument..
	   read arguden 
	   exec ./myprog1.sh "$arguden";;

 	2) echo " "
	   echo Please enter an path or press enter if you want to continue without argument..
	   read arguden
	   exec ./myprog2.sh "$arguden";;

 	3) echo Please enter the argument..
	   read arguden 
           vars=( $arguden )
	   exec ./myprog3.sh "${vars[0]}" "${vars[1]}";;

 	4) echo Please enter the word with or without path..
	   read arguden
	   vars=( $arguden )	
	   exec ./myprog4.sh "${vars[0]}" "${vars[1]}";;

	5) echo Please press enter to find and delete empty files. If you want, you can search and delete the path argument or recursively with the -R argument.
	   read arguden
	   vars=( $arguden )	
	   exec ./myprog5.sh "${vars[0]}" "${vars[1]}";;

 	6) exit 0 ;;
	*) echo "Opps!!! Please select choice 1,2,3,4,5 or 6";
 	   echo "Press a key. . ." ; read ;;
     esac
 done
