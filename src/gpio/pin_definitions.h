/**
 * @file    pin_definitions.h
 * @brief   Определения пинов для платы REV
 * @note    Все определения сгруппированы по функциональным блокам
 */

/* ===================== БЛОК ОПРЕДЕЛЕНИЯ РЕВИЗИИ ПЛАТЫ ===================== */
/**
 * @brief Пин определения ревизии печатной платы (Printed Circuit Board revision)
 * @details Используется для идентификации аппаратной версии платы,
 *          позволяет ПО адаптироваться к разным ревизиям железа
 */
#define GPIO_REV_PCB_REV    GPIOA
#define PIN_REV_PCB_REV     1

/* ===================== БЛОК АНАЛОГОВОЙ ЧАСТИ ===================== */
/**
 * @brief Пин управления ЦАП (Digital-to-Analog Converter) для генерации Vramp
 * @details Управляет формированием нарастающего напряжения (ramp voltage),
 *          используется в схемах управления мощностью
 */
#define GPIO_REV_VRAMP_M    GPIOA
#define PIN_REV_VRAMP_M     4

/* ===================== БЛОК USB ИНТЕРФЕЙСА ===================== */
/**
 * @brief Отрицательная дифференциальная линия USB (USB D-)
 * @note  Требует аккуратной разводки на плате для соблюдения импеданса
 */
#define GPIO_REV_USB_N      GPIOA
#define PIN_REV_USB_N       11

/**
 * @brief Положительная дифференциальная линия USB (USB D+)
 * @note  Требует аккуратной разводки на плате для соблюдения импеданса
 */
#define GPIO_REV_USB_P      GPIOA
#define PIN_REV_USB_P       12

/* ===================== БЛОК ОТЛАДКИ (SWD) ===================== */
/**
 * @brief Пин данных интерфейса Serial Wire Debug (SWDIO)
 * @details Используется для отладки и программирования микроконтроллера
 */
#define GPIO_REV_SWDIO      GPIOA
#define PIN_REV_SWDIO       13

/**
 * @brief Пин тактирования интерфейса Serial Wire Debug (SWDCLK)
 */
#define GPIO_REV_SWD_CLK    GPIOA
#define PIN_REV_SWD_CLK     14

/* ===================== БЛОК SPI ИНТЕРФЕЙСА ТРАНСИВЕРА ===================== */
/**
 * @brief Сигнал выбора ведомого (Slave Select) для SPI3 трансивера
 * @details Активный низкий уровень, выбирает трансивер для обмена данными
 */
#define GPIO_REV_SPI3_NSS   GPIOA
#define PIN_REV_SPI3_NSS    15

/**
 * @brief Тактовый сигнал SPI3 трансивера (Serial Clock)
 */
#define GPIO_REV_SPI3_SCK   GPIOB
#define PIN_REV_SPI3_SCK    3

/**
 * @brief Линия данных Master-In-Slave-Out для SPI3 трансивера
 * @details Данные от трансивера к микроконтроллеру
 */
#define GPIO_REV_SPI3_MISO  GPIOB
#define PIN_REV_SPI3_MISO   4

/**
 * @brief Линия данных Master-Out-Slave-In для SPI3 трансивера
 * @details Данные от микроконтроллера к трансиверу
 */
#define GPIO_REV_SPI3_MOSI  GPIOB
#define PIN_REV_SPI3_MOSI   5

/* ===================== БЛОК УПРАВЛЕНИЯ УСИЛИТЕЛЕМ МОЩНОСТИ ===================== */
/**
 * @brief Пин управления режимом обхода (bypass) усилителя мощности
 * @details В режиме bypass сигнал проходит мимо усилителя,
 *          используется для режимов с низкой мощностью
 */
#define GPIO_REV_TR_BYPM    GPIOB
#define PIN_REV_TR_BYPM     7

/**
 * @brief Пин включения/выключения усилителя мощности
 * @details Управляет питанием усилителя мощности
 */
#define GPIO_REV_TR_ENM     GPIOB
#define PIN_REV_TR_ENM      8

/**
 * @brief Пин переключения режима передачи/приема (TX/RX) усилителя
 * @details Определяет, находится ли усилитель в режиме передачи (TX)
 *          или приема (RX)
 */
#define GPIO_REV_TR_TRM     GPIOB
#define PIN_REV_TR_TRM      9

/* ===================== БЛОК I2C ИНТЕРФЕЙСА (EEPROM) ===================== */
/**
 * @brief Тактовый сигнал I2C2 для внешней памяти EEPROM
 */
#define GPIO_REV_I2C2_SCL   GPIOB
#define PIN_REV_I2C2_SCL    10

/**
 * @brief Линия данных I2C2 для внешней памяти EEPROM
 */
#define GPIO_REV_I2C2_SDA   GPIOB
#define PIN_REV_I2C2_SDA    11

/* ===================== БЛОК СВЕТОДИОДОВ ИНДИКАЦИИ ===================== */
/**
 * @brief Красный светодиод (LED1)
 * @details Обычно используется для индикации ошибок или статуса питания
 */
#define GPIO_REV_LED_1      GPIOB
#define PIN_REV_LED_1       15

/**
 * @brief Зеленый светодиод (LED2)
 * @details Обычно используется для индикации рабочего состояния
 */
#define GPIO_REV_LED_2      GPIOC
#define PIN_REV_LED_2       7

/* ===================== БЛОК ЦИФРОВЫХ ВХОДОВ/ВЫХОДОВ ТРАНСИВЕРА ===================== */
/**
 * @brief Цифровой вход/выход 0 трансивера (DIO0)
 * @details Часто используется для прерываний или сигналов готовности
 */
#define GPIO_REV_DIO0_M     GPIOC
#define PIN_REV_DIO0_M      0

/**
 * @brief Цифровой вход/выход 1 трансивера (DIO1)
 * @details Используется для различных служебных сигналов трансивера
 */
#define GPIO_REV_DIO1_M     GPIOC
#define PIN_REV_DIO1_M      1

/**
 * @brief Цифровой вход/выход 2 трансивера (DIO2)
 * @details Используется для различных служебных сигналов трансивера
 */
#define GPIO_REV_DIO2_M     GPIOC
#define PIN_REV_DIO2_M      2

/* ===================== БЛОК UART ИНТЕРФЕЙСА ===================== */
/**
 * @brief Линия передачи данных USART3 (TX)
 * @details Для последовательной связи с внешними устройствами
 */
#define GPIO_REV_USART3_TX  GPIOC
#define PIN_REV_USART3_TX   10

/**
 * @brief Линия приема данных USART3 (RX)
 * @details Для последовательной связи с внешними устройствами
 */
#define GPIO_REV_USART3_RX  GPIOC
#define PIN_REV_USART3_RX   11

/* ===================== БЛОК УПРАВЛЕНИЯ СБРОСОМ ===================== */
/**
 * @brief Пин сброса (RESET) трансивера
 * @details Активный низкий уровень, используется для аппаратного сброса
 *          трансивера в начальное состояние
 */
#define GPIO_REV_RESET_TR   GPIOC
#define PIN_REV_RESET_TR    15
