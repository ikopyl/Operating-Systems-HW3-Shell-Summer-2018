Student's name : Ilya Kopyl
Student's ID: 917921711
Student's Email: ikopyl@mail.sfsu.edu


Build instructions:

Run instructions:


## FEATURES SHOWCASE

========================= cd, pwd and exit builtins showcase ==========================

ilya.kopyl@ilyakopyl-ltm csc415-shell-program-ikopyl (dev-ikopyl)$ ./myshell

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ cd /usr/local/bin

msh /usr/local/bin $ ls -la | wc -l
    1306

msh /usr/local/bin $ ls -la | wc -l > ~/a.txt

msh /usr/local/bin $ cat ~/a.txt
    1306

msh /usr/local/bin $ cd -

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ pwd
/Users/ilya.kopyl/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ sudo su

root@ilyakopyl-ltm /Users/ilya.kopyl/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl$ cd ~

root@ilyakopyl-ltm ~$ pwd
/var/root

root@ilyakopyl-ltm ~$ cd -
/Users/ilya.kopyl/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl

root@ilyakopyl-ltm /Users/ilya.kopyl/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl$ exit
exit

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ cd ~

msh ~ $ pwd
/Users/ilya.kopyl

msh ~ $ cd -
msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ exit

=======================================================================================


myShell >> pwd
/home/csc415/depot/csc415-shell-program-ikopyl/cmake-build-debug

myShell >> ls -la
total 96
drwxrwxr-x 3 csc415 csc415  4096 Jul  6 21:01 .
drwxrwxr-x 5 csc415 csc415  4096 Jul  6 21:06 ..
-rw-r--r-- 1 csc415 csc415     0 Jul  6 12:12 a.txt
-rw-rw-r-- 1 csc415 csc415 34098 Jun 25 21:50 CMakeCache.txt
drwxrwxr-x 5 csc415 csc415  4096 Jul  6 21:01 CMakeFiles
-rw-rw-r-- 1 csc415 csc415  1561 Jun 25 21:50 cmake_install.cmake
-rwxrwxr-x 1 csc415 csc415 25592 Jul  6 21:01 csc415_p3
-rw-rw-r-- 1 csc415 csc415  5946 Jun 25 21:50 csc415_p3.cbp
-rw-rw-r-- 1 csc415 csc415  5130 Jun 25 21:50 Makefile
-rw-rw-r-- 1 csc415 csc415     0 Jul  4 00:05 myfile

myShell >> ls -la | grep a
total 96
-rw-r--r-- 1 csc415 csc415     0 Jul  6 12:12 a.txt
-rw-rw-r-- 1 csc415 csc415 34098 Jun 25 21:50 CMakeCache.txt
drwxrwxr-x 5 csc415 csc415  4096 Jul  6 21:01 CMakeFiles
-rw-rw-r-- 1 csc415 csc415  1561 Jun 25 21:50 cmake_install.cmake
-rw-rw-r-- 1 csc415 csc415  5130 Jun 25 21:50 Makefile

myShell >> grep Make < ~/a.txt > ~/b.txt
myShell >> cat ~/b.txt
-rw-rw-r-- 1 csc415 csc415 34098 Jun 25 21:50 CMakeCache.txt
drwxrwxr-x 5 csc415 csc415  4096 Jul  6 21:01 CMakeFiles
-rw-rw-r-- 1 csc415 csc415  5130 Jun 25 21:50 Makefile

myShell >> grep Make < ~/a.txt >> ~/b.txt
myShell >> cat ~/b.txt
-rw-rw-r-- 1 csc415 csc415 34098 Jun 25 21:50 CMakeCache.txt
drwxrwxr-x 5 csc415 csc415  4096 Jul  6 21:01 CMakeFiles
-rw-rw-r-- 1 csc415 csc415  5130 Jun 25 21:50 Makefile
-rw-rw-r-- 1 csc415 csc415 34098 Jun 25 21:50 CMakeCache.txt
drwxrwxr-x 5 csc415 csc415  4096 Jul  6 21:01 CMakeFiles
-rw-rw-r-- 1 csc415 csc415  5130 Jun 25 21:50 Makefile

myShell >> pwd
/home/csc415/depot/csc415-shell-program-ikopyl

myShell >> pwd ~
/home/csc415/depot/csc415-shell-program-ikopyl

myShell >> cd ~
myShell >> pwd
/home/csc415

myShell >> cd -
myShell >> pwd
/home/csc415/depot/csc415-shell-program-ikopyl

myShell >> cd ~/depot   
myShell >> ls -la
total 24
drwxrwxr-x  6 csc415 csc415 4096 Jun 25 21:48 .
drwxr-xr-x 35 csc415 csc415 4096 Jul  6 15:35 ..
drwxrwxr-x  3 csc415 csc415 4096 Jun 25 21:47 csc415-p1-ikopyl
drwxrwxr-x  3 csc415 csc415 4096 Jun 25 21:48 csc415-p2-ikopyl
drwxrwxr-x  5 csc415 csc415 4096 Jul  6 21:06 csc415-shell-program-ikopyl
drwxrwxr-x  3 csc415 csc415 4096 Jun  8 16:10 dotfiles

myShell >> cd -
myShell >> pwd
/home/csc415/depot/csc415-shell-program-ikopyl

>> ls -la | grep a
total 584
drwxrwxr-x 3 csc415 csc415   4096 Jul  6 21:01 cmake-build-debug
-rw-rw-r-- 1 csc415 csc415    159 Jun 25 21:48 CMakeLists.txt
drwxrwxr-x 3 csc415 csc415   4096 Jul  6 21:06 .idea
-rw-rw-r-- 1 csc415 csc415     77 Jun 25 21:48 Makefile
-rw-rw-r-- 1 csc415 csc415      4 Jul  4 00:09 readme.txt

myShell >> ls -la | grep a > ~/a.txt
myShell >> cat ~/a.txt
total 584
drwxrwxr-x 3 csc415 csc415   4096 Jul  6 21:01 cmake-build-debug
-rw-rw-r-- 1 csc415 csc415    159 Jun 25 21:48 CMakeLists.txt
drwxrwxr-x 3 csc415 csc415   4096 Jul  6 21:06 .idea
-rw-rw-r-- 1 csc415 csc415     77 Jun 25 21:48 Makefile
-rw-rw-r-- 1 csc415 csc415      4 Jul  4 00:09 readme.txt

