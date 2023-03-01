# Linux-Device-Driver

In Edge Cloud Lab, we made presentation and materials for future under-graduate students.

<br/>

We aimed to learn about how to operates the file I/O system in linux kernel.

<br/>

As follows, these all things are the modules for practicing to test.

- Environment Setting
  - Linux 6.1.2
  - Ubuntu 22.04

- hello
  - 'hello, world!' module
  - Test to know **how to insert module** in kernel
  - 'hellop' module is the source code using parameter as like ' **insmod hellop.ko PARAMETER** '
  
- scull
  - 'scull' module
  - Test to know how to transfer object as like **a plain text** between user space and kernel space

- ioctl
  - 'ioctl' module
  - Test to know how to use **ioctl** function in character device driver
  - Use the function as like '**copy_to_user()**', '**copy_from_user()**' to transfer data between user space and kernel space

- sbull
  - 'sbull' module
  - Test to know how to transfer object as like a block space between user space and kernel space
  - Use **Linux 2.6** and use **the example codes in LDD3** (this code could not operates VFS in Linux 6.1.2)
 
<br/>

Futhermore, You can see the presentation and notes in this repository [**notes folder**](https://github.com/DevTae/Linux-Device-Driver/tree/main/notes).
 
<br/>

Thank you for reading.
