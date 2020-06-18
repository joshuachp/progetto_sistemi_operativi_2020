---
title: System Calls
author: Joshua Samuel Chapman
date: 18-06-2020
---

# Progetto Sistemi Operativi \- System Calls

[![pipeline status](https://gitlab.com/joshuachp/progetto_sistemi_operativi/badges/master/pipeline.svg)](https://gitlab.com/joshuachp/progetto_sistemi_operativi/-/commits/master)
Progetto sistemi operativi UNIVR 2019/2020 sulle system calls.

## Building

Il progetti include un `Makefile` per la compilazione, la quale può avvenire in
due modi diversi.

- Cmake e Ninja
- Make

Il metodo di default è tramite `cmake`, mentre il secondo è per compatibilità
in mancanza dei build tools su un sistema. Con `cmake` si compilano anche i
test per il programma.

La compilazione tramite `cmake` produce due tipi di file binari: Debug e Release.

In modalità debug il programma permette la terminazione del server con `<C-c>`
o `SIGINT` e utilizza la system call `mmap` per leggere interamente il file delle
posizioni.

In modalità release il programma blocca tutti i segnali a eccezione di `SIGTERM`
e legge il file delle posizioni a chunks con la system call `read`.

Questo avviene settando la flag `NDEBUG`.

### Debug

Per compilare in modalità Debug:

```bash
make cmake-debug
```

### Release

Per compilare in release:

```bash
# Cmake & Ninja
make cmake
# Make
make legacy
```

## Test

Per testare il programma si utilizza il framework `google-test` e `ctest`. Per
invocare i test:

```bash
make test
```
