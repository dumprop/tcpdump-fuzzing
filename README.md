# Фаззинг tcpdump

Проект по фаззингу tcpdump'a. Для этого применяется AFL++.


## Описание сборки Dockerfile

1. Производится сборка AFL++ (/afl/AFLplusplus-4.09c)
2. Производится сборка tcpdump (/tcpdump)
2.1 Производится сборка библиотеки libpcap (/tcpdump/tcpdump-tcpdump-4.9.1/libpcap) 



## Запуск фаззинга без собственного харнесса (1 вариант)

Производим сборку Docker контейнера:
``` bash
docker build -f Dockerfile --tag=tcpdump_fuzzing .
```

Запускаем фаззинг:
``` bash
mkdir afl_out
afl-fuzz -i tests/ -o afl_out/ -t 1000 -- ./tcpdump -r @@
```

## Запуск фаззинга с применением собственного харнесса (2 вариант)

Производим сборку Docker контейнера:
``` bash
docker build -f Dockerfile --tag=tcpdump_fuzzing .
```

Производим компиляцию харнесса:
``` bash

afl-cc -ffloat-store -DHAVE_CONFIG_H   -D_U_="__attribute__((unused))" -I. -I./libpcap  -g -O2 -c ./fuzzing_harness.c
afl-cc -ffloat-store -DHAVE_CONFIG_H   -D_U_="__attribute__((unused))" -I. -I./libpcap  -g -O2  -o fuzzing_harness setsignal.o version.o addrtoname.o addrtostr.o af.o ascii_strcasecmp.o checksum.o cpack.o gmpls.o gmt2local.o in_cksum.o ipproto.o l2vpn.o machdep.o nlpid.o oui.o parsenfsfh.o print.o print-802_11.o print-802_15_4.o print-ah.o print-ahcp.o print-aodv.o print-aoe.o print-ap1394.o print-arcnet.o print-arp.o print-ascii.o print-atalk.o print-atm.o print-babel.o print-beep.o print-bfd.o print-bootp.o print-bt.o print-calm-fast.o print-carp.o print-cdp.o print-cfm.o print-chdlc.o print-cip.o print-cnfp.o print-dccp.o print-decnet.o print-dhcp6.o print-domain.o print-dtp.o print-dvmrp.o print-eap.o print-egp.o print-eigrp.o print-enc.o print-esp.o print-ether.o print-fddi.o print-forces.o print-fr.o print-frag6.o print-ftp.o print-geneve.o print-geonet.o print-gre.o print-hncp.o print-hsrp.o print-http.o print-icmp.o print-icmp6.o print-igmp.o print-igrp.o print-ip.o print-ip6.o print-ip6opts.o print-ipcomp.o print-ipfc.o print-ipnet.o print-ipx.o print-isakmp.o print-isoclns.o print-juniper.o print-krb.o print-l2tp.o print-lane.o print-ldp.o print-lisp.o print-llc.o print-lldp.o print-lmp.o print-loopback.o print-lspping.o print-lwapp.o print-lwres.o print-m3ua.o print-medsa.o print-mobile.o print-mobility.o print-mpcp.o print-mpls.o print-mptcp.o print-msdp.o print-msnlb.o print-nflog.o print-nfs.o print-nsh.o print-ntp.o print-null.o print-olsr.o print-openflow-1.0.o print-openflow.o print-ospf.o print-ospf6.o print-otv.o print-pgm.o print-pim.o print-pktap.o print-ppi.o print-ppp.o print-pppoe.o print-pptp.o print-radius.o print-raw.o print-resp.o print-rip.o print-ripng.o print-rpki-rtr.o print-rrcp.o print-rsvp.o print-rt6.o print-rtsp.o print-rx.o print-sctp.o print-sflow.o print-sip.o print-sl.o print-sll.o print-slow.o print-smtp.o print-snmp.o print-stp.o print-sunatm.o print-sunrpc.o print-symantec.o print-syslog.o print-tcp.o print-telnet.o print-tftp.o print-timed.o print-tipc.o print-token.o print-udld.o print-udp.o print-usb.o print-vjc.o print-vqp.o print-vrrp.o print-vtp.o print-vxlan.o print-vxlan-gpe.o print-wb.o print-zephyr.o print-zeromq.o netdissect.o signature.o strtoaddr.o util-print.o print-smb.o smbutil.o strlcat.o strlcpy.o fuzzing_harness.o ./libpcap/libpcap.a
```
INFO: в файл fuzzing_harness.c нужно поместить текст из файла bgp_attr_print-harness (в таком случае без применяться обычный харнесс) или bgp_attr_print-harness_persistent (в таком случае будет применяться харнесс с persistent_mode для лучшей производительности)

Запускаем фаззинг:
``` bash
afl-fuzz -i tests/ -o afl_out/ -t 1000 -- ./fuzzing_harness @@
```



## Сбор покрытия
Типовая сборка для оценки покрытия
``` bash
./configure CFLAGS="-fprofile-arcs -ftest-coverage" CXXFLAGS="-fprofile-arcs -ftest-coverage"
make
```

Оценка покрытия по собранным сэмплам 
``` bash
for file in afl_out/default/queue/*; do
    ./tcpdump -r "$file"
done
```

Генерация отчёта покрытия кода
``` bash
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory out
```
