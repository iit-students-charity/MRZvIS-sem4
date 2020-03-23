#include "BinaryCalculator.h"
#include <vector>
#include <iostream>

BinaryCalculator::BinaryCalculator() {
}

BinaryCalculator::BinaryCalculator(int inputFirstNumber, int inputSecondNumber, int pairNumber, int taktStart,
                                   int taktMultiplier) {
    this->taktMultiplier = taktMultiplier;
    this->taktStart = taktStart;
    this->inputFirstNumber = inputFirstNumber;                  //делимое в десятичной форме
    this->inputSecondNumber = inputSecondNumber;                //делитель в десятичной форме
    this->firstBinaryNumber = _10_to_2_(inputFirstNumber);      //делимое в двоичной форме
    this->secondBinaryNumber = _10_to_2_(inputSecondNumber);    //делитель в двоичной форме
    this->secondBinaryNumberAddCode = (
            this->secondBinaryNumber);                          //делитель в двоичной дополнительном коде
    this->pairNumber = pairNumber;                              //номер пары
    this->div = std::vector<int>(4, 0);                 //частное
    this->summ = std::vector<int>(5, 0);                //остаток
    this->k = taktStart * taktMultiplier;
}

std::vector<int> BinaryCalculator::_10_to_2_(int x) {
    int i;
    int mod;
    std::vector<int> result;
    long double_ = 0;

    for (i = 0; x > 0; i++) {

        mod = x % 2;
        x = (x - mod) / 2;
        result.push_back(mod);
    }

    for (int i = 0; i < result.size() - 4; ++i)
        result.push_back(0);
    return result;
}

std::vector<int> BinaryCalculator::binaryAddition(std::vector<int> num_1, std::vector<int> num_2) {
    std::vector<int> sum(SIZE, 0);

    int carry = 0;
    for (int i = 0; i < SIZE; i++) {
        sum[i] = ((num_1[i] ^ num_2[i]) ^ carry); // c is carry
        carry = ((num_1[i] & num_2[i]) | (num_1[i] & carry)) | (num_2[i] & carry);
    }

    if (num_1[SIZE - 1] == 1 && num_2[SIZE - 1] == 1)
        sum = _2_to_additional_code(sum);

    return sum;
}

std::vector<int> BinaryCalculator::_2_to_inverted_code(std::vector<int> binaryNumber) {
    for (int i = 0; i < binaryNumber.size() - 1; ++i) {
        binaryNumber[i] = !binaryNumber[i];
    }
    return binaryNumber;
}

std::vector<int> BinaryCalculator::_2_to_additional_code(std::vector<int> binaryNumber) {
    if (binaryNumber[SIZE - 1] == 0)
        return binaryNumber;
    return binaryAddition(_2_to_inverted_code(binaryNumber), ONE);
}

void BinaryCalculator::shift() {
    std::vector<int> temp = std::vector<int>(this->summ.begin(), this->summ.end() - 1);
    temp.emplace(temp.begin(), 0);
    this->summ = temp;
    temp = std::vector<int>(this->firstBinaryNumber.begin(), this->firstBinaryNumber.end() - 1);
    temp.emplace(temp.begin(), 0);
    this->firstBinaryNumber = temp;
}

void BinaryCalculator::getSumm() {
    if (this->summ[SIZE - 1] == 0) {
        this->summ = binaryAddition(this->summ, this->secondBinaryNumberAddCode);
    } else {
        this->summ = binaryAddition(this->summ, this->secondBinaryNumber);
    }
}

void BinaryCalculator::fixResult() {
    if (this->summ[SIZE - 1] == 0) {
        std::vector<int> temp = std::vector<int>(this->div.begin(), this->div.end() - 1);
        temp.emplace(temp.begin(), 1);
        this->div = temp;
    } else {
        std::vector<int> temp = std::vector<int>(this->div.begin(), this->div.end() - 1);
        temp.emplace(temp.begin(), 0);
        this->div = temp;
    }
}

void BinaryCalculator::restoreRemainder() {
    if (this->summ[SIZE - 1] == 1) {
        this->summ = binaryAddition(this->summ, this->secondBinaryNumber);
    }
}

std::string stringify(std::vector<int> vector) {
    std::string val;
    for (int i = 0; i < vector.size(); i++) {
        val = std::to_string(vector[i]) + val;
    }
    return val;
}


bool BinaryCalculator::oneStep() {

    if (state == 0) {
        step_1();
    }

    if ((state == 1) || (state == 4) || (state == 7) || (state == 10)) {
        step_2();
    }

    if ((state == 2) || (state == 5) || (state == 8) || (state == 11)) {
        step_3();
    }

    if ((state == 3) || (state == 6) || (state == 9) || (state == 12)) {
        step_4();
    }

    if (state >= 13) {
        step_5();
        done = true;
        state = 99;
    }

//    if (firstBinaryNumber == ZERO) {
//        state = 99;
//    }

    state++;
    k = k + taktMultiplier;
    std::cout << "\n";
    return state <= 13;
}

void BinaryCalculator::step_1() {
    std::cout << "Пара " + std::to_string(pairNumber) + " Превращена в двоичное\n" +
                 "Tакт: " + std::to_string(k) + "\n" +
                 "Число №1: " + stringify(firstBinaryNumber) + "\n" +
                 "Число №2: " + stringify(secondBinaryNumber) + "\n";
}

void BinaryCalculator::step_2() {
    this->shift();
    std::cout << "Пара " + std::to_string(pairNumber) + " Сдвиг\n" +
                 "Tакт: " + std::to_string(k) + "\n" +
                 "Число №1: " + stringify(firstBinaryNumber) + "\n" +
                 "Число №2: " + stringify(secondBinaryNumber) + "\n";
}

void BinaryCalculator::step_3() {
    this->getSumm();
    std::cout << "Пара " + std::to_string(pairNumber) + " Сумма\n" +
                 "Tакт: " + std::to_string(k) + "\n" +
                 "Число №1: " + stringify(firstBinaryNumber) + "\n" +
                 "Число №2: " + stringify(secondBinaryNumber) + "\n" +
                 "summ: " + stringify(summ) + "\n";
}

void BinaryCalculator::step_4() {
    this->fixResult();
    std::cout << "Пара " + std::to_string(pairNumber) + " Обновить результат\n" +
                 "Tакт: " + std::to_string(k) + "\n" +
                 "Число №1: " + stringify(firstBinaryNumber) + "\n" +
                 "Число №2: " + stringify(secondBinaryNumber) + "\n" +
                 "Остаток: " + stringify(summ) + "\n";

}

void BinaryCalculator::step_5() {
    this->restoreRemainder();
    std::cout << "Пара " + std::to_string(pairNumber) + " Остаток \n" +
                 "Tакт: " + std::to_string(k) + "\n" +
                 "Число №1: " + std::to_string(inputFirstNumber) + "\n" +
                 "Число №2: " + std::to_string(inputSecondNumber) + "\n" +
                 "Остаток: " + stringify(summ) + "\n";
}
