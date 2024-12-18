## Current status ##

На данный момент программа поддерживает работу с :

- Функциями
	- log()
	- log2()
	- log10()
	- sin()
	- cos()
	- tan()
	- asin()
	- acos()
	- atan()
	- sqrt()
	- abs()
	- exp()
	
- Операторами
	- Сложение +
	- Вычетание -
	- Умножение *
	- Деление /
	- Взятие процента %
	- Возведение в степень ^
	- Факториал !
	- Так же поддерживается обработка унарного отрицания

- Константами
	- Число Пи (PI)
	- Число Ейлера (Е)
	- Золотое сечение (PHI)
	- Константа Пифагора (SQRTP)
	- Постоянная Апери (APE)

## Install ##


#### Compiling

```bash
git clone git@github.com:Pofxe/calculate_hard_.git
cd calculate_hard_
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage ##

```bash
./calculate <expression>
```
Example
```bash
./calculate "!3 + 4 - (2 - 4) + 3 ^ 2 ^ 3 + log10(2) + APE"
```
