# Introduction

This is a generict threading lib, very minimalistic. It is a non interruptable
kind of threading algorithm. It is also tickless, and easily portable by
suplying it with only two functions that have to be platform defined. Those are
the millis() function and delay() function.

# How to use

main.c serves just as an example of how to use this lib. In order to incorporate
it into a project only a thread.h and thread.c are needed.
