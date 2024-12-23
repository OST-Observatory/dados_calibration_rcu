# DADOS remote calibration unit (RCU)
For wavelength and flat field calibration of our [DADOS spectrograph](https://www.baader-planetarium.com/en/instruments/spectroscopy/dados-slit-spectrograph.html?page=1) we use the [calibration module for Alpy 600 and UVEX from Shelyak](https://www.shelyak.com/produit/pf0037-module-detalonnage-alpy/?lang=en). For the remote control of this calibration module Shelyak offers a ready to use solution, but out of interest and for the fun of it we decided to build our own [Remote Calibration Unit](https://polaris.astro.physik.uni-potsdam.de/wiki/doku.php?id=en:ost:spektrograph:dados_calibration) (RCU) based on an Arduino. This repository contains the necessary code for this RCU:
* remote_dados_rcu.ino: contains the Arduino code
* control_dados_rcu.py: allows to control the RCU from a connected computer 

