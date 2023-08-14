# Unit Testing

(See the README.md file in the upper level 'examples' directory for more information about examples.)

This is the refactored code for hygienie-2022 refactored project that uses the esp-idf framework. This means that source code has been refactored into modules, unit-tests have been written for each module and can be run on the esp32 itself for regression testing, and that project values such as wifi network, password, device id, and sleep duration can be entered in a terminal menu.


## Project layout

```
hygienie_project                      — Application project directory
  - components                 — Components of the application project
    + appdata                  — Module that describes how the data is structured.
    + appgraphics              — Module that uses graphics libary and displays animations
    + appmqtt                  — Module that encapsulates mqtt client
    + appsleep                 — Module that controls esp32 sleep  mode
    + appwifi                  — Module that encapsulates wifi libraries
  + main                       - Main source files of the application project
  - test                       — Test project directory
    - main                       — test main directory for test runner
      + hygienie_unit_test.c   — hygienie test runner.
  Makefile / CMakeLists.txt    - Makefiles of the application project
```

The two projects, main, and test/main, can implement different application behavior when running normally, and when running the unit tests. Top level project is the actual application being developed. Test project included within is a simple application which only runs the unit tests.

## Unit tests for a component

Inside the `testable` component, unit tests are added into `test` directory. `test` directory contains source files of the tests and the component makefile (component.mk / CMakeLists.txt).

```
unit_test
  - components                              - Components of the application project
    - testable
      - include
      - test                                - Test directory of the component
        * component.mk / CMakeLists.txt     - Component makefile of tests
        * test_mean.c                       - Test source file
      * component.mk / CMakeLists.txt       - Component makefile
      * mean.c                              - Component source file
```

When the main application project is compiled, tests are not included. Test project includes the tests by setting `TEST_COMPONENTS` variable in the project makefile.

## Requirements

### Hardware required

* FireBeetle DFR0478, but any esp32 board will work.
* USB cable - USBA/


### Software Requirements
Please look at https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/
for installing esp-idf and https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html#get-started-linux-macos-first-steps
for configuring and building project on command line.

It is recommended that you run:
```
. $HOME/esp/esp-idf/export.sh
```
in your terminal.

and create the alias:
```
alias get_idf='. $HOME/esp/esp-idf/export.sh'# Configure the project
```
For the menu_config:


For the test project, you can open the project configuration menu (`idf.py menuconfig`) and explore a few options related to Unity under Component Config, Unity unit testing library.

### Build and flash

As explained above, this example contains two projects: application project and test project.

1. Application project calls an API defined in the component, and displays the results. It is not of much value to run. Application project is provided mainly to illustrate the layout of all the files. If you decide to run this project, the procedure is:

    * Run `idf.py -p PORT flash monitor` in the current directory (`unit_test`).
    * Observe the output: a list of random numbers and their mean value.

2. Test project is responsible for running the tests.

	* Enter `test` directory (`unit_test/test`), and run `idf.py -p PORT flash monitor`.
	* Observe the output: results of test case execution.


See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.



## Unit Testing

Inside the `testable` component, unit tests are added into `test` directory. `test` directory contains source files of the tests and the component makefile (component.mk / CMakeLists.txt).

```
unit_test
  - components                              - Components of the application project
    - testable
      - include
      - test                                - Test directory of the component
        * component.mk / CMakeLists.txt     - Component makefile of tests
        * test_mean.c                       - Test source file
      * component.mk / CMakeLists.txt       - Component makefile
      * mean.c                              - Component source file
```

When the main application project is compiled, tests are not included. Test project includes the tests by setting `TEST_COMPONENTS` variable in the project makefile.
### Application project output

```
In main application. Collecting 32 random numbers from 1 to 100:
   5   66   86   98   62   66   40   42    7   62
  33   77   40   54    8   75   39   82   73    4
  22    5   72   78   53    1    6   48   23    1
  84   55
Mean: 45
```

### Test project output

```
#### Executing one test by its name #####

Running Mean of an empty array is zero...
/Users/ivan/e/esp-idf/examples/system/unit_test/components/testable/test/test_mean.c:16:Mean of an empty array is zero:PASS

-----------------------
1 Tests 0 Failures 0 Ignored
OK

Running Mean of an empty array is zero...
/home/user/esp/esp-idf/examples/system/unit_test/components/testable/test/test_mean.c:15:Mean of an empty array is zero:PASS
Test ran in 16ms

-----------------------
1 Tests 0 Failures 0 Ignored
OK
Enter next test, or 'enter' to see menu
```
