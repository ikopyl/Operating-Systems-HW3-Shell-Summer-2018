Student's name : Ilya Kopyl
Student's ID: 917921711
Student's Email: ikopyl@mail.sfsu.edu

Build instructions:
$ make

Target cleanup (removing the previous build)
$ make clean

Run instructions:
$ ./myshell


## FEATURES SHOWCASE

=======================================================================================
        1. Execute a signle command with up to four command line arguments.
=======================================================================================

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ ls -l
total 1120
-rw-r--r--  1 ilya.kopyl  wheel  508518 Jun 25 15:39 415_HW3.pdf
-rw-r--r--  1 ilya.kopyl  wheel     159 Jun 25 15:39 CMakeLists.txt
-rw-r--r--  1 ilya.kopyl  wheel    1074 Jun 25 15:39 LICENSE
-rw-r--r--  1 ilya.kopyl  wheel      77 Jun 25 15:39 Makefile
-rw-r--r--  1 ilya.kopyl  wheel      11 Jun 25 15:39 README.md
drwxr-xr-x  7 ilya.kopyl  wheel     224 Jul  8 12:27 cmake-build-debug
-rwxr-xr-x  1 ilya.kopyl  wheel   16028 Jul  8 18:16 myshell
-rw-r--r--  1 ilya.kopyl  wheel   17680 Jul  8 13:14 myshell.c
-rw-r--r--  1 ilya.kopyl  wheel    4781 Jul  8 13:18 readme.txt

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ cat Makefile
fc: myshell.c
	gcc -I -Wall myshell.c -o myshell

clean:
	rm myshell

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ ls -al /usr/local/
total 0
drwxr-xr-x    22 root        wheel    704 Jun 11 18:43 .
drwxr-xr-x@   12 root        wheel    384 Jan 25 08:49 ..
-rw-r--r--     1 root        wheel      0 Jun  6 10:21 .com.apple.installer.keep
drwxr-xr-x     4 ilya.kopyl  admin    128 Jan  4  2018 .pyenv
drwxr-xr-x    27 ilya.kopyl  staff    864 Jun  9 15:32 Caskroom
drwxr-xr-x   190 ilya.kopyl  admin   6080 Apr 26 00:31 Cellar
drwxr-xr-x     3 ilya.kopyl  admin     96 Mar 10 15:10 Frameworks
drwxr-xr-x    17 ilya.kopyl  admin    544 Jun  6 10:35 Homebrew
drwxr-xr-x  1305 ilya.kopyl  admin  41760 Jul  2 13:23 bin
drwxr-xr-x     3 root        wheel     96 Feb 21 16:49 cuda
drwxr-xr-x    22 ilya.kopyl  admin    704 Jun  6 10:38 etc
drwxr-xr-x   386 ilya.kopyl  admin  12352 Jul  2 13:23 include
drwxr-xr-x     3 root        wheel     96 Jan  4  2018 jamf
drwxr-xr-x   550 ilya.kopyl  admin  17600 Jul  2 13:23 lib
drwxr-xr-x     5 root        wheel    160 Jan  4  2018 libexec
drwxr-xr-x   218 ilya.kopyl  admin   6976 Jul  2 13:24 opt
dr-xr-x---    12 root        ossec    384 Jan  4  2018 ossec
drwxr-xr-x     3 root        wheel     96 Jan  4  2018 remotedesktop
drwxr-xr-x     4 ilya.kopyl  admin    128 Jan  4  2018 sbin
drwxr-xr-x    87 ilya.kopyl  admin   2784 Jul  2 13:24 share
drwxr-xr-x     4 root        wheel    128 Jan  4  2018 texlive
drwxr-xr-x    12 ilya.kopyl  admin    384 Apr 26 00:03 var



=======================================================================================
        2. Execute a command in background.
=======================================================================================

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ ps auxwww | grep htop
ilya.kopyl       68185   0.0  0.0  4286200    936 s104  S+    6:23PM   0:00.00 grep htop

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ htop &

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ ps auxwww | grep htop
ilya.kopyl       68190   0.0  0.0  4275960    912 s104  S+    6:24PM   0:00.00 grep htop
ilya.kopyl       68187   0.0  0.0  4278368   1948 s104  T     6:23PM   0:00.01 htop

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ kill 68187

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ ps auxwww | grep htop
ilya.kopyl       68202   0.0  0.0  4295416    956 s104  S+    6:24PM   0:00.00 grep htop



=======================================================================================
        3. Redirect the standard output of a command to a file.
=======================================================================================

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ cat ~/Documents/a.txt
cat: /Users/ilya.kopyl/Documents/a.txt: No such file or directory

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ ls -la /Library/Java/JavaVirtualMachines > ~/Documents/a.txt

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ cat ~/Documents/a.txt
total 0
drwxr-xr-x  5 root  wheel  160 Apr  3 12:38 .
drwxr-xr-x  5 root  wheel  160 Apr  3 12:33 ..
drwxr-xr-x  3 root  wheel   96 Dec  1  2017 jdk-9.0.1.jdk
drwxr-xr-x  3 root  wheel   96 Apr  7  2016 jdk1.7.0_80.jdk
drwxr-xr-x  3 root  wheel   96 Apr  3 12:33 jdk1.8.0_162.jdk

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ ls -la /Library/Java/JavaVirtualMachines | wc -l >> ~/Documents/a.txt

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ cat ~/Documents/a.txt
total 0
drwxr-xr-x  5 root  wheel  160 Apr  3 12:38 .
drwxr-xr-x  5 root  wheel  160 Apr  3 12:33 ..
drwxr-xr-x  3 root  wheel   96 Dec  1  2017 jdk-9.0.1.jdk
drwxr-xr-x  3 root  wheel   96 Apr  7  2016 jdk1.7.0_80.jdk
drwxr-xr-x  3 root  wheel   96 Apr  3 12:33 jdk1.8.0_162.jdk
       6

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ ls -la > ~/Documents/a.txt

msh ~/depot/git_home/csc415/homework_assignments/hw3/csc415-shell-program-ikopyl $ cat ~/Documents/a.txt
total 1128
drwxr-xr-x  14 ilya.kopyl  wheel     448 Jul  8 18:21 .
drwxr-xr-x   4 ilya.kopyl  wheel     128 Jul  8 12:27 ..
drwxr-xr-x  15 ilya.kopyl  wheel     480 Jul  8 18:34 .git
-rw-r--r--   1 ilya.kopyl  wheel     507 Jun 25 15:41 .gitignore
drwxr-xr-x   9 ilya.kopyl  wheel     288 Jul  8 13:11 .idea
-rw-r--r--   1 ilya.kopyl  wheel  508518 Jun 25 15:39 415_HW3.pdf
-rw-r--r--   1 ilya.kopyl  wheel     159 Jun 25 15:39 CMakeLists.txt
-rw-r--r--   1 ilya.kopyl  wheel    1074 Jun 25 15:39 LICENSE
-rw-r--r--   1 ilya.kopyl  wheel      77 Jun 25 15:39 Makefile
-rw-r--r--   1 ilya.kopyl  wheel      11 Jun 25 15:39 README.md
drwxr-xr-x   7 ilya.kopyl  wheel     224 Jul  8 12:27 cmake-build-debug
-rwxr-xr-x   1 ilya.kopyl  wheel   16028 Jul  8 18:21 myshell
-rw-r--r--   1 ilya.kopyl  wheel   17680 Jul  8 13:14 myshell.c
-rw-r--r--   1 ilya.kopyl  wheel    4781 Jul  8 13:18 readme.txt



=======================================================================================
        4. Redirect standard input of a command to come from a file.
=======================================================================================

msh ~/depot/csc415-shell-program-ikopyl $ grep .* < myshell.c | wc -l
623

msh ~/depot/csc415-shell-program-ikopyl $ grep -v \w < myshell.c | wc -l
246

msh ~/depot/csc415-shell-program-ikopyl $ grep \w < myshell.c | wc -l
377

msh ~/depot/csc415-shell-program-ikopyl $ grep \w < myshell.c | wc -l > ~/Documents/a.txt

msh ~/depot/csc415-shell-program-ikopyl $ grep \w < myshell.c | wc -l >> ~/Documents/a.txt

msh ~/depot/csc415-shell-program-ikopyl $ cat ~/Documents/a.txt
377
377

msh ~/depot/csc415-shell-program-ikopyl $ grep while < myshell.c | wc -l
7

msh ~/depot/csc415-shell-program-ikopyl $ grep [a-z] < Makefile
fc: myshell.c
	gcc -I -Wall myshell.c -o myshell
clean:
	rm myshell

msh ~/depot/csc415-shell-program-ikopyl $ grep [a-z] < Makefile | wc -l > a.txt

msh ~/depot/csc415-shell-program-ikopyl $ cat a.txt
4

msh ~/depot/csc415-shell-program-ikopyl $ grep [a-z] < Makefile >> a.txt

msh ~/depot/csc415-shell-program-ikopyl $ cat a.txt
4
fc: myshell.c
	gcc -I -Wall myshell.c -o myshell
clean:
	rm myshell


msh ~/depot/csc415-shell-program-ikopyl $ grep shell < ~/a.txt >> ~/a.txt
grep: input file ‘(standard input)’ is also the output



=======================================================================================
        5. Execute multiple commands connected by a signle shell pipe.
=======================================================================================

msh ~/depot/csc415-shell-program-ikopyl $ ls -la /usr/src | grep linux
drwxr-xr-x 27 root root 4096 Nov 24  2017 linux-headers-4.10.0-38
drwxr-xr-x  7 root root 4096 Nov 24  2017 linux-headers-4.10.0-38-generic

msh ~/depot/csc415-shell-program-ikopyl $ ps auxwww | grep myshell
csc415   31256  0.0  0.0   4356   648 pts/0    S+   20:54   0:00 ./myshell
csc415   31285  0.0  0.0  14220   940 pts/0    S+   20:54   0:00 grep myshell

msh ~/depot/csc415-shell-program-ikopyl $ ./myshell
msh ~/depot/csc415-shell-program-ikopyl $ ./myshell
msh ~/depot/csc415-shell-program-ikopyl $ ./myshell
msh ~/depot/csc415-shell-program-ikopyl $ ./myshell

msh ~/depot/csc415-shell-program-ikopyl $ ps auxwww | grep myshell
csc415   31256  0.0  0.0   4356   648 pts/0    S+   20:54   0:00 ./myshell
csc415   31298  0.0  0.0   4356   632 pts/0    S+   20:54   0:00 ./myshell
csc415   31305  0.0  0.0   4356   732 pts/0    S+   20:54   0:00 ./myshell
csc415   31318  0.0  0.0   4356   628 pts/0    S+   20:54   0:00 ./myshell
csc415   31328  0.0  0.0   4356   640 pts/0    S+   20:54   0:00 ./myshell
csc415   31351  0.0  0.0  14220   960 pts/0    S+   20:54   0:00 grep myshell

msh ~/depot/csc415-shell-program-ikopyl $ exit
msh ~/depot/csc415-shell-program-ikopyl $ exit
msh ~/depot/csc415-shell-program-ikopyl $ exit
msh ~/depot/csc415-shell-program-ikopyl $ exit

msh ~/depot/csc415-shell-program-ikopyl $ ps auxwww | grep myshell
csc415   31256  0.0  0.0   4356   648 pts/0    S+   20:54   0:00 ./myshell
csc415   31616  0.0  0.0  14220   976 pts/0    S+   20:56   0:00 grep myshell



=======================================================================================
        6. Execute the cd and pwd commands
=======================================================================================

msh ~/depot/csc415-shell-program-ikopyl $ cd /usr/local/bin

msh /usr/local/bin $ ls -la | wc -l
9

msh /usr/local/bin $ pwd
/usr/local/bin

msh /usr/local/bin $ cd -

msh ~/depot/csc415-shell-program-ikopyl $ pwd
/home/csc415/depot/csc415-shell-program-ikopyl

msh ~/depot/csc415-shell-program-ikopyl $ ls -la | wc         -l
15



csc415@csc415-vb csc415-shell-program-ikopyl (dev-ikopyl)*$ pwd
/home/csc415/depot/csc415-shell-program-ikopyl

csc415@csc415-vb csc415-shell-program-ikopyl (dev-ikopyl)*$ pwd > ~/a.txt

csc415@csc415-vb csc415-shell-program-ikopyl (dev-ikopyl)*$ pwd >> ~/a.txt

csc415@csc415-vb csc415-shell-program-ikopyl (dev-ikopyl)*$ cat ~/a.txt
/home/csc415/depot/csc415-shell-program-ikopyl
/home/csc415/depot/csc415-shell-program-ikopyl

csc415@csc415-vb csc415-shell-program-ikopyl (dev-ikopyl)*$ pwd > ~/a.txt ~/b.txt

csc415@csc415-vb csc415-shell-program-ikopyl (dev-ikopyl)*$ cat ~/a.txt
/home/csc415/depot/csc415-shell-program-ikopyl

csc415@csc415-vb csc415-shell-program-ikopyl (dev-ikopyl)*$ cat ~/b.txt
cat: /home/csc415/b.txt: No such file or directory



msh ~/depot/csc415-shell-program-ikopyl $ sudo su
[sudo] password for csc415:

csc415-vb csc415-shell-program-ikopyl # cd /home/csc415/depot/csc415-shell-program-ikopyl

csc415-vb csc415-shell-program-ikopyl # ./myshell

msh /home/csc415/depot/csc415-shell-program-ikopyl $ pwd
/home/csc415/depot/csc415-shell-program-ikopyl

msh /home/csc415/depot/csc415-shell-program-ikopyl $ cd ~

msh ~ $ pwd
/root

msh ~ $ cd -

msh /home/csc415/depot/csc415-shell-program-ikopyl $ pwd
/home/csc415/depot/csc415-shell-program-ikopyl

msh /home/csc415/depot/csc415-shell-program-ikopyl $ exit

csc415-vb csc415-shell-program-ikopyl # exit
exit

msh ~/depot/csc415-shell-program-ikopyl $ pwd
/home/csc415/depot/csc415-shell-program-ikopyl

