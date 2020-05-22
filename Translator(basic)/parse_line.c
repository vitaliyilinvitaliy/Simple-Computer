#include "parse_line.h"


int if_pars(char *com, char *par, char *a) {
  // com - куда запишем вытянутую команду, par - параметры, a - строка с
  // параметрами IF
  
  int i = -1, j = 0, space = 0, flag = 0, if_uslovie = 0, stop = 1;
  do {
    ++i;
    if (a[i] != ' ' && a[i] != '\t') {
      ++if_uslovie;
    }
  } while (if_uslovie != 3); //пока не считается три параметра условия

  if (a[i] >= '0' && a[i] <= '9') {
    while (a[i] != ' ' && a[i] != '\t') {
      ++i;
    }
  } //если последний параметр - число, то идем до его конца

  if (a[i] >= 'A' && a[i] <= 'Z') {
      ++i;
  }

  char add[256] = {0};
  for (i; i < strlen(a); ++i, ++j) {
    add[j] = a[i];
  } //все оставшееся(add) - то, из чего будем вытаскивать команду и ее параметры
  
  sscanf(add, "%s", com); // вытащили команду(com)

  flag = 0;
  i = 0;
  if (add[0] == ' ' || add[0] == '\t')
    stop++;
  while (stop != 0) {
    if ((add[i] == ' ' || add[i] == '\t') && flag == 0) {
      stop--;
      flag = 1;
    } else if (add[i] != ' ' && add[i] != '\t') {
      flag = 0;
    }
    ++i;
  } //идем до конца команды(для этого нам нужно пройти stop пропусков, т.е.
    //каждый пропуск stop--)

  for (i, j = 0; i < strlen(add); ++i) {
 //   if (add[i] != ' ' && add[i] != '\t') {
      par[j] = add[i];
      ++j;
   // }
  } //переписываем параметры команды БЕЗ ПРОБЕЛОВ в другой массив(par)
  return 0;
}


int calcpars(char *a) {
  if (a[0] < 'A' || a[0] > 'Z') {
    printf("NOT A LETTER\n");
    return 1;
    //ОШИБКА: НЕ БУКВА
  }
  if (a[1] != '=') {
    printf("NO '=' OR TOO LONG VARIABLE \n");
    return 1;
    //ОШИБКА: ОТСУТСТВУЕТ ЗНАК РАВЕНСТВА ИЛИ СЛИШКОМ ДЛИННАЯ ПРИСВАИВАЕМАЯ
    //ПЕРЕМЕННАЯ
  }
  if (a[2] == '+' || a[2] == '*' || a[2] == '/' || a[2] == ')' ||
      a[strlen(a) - 1] == '+' || a[strlen(a) - 1] == '-' ||
      a[strlen(a) - 1] == '*' || a[strlen(a) - 1] == '/' ||
      a[strlen(a) - 1] == '(') {
    printf("YOU CANT START OR END IT WITH THIS SIGN\n");
    return 1;
    //ОШИБКА: ПЕРВЫЙ ИЛИ ПОСЛЕДНИЙ СИМВОЛ НЕ МОЖЕТ ЯВЛЯТЬСЯ ЗНАКОМ ИЛИ
    //НЕСООТВЕТСТВУЮЩЕЙ СКОБКОЙ
  }
  int flag = 0,
      p_flag = 0; // flag для счета количества разрядов в числе(чтобы выкинуть
                  // ошибку в случае, если оно больше допустимого), p_flag для
                  // счета открытых скобок - если после цикла она не равна 0, то
                  // значит, что где-то не закрыта скобка
  for (int i = 2; i < strlen(a); i++) {
    if ((a[i] == '+' || a[i] == '-' || a[i] == '*' || a[i] == '/' ||
         a[i] == '(') &&
        (a[i + 1] == '+' || a[i + 1] == '*' || a[i + 1] == '/' ||
         a[i + 1] == ')')) {
      printf("TWO SIGNS IN A ROW\n");
      return 1;
      //ОШИБКА: подряд идущие знаки
    }
    if (((a[i] >= 'A' && a[i] <= 'Z') &&
         (a[i + 1] >= '0' && a[i + 1] <= '9')) ||
        ((a[i + 1] >= 'A' && a[i + 1] <= 'Z') &&
         (a[i] >= '0' && a[i] <= '9')) ||
        ((a[i + 1] >= 'A' && a[i + 1] <= 'Z') &&
         (a[i] >= 'A' && a[i] <= 'Z'))) {
      printf("A LETTER AND A NUMBER IN A ROW OR TOO LONG VARIABLE\n");
      return 1;
      //ОШИБКА: подряд идущие переменная и число или слишком длинная
      //переменная
    }
    if (a[i] >= 'A' && a[i] <= 'Z') {
    } else if (a[i] >= '0' && a[i] <= '9') {
      flag++;
      if (flag > 5) {
        printf("NOT AVAILABLE VALUE\n");
        return 1;
        //ОШИБКА: НЕДОПУСТИМОЕ ЗНАЧЕНИЕ
      } else if (flag == 5) {
        int check = (a[i - 4] - '0') * 10000 + (a[i - 3] - '0') * 1000 +
                    (a[i - 2] - '0') * 100 + (a[i - 1] - '0') * 10 +
                    (a[i] - '0');
        if (check > 16383) {
          printf("NOT AVAILABLE VALUE\n");
          return 1;
          //ОШИБКА: НЕДОПУСТИМОЕ ЗНАЧЕНИЕ
        }
      }
    } else if (a[i] == '+' || a[i] == '-' || a[i] == '/' || a[i] == '*') {
      flag = 0;
    } else if (a[i] == ')' || a[i] == '(') {
      if (a[i] == '(') {
        p_flag++;
      } else if (a[i] == ')' && p_flag != 0) {
        flag = 0;
        p_flag--;
      } else {
        printf("NOT ENOUGH SCOBKI\n");
        return 1;
        //ОШИБКА: не закрыты скобки
      }
    } else {
      printf("UNKNOWN SYMBOl\n");
      return 1;
      //ОШИБКА: используется неизвестный символ
    }
  }
  if (p_flag != 0) {
    printf("NOT ENOUGH PARENTHESES\n");
    return 1;
    //ОШИБКА: не закрыты скобки
  }
  return 0;
}