// Таймер. Одной кнопкой прибавляется время, второй — уменьшается. При нажатии на обе кнопки сразу значение на LED-дисплее обнуляется
// Кнопка "старт/стоп" для запуска и остановки таймера
// Буззер — звуковой сигнал при обнулении значения на экране

int buttonState = 0; // переменная для хранения состояния кнопки "часы"
int buttonState1 = 0; // переменная для хранения состояния кнопки "минуты"

int counter = 0; // переменная для подсчёта нажатий на кнопки "часы"
int counter1 = 0; // переменная для подсчёта нажатий на кнопки "минуты"

int ARRAY[4]; // массив для хранения количества нажатий кнопок "часы" и "минуты"

unsigned long halfsec; // переменная для работы мигающего разделителя
boolean dots; // переменная для работы мигающего разделителя// переменная для работы мигающего разделителя

boolean flag = 0; // логическая переменная, которая хранит последнее состояние кнопки
boolean flag1 = 0; // логическая переменная, которая хранит последнее состояние кнопки 2
boolean flag2 = 0; // логическая переменная, которая хранит последнее состояние кнопки
boolean flag3 = 0; // логическая переменная, которая хранит последнее состояние кнопки 2

#include <TM1637.h> //подключаем библиотеку модуля TM1637
#define CLK 3 // Определяем пины для подключения TM1637 к плате Arduino
#define DIO 4 // Определяем пины для подключения TM1637 к плате Arduino
#define buttonPin 2 // Определяем пин для подключения кнопки "увеличение часов"
#define buttonPin2 5// Определяем пин для подключения кнопки "уменьшение часов"
#define buttonPin3 6 // Определяем пин для подключения кнопки "увеличение минут"
#define buttonPin4 7 // Определяем пин для подключения кнопки "уменьшение минут"

TM1637 tm1637(CLK, DIO); // создаём объект класса TM1637, в качестве параметров используем номера пинов подключения дисплея к Ардуине

void setup() {

  Serial.begin(9600);

  tm1637.init(); // инициализация модуля TM1637

  tm1637.set(BRIGHT_TYPICAL); // устанавливаем яркость дисплея

  pinMode(buttonPin, INPUT); // инициализируем пин, подключенный к кнопке "увеличение часов", как вход
  pinMode(buttonPin2, INPUT); // инициализируем пин, подключенный к кнопке "уменьшение часов", как вход
  pinMode(buttonPin3, INPUT); // инициализируем пин, подключенный к кнопке "увеличение минут", как вход
  pinMode(buttonPin4, INPUT); // инициализируем пин, подключенный к кнопке "уменьшение минут", как вход
}

void loop() {
  //tm1637.point(true); // включаем разделитель (двоеточие). Выключить (закомментить), если хотим использовать моргание двоеточия по рецепту Гайвера ниже

  // Включаем моргание разделительных точек по рецепту Алекса Гайвера: http://alexgyver.ru/tm1637_display/http://alexgyver.ru/tm1637_display/

  if (millis() - halfsec > 500) {  // если прошло полсекунды
    halfsec = millis();            // сбросить таймер
    dots = !dots;                  // переключить точки
    if (dots) tm1637.point(0);     // выключить точки
    else tm1637.point(1);          // включить точки
  }

  // с помощью написанной функции выводим на дисплей значения часов и минут. Сначала они нулевые.
  //После выполнения программы в эту функцию записываются обновлённые значения. И, если нужно вывести эти обновлённые значения, можно снова вызвать функцию

  UpdateTimer();

  buttonState = digitalRead(buttonPin); // считываем значения с входа кнопки "увеличение часов" таймера

  if (buttonState == HIGH && flag == 0) { // если кнопка нажата и переменная flag = 0
    counter ++; // прибавляем единицу к переменной счётчика
    flag = 1; // происходит только одно действие + защита от "дребезга"

    if (counter > 99) { // если значение > 99, то счётчик сбрасывается на ноль
      counter = 0;
    }

    ARRAY[0] = counter / 10; // записываем в 1 ячейку массива состояние кнопки "часы" (первая цифра перед запятой после деления на 10)
    ARRAY[1] = counter % 10; // записываем во 2 ячейку массива состояние кнопки "часы" (остаток от деления на 10)

    Serial.print("Часы: "); Serial.println(counter); // выводим в COM-порт состояние кнопки = переменной counter

  }

  if (buttonState == LOW && flag == 1) {
    flag = 0;
  }

  // вторая кнопка уменьшает значение на единицу

  if (digitalRead(buttonPin2) == HIGH && flag1 == 0) {
    counter--;
    flag1 = 1;

    if (counter <= 0) { // если значение <= нулю, то сброс останавливаем
      counter = 0;
    }

    ARRAY[0] = counter / 10; // записываем в 1 ячейку массива состояние кнопки "часы" (первая цифра перед запятой после деления на 10)
    ARRAY[1] = counter % 10; // записываем во 2 ячейку массива состояние кнопки "часы" (остаток от деления на 10)

    Serial.print("Часы: "); Serial.println(counter); // выводим в COM-порт состояние кнопки = переменной counter

  }

  if (digitalRead(buttonPin2) == LOW && flag1 == 1) {
    flag1 = 0;
  }

  // при нажатии обеих кнопок сразу, значение на LED-дисплее обнуляется

  if (flag == 1 && flag1 == 1) {
    counter = 0;
  }

  // ВТОРАЯ ЧАСТЬ КОДА ДЛЯ УСТАНОВКИ МИНУТ

  buttonState1 = digitalRead(buttonPin3); // считываем значения с входа кнопки "увеличение минут" таймера

  if (buttonState1 == HIGH && flag2 == 0) { // если кнопка нажата и переменная flag = 0
    counter1 ++; // прибавляем единицу к переменной счётчика
    flag2 = 1; // происходит только одно действие + защита от "дребезга"

    if (counter1 > 60) { // если значение > 60, то счётчик сбрасывается на ноль
      counter1 = 0;
    }

    ARRAY[2] = counter1 / 10; // записываем в 1 ячейку массива состояние кнопки "часы" (первая цифра перед запятой после деления на 10)
    ARRAY[3] = counter1 % 10; // записываем во 2 ячейку массива состояние кнопки "часы" (остаток от деления на 10)

    Serial.print("Минуты: "); Serial.println(counter1); // выводим в COM-порт состояние кнопки = переменной counter

  }
  if (buttonState1 == LOW && flag2 == 1) {
    flag2 = 0;
  }

  // вторая кнопка уменьшает значение на единицу

  if (digitalRead(buttonPin4) == HIGH && flag3 == 0) {
    counter1 --;
    flag3 = 1;

    if (counter1 <= 0) { // если значение <= нулю, то сброс останавливаем
      counter1 = 0;
    }

    ARRAY[2] = counter1 / 10; // записываем в 1 ячейку массива состояние кнопки "часы" (первая цифра перед запятой после деления на 10)
    ARRAY[3] = counter1 % 10; // записываем во 2 ячейку массива состояние кнопки "часы" (остаток от деления на 10)

    Serial.print("Минуты: "); Serial.println(counter1); // выводим в COM-порт состояние кнопки = переменной counter

  }

  if (digitalRead(buttonPin4) == LOW && flag3 == 1) {
    flag3 = 0;
  }

  // при нажатии обеих кнопок сразу, значение на LED-дисплее обнуляется

  if (flag2 == 1 && flag3 == 1) {
    counter1 = 0;
  }
}

// Фунция вывода на дисплей нулевых значений

void UpdateTimer() 

// Цикл for, которым можно заменить повторяющиеся операции вывода на дисплей элементов массива
// В цикле: перебираем переменные от 0 до 4 (4 элемента массива) и выводим на дисплей i-й элемент массива на i-е место

{
for (int i=0;i<4;i++){
  tm1637.display(i, ARRAY[i]); 
}

/*
 // Первый варинат функии, который можно заменить более компактным циклом for (см. выше)
  
  tm1637.display(0, ARRAY[0]); //выводим состояние кнопки "часы" в ячейку 1 дисплея
  tm1637.display(1, ARRAY[1]); //выводим состояние кнопки "часы" в ячейку 2 дисплея
  tm1637.display(2, ARRAY[2]); //выводим состояние кнопки "минуты" в ячейку 3 дисплея
  tm1637.display(3, ARRAY[3]); //выводим состояние кнопки "минуты" в ячейку 4 дисплея
  */

}
