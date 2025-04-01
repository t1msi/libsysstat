## sysstat - System performance tools for the Linux operating system
(C) 1999-2025 Sebastien GODARD (sysstat (at) orange (dot) fr)
2025 - patched by refty

### Introduction

The sysstat package contains various utilities, common to many commercial Unixes, to monitor system performance and usage activity:

* **iostat** reports CPU statistics and input/output statistics for block devices and partitions.
* **mpstat** reports individual or combined processor related statistics.
* **pidstat** reports statistics for Linux tasks (processes) : I/O, CPU, memory, etc.

Sysstat also contains tools you can schedule via cron or systemd to collect and historize performance and activity data:

* **sar** collects, reports and saves system activity information (see below a list of metrics collected by sar).
* **sadc** is the system activity data collector, used as a backend for sar.
* **sadf** displays data collected by sar in multiple formats (CSV, XML, JSON, etc.) and can be used for data exchange with other programs. This command can also be used to draw graphs for the various activities collected by sar using SVG (Scalable Vector Graphics) format.

Default sampling interval is 10 minutes but this can be changed of course (it can be as small as 1 second).

#### System statistics collected by sar:
- Input / Output and transfer rate statistics (global, per device, per partition and per network filesystem)
- CPU statistics (global and per CPU), including support for virtualization architectures
- Memory, hugepages and swap space utilization statistics
- Virtual memory, paging and fault statistics
- Process creation activity
- Interrupt statistics (global, per CPU and per interrupt, including potential APIC interrupt sources, hardware and software interrupts)
- Extensive network statistics: network interface activity (number of packets and kB received and transmitted per second, etc.) including failures from network devices; network traffic statistics for IP, TCP, ICMP and UDP protocols based on SNMPv2 standards; support for IPv6-related protocols
- Fibre Channel traffic statistics
- Software-based network processing (softnet) statistics
- NFS server and client activity
- Sockets statistics
- Run queue and system load statistics
- Kernel internal tables utilization statistics
- Swapping statistics
- TTY devices activity
- Power management statistics (instantaneous and average CPU clock frequency, fans speed, devices temperature, voltage inputs)
- USB devices plugged into the system
- Filesystems utilization (inodes and blocks)
- Pressure-Stall Information statistics

#### Sysstat key features:
- Display average statistics values at the end of the reports.
- On-the-fly detection of new devices (disks, network interfaces, etc.) that are created or registered dynamically.
- Support for UP and SMP machines, including machines with hyperthreaded or multi-core processors.
- Support for hotplug CPUs (it detects automagically processors that are disabled or enabled on the fly) and tickless CPUs.
- Works on many different architectures, whether 32- or 64-bit.
- Needs very little CPU time to run (written in C).
- System statistics collected by sar/sadc can be saved in a file for future inspection. You can configure the length of data history to keep. There is no limit for this history length but the available space on your storage device.
- System statistics collected by sar/sadc can be exported in various different formats (CSV, XML, JSON, SVG, etc.). DTD and XML Schema documents are included in sysstat package. JSON output format is also available for mpstat and iostat commands.
- iostat can display statistics for devices managed by drivers in userspace like spdk.
- Smart color output for easier statistics reading.

### Build
```
cmake --build .

cmake --build . --target clean
```
