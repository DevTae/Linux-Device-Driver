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
  - Use **Linux 2.6** and use **the example codes in LDD3**
  - The codes are written for Linux 6.1.2 (If you couldn't run block device driver, then you downgrade your linux below 5)
 
<br/>

Futhermore, You can see the presentation and notes in this repository [**notes folder**](https://github.com/DevTae/Linux-Device-Driver/tree/main/notes).

And, this [link](https://mellow-citrine-e50.notion.site/2023-Edge-Cloud-Lab-0ebb1631ff2249668008b48a6d9d7345?pvs=4) is notion site.
 
<br/>

Thank you for reading.
