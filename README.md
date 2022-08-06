# README
This is a program to mimic the working of the Unix command `diff` used to display the differences in the files by comparing the files line by line. (implementation in C)

Refer : [diff command](https://man7.org/linux/man-pages/man1/diff.1.html)

## Note 
- Max Characters per line : 128
- Max Lines per file : 128

## Executing the program

### To make the makefile

`make`

### To execute the program

`./diff <file1_name> <file2_name> [options]`

###### For Example 

`./diff file1.txt file2.txt`

## Output 

![Screenshot from 2022-05-31 17-59-01](https://user-images.githubusercontent.com/92677342/171175597-492bf481-78d9-4902-94d8-8c8ad75f4f72.png)
![Screenshot from 2022-05-31 17-56-21](https://user-images.githubusercontent.com/92677342/171175849-a1c391ab-9cb6-4a78-b1aa-6a1d1d87bb51.png)

## Time taken
The time taken by the program to compare files is less than that of the diff command for files of small sizes and nearly the same for files of large size
![Screenshot from 2022-05-31 18-00-18](https://user-images.githubusercontent.com/92677342/171176214-81dfd718-e774-42f1-bab7-305cb15fc295.png)

### Different ways to view diff command

`./diff <file1_name> <file2_name> --normal`
![image](https://user-images.githubusercontent.com/92677342/183258609-67f8af79-7ea0-4a25-b6dc-5b1823091bf1.png)

`./diff <file1_name> <file2_name> --color`
![image](https://user-images.githubusercontent.com/92677342/183258613-9e48087c-f6fb-4e24-ab15-5b17414cf4d0.png)

`./diff <file1_name> <file2_name> -s`
`./diff <file1_name> <file2_name> -q`
![image](https://user-images.githubusercontent.com/92677342/183258622-70a6f43c-d814-44ee-a936-2dc971e597e9.png)

`./diff <file1_name> <file2_name> -u`
![image](https://user-images.githubusercontent.com/92677342/183258639-c40ab75b-f093-4611-8c83-c3da17885a77.png)

`./diff <file1_name> <file2_name> -c`
![image](https://user-images.githubusercontent.com/92677342/183258648-a8034765-33a5-41bc-bd08-3f440a0fcf30.png)



