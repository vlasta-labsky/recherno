# recherno

recherno is project aimed to reverse engineer 
an old nuclear power plant simulator game *Chernobyl: The Legacy Continues*

# How it works
In this stage, recherno runs original Fortran/win32 game engine in custom virtual machine.

Virtual machine uses [unicorn](https://github.com/unicorn-engine/unicorn)
for running the win32 code and [capstone](https://github.com/capstone-engine/capstone)
for debugging purposes

Work is being done to reverse engineer all the API functions of the engine 
and build a Qt GUI to control all game functionality.

After that, plan is to do complete reverse engineering to get rid of blobs.

# Running
You need to place original game win32 dll (chrnob_32.dll - not the DOS one) and icd file (shutdown.icd)
next to the executable.
