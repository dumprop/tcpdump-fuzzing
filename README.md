# Фаззинг tcpdump

Проект по фаззингу tcpdump'a. Для этого применяется AFL++.


## Описание сборки Dockerfile

1. Производится сборка AFL++ (/afl/AFLplusplus-4.09c)
2. Производится сборка tcpdump (/tcpdump)
2.1 Производится сборка библиотеки libpcap (/tcpdump/tcpdump-tcpdump-4.9.1/libpcap) 



## Запуск фаззинга через Dockerfile_simple - простой, без собственного харнесса

Производим сборку Docker контейнера:
``` bash
docker build -f Dockerfile_simple --tag=tcpdump_simple .
```

Запускаем фаззинг:
``` bash
cd /tcpdump/tcpdump-tcpdump-4.9.1/
mkdir afl_out
afl-fuzz -i tests/ -o afl_out/ -t 1000 -- ./tcpdump -r @@
```


## Сбор покрытия
Типовая сборка для оценки покрытия
``` bash
./configure CFLAGS="-fprofile-arcs -ftest-coverage" CXXFLAGS="-fprofile-arcs -ftest-coverage"
make
```

Оценка покрытия по собранным сэмплам 
``` bash
for file in afl_out/queue/*; do
    ./tcpdump -r "$file"
done
```

Генерация отчёта покрытия кода
``` bash
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory out
```
