# payload-fw

Прошивка для электроники летадла космической программы B4CKSP4CE.

## Building

* Install [platformio](https://platformio.org/install)
* do `pio run` for build
* do `pio run --target upload` for upload

## HW

STM32F013C8T6 / BluePill.

ST-Link for upload.

## Contributing

* Старайтесь добавлять код в Core/app.
* Используйте FreeRTOS API + STM32 HAL.
* Старайтесь работать в ветках и делать pull request'ы.
