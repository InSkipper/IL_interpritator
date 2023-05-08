# IL_interpritator

## Потерялся заголовочник **comqueue.h**, добавлю попозже


### Описание задания
В рамках данного задания необходимо разработать интерпретатор языка Instraction List (список команд здесь: https://sm1820.github.io/beremiz/iec_guide/il_guide.html).
Основой языка IL является аккумулятор. Почти все команды выполняют работу с ним. Результат выполнения команды записывается в аккумулятор. 


### Особенности реализации
- Парсер читает програму из файла prog.txt из директории программы
- Из типов данных поддерживается только int64
- Вместо **ld in**, **st out** используется **read** и **print** соответственно
- Ошибки не очень обрабатываются, парсер указывает только на отсутсваие команды


### Парсер
**Парсер** читает программу из файла **prog.txt** и переводит её в **очередь команд** c помощью метода [**parse**](https://github.com/InSkipper/IL_interpritator/blob/20ce585ad05d9150a57f002e7b7e0ddc7a9aeed2/parser.c#L233). Как уже было сказано выше парсер не обрабатывает ошибки, но может указать на неправильно введенную команду и прекратить работу программы.

### Интерпритатор
Интерпритатор получает очередь команд от парсера и выполняет их ([метод **interpret_program**](https://github.com/InSkipper/IL_interpritator/blob/20ce585ad05d9150a57f002e7b7e0ddc7a9aeed2/inter.c#L350)).

### Очередь команд
Вспомогательный модуль для сбора команд. Работает по принципу **FIFO**.

## 

**Список доступных комманд**
| Оператор | Описание |
|---|---|
READ|Загрузка значения операнда в аккумулятор
PRINT|Вывод значения аккумулятора
LD _arg_|Загрузить значение операнда в аккумулятор
LDN _arg_|Загрузить обратное значение операнда в аккумулятор
AND _arg_|Поразрядное И аккумулятора и операнда
ANDN _arg_|Поразрядное И аккумулятора и обратного операнда
OR _arg_|Поразрядное ИЛИ аккумулятора и операнда
ORN _arg_|Поразрядное ИЛИ аккумулятора и обратного операнда
XOR _arg_|Поразрядное разделительное ИЛИ аккумулятора и операнда
XORN _arg_|Поразрядное разделительное ИЛИ аккумулятора и обратного операнда
NOT|Поразрядная инверсия аккумулятора
ADD _arg_|Сложение аккумулятора и операнда, результат записывается в аккумулятор
SUB _arg_|Вычитание операнда из аккумулятора, результат записывается в аккумулятор
MUL _arg_|Умножение аккумулятора на операнд, результат записывается в аккумулятор
DIV _arg_|Деление аккумулятора на операнд, результат записывается в аккумулятор
GT _arg_|Значение аккумулятора сравнивается со значением операнда (>(greater than)). Значение (TRUE (1) или FALSE (0)) записывается в аккумулятор
GE _arg_|Значение аккумулятора сравнивается со значением операнда (>=greater than or equal)). Значение (TRUE (1) или FALSE (0)) записывается в аккумулятор
EQ _arg_|Значение аккумулятора сравнивается со значением операнда (=(equal)). Значение (TRUE (1) или FALSE (0)) записывается в аккумулятор
NE _arg_|Значение аккумулятора сравнивается со значением операнда (<>(not equal). Значение (TRUE (1) или FALSE (0)) записывается в аккумулятор
LE _arg_|Значение аккумулятора сравнивается со значением операнда (<=(less than or equal to)). Значение (TRUE (1) или FALSE (0)) записывается в аккумулятор
LT _arg_|Значение аккумулятора сравнивается со значением операнда (<(less than)). Значение (TRUE (1) или FALSE (0)) записывается в аккумулятор
JMP _arg_|Переход к метке
JMPC _arg_|Переход к метке при условии, что значение аккумулятора TRUE (1)
JMPCN _arg_|Переход к метке при условии, что значение аккумулятора FALSE (0)
RET|Выход из програмы
RETC|Выход из програмы при условии, что значение аккумулятора TRUE (1)
RETCN|Выход из програмы при условии, что значение аккумулятора FALSE (0)

##

### Пример програмы
```
read
add 10
gt 15
retc
ld 15
print
ret
```
**Вывод:**

Добавлю позже
