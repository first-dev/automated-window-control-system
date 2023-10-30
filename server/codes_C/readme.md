## dht11

a simple driver for the dht11 module

## 28BYJ-48

a simple driver for the 28BYJ-48 stepper motor

## pi_mmio

a simple driver for the raspberry pi's GPIOs

## timing

a helper module for precise delays and precise time measurements

## control_window.c

a program to control the stepper motor with a simple

### usage

```bash
./control_window <state>
```

where state is either 0 or 1

## record_temp.c

a program to record the temperature from the dht11 module and save it to an sqlite3 database

### usage

```bash
./record_temp <database>
```

where database is the path to the sqlite3 database
