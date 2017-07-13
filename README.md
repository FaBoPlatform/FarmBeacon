# FarmBeacon

> An attempt to transmit the sensor value of the farm by the iBeacon.

## MoistBeacon

> The iBeacon using moisture sensor data.  
> It transmit the sensor data in the Minor value.


### Requirement

- Board
Nordic nRF52 Development Kit board
http://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.nrf52%2Fdita%2Fnrf52%2Fdevelopment%2Fnrf52_dev_kit.html

- Moisture Sensor
https://www.dfrobot.com/wiki/index.php/Moisture_Sensor_(SKU:SEN0114)  
Connect the sensor to the AIN1(P0.03) pin on the dev board.

### How to build

1. Install the ARM compiler  
```
brew cask install gcc-arm-embedded
```

2. Install the nRF5x tools  
```
brew cask install nrf5x-command-line-tools
```

3. Install Visual Studio Code  
https://code.visualstudio.com/

4. Run build tasks  
https://code.visualstudio.com/docs/editor/tasks
