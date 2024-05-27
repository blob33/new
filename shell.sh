//commands
#!bin/bash
echo "-----Options-----"
echo "1.Display date"
echo "2.Dispaly Current Working Directory of user"
echo "3.List all files"
echo "4.Exit"

echo "Choose one option:"
read option

case $option in
 1)  echo "Current date is: "
    echo ` date `;;
 2) echo "Current working directory of use is: "
    echo `pwd`;;
 3) echo "All files are in current directory: "
    echo `ls`;;
 4) echo "Exiting from system...";;
 *) echo "Please choose correct option";;
 esac

 # cmd argument
 #!bin/bash
echo "Cube of $1 is:" $(($1*$1*$1))

# greater among three
#!bin/bash
echo "Enter 3 number:"
read num1
read num2
read num3

if [ $num1 -gt $num2 ]; then 

if [ $num1 -gt $num3 ]; then 
    echo $num1 is greater
else 
    echo $num3 is greater
fi

else 

if [ $num2 -gt $num3 ]; then
    echo $num2 is greater 
else 
    echo $num3 is greater
fi

fi

# minimum from array
#!bin/sh
minimiun(){
min=${array[0]}

for i in ${array[@]};
do
    if [ $i -lt $min ]; then
        min=$i
    fi 
done

echo $min
}

array=(22 3 4 11 32)

minimiun

# sum of digits
#!bin/bash

echo "Enter number:"
read num

rem=0
sum=0
temp = $num

while [ $num -gt 0 ];
do
  rem=$(($num%10))
  num=$(($num/10))
  sum=$(($sum+$rem))
done

echo "Sum of digits of $temp is: $sum"

# 
