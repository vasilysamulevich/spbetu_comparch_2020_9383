#include <iostream>
#include <fstream>

#define N 81
using namespace std;

void printINFO()
{
    cout << "-------------------------------------------------------------" << '\n';
    cout << "| Программу разработала: Чебесова Ирина, группа 9383        |" << '\n';
    cout << "| Задание: преобразовать все заглавные латинские буквы      |" << '\n';
	cout << "| входной строки в строчные, а десятичные цифры в           |" << '\n';
	cout << "| инверсные, остальные символы входной строки передаются в  |" << '\n';
	cout << "| выходную строку непосредственно                           |" << '\n';
    cout << "-------------------------------------------------------------" << '\n';
    cout << '\n';
}

char* change(char* str_in)
{
    char* str_out = new char[N];
    asm(
        "mov r8, %0\n" //записываем в регистр адрес начала выходной строки
        "mov rdi, %1\n" //записываем в регистр адрес начала входной строки

        "changeChar:\n"
        "mov al, [rdi]\n" //берем текущий символ
        "inc rdi\n" //сдвигаемся к следующему символу
        "cmp al, 0\n" //проверяем является ли символ концом строки
        "je end\n"  //если это так, то прыгаем к концу

        "cmp al, 0x80\n" //вышел за таблицу
        "jb changeLat\n" //rus

        //rus
        "changeRus:\n"
        "mov ah, al\n"
        "mov al, [rdi]\n"
        "inc rdi\n" //сдвигаемся к следующему символу
        "cmp ax, 0xD090\n" //А
        "jl writeChar2\n"
        "cmp ax, 0xD0AF\n" //Я
        "jg writeChar2\n"
        "add ax, 0x0020\n"
        "jmp writeChar2\n"


        //меняем заглавные на прописные
        "changeLat:\n"
        "cmp al, 0x5a\n"  //сравниваем с Z
		"jg writeChar\n" //если символ имеет больший код, а следовательно идет дальше в таблице, то сразу переходим к выводу
		"cmp al, 0x41\n" //сравниваем сивол с A
		"jl changeNumber\n" //если он имеет меньший код - то это цифра - уходим в проверку цифры
		"add al, 0x20\n"  //добавлем к коду символа 20, тем самым смещая его в таблице на строчную букву (A - имеет код 0х41, a - имеет код 0х61)
		"jmp writeChar\n" //записываем символ в выходную строку



        //инвертируем цифры
        "changeNumber:\n"
		"cmp al, 0x30\n"  //сравниваем текущий символ с цифрой 0
		"jl writeChar\n"  //если меньше, чем цифра 0 - то сразу пишем
		"cmp al, 0x39\n"  //сравниваем текущий символ с цифрой 9
		"jg writeChar\n"  //если больше, чем цифра 9 - то сразу пишем
		"sub al, 0x39\n"  //вычитаем, чтобы получить инверсированную цифру со знаком "-" - например 0х33 (3) - 0х39 (9) = -6
        "neg al\n"        //убираем знак "-"
		"add al, 0x30\n"  //добавляем обратно для получения цифры

        "writeChar:\n"
        "mov [r8], al\n" //записываем в выходную строку символ
        "inc r8\n" //сдвигаем на следующий символ выходную строку, в которую будем после записываеть
        "jmp changeChar\n"  //снова идем в первоначальную функцию - цикл



        "writeChar2:\n"
        "xchg ah, al\n"
        "mov [r8], ax\n" //записываем в выходную строку символ
        "inc r8\n"
        "inc r8\n"
        "jmp changeChar\n"


        "end:\n"
        :"=m"(str_out) //выходная строка с номером 0
        :"m"(str_in)  //входная строка с номером 1
	);
    return str_out;
}

int main(){
    printINFO();

    char str_in[N];
    fgets(str_in, N, stdin);
    char* str_out = change(str_in);

    ofstream f("result.txt");
    f << str_out;

    cout << str_out << '\n';
    delete[] str_out;
    return 0;
}