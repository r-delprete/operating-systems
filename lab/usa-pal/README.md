# Project usaPal

Questo progetto utilizza il comando 'make' per poter eseguire il linking di più file tramite un solo comando.
Tale comando prevede questa sintassi:

```zsh
make -f make_nomefile
```

 Nel progetto in questione, il comando sarà:

 ````zsh
 make -f makepal
 ````

## File makepal

Il file makepal avrà il seguente contenuto:

````zsh
usaPal: usaPal.o reverse.o palindroma.o
usaPal.o: palindroma.h
reverse.o: reverse.h
palindroma.o: palindroma.h reverse.h
````

Dove:

+ usaPal, usaPal.o, reverse.o palindroma.o sono definiti "target"
+ palindroma.h, reverse.h sono definiti "dependency", ovvero l'insieme di file da linkare per ottenere l'intero progetto – in questo caso il progetto è usaPal
`
