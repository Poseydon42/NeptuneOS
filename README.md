# NeptuneOS
A home-made, hobby operating system written for AMD64.
## About
This is a hobby project that I've started when I was really bored in the evenings, without thinking too much in advance. I was doing it just for fun and to get some experience in the system programming.
## How to build
1. Install required packages
	`sudo apt-get install build-essential ninja cmake flex libgmp3-dev libmpc-dev libmpfr-dev texinfo qemu`
2. Rebuild toolchain: run `Toolchain/build-toolchain.sh`. It will download and set up tools such as binutils and GCC for cross-compilation
3. Run `create-projects.sh`. It will run CMake to create all necessary build files. You would also have to run this file whenever you make any changes to the CMake project files(e.g. add new cpp file to the project).
4. During the previous step, the script would have created two separate folders - `Build/System` and `Build/Tests`. First one is cross-compiled code and contains actual OS binaries. Second one is a hosted compilation of some of the modules to be able to run unit tests on them.
5. To run the OS, you have to first build it. Run `ninja && ninja image && ninja run` to build the OS, create a disk image and run it in qemu. This should be done in `Build/System` folder.
6. To run unit tests, run `ninja && ninja tests` in `Build/Tests`.
## How to contribute
Please feel free to contribute to the project however you would like to. Any help, tips or constructive criticism is welcome.
## License
Neptune OS is licensed under MIT License
