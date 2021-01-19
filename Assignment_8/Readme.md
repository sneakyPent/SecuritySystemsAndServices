# Security System and Services

## NIKOLAS ZACHARIOUDAKIS 2016030073

8th Assignment: Buffer overflow exploitation.

## Informations

* Every part of the assignment is completed and works properly according to the given details of the assignment.

## Description
The main idea for the buffer overflow exploitation is that we are using something ,i.e. c function, to write a string or other variable, into a sort length piece of memory. By writting in something longer than the length we can overwrite the later memory addresses.  

When we call readString function we are going in the stack part of the memory, which is as shown below. <br>
| Stack      |
| :-----------: |
| function      |
| arguments   |
| return address   |
| base pointer   |
| buf   |

<br>
So our aim is to overwrite the return address with a static address in which, we have stored our shellcode, so as to run over and over again. That is why we create an exploit input file as (shellcode + noOps + Address). The address we have to use is the address of Name variable which is always the same every time we run the program and the address we found with gdb is 0x80dacc0. Now in order to find the length of no ops we give as input a specific length of A's, so as to see when the segmentation fault is exact 0x41414141 and that means that our programm tried to access the return address 0x41414141 unsuccessfully. That length was 52. So the exploit input file will consists of shellcode(m byte length), address (n bytes), noOps(52-m-n bytes). By giving the previous input in the greeter program we tell it to run a shell then do nothing and after this return to the given address (address of Name) which contains the exact same input. As we can see, there is a loop, exec shellcode - do nothing - return to the start of shellcode.  And that is how we achieve buffer overflow exploitation. 

## Commands

1) Testing the payload

```
./test -p $(echo $(cat payload))
```

2) Input file generation

```
python2.7 generator.py $(echo $(cat payload )) $(echo $(cat address))
```

3) Testing the payload

```
(cat exploitInputFile; cat)|./Greeter
```

## Extra info

* My payload is in the payload file, the Name variable address is in the address file, and when you run the second command you will get the exploitInputFile.
* If you want to test an other payload with the test program you can just give as input an other file which contains the payload in hex form.
* I use your given Greeter executable for my exercice and I attach it in my sen folder too.