# feup-sope-project
 
This programs were created during the **2nd semester** of the **2nd year** of **Integrated Master in Informatics and Computing Engineering**, in **Operating Systems** curricular unity (FEUP).

[SOPE FEUP-Sigarra](https://sigarra.up.pt/feup/en/UCURR_GERAL.FICHA_UC_VIEW?pv_ocorrencia_id=368704 "Curricular Unity Homepage")

## Program Purpose

The program purpose is a tool to obtain the space use of a file or directory. If it's a directory, the given information should contain subdirectories and files that may be inside it. It's supposed to be similar to the **du** UNIX/LINUX command, with some variations.

## Note

Before running the program, make sure that the **LOG_FILENAME** environment variable is set to **registers.txt**. Otherwise, it will generate an error.

To make that, just run the following commands:

```console
joaocarlosmrp@joaocarlosmrp:~$ LOG_FILENAME=registers.txt
joaocarlosmrp@joaocarlosmrp:~$ export LOG_FILENAME
```
