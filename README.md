# Странички семинаров по курсу CAOS (Computer Architecture and Operating Systems) HSE

# Темы текущего года

1. Терминал, vim, tmux -  [конспект](01-intro), [видео](https://youtu.be/WGxA9WTWyNQ?si=19kf_FQjDpgLqb_1)

## Прошлые года

[2021 год. 207 группа](2021_207)

[2022 год. 216 группа](2022_216)

## Правила игры

### Форматирование ASM

1. В одном блоке кода (участок кода отделенный новой строкой сверху и снизу) между командами и аргументами должен быть отступ одного размера. Отступ должен быть сделан табом, а не пробелом. То есть все аргументы должны быть выравнены.

```
        pushl   %ebp
        movl    %esp, %ebp

        subl    $4, %esp
        and     $-16, %esp
        leal    -4(%ebp), %eax
```

### Реджекты

Сданный код проходит тестирование в тестирующей системе, а затем проверяется глазами семинариста и ассистентов.

Прошедший тесты код может быть отправлен на исправление (получен статус reject), если решение является неоптимальным, если нарушает условия, установленные в задаче или если нарушает форматирование.

### Списывание

Проверка на списывание запускается в случайное время. При обнаружении списывания обнуляется ВЕСЬ контест и составляется записка в УО.

В проверке на списывание участвуют все доступные онлайн материалы, в том числе известные репозитории с решениями и решения прошлых годов.

Если вы берете код из общедоступного источника, то нужно прикладывать ссылку на этот источник и пояснять комментариями код, чтобы было видно, что вы этот код понимаете.



## Полезные ссылки

### Материалы

* [Материалы курса 2020 года](https://github.com/blackav/hse-caos-2020)
* [Материалы физтеха](https://github.com/victor-yacovlev/fpmi-caos)
* [Вики учебник по asm](https://ru.wikibooks.org/wiki/%D0%90%D1%81%D1%81%D0%B5%D0%BC%D0%B1%D0%BB%D0%B5%D1%80_%D0%B2_Linux_%D0%B4%D0%BB%D1%8F_%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%81%D1%82%D0%BE%D0%B2_C)

### Тулзы

* [Godbolt - compiler explorer](https://gcc.godbolt.org/)

## Файлы

* [C11 стандарт](C11_standard.pdf)
