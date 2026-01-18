## stats
### General
- **stats** - переделаный проект https://github.com/sysstat/sysstat. Запускается через как сервис и посылает данные о системе через udp в json-формате на определенный порт

Проект состоит из нескольких модулей:
- stats - отправка json по сети
- saf - форматирование статистики в json
- sadc - сбор статистики

Каждый модуль, кроме stats, отправляет данные в STDOUT, а получает через STDIN.

System statistics collected by saf:
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

### Build
Сборка
```
cmake .
cmake --build .
```
Очистка от CMake файлов и бинарников
```
cmake --build . --target clean
```

## Установка

>**Внимание:**
- Установочный скрипт только для Debian-based систем и aarch64 архитектуры. Для OpenSUSE надо изменить название libsensors4-dev на libsensors4-devel

Скачать папку с проектом в ~/app

```
cd ~/app/
git clone git@github.com:t1msi/libsysstat.git stats/
```

Для установки в систему необходимо выполнить скрипт ```install.bash```, расположенный в папке ```scripts```. Ввести пароль для sudo

```
cd ~/app/stats/scripts
bash install.bash
```

Можно сделать uninstall из системы, необходимо выполнить скрипт ```uninstall.bash```

## Запуск
- Сервис stats автоматически начнет свою работу. После перезагрузки тоже. По умолчанию запускается на адрес ```127.0.0.1:10000```

### Troubleshooting
- На порт не приходят данные
Нужно перезапустить сервис
```
sudo systemctl restart stats
```
- На показываются данные о батарее
На Ubuntu Bionic 18.04.06 LTS батарея находится в корне /sys/class/power_supply, sadc ожидает увидеть подпапку power_supply/BAT0. Проверьте что ссылка с именем BAT0 на power_supply в папке stats/tmp создалась

Такая же ситуация с Serial на raspbian
Но нужно уже проверить в include/rd_stats.h путь
```
#define SERIAL /proc/tty/driver/ttyAMA
```
- Проблема с CMakeCache, после того как сделали rsync во время разработки. Не собирается stats
```
cd projects/stats/
rm -rf CMakeFiles/
cmake .
cmake --build . --target clean-all
cmake --build .
```

### Known bugs
- Почему то нагрузка на CPU неправильно форматируется, из за чего idle показывается неправильно. Когда пытался дебажить, то дошел до пересчета jiffies, возможно там проблема
- Отсутствует pretty-print, из-за чего числа, которые находятся внутри json невозможно интерпретировать правильно. Но это относится не ко всем топикам


