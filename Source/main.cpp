#include <iostream>
#include <string>
#include <string_view>
#include <map>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <ctime>


using namespace std;

/*
Шерлок и Анаграммы
Две строки являются анаграммами друг друга, если буквы одной строки можно переставить, чтобы 
получить другую строку. Дана строка, найдите количество пар подстрок этой строки, которые 
являются анаграммами друг друга.

Описание алгоритма:

Два слова являются анаграммами друг друга, если одно слово может быть получено из другого путем 
смены порядка символов. Реверс слово является лишь частным случаем анаграммы.
Очевидно, что у всех анаграмм одного и того же слова является неизменной одна характеристика: набор
символов, встречаемых в анаграмме и количество встречаемости каждого символа в анаграмме. Это своего
рода детерминант анаграммы. Понятно, что если мы отсортируем все символы в анаграмме по возрастанию, 
мы тоже получим детерминант анаграммы.
В этой программе для получения детерминанта анаграммы мы будем использовать сортировку.
Две строки являются анаграммами друг друга, если результаты сортировки каждой из них равны.

Алгоритм:

Объявляем целое число anogramSum = 0;

Внешний цикл: идем по длине подстроки len = от 1 до (длина входной строки -1).
{
    Создаем ассоциативный массив map<string, long>.
    в этом массиве мы будем накапливать детерминанты анаграмм и сколько раз они встретились в исходной строке.
    
    Первый вложенный цикл: 
    мы берем последовательные подстроки входной строки длиной переменной len из внешнего цикла
    {

        Каждую такую подстроку мы сортирует во возрастанию по символа - получаем детерминант строки.
        И увеличиваем значение map[детерминант]++;

        цель - подсчитать прямое количество, сколько раз детерминант анаграммы встречаются в строке
    }
    
    Второй вложенный цикл.
    Идем по всем элементам ассоциативного массива map (по всем анаграммам)
    {
        Расчитываем количество пар для текущей арангаммы.
        У нас есть общее количество встречаемости анаграммы N.
        Количество пар: (N-1)+(N-2)+ ... +1

        (1 + (N-1))
        ----------- (N-1) = N*(N-1)
             2

        Это арифметическая прогрессия, сумма которой = N*(N-1)/2
        При операциях приводить к дробному и округлять не надо, т.к. сумма целых всегда целая.
        А также потому, что из двух последовательных чисел (N-1) и N четное. 

        anogramSum += КоличествоПар
    }
}

вернуть anogramSum 
*/

struct ResultStruct {
    long anagramCount;
    long operationCount;
};

ResultStruct AnagramCount(string str) {
    long strLen = str.length();

    long anagramCount = 0;
    long operationCount = 0;

    for (size_t len = 1; len < strLen; len++)
    {
        map<string, long> frequencyMap;

        // Заполняем карту частоты встречаемости подстрок
        for (size_t pos = 0; pos <= strLen-len; pos++)
        {
            operationCount++;

            // ключем является отсортированная Анаграмма
            // ибо анаграмма - это любая перестановка букв в слове
            // если отсортировать буквы в анаграмме, то для всех анаграмм будет одно и то же значение
            // фактически, это инвариант анаграммы
            auto substr = str.substr(pos, len);
            if (len > 1) {
                sort(substr.begin(), substr.end());
            }
                
            frequencyMap[substr]++;
        }

        // По карте встречаемости слов вычисляем количество повторений
        for (auto [word, wordCount] : frequencyMap)
        {
            operationCount++;

            if (wordCount > 1) {
                // вычисляем количество повторений
                // (wordCount-1)+(wordCount-2)+ ... +1
                // Это просто среднее арифметическое
                // Ср. арифметическое считает по формуле (первый+поледний)*количество/2
                long combinationCount = (wordCount - 1) * wordCount / 2;
                anagramCount += combinationCount;
            }
        }
    }

    //ResultStruct

    return ResultStruct{
        anagramCount = anagramCount,
        operationCount = operationCount
    };
}



void TestAnagramCount(string line, long value, string message = "Ошибка алгоритма. Д.б. ") {
    auto result = AnagramCount(line);

    if (result.anagramCount != value) {
        throw logic_error(message + to_string(value));
    }
}


void Test() {
    // все тесты взяты из задания
    // более в задании тестов нет
    TestAnagramCount("mom", 2);
    TestAnagramCount("abba", 4);
    TestAnagramCount("abcd", 0);
    TestAnagramCount("ifailuhkqq", 3);
    TestAnagramCount("kkkk", 10);
    TestAnagramCount("cdcd", 5);
}

/// <summary>
/// Генерирует строку длиной strLength заполненную случайными  символами от 'a' до 'z'
/// Важно: до использования функции вызвать srand(time(0)) для инициализации датчика случайных чисел
/// </summary>
/// <param name="strLength">длина строки</param>
/// <returns>строка со случайными символами длиной strLength</returns>
string GenerateRandomString(size_t strLength) {
    string result(strLength, ' ');
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    size_t charsetSize = sizeof(charset) - 1;

    // Формирование строки
    for (size_t i = 0; i < strLength; ++i) {
        result[i] = charset[rand() % charsetSize];
    }

    return result;
}

int main()
{
    // тестируем на тестах, которые есть в задании
    // чтобы быть уверенным, что алгоритм работает правильно
    Test();

    // Инициализация генератора случайных чисел
    // Это нужно для формирования случайных строк
    srand(time(0));

    // выводим таблицу зависимости количества операций и количесства анаграм
    // в зависимости от длины строки
    // строка заполняется случайными символами от 'a' до 'z'
    // на основе этой таблицы будет выведена подтвержденная сложность
    // в нашем случае сложность квадратичная и график функции будет параболой
    cout << "String length\tOperation count\tAnagram count";
    for (size_t stringLength = 10; stringLength <= 300; stringLength+=10)
    {
        auto randomString = GenerateRandomString(stringLength);
        auto result = AnagramCount(randomString);

        cout << endl << stringLength << '\t' << result.operationCount << '\t' << result.anagramCount;
    }
}

