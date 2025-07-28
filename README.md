# bin-lang (Just the Lexer) 

This project was my first attempt at building a programming language, and my first time in a long time writing C. I went into it with a solid understanding of most of the concepts, and if I did more prep work, I might have been able to finish it, but for now I just have a basic lexer. Not nearly as much as I'd hoped, but I guess it's decent progress. I probably could have also finished the parser, but I was uneasy knowing nothing about how I would implement the runtime*.

How to run:

    make lexer_test
    ./lexer_test

## About language itself

bin-lang is going to be a functional language designed for representing and simulating digital circuits. Since I started learning about computers on a lower level, I've always wanted to build at least a basic computer from scratch; from breadboards and logic gates. But building that sort of thing is quite the endeavor, especially if you're just running of your head.
The goal of bin-lan is to provide a more abstracted way to think about these circuits, and at the same time to allow you to simulate the hardware without the hardware.

Circuits are a challenging subject here, because they range widely in stability and hierarchy. The first iteration of bin-lang is going to focusing only on Combinational circuits, which are strictly heirarchichal.

